/*
 * GlobalHelper.cpp
 *
 *  Created on: 2013-4-4
 *      Author: root
 */

#include "GlobalHelper.h"
namespace poseidon {
GlobalHelper::GlobalHelper() {
	split_char_betwen_pages = "\a";
	SLAVE_CONF="./conf/slave.conf";
}

GlobalHelper::~GlobalHelper() {
	// TODO Auto-generated destructor stub
}
}
