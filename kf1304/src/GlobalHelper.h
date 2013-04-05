/*
 * GlobalHelper.h
 *
 *  Created on: 2013-4-4
 *      Author: root
 */

#ifndef GLOBALHELPER_H_
#define GLOBALHELPER_H_

#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
//#include <time.h>
#include <sstream>
#include <string>
#include <iostream>
#include <sys/time.h>

using namespace std;
namespace poseidon
{

class GlobalHelper
{
private:
//	clock_t clockBegin;
//	clock_t clockEnd;
	struct timeval start, stop, diff;
	int tim_subtract(struct timeval *result, struct timeval *start,
			struct timeval *stop)
	{
		if (start->tv_sec > stop->tv_sec)
			return -1;
		if ((start->tv_sec == stop->tv_sec) && (start->tv_usec > stop->tv_usec))
			return -1;
		result->tv_sec = (stop->tv_sec - start->tv_sec);
		result->tv_usec = (stop->tv_usec - start->tv_usec);
		if (result->tv_usec < 0)
		{
			result->tv_sec--;
			result->tv_usec += 1000000;
		}
		return 0;
	}
public:
	GlobalHelper();
	virtual ~GlobalHelper();

	void timing_begin()
	{
		gettimeofday(&start, 0);
	}
	void timing_end()
	{
		gettimeofday(&stop, 0);
	}
	string cast_time()
	{
		tim_subtract(&diff, &start, &stop);
		return ld2str(diff.tv_sec) + "." + ld2str(diff.tv_usec);
	}
	void time_test2()
	{
		timing_begin();
		usleep(1);
		timing_end();
		cout << "cast_time" << cast_time() << "秒" << endl;
	}
//num to string
	string num2str(int i)
	{
		stringstream ss;
		ss << i;
		return ss.str();
	}
	string ld2str(long int i)
	{
		stringstream ss;
		ss << i;
		return ss.str();
	}

	//实现string到unsigned int的转换
	unsigned int string_to_unsigned_int(string str)
	{
		unsigned int result(0); //最大可表示值为4294967296（=2‘32-1）
		//从字符串首位读取到末位（下标由0到str.size() - 1）
		for (int i = str.size() - 1; i >= 0; i--)
		{
			unsigned int temp(0), k = str.size() - i - 1;
			//判断是否为数字
			if (isdigit(str[i]))
			{
				//求出数字与零相对位置
				temp = str[i] - '0';
				while (k--)
					temp *= 10;
				result += temp;
			}
			else
			{
				break;
			}
			//exit(-1);

		}
		return result;
	}

	//unit is byte
	int file_size(string filename)
	{
		FILE *stream;
		stream = fopen(filename.c_str(), "r");
		fseek(stream, 0L, SEEK_END);
		long length = ftell(stream);
		fclose(stream);
		return length;
	}
	void string_contain_test()
	{
		string s1 = "HTTP/1.1 200 OK\r\n"
				"Cache-Control: max-age=86400\r\n"
				"Date: Thu, 04 Apr 2013 10:34:04 GMT\r\n"
				"Content-Length: 20035\r\n"
				"Content-Type: text/html\r\n";

		string s2;
		if (string_contain(s1, "Content-Length", &s2))
		{
			cout << s2 << endl;
		}
		string::size_type len;
		if (Content_length(s1, &len))
		{
			cout << len << endl;
		}

	}
	bool Content_length(string s1, string::size_type *length)
	{
		string s2;
		if (string_contain(s1, "Content-Length", &s2))
		{
			*length = this->string_to_unsigned_int(s2);
			return true;
		}
		return false;
	}
	bool string_contain(string str, string key, string *result)
	{
		unsigned int pos;
		unsigned int pos1;
		unsigned int pos2;
//STRING.FIND 查找，找到第一个取其：后\r\n 前的值。
		if ((pos = str.find(key, 0)) == string::npos)
		{
			return false;
		}
		if ((pos1 = str.find(":", pos)) == string::npos)
		{
			return false;
		}
		pos1++;
		if ((pos2 = str.find("\r", pos)) == string::npos)
		{
			return false;
		}
		cout << "pos=" << pos << " pos1=" << pos1 << " pos2=" << pos2 << endl;
		if (pos1 > pos2)
		{
			return false;
		}
		*result = str.substr(pos1, pos2 - pos1);
		return true;
	}

//是否以给定字符串结尾
	bool tail_with_feature(char* buf, int count, const char* feature)
	{

		int feature_len = strlen(feature);

		if (count < feature_len || 0 >= feature_len)
		{
			return false;
		}
		for (int j = 0; j < feature_len; ++j)
		{
			if (buf[count - j - 1] != feature[feature_len - j - 1])
			{
				return false;
			}
		}
		return true;
	}
}
;

}
#endif /* GLOBALHELPER_H_ */