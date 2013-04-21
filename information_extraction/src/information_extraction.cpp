//============================================================================
// Name        : information_extraction.cpp
// Author      : andrew
// Version     :
// Copyright   : newplayer
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "GlobalHelper.h"
#include "analyser.h"

using namespace std;
using namespace poseidon;
int main() {
	//GlobalHelper* gh = new GlobalHelper();
	//gh->is_html_end_test();
	//(new GlobalHelper())->replace_all_test();
	//(new analyser())->replace_html_badchar_test();
	//(new analyser())->extract_kv_test();

	//(new analyser())->extract_str_from_htmltag_test();

	(new analyser())->just_work();

	//cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
