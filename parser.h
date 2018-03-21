// Copyright 2017 Baidu Inc. All Rights Reserved.
// Author: Fucheng zhang (zhangfucheng@baidu.com)
//
// define four important class: Parse Parser LineParser DictParser

#ifndef GOODCODER_PARSER_H
#define GOODCODER_PARSER_H

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <fstream>

#include <com_log.h>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName&); \
        TypeName& operator=(const TypeName&)

namespace baidu {

/**
 * Parse is a function-like template class just like std::unordered_map::hash<Key>
 * it's operator() is used to parse a string column
 * for using it, we should specilize it for specific type
 * otherwise, it will throw std::exception
 */
template <typename T>
class Parse {
public:
    T operator()(const std::string& /*str*/) const throw(std::exception) {
        throw std::invalid_argument("Parse is not implement for this type");
        T t;
        return t;
    }
};

/**
 * specilize Parse for 'int'
 * may throw std::exception when called std::stoi
 */
template <>
class Parse<int> {
public:
    int operator()(const std::string& s) const throw(std::exception) {
        return std::stoi(s);
    }
};

/**
 * specilize Parse for 'float'
 * may throw std::exception when called std::stof
 */
template <>
class Parse<float> {
public:
    float operator()(const std::string& s) const throw(std::exception) {
        return std::stof(s);
    }
};

/**
 * specilize Parse for 'double'
 * may throw std::exception when called stod
 */
template <>
class Parse<double> {
public:
    double operator()(const std::string& s) const throw(std::exception) {
        return std::stod(s);
    }
};

/**
 * specilize Parse for 'std::string'
 */
template <>
class Parse<std::string> {
public:
    std::string operator()(const std::string& s) const {
        return s;
    }
};

/**
 * specilize Parse for 'std::vector<T>'
 */
template <typename T>
class Parse<std::vector<T>> {
public:
    /**
     * @brief template member function for different element type
     *        will be called by Parser::parse
     * @param [in] std::string s
     * @return std::vector<T>
     * @retval the consequence of this call
     * @see
     * @author zhangfucheng
     * @date 2017.11.7
    **/
    template<typename pars = Parse<T>>
    std::vector<T> operator()(const std::string& s) const throw(std::exception) {
        std::vector<T> t;
        std::string::size_type pos = s.find(':');
        if (pos == std::string::npos) {
            throw std::invalid_argument("can not find ':' for vector");
        }
        int num = std::stoi(s.substr(0, pos));
        std::stringstream ss(s.substr(pos + 1, s.size() - pos - 1));
        std::string sub_str;
        for (int i = 0; i < num; i++) {
            if (ss.eof()) {
                throw std::invalid_argument("parse vector");
            }
            getline(ss,sub_str,',');
            t.push_back(pars()(sub_str));
        }
        if (!ss.eof()) {
            throw std::invalid_argument("parse vector");
        }
        return t;
    }
};

/**
 * ParserBase is the base class for different Parser
 * it defined virtual parse function, which will be called by LineParse
 */
class ParserBase {
public:
    virtual int parse(const std::string& str) = 0;
    virtual ~ParserBase() {};
};

/**
 * Parser is template class inherited from ParserBase used to hold the parse consequence
 * it should be used just like std::unordered_map
 * first template parameter is the type of this column
 * the second template parameter 'pars' is used to parse a string
 * 'pars' default is Parse<T> for built-in types
 * user should specific 'pars' for user defined type T
 */
template <typename T, typename pars = Parse<T> >
class Parser : public ParserBase  {
public:
    Parser() {};
    /**
     * @brief call pars()(str) to parse and set the consequence to data
     *        whill be called by LineParser::parse
     * @param [in] std::string str
     * @return void
     * @retval
     * @exception throw std::exception when called pars()(str)
     * @author zhangfucheng
     * @date 2017.11.7
    **/
    virtual int parse(const std::string& str) override {
        try {
            _data = pars()(str);
        } catch (std::exception& e) {
            CNOTICE_LOG("parse error:%s", e.what());
            return -1;
        }
        return 0;
    }

    T& data() {
        return _data;
    }
private:
    T _data;
    DISALLOW_COPY_AND_ASSIGN(Parser);
};

/**
 *LineParser is used to parse a line
 */
class LineParser {
public:
    LineParser() {};
    /**
     * @brief add Parser to parse a column
     * @param [in] ParserBase* p, should be a pointer to a Parser<> object
     * @return void
     * @retval
     * @see
     * @author zhangfucheng
     * @date 2017.11.7
    **/
    void add_parser(ParserBase* p) {
        _v.push_back(p);
    }

    /**
     * @brief start parse a line
     * @param std::string line
     * @return int
     * @retval 0:succeed to parse a line, -1:line parse error
     * @author zhangfucheng
     * @date 2017.11.7
    **/
    int parse(std::string line) const;
private:
    //should not be shared_ptr, because the element it point to may in stack
    std::vector<ParserBase*> _v;
    DISALLOW_COPY_AND_ASSIGN(LineParser);
};
/**
 * DictParser is used to parse a file
 * it parse a line every time
 */
class DictParser {
public:
    DictParser(std::string path) : _fs(path) {};
    ~DictParser() {
        _fs.close();
    }

    /**
     * @brief add column
     * @param ParserBase* p, should be a pointer to a Parser<> object
     * @return void
     * @retval
     * @author zhangfucheng
     * @date 2017.11.7
    **/
    void add_column(ParserBase* p) {
        _lp.add_parser(p);
    }

    /**
     * @brief parse next line
     * @param
     * @return int
     * @retval 0:succeed to parse a line, -1:line parse error
     * @author zhangfucheng
     * @date 2017.11.7
    **/
    int parse_next_line() {
        std::string line;
        std::getline(_fs, line);
        return _lp.parse(line);
    }

    /**
     * @brief judge the file end
     * @param
     * @return bool
     * @retval true : file ended or read error, false : not reach the end
     * @author zhangfucheng
     * @date 2017.11.7
    **/
    bool is_file_end() {
        if (!_fs.good()) {
            return true;
        } else {
            return false;
        }
    }
    /**
     * @brief reset file
     * @param std::string path
     * @return void
     * @retval
     * @author zhangfucheng
     * @date 2017.11.7
    **/
    void reset_file(std::string path) {
        _fs.close();
        _fs.open(path);
    }
private:
    std::fstream _fs;
    LineParser _lp;
    DISALLOW_COPY_AND_ASSIGN(DictParser);
};

}
#endif // GOODCODER_PARSER_H
