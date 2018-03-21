// Copyright 2017 Baidu Inc. All Rights Reserved.
// Author: Fucheng zhang (zhangfucheng@baidu.com)
//
// call gtest to test parser

#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

#include "parser.h"

namespace test {

using baidu::Parser;
using baidu::LineParser;
using baidu::DictParser;

// test LineParser, for only int
TEST(LineParser, int) {
    Parser<int> p0;
    LineParser lp;
    lp.add_parser(&p0);

    EXPECT_EQ(lp.parse("1"), 0);
    EXPECT_EQ(p0.data(), 1);

    EXPECT_EQ(lp.parse("-1"), 0);
    EXPECT_EQ(p0.data(), -1);

    EXPECT_EQ(lp.parse("0"), 0);
    EXPECT_EQ(p0.data(), 0);

    EXPECT_EQ(lp.parse("1.2"), 0);
    EXPECT_EQ(p0.data(), 1);

    EXPECT_EQ(lp.parse("abc"), -1);

    EXPECT_EQ(lp.parse(""), -1);
}

// test LineParser, for only float
TEST(LineParser, float) {
    Parser<float> p0;
    LineParser lp;
    lp.add_parser(&p0);

    EXPECT_EQ(lp.parse("1.1"), 0);
    EXPECT_FLOAT_EQ(p0.data(), 1.1);

    EXPECT_EQ(lp.parse("-1.1"), 0);
    EXPECT_FLOAT_EQ(p0.data(), -1.1);

    EXPECT_EQ(lp.parse("0.0"), 0);
    EXPECT_FLOAT_EQ(p0.data(), 0);

    EXPECT_EQ(lp.parse("abc"), -1);

    EXPECT_EQ(lp.parse(""), -1);
}

//test LineParser, for only double
TEST(LineParser, double) {
    Parser<double> p0;
    LineParser lp;
    lp.add_parser(&p0);

    EXPECT_EQ(lp.parse("1.1234567"), 0);
    EXPECT_DOUBLE_EQ(p0.data(), 1.1234567);

    EXPECT_EQ(lp.parse("-1.1234567"), 0);
    EXPECT_DOUBLE_EQ(p0.data(), -1.1234567);

    EXPECT_EQ(lp.parse("0.0"), 0);
    EXPECT_DOUBLE_EQ(p0.data(), 0);

    EXPECT_EQ(lp.parse("11"), 0);
    EXPECT_DOUBLE_EQ(p0.data(), 11);

    EXPECT_EQ(lp.parse("abc"), -1);

    EXPECT_EQ(lp.parse(""), -1);
}

//test LineParser, for only string
TEST(LineParser, string) {
    Parser<std::string> p0;
    LineParser lp;
    lp.add_parser(&p0);

    EXPECT_EQ(lp.parse("abc"), 0);
    EXPECT_STREQ(p0.data().c_str(), "abc");

    EXPECT_EQ(lp.parse("<>"), 0);
    EXPECT_STREQ(p0.data().c_str(), "<>");
}

//test LineParser, for vector
TEST(LineParser, vector) {

    //vector<int>
    Parser<std::vector<int>> p0;
    LineParser lp0;
    lp0.add_parser(&p0);

    EXPECT_EQ(lp0.parse("1:1234"), 0);
    ASSERT_EQ(p0.data().size(), 1);
    EXPECT_EQ(p0.data()[0], 1234);

    EXPECT_EQ(lp0.parse("2:1234,4321"), 0);
    ASSERT_EQ(p0.data().size(), 2);
    EXPECT_EQ(p0.data()[1], 4321);

    EXPECT_EQ(lp0.parse("1"), -1);

    EXPECT_EQ(lp0.parse(""), -1);

    //vector<float>
    Parser<std::vector<float>> p1;
    LineParser lp1;
    lp1.add_parser(&p1);

    EXPECT_EQ(lp1.parse("1:1234.1234"), 0);
    ASSERT_EQ(p1.data().size(), 1);
    EXPECT_FLOAT_EQ(p1.data()[0], 1234.1234);

    EXPECT_EQ(lp1.parse("2:1234.1,4321.1"), 0);
    ASSERT_EQ(p1.data().size(), 2);
    EXPECT_FLOAT_EQ(p1.data()[1], 4321.1);

    EXPECT_EQ(lp1.parse("1"), -1);

    EXPECT_EQ(lp1.parse(""), -1);

    //vector<double>
    Parser<std::vector<double>> p2;
    LineParser lp2;
    lp2.add_parser(&p2);

    EXPECT_EQ(lp2.parse("1:1234.1234"), 0);
    ASSERT_EQ(p2.data().size(), 1);
    EXPECT_DOUBLE_EQ(p2.data()[0], 1234.1234);

    EXPECT_EQ(lp2.parse("2:1234.1,4321.1"), 0);
    ASSERT_EQ(p2.data().size(), 2);
    EXPECT_DOUBLE_EQ(p2.data()[1], 4321.1);

    EXPECT_EQ(lp2.parse(""), -1);
}

//user defined structure
struct St {
    int i;
    float f;
};

//user defined class for parse user structure
class Par {
public:
    St operator()(std::string s) {
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

//test LineParser for user-defined structure
TEST(LineParser, user_defined) {
    Parser<St, Par> p0;
    LineParser lp;
    lp.add_parser(&p0);

    EXPECT_EQ(lp.parse("5,3.1"), 0);
    EXPECT_EQ(p0.data().i, 5);
    EXPECT_FLOAT_EQ(p0.data().f, 3.1);

    EXPECT_EQ(lp.parse("5.3,3.1"), 0);
    EXPECT_EQ(p0.data().i, 5);
    EXPECT_FLOAT_EQ(p0.data().f, 3.1);

    EXPECT_EQ(lp.parse("1.1"), -1);

    //user defined structure without second template parameter
    Parser<St> p1;
    LineParser lp1;
    lp1.add_parser(&p1);
    EXPECT_EQ(lp1.parse("5,3.1"), -1);

}

//test LineParser with mixed type
TEST(LineParser, mixed) {
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

    EXPECT_EQ(ls.parse("11\t32.67\t23.123456\tzhang\t"
                "3:1.1,2.2,6.4\t2:13,11\t12,23.123"), 0);
    EXPECT_EQ(p0.data(), 11);
    EXPECT_FLOAT_EQ(p1.data(), 32.67);
    EXPECT_DOUBLE_EQ(p2.data(), 23.123456);
    EXPECT_STREQ(p3.data().c_str(), "zhang");
    ASSERT_EQ(p4.data().size(), 3);
    EXPECT_FLOAT_EQ(p4.data()[2], 6.4);
    ASSERT_EQ(p5.data().size(), 2);
    EXPECT_EQ(p5.data()[1], 11);
    EXPECT_EQ(p6.data().i, 12);
    EXPECT_FLOAT_EQ(p6.data().f, 23.123);

    //invalid line
    EXPECT_EQ(ls.parse("11\t32.67\t23.123456\tzhang\t"
                "2:13,11\t12,23.123"), -1);

    EXPECT_EQ(ls.parse("11\t32.67\t23.123456\tzhang\t"), -1);

    EXPECT_EQ(ls.parse("abcd\tadf"), -1);
}

//test DictParser
TEST(DictParser, file) {
    Parser<int> p0;
    Parser<float> p1;
    Parser<double> p2;
    Parser<std::string> p3;
    Parser<std::vector<float>> p4;
    Parser<std::vector<int>> p5;
    Parser<St, Par> p6;
    //file not exist
    DictParser dp("no.txt");
    dp.add_column(&p0);
    dp.add_column(&p1);
    dp.add_column(&p2);
    dp.add_column(&p3);
    dp.add_column(&p4);
    dp.add_column(&p5);
    dp.add_column(&p6);
    EXPECT_TRUE(dp.is_file_end());

    //file exist and have only one line
    dp.reset_file("oneline.txt");
    EXPECT_FALSE(dp.is_file_end());
    EXPECT_EQ(dp.parse_next_line(), 0);
    EXPECT_EQ(p0.data(), 11);
    EXPECT_FLOAT_EQ(p1.data(), 32.67);
    EXPECT_DOUBLE_EQ(p2.data(), 23.123456);
    EXPECT_STREQ(p3.data().c_str(), "zhang");
    ASSERT_EQ(p4.data().size(), 3);
    EXPECT_FLOAT_EQ(p4.data()[2], 6.4);
    ASSERT_EQ(p5.data().size(), 2);
    EXPECT_EQ(p5.data()[1], 11);
    EXPECT_EQ(p6.data().i, 12);
    EXPECT_FLOAT_EQ(p6.data().f, 23.123);
    EXPECT_TRUE(dp.is_file_end());

    //more line
    dp.reset_file("moreline.txt");
    EXPECT_FALSE(dp.is_file_end());
    EXPECT_EQ(dp.parse_next_line(), -1);
    EXPECT_FALSE(dp.is_file_end());
    EXPECT_EQ(dp.parse_next_line(), 0);
    EXPECT_EQ(p0.data(), 11);
    EXPECT_FLOAT_EQ(p1.data(), 32.67);
    EXPECT_DOUBLE_EQ(p2.data(), 23.123456);
    EXPECT_STREQ(p3.data().c_str(), "zhang");
    ASSERT_EQ(p4.data().size(), 3);
    EXPECT_FLOAT_EQ(p4.data()[2], 6.4);
    ASSERT_EQ(p5.data().size(), 2);
    EXPECT_EQ(p5.data()[1], 11);
    EXPECT_EQ(p6.data().i, 12);
    EXPECT_FLOAT_EQ(p6.data().f, 23.123);
    EXPECT_TRUE(dp.is_file_end());
}

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
