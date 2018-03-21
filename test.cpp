// Copyright 2017 Baidu Inc. All Rights Reserved.
// Author: Fucheng zhang (zhangfucheng@baidu.com)
//
//test parser

#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

#include "parser.h"

namespace test{

using baidu::Parser;
using baidu::LineParser;
//test one column for only int
TEST(one_column, int){
    Parser<int> p0;
    LineParser lp;
    lp.add_parser(&p0);

    EXPECT_EQ(lp.parse(), -1);

    lp.set_line("<1>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_EQ(p0.get_data(), 1);

    lp.set_line("<-1>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_EQ(p0.get_data(), -1);

    lp.set_line("<0>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_EQ(p0.get_data(), 0);

    lp.set_line("<1.2>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_EQ(p0.get_data(), 1);

    lp.set_line("<abc>");
    EXPECT_THROW(lp.parse(), std::invalid_argument);

    lp.set_line("1>");
    EXPECT_EQ(lp.parse(), -1);

    lp.set_line("<>");
    EXPECT_THROW(lp.parse(), std::invalid_argument);
}
//test one column for only float
TEST(one_column, float){
    Parser<float> p0;
    LineParser lp;
    lp.add_parser(&p0);

    EXPECT_EQ(lp.parse(), -1);

    lp.set_line("<1.1>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_FLOAT_EQ(p0.get_data(), 1.1);

    lp.set_line("<-1.1>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_FLOAT_EQ(p0.get_data(), -1.1);

    lp.set_line("<0.0>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_FLOAT_EQ(p0.get_data(), 0);

    lp.set_line("<11>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_FLOAT_EQ(p0.get_data(), 11);

    lp.set_line("<abc>");
    EXPECT_THROW(lp.parse(), std::invalid_argument);

    lp.set_line("1.11>");
    EXPECT_EQ(lp.parse(), -1);

    lp.set_line("<>");
    EXPECT_THROW(lp.parse(), std::invalid_argument);
}
//test one column for only double
TEST(one_column, double){
    Parser<double> p0;
    LineParser lp;
    lp.add_parser(&p0);

    EXPECT_EQ(lp.parse(), -1);

    lp.set_line("<1.1234567>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_DOUBLE_EQ(p0.get_data(), 1.1234567);

    lp.set_line("<-1.1234567>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_DOUBLE_EQ(p0.get_data(), -1.1234567);

    lp.set_line("<0.0>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_DOUBLE_EQ(p0.get_data(), 0);

    lp.set_line("<11>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_DOUBLE_EQ(p0.get_data(), 11);

    lp.set_line("<abc>");
    EXPECT_THROW(lp.parse(), std::invalid_argument);

    lp.set_line("1.11>");
    EXPECT_EQ(lp.parse(), -1);

    lp.set_line("<>");
    EXPECT_THROW(lp.parse(), std::invalid_argument);
}
//test one column for only string
TEST(one_column, string){
    Parser<std::string> p0;
    LineParser lp;
    lp.add_parser(&p0);

    EXPECT_EQ(lp.parse(), -1);

    lp.set_line("<abc>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_STREQ(p0.get_data().c_str(), "abc");

    lp.set_line("1>");
    EXPECT_EQ(lp.parse(), -1);

    lp.set_line("<>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_STREQ(p0.get_data().c_str(), "");
}
//test one column for vector
TEST(one_column, vector){
    //vector<int>
    Parser<std::vector<int>> p0;
    LineParser lp0;
    lp0.add_parser(&p0);

    EXPECT_EQ(lp0.parse(), -1);

    lp0.set_line("<1:1234>");
    EXPECT_EQ(lp0.parse(), 0);
    ASSERT_EQ(p0.get_data().size(), 1);
    EXPECT_EQ(p0.get_data()[0], 1234);

    lp0.set_line("<2:1234,4321>");
    EXPECT_EQ(lp0.parse(), 0);
    ASSERT_EQ(p0.get_data().size(), 2);
    EXPECT_EQ(p0.get_data()[1], 4321);

    lp0.set_line("1>");
    EXPECT_EQ(lp0.parse(), -1);

    lp0.set_line("<>");
    EXPECT_THROW(lp0.parse(), std::invalid_argument);
    //vector<float>
    Parser<std::vector<float>> p1;
    LineParser lp1;
    lp1.add_parser(&p1);

    EXPECT_EQ(lp1.parse(), -1);

    lp1.set_line("<1:1234.1234>");
    EXPECT_EQ(lp1.parse(), 0);
    ASSERT_EQ(p1.get_data().size(), 1);
    EXPECT_FLOAT_EQ(p1.get_data()[0], 1234.1234);

    lp1.set_line("<2:1234.1,4321.1>");
    EXPECT_EQ(lp1.parse(), 0);
    ASSERT_EQ(p1.get_data().size(), 2);
    EXPECT_FLOAT_EQ(p1.get_data()[1], 4321.1);

    lp1.set_line("1>");
    EXPECT_EQ(lp1.parse(), -1);

    lp1.set_line("<>");
    EXPECT_THROW(lp1.parse(), std::invalid_argument);
    //vector<double>
    Parser<std::vector<double>> p2;
    LineParser lp2;
    lp2.add_parser(&p2);

    EXPECT_EQ(lp2.parse(), -1);

    lp2.set_line("<1:1234.1234>");
    EXPECT_EQ(lp2.parse(), 0);
    ASSERT_EQ(p2.get_data().size(), 1);
    EXPECT_DOUBLE_EQ(p2.get_data()[0], 1234.1234);

    lp2.set_line("<2:1234.1,4321.1>");
    EXPECT_EQ(lp2.parse(), 0);
    ASSERT_EQ(p2.get_data().size(), 2);
    EXPECT_DOUBLE_EQ(p2.get_data()[1], 4321.1);

    lp2.set_line("1>");
    EXPECT_EQ(lp2.parse(), -1);

    lp2.set_line("<>");
    EXPECT_THROW(lp2.parse(), std::invalid_argument);
}
//user defined structure
struct St{
    int i;
    float f;
};
//user defined class for parse user structure
class Par{
public:
    St operator()(std::string s){
        St temp;
        std::string::size_type pos = s.find(',');
        if (pos == std::string::npos){
            throw std::invalid_argument("can not find ',' for st");
        }
        temp.i = stoi(s.substr(0, pos));
        temp.f = stof(s.substr(pos+1, s.size()-pos-1));
        return temp;
    }
};
//test one column for user-defined structure
TEST(one_column, user_defined){

    Parser<St, Par> p0;
    LineParser lp;
    lp.add_parser(&p0);

    EXPECT_EQ(lp.parse(), -1);

    lp.set_line("<5,3.1>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_EQ(p0.get_data().i, 5);
    EXPECT_FLOAT_EQ(p0.get_data().f, 3.1);

    lp.set_line("<5.3,3.1>");
    EXPECT_EQ(lp.parse(), 0);
    EXPECT_EQ(p0.get_data().i, 5);
    EXPECT_FLOAT_EQ(p0.get_data().f, 3.1);

    lp.set_line("<1.1>");
    EXPECT_THROW(lp.parse(), std::invalid_argument);
    //user defined structure without second template parameter
    Parser<St> p1;
    LineParser lp1;
    lp1.add_parser(&p1);
    lp1.set_line("<5,3.1>");
    EXPECT_THROW(lp1.parse(), std::invalid_argument);

}
//test more columns with mixed type
TEST(more_columns, mixed){
    LineParser ls;

    Parser<int> p0;
    Parser<float> p1;
    Parser<double> p2;
    Parser<std::string> p3;
    Parser<std::vector<float>> p4;
    Parser<std::vector<int>> p5;
    Parser<St, Par> p6;

    ls.add_parser(&p0);
    ls.add_parser(&p1);
    ls.add_parser(&p2);
    ls.add_parser(&p3);
    ls.add_parser(&p4);
    ls.add_parser(&p5);
    ls.add_parser(&p6);

    EXPECT_EQ(ls.parse(), -1);

    ls.set_line("<11>\t<32.67>\t<23.123456>\t<zhang>\t"
                "<3:1.1,2.2,6.4>\t<2:13,11>\t<12,23.123>");
    EXPECT_EQ(ls.parse(), 0);
    EXPECT_EQ(p0.get_data(), 11);
    EXPECT_FLOAT_EQ(p1.get_data(), 32.67);
    EXPECT_DOUBLE_EQ(p2.get_data(), 23.123456);
    EXPECT_STREQ(p3.get_data().c_str(), "zhang");
    ASSERT_EQ(p4.get_data().size(), 3);
    EXPECT_FLOAT_EQ(p4.get_data()[2], 6.4);
    ASSERT_EQ(p5.get_data().size(), 2);
    EXPECT_EQ(p5.get_data()[1], 11);
    EXPECT_EQ(p6.get_data().i, 12);
    EXPECT_FLOAT_EQ(p6.get_data().f, 23.123);
    
    ls.set_line("<11><32.67>\t<23.123456>\t<zhang>\t"
                "<3:1.1,2.2,6.4>\t<2:13,11>\t<12,23.123>");
    EXPECT_THROW(ls.parse(), std::invalid_argument);
    
    ls.set_line("<11>\t<32.67>\t<23.123456>\t<zhang>\t"
                "<2:13,11>\t<12,23.123>");
    EXPECT_THROW(ls.parse(), std::invalid_argument);
    
    ls.set_line("<11>\t<32.67>\t<23.123456>\t<zhang>\t");
    EXPECT_EQ(ls.parse(), -1);
}

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
