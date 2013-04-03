/*
 * GlobalHelper.h
 *
 *  Created on: 2013-4-3
 *      Author: root
 */

#ifndef GLOBALHELPER_H_
#define GLOBALHELPER_H_

namespace poseidon
{
clock_t clockBegin, clockEnd;

void timing_begin()
{
//用clock()来计时  毫秒
	clockBegin = clock();
//	printf("%ld\n", clockEnd - clockBegin);

}
clock_t timing_end()
{
	//用clock()来计时  毫秒
	clockEnd = clock();
	return (clockEnd - clockBegin);

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
#endif /* GLOBALHELPER_H_ */
