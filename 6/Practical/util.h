#ifndef PRACTICAL_UTIL_H
#define PRACTICAL_UTIL_H

#endif //PRACTICAL_UTIL_H

#include <algorithm>
#include <string>
#include <vector>

static inline std::string ltrim(std::string s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

static inline std::string rtrim(std::string s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

static inline std::string trim(std::string s) {
    return ltrim(rtrim(s));
}

std::vector<std::string> split_string(std::string s, const std::string &delimiter) {
    std::vector<std::string> result;
    size_t pos;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        result.push_back(trim(token));
        s.erase(0, pos + delimiter.length());
    }
    result.push_back(trim(s));
}