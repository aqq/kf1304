/*
 * analyser.h
 *
 *  Created on: 2013-4-21
 *      Author: andrew
 */

#ifndef ANALYSER_H_
#define ANALYSER_H_

#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>
#include <sys/time.h>
#include "map"
#include <iostream>     // std::cout
#include <algorithm>    // std::find_if
#include <vector>       // std::vector
#include <iostream>
#include "fstream"
#include "assert.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
//disk
#include <sys/statfs.h>
#include "GlobalHelper.h"

using namespace std;
namespace poseidon {

class analyser {
public:
	GlobalHelper* gh;
	string html_content;
	analyser();
	void get_url() {
		gh = new GlobalHelper();
	}
	void get_str() {

	}
	void read_file(string fname) {

	}
	void extract(string html) {

	}
	void get_enterprise_type_and_id(vector<string> str_vec,
			string& enterprise_type, string& enterprise_id) {
		//待抽取URL的格式
		//case 1: smt.114chn.com/Webpub/370800/120619000039/index.shtml
		//case 2: ts.114chn.com/tradehtml/620500/6205001209050001/index.html
		//case 3: ahmc.114chn.com/m/web/shop/Product.aspx?shopid=3416221005280001&pid=34162210052800010009
		if (str_vec[1] == "Webpub") {
			enterprise_type = "Webpub";
			enterprise_id = str_vec[1] + "_" + str_vec[2] + "_" + str_vec[3];
		}
		if (str_vec[1] == "tradehtml") {
			enterprise_type = "trade";
			enterprise_id = str_vec[1] + "_" + str_vec[2] + "_" + str_vec[3];
		}
		if (str_vec[1] == "m" && str_vec[2] == "web" && str_vec[3] == "shop") {
			enterprise_type = "shop";
			size_t pos1 = str_vec[4].find_first_of('=', 0);
			enterprise_id = str_vec[4].substr(pos1 + 1,
					str_vec[4].find_first_of('&', 0) - pos1 - 1);
		}
	}

	void replace_html_badchar(string& key) {
		//theString = theString.Replace(">", "&gt;");
		//theString = theString.Replace("<", "&lt;");
		//theString = theString.Replace(" ", " &nbsp;");
		//theString = theString.Replace(" ", " &nbsp;");
		//theString = theString.Replace("\"", "&quot;");
		//	theString = theString.Replace("\'", "&#39;");
		//theString = theString.Replace("\n", "<br/> ");
		key = gh->replace_all(key, "", " ");
		key = gh->replace_all(key, "", "\n");
		key = gh->replace_all(key, "", "\r");
		key = gh->replace_all(key, "", "\t");
		key = gh->replace_all(key, "", "\'");
		key = gh->replace_all(key, "", "\"");
	}
	void replace_html_badchar_test() {
		string s1 = "a b\r\n \\ adsa213";
		replace_html_badchar(s1);
		cout << s1 << endl;

	}
	vector<string> extract_str_from_htmltag(string html) {

		//4 	 extract  html  str_map
		map<string, string> str_map;
		std::string::iterator it_char = html.begin();
		vector<string> content_vec;
		size_t pos1 = 0;
		size_t pos2 = 0;
		bool pos1_b = false;
		for (size_t pos = 0; it_char != html.end(); it_char++, pos++) {
			//	cout << *it_char;
			// if '>' then log it one time;
			if (*it_char == '>') {
				pos1 = pos;
				if (!pos1_b) {
					pos1_b = !pos1_b;
				}
			} else if (*it_char == '<' && pos1_b) {
				// if '<' then substr them and push
				pos2 = pos;
				if (pos2 <= pos1) {
					continue;
				}
				string t_str = html.substr(pos1 + 1, pos2 - pos1 - 1);
				this->replace_html_badchar(t_str);
				if (t_str == "") {
					continue;
				}
				content_vec.push_back(t_str);
				//			cout << "[" << t_str << "]" << endl;
			} //end else if

		} //end for

		return content_vec;
	}
	void extract_str_from_htmltag_test() {
		string c1 = " <div>"
				"联系人：<a id=\"d\">郑彩芸</a><br />"
				"电话：<a id=\"d\">13943240757</a><br />"
				"传真：<a id=\"d\">0432-65223317</a><br />"
				"邮箱：<a id=\"d\">sjtlc_114@163.com</a><br />"
				"地址：<a id=\"d\">磐石市瓦房道口</a><br />"
				"        </div>";
		c1 = "<br>"
				"联系人："
				"<span id=\"hpLeft_Left1_lblLinkMan\">xkbz</span><br>"
				"电话："
				"<span id=\"hpLeft_Left1_lblTel\">3181166</span><br>"
				"传真："
				"<span id=\"hpLeft_Left1_lblFax\">3181166</span><br>"
				"邮编："
				"<span id=\"hpLeft_Left1_lblPostalCode\">3181166</span><br>"
				"电邮："
				"<span id=\"hpLeft_Left1_lblEmail\">623472513@qq.com</span><br>"
				"网址："
				"<a id=\"hpLeft_Left1_hlkHomePage\" ></a><br>"
				"地址："
				"<span id=\"hpLeft_Left1_lblAddress\">3181166</span></td>"
				"</tr>";
		vector<string> str_vec = extract_str_from_htmltag(c1);
		for (vector<string>::iterator it2 = str_vec.begin();
				it2 != str_vec.end(); ++it2) {
			std::cout << " " << " => " << *it2 << '\n';
		}
	}
//return need_skip
	bool extract_kv(string it2_str, string it2_next_str, string& key,
			string& value, string split_char_str) {
		//CASE 1: XX:YYY
		//split_char_str=":" or "："
		if (it2_str.find(split_char_str, 0) == string::npos) {
			return 1;
		}

		vector<string> str_kv_vec;
		gh->split(it2_str, split_char_str, str_kv_vec);

		if (str_kv_vec.size() == 0) {
			return 1;
		}
		//
		key = str_kv_vec.at(0);
		if (str_kv_vec.size() > 1) {
			value = str_kv_vec.at(1);
		}
		//
		//	cout << "[" << value << "]" << endl;
		this->replace_html_badchar(value);
		//	cout << "[" << value << "]" << endl;
		if (it2_next_str.find(split_char_str, 0) == string::npos) {
			//CASE 2: XX:<>YY<>
			value = it2_next_str;
			return 0;
		}

		//CASE 2: XX:<>YY<>
		//value.size == 0
		vector<string> str_kv_next_vec;
		//	gh->split(it2_next_str, split_char_str, str_kv_next_vec);

		//	if (it2_next_str.find(split_char_str, 0) == string::npos) {
		//		value = it2_next_str;
//			skip_next_it = 1;
//		} else {
		//		skip_next_it = 0;
//		}

		return 0;
	}
	void extract_kv_test() {
		string key = "";
		string value = "";
		string it2_str = "xx：x		 x";
		string it2_next_str = "";
		bool skip_next_it = 0;
		bool need_skip = extract_kv(it2_str, it2_next_str, key, value, "：");
		cout << "skip?" << need_skip << " key:" << key << ";value:" << value
				<< endl;

		key = "";
		value = "";
		it2_str = " 联系人：";
		it2_next_str = "郑彩芸";
		skip_next_it = 0;
		need_skip = extract_kv(it2_str, it2_next_str, key, value, "：");
		cout << "skip?" << need_skip << " key:" << key << ";value:" << value
				<< endl;

		key = "";
		value = "";
		it2_str = " ";
		it2_next_str = "小名";
		skip_next_it = 0;
		need_skip = extract_kv(it2_str, it2_next_str, key, value, "：");
		cout << "skip?" << need_skip << " key:" << key << ";value:" << value
				<< endl;

	}
	void just_work() {
		vector<string> fnames_evc;
		string path = "./pages/";
		gh->trave_dir_into_vec_ie(path, fnames_evc);
		for (vector<string>::iterator it2 = fnames_evc.begin();
				it2 != fnames_evc.end(); ++it2) {
			std::cout << " it2  " << *it2 << endl;
			analysis_page(*it2, path);

		}

	}
//
	void analysis_page(string fname, string path) {
		//1 get url
		string url = gh->read_line(path + fname, 2);
//		cout << url << endl;
		//2 判断该URL是否属于我要抽取的URL,若是则分配一个ID
		vector<string> str_vec;
		gh->split(url, "/", str_vec);
//		cout << str_vec.size() << endl;
		if (str_vec.size() != 5) {
			return;
		}
		string enterprise_id = "";
		string enterprise_type = "";
		get_enterprise_type_and_id(str_vec, enterprise_type, enterprise_id);
		cout << enterprise_id << endl;
		cout << enterprise_type << endl;
		//
		//3继续抽取该URL的内容找出 K和V
		string html_all = gh->read_all_file(path + fname);
		string html = html_all.substr(html_all.find_first_of('<') - 1);

		//4 	 extract  html  str_map
		vector<string> content_vec = extract_str_from_htmltag(html);

		for (vector<string>::iterator it2 = content_vec.begin();
				(it2 + 1) != content_vec.end(); ++it2) {

			string it2_str = *it2;
			string it2_next_str = *(it2 + 1);

			//gh->log4("k_v", *it2);
			//	cout << "[" << it2_str << "]" << endl;
			//5 extract key and value
			string key = "";
			string value = "";

			bool need_skip = extract_kv(it2_str, it2_next_str, key, value, "：");

			if (need_skip) {
				continue;
			}
			//
			replace_html_badchar(key);
			replace_html_badchar(value);
			//gh->log4("k_v", *it2);
			//gh->log2("key:", key, "  value:", value, "k_v");
			//cout << "key:" << key << ";value:" << value << endl;
			cout << key << " ==> " << value << endl;

			//7 origin into a sql
			string content_str = "";
			content_str +=
					"insert into  company_info_tb (Company_id,Company_type,Key,Value) values("
							+ enterprise_id + "," + enterprise_type + ",'" + key
							+ "','" + value + "');\n";

			//INSERT INTO table_name (列1, 列2,...) VALUES (值1, 值2,....)
			//*company_info_id 		Company_id 		Company_type 		Key 		Value
			//	gh->log2(content_str, "sql");
			//	cout << "content_str:" << content_str << endl;

			//8 out put to asql
			string d_path = path + "kv.sql";
			gh->log(d_path, content_str);
		} //end for

	}
	virtual ~analyser();
};

} /* namespace poseidon */
#endif /* ANALYSER_H_ */
