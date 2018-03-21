// Copyright 2017 Baidu Inc. All Rights Reserved.
// Author: Fucheng zhang (zhangfucheng@baidu.com)
//
// implement LineParser::parse()

#include "parser.h"

#include <sstream>

namespace baidu {
/** 
 * @brief start parse a line 
 * @param 
 * @return int 
 * @retval 0:succeed to parse a line -1:line format error 
 * @exception throw std::exception if failed to parse a column
 * @author zhangfucheng
 * @date 2017.11.7 
**/
int LineParser::parse(std::string line) const {
    std::stringstream ss(line);
    std::string sub_str;
    unsigned int i = 0;
    while (getline(ss, sub_str, '\t') && i < _v.size()) {
        int cons = _v[i]->parse(sub_str);
        if (cons < 0) {
            return -1;
        }
        i++;
    }
    if (ss.eof() && (i == _v.size())) {
        return 0;
    }
    return -1;
}

}
