#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include "Welford.h"
#include <functional>
#include <concepts>
#include <type_traits>
#include <iostream>
#include <algorithm>
#include <mutex>

template <typename T>
concept number = std::is_floating_point_v<T> || std::is_integral_v<T>; //Must be a number(必须为数字)

namespace thread
{
	template <typename T>
	class pool
	{
		struct result
		{
			long double variance;
			long double mean;
			long long count;
		};
		std::vector<std::thread> work;   //Group
		std::queue<std::vector<T>> run;
		std::condition_variable cv;
		std::mutex mtx;
		bool stop{ false };

		std::vector<result> Result_return;
		std::atomic<size_t> count{ 0 };
		std::atomic<size_t> th{ 0 };
		void pool_work()
		{
			for (;;)
			{
				std::vector<T> task;
				{
					std::unique_lock<std::mutex> lock(mtx);
					cv.wait(lock, [&] { return stop || !run.empty(); });
					if (stop && run.empty()) return;
					task = std::move(run.front());
					run.pop();
				}
				math::data get;
				for (size_t it = 0; it < task.size();++it)
				{
					get.update(task[it]);
				}
				size_t slot = count.fetch_add(1);
				Result_return[slot] = { get.fluc_Result(), get.diff_Result(), get.sample_Result() };
				if (count.load(std::memory_order_seq_cst) == th)
				{
					cv.notify_one();
				}
			}
		}
	public:
		explicit pool(size_t n = std::thread::hardware_concurrency()) : Result_return(n * 4)  //Allocate space (分配空间)
		{
			for (size_t i = 0; i < n; ++i)
			{
				work.emplace_back([this] { pool_work();});  //Constructing threads
			}
		}

		template <number N>
		void Export(const std::vector<N>& y, const size_t& Group_size)
		{
			if (y.empty()) { std::cerr << "error : No data\n"; return; }  // y empty
			if (y.size() < Group_size || Group_size == 0) { std::cerr << "error : Group cannot be greater than the number of data entries \n";return; } // Group > y size
			th = (y.size() + Group_size - 1) / Group_size;
			count.store(0, std::memory_order_seq_cst);
			Result_return.resize(th);
			{
				std::lock_guard<std::mutex> lock(mtx);
				for (size_t i = 0; i < y.size(); i += Group_size)
				{
					size_t end = std::min(i + Group_size, y.size());
					run.emplace(
						std::make_move_iterator(y.begin() + i),  //begin
						std::make_move_iterator(y.begin() + end));//end
				}
			}
			cv.notify_all();
		}

		std::vector<result> wait()
		{
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock, [&] {return count.load() == th; });
			return Result_return;
		}

		~pool()
		{
			{
				std::lock_guard<std::mutex> lock(mtx);
				stop = true;
			}
			cv.notify_all();
			for (auto& it : work)
			{
				it.join();
			}
		}

		pool(const pool&) = delete;
		pool& operator=(const pool&) = delete;
	};
}
