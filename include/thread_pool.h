/*
   @Main155 
*/
#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include "Welford.h"
#include <concepts>
#include <type_traits>
#include <iostream>
#include <mutex>
#include <algorithm>

template <typename T>
concept number = std::is_floating_point_v<T> || std::is_integral_v<T>; 
/*Must be a number(必须为数字)*/

namespace thread
{	
	template<number T>
	class pool 
	{
		struct result
		{  //Result
			long double variance;
			long double mean;
			long long count;
		};
		/* Processing center data */
		std::vector<std::thread> work;    //run thread
		std::queue<std::vector<T>> run; //Group -> pool_work
		std::condition_variable cv;     //Notice
		std::mutex mtx; //lock()
		std::atomic<bool> stop{ false };/* * Condition * */
		//-------------------------
	    /* Result returned ( wait() function ) */
		std::vector<result> Result_return; //Return result
		//-----------------------
		/*  compare */
		std::atomic<size_t> count{ 0 }; // count ->pool_work() -> slot (Processed quantity)
		std::atomic<size_t> completed{ 0 };  //execution count
		std::atomic<size_t> th{ 0 };         //thread count
		/* ============== */
		void pool_work()
		{ /* Main function */
			for (;;)
			{	 
				std::vector<T> task;
				{
					std::unique_lock<std::mutex> lock(mtx);
					cv.wait(lock, [&] { return stop || !run.empty(); }); 
					if (stop && run.empty()) return;  //cheak
                    //The data is handed over to the task.(数据交给task)
					task = std::move(run.front());
					run.pop();           //delete
				}//Increment a set of counters by 1. (运行一组count加1)
				size_t slot = count.fetch_add(1, std::memory_order_seq_cst);
				/* center -> Welforf.h* **/
				try
				{/* Capture exception data. -> Welforf.h**  */
					math::data get;  //Weiford.h 
					for (size_t it = 0; it < task.size();++it)
					{
						get.update(task[it]);  //update data
					}
					Result_return[slot] =    //Write result （写入结果）
					{ get.fluc_Result(), get.diff_Result(), get.sample_Result() };
				}
				catch (std::runtime_error& e)
				{  /*Error handling *** */
					Result_return[slot] = { NAN, NAN, 0 };  //Error logging
					std::cerr << "Worker error at slot " << slot << ": " << e.what() << '\n'; //Print error
				}
				if(completed.fetch_add(1, std::memory_order_acq_rel) + 1 == th)
				{   /* Notify the wait() function once processing is complete.*/
					std::unique_lock<std::mutex> lock(mtx);
					cv.notify_one(); //Notice
				}
			}
		}
	public:
		explicit pool(size_t n = std::thread::hardware_concurrency()) 
		{
			if (n == 0) { n = 1; }//special case
			Result_return.reserve(n * 4);  //allocate memory
			for (size_t i = 0; i < n ; ++i)
			{
				/* pool_work -> thread */
				work.emplace_back([this] { pool_work();});  
				//Constructing threads
			}
		}

		/* Main exprot*/
		void Export(const std::vector<T>& y, const size_t& Group_size)
		{
			static_assert(std::is_floating_point_v<T> || sizeof(T) >= 4,
				"Integer types smaller than 32-bit may lose precision in Welford accumulation");
			/* cheak */
			if (count.load(std::memory_order_seq_cst) != th.load(std::memory_order_seq_cst)
				  && th.load(std::memory_order_seq_cst) > 0 )
			{   /* * Have you finished the task? * */
				std::unique_lock<std::mutex> lock(mtx);
				cv.wait(lock, [&] { return completed.load() == th; });
			}
			/* Basic examination data */
			if (y.empty()) { std::cerr << "error : No data\n"; return; }  // y empty
			if (y.size() < Group_size || Group_size == 0)  
			// Group > y size    //principle
			       { std::cerr << "error : Group cannot be greater than the number of data entries \n";return; } 
			th = (y.size() + Group_size - 1) / Group_size; //How many groups = th
            /*Initialize*/
			count.store(0 , std::memory_order_seq_cst);
			completed.store(0, std::memory_order_relaxed);
			/* Allocate memory space */
			Result_return.resize(th); 
			/* Write */
			{
				std::lock_guard<std::mutex> lock(mtx);
				for (size_t i = 0; i < y.size(); i += Group_size)  //Grouping 
				{
					size_t end = std::min(i + Group_size, y.size());
					run.emplace(
						std::make_move_iterator(y.begin() + i),  //begin
						std::make_move_iterator(y.begin() + end));//end
				}
			}
			cv.notify_all();//Notice -> pool_work()
		}

		/* Return interface */
		std::vector<result> wait()
		{
			std::unique_lock<std::mutex> lock(mtx);
			/* Has every group been processed */
			cv.wait(lock, [&] 
				{return completed.load(std::memory_order_seq_cst) == th; }); 
			return Result_return; //Interface response result
		}

		~pool()
		{		
			//Running thread
			{
				std::lock_guard<std::mutex> lock(mtx);
				stop.store(true, std::memory_order_seq_cst);
			}
			cv.notify_all();
			for (auto & it : work)
			{
				//Run in sequence
				it.join();
			}
		}
		/* No copying allowed */
		pool(const pool&) = delete;
		pool& operator=(const pool&) = delete;
	};
}
