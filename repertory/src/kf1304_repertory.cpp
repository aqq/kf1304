//============================================================================
// Name        : kf1304_repertory.cpp
// Author      : andrew
// Version     :
// Copyright   : newplayer
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "GlobalHelper.h"
#include "rep_test.h"
using namespace std;
using namespace poseidon;
#define DEBUG

using namespace std;

int main(int argc, char *argv[]) {
	if (argc == 1) {
		rep_test();
		return 1;
	}

	if (argc == 2) {
		(new GlobalHelper())->split_file_test();
		return 1;
	}
	cout << "usage:" << argv[0] << " " << "[split]." << endl;
//	disk_space_test();
//report_disk_space_test();

//(new GlobalHelper())->split_file_test();
//	split_file_test();
//
//	rep_feedback_test();
//read_config_test();

	return 0;
}
