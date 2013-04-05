/*
 * master_test.cpp
 *
 *  Created on: 2013-4-1
 *      Author: root
 */
#ifndef MASTER_TEST_H
#define MASTER_TEST_H
#include "master.h"

namespace poseidon
{
void master_test2slave()
{
	master * mt = new master();
	mt->service();

}
} /* namespace poseidon */
#endif
