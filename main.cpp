// Copyright 2017 Baidu Inc. All Rights Reserved.
// Author: Fucheng zhang (zhangfucheng@baidu.com)
//
// demo for parser

#include <iostream>
#include <exception>

#include <com_log.h>

#include "parser.h"

namespace my {
/**
 * user defined structure
 */
struct St {
    int i;
    float f;
};

/**
 * user defined class for parse user structure
 * should implement operator() function
 * operator() function should take a string as parameter and return a user structure
 */
class Parse {
public:
    St operator()(const std::string& s) throw(std::exception) {
        St temp;
        std::string::size_type pos = s.find(',');
        if (pos == std::string::npos) {
            throw std::invalid_argument("can not find ',' for st");
        }
        temp.i = stoi(s.substr(0, pos));
        temp.f = stof(s.substr(pos+1, s.size()-pos-1));
        return temp;
    }
};

}

int main() {
    //parser is a template class used to parse a column and hold the consequence
    using baidu::Parser;
    //for build-in types,only need one template parameter
    Parser<int> p0;
    Parser<float> p1;
    Parser<double> p2;
    //for string and vector of build-in types, need one template parameter
    Parser<std::string> p3;
    Parser<std::vector<float>> p4;
    Parser<std::vector<int>> p5;
    //for user defined structure, need two template parameter, the first is structure type ,the second is parse type
    Parser<my::St, my::Parse> p6;

    //first usage : use the LineParser parse a line directly
    using baidu::LineParser;//line_Parser is used to parse a line
    LineParser ls;
    //add Parser to line_Parser in order, so that line_Parser can parse every column
    ls.add_parser(&p0);
    ls.add_parser(&p1);
    ls.add_parser(&p2);
    ls.add_parser(&p3);
    ls.add_parser(&p4);
    ls.add_parser(&p5);
    ls.add_parser(&p6);
    // now, start parse a line, it returns -1 when the format of line is invalid
    if (ls.parse("-1\t32.67\t23.123456\tzhang\t"
                "3:1.1,2.2,6.4\t2:13,11\t12,23.123") == 0) {
        std::cout << p0.data() << " " << p1.data() << " "
        << p2.data() << " " << p3.data() << " "
        << p4.data()[0] << " " << p5.data()[0] << " "
        << p6.data().i << " " << p6.data().f << " " << std::endl;
    } else {
        CNOTICE_LOG("line format error");
    }
    //for next line
    if (ls.parse("0\t3.67\t23.6666\tfucheng\t"
                "2:10.1,20.2\t3:133,111,233\t17,0.3") == 0) {
        std::cout << p0.data() << " " << p1.data() << " "
         << p2.data() << " " << p3.data() << " "
        << p4.data()[0] << " " << p5.data()[0] << " "
        << p6.data().i << " " << p6.data().f << " " << std::endl;
    } else {
        CNOTICE_LOG("line format error");
    }

    //second usage : use DictParser to parse a file
    baidu::DictParser dp("demo.txt"); //DictParser is used to parse a file
    dp.add_column(&p0);
    dp.add_column(&p1);
    dp.add_column(&p2);
    dp.add_column(&p3);
    dp.add_column(&p4);
    dp.add_column(&p5);
    dp.add_column(&p6);
    while (!dp.is_file_end()) {
        if (dp.parse_next_line() == 0) {
            std::cout << p0.data() << " " << p1.data() << " "
                << p2.data() << " " << p3.data() << " "
                << p4.data()[0] << " " << p5.data()[0] << " "
                << p6.data().i << " " << p6.data().f << " " << std::endl;
        } else {
            CNOTICE_LOG("line format error");
        }
    }

    return 0;
}
