#pragma once

#include <cmath>
#include <stdexcept>

namespace math
{
	void check(const long double& data)
	{
		if (std::isinf(data) || std::isnan(data))//Check for overflow &&  underflow(检查是否溢出&&溢下)
	         {  throw std::runtime_error("error : T overflow or T underflow");  }  }
	struct  data
	{
		long long count = 0;    //data(数据)
		long double M2 = 0.0;    //Accumulated deviation(累计偏差)
		long double mean = 0.0;  //average value of data calculation(数据计算的平均值)

		//data update
		void update(long double x)
		{
			count += 1;                 //Enter data (进入数据)
			long double delta1 = x - mean ; //Translating new data minus average data yields the deviation (新数据 - 平均数据 得到偏差)
			mean += delta1 / count;     // Data deviation -> Averaging deviation  (数据偏差 -> 平均化偏差)
			long double delta2 = x - mean ; // Process new data(处理新数据)
			//Accumulate the new data deviation and old data deviation together (将新数据偏差和旧数据偏差累计到一起)
			M2 += delta1 * delta2;      
		}
		//Process
		void Set(const data& get)
		{
			if(get.count == 0)
			   { return; }  //No data return (无数据返回)
            // set_n = Number of data + Number of processed data (set_n = 处理数据 + 数据数量)
			long double set_n = count + get.count; 
            // set_iff = Handling Data Discrepancies - Raw Data Discrepancies (set_iff =  处理数据差异 - 原始数据差异)
			long double set_diff = get.mean - mean;
			//(Sum of Average Data Differences + Sum of Processed Data Differences))/Process data samples plus data samples(数据差异之和加上处理数据之和除以处理数据样本加上数据样本)
			mean = (count * mean + get.count * get.mean) / (count + get.count);
			//within-group variation + distance between groups -> total variation (组内波动 + 间组之间拉开的距离 -> 总波动)
			M2 = M2 + get.M2 + set_diff * set_diff * (count * get.count) / (count + get.count);
			//sum
			count = set_n;
		}//--------------- return result
		long double diff_Result() 
		{                        // return data differences result 
			check(mean);
			return mean; 
		} 
		long double fluc_Result()
			//If the data size is greater than 1, return the average fluctuation; otherwise, return 0.0.
		{
			check(M2);
			return (count > 1) ? M2 / (count - 1) : 0.0 ;
		}
		long long sample_Result()
		{  return count; }  //return sample
	};
	
}
