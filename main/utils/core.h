#pragma once

#include <functional>
#include <algorithm>
#include <optional>
#include <vector>
#include <string>
#include <any>

#define _UNUSED(x) (void)(x);
#undef emit

namespace cardian::core {
class _string: public std::string {
public:
    template <typename Type>
    _string(Type val): std::string(val)  {}
    _string(const std::string &str) : std::string(str) {}
    _string() {}

    _string toupper() const { return _string(*this).toupper(); }
    _string tolower() const { return _string(*this).tolower(); }
    _string &toupper() {
        for(char &ch: *this) ch = std::toupper(ch);
        return *this;
    }
    _string &tolower() {
        for(char &ch: *this) ch = std::tolower(ch);
        return *this;
    }

    bool includes(const _string &str) { return find(str) != npos; }
    bool startsWith(const _string &in) const {
        return in.length() < length() && in == substr(0, in.length());
    }

    std::vector<_string> split(const _string &delim) const {
        size_t dsize = delim.size(), pos = 0, start = 0;
        std::vector<_string> strlist{};

        while(pos != npos) {
            pos = find(delim, pos + dsize);
            if(start < pos) strlist.push_back(substr(start, pos - start));
            start = pos + dsize;
        }

        return strlist;
    }

    _string &replace(const _string &old, const _string &_new) {
        size_t pos = find(old);
        if(pos != npos) std::string::replace(pos, old.size(), _new);
        return *this;
    }

    _string &replaceAll(const _string &old, const _string &_new) {
        size_t pos = find(old), os = old.size();

        while(pos != npos) {
            std::string::replace(pos, os, _new);
            pos = find(old, pos + os);
        }

        return *this;
    }

    _string &trim(char ch = ' ') {
        size_t start = 0, end = size() - 1;
        while(at(start) == ch && start < end) start++;
        while(at(end) == ch && start < end) end--;
        *this = std::move(substr(start, end - start));
        return *this;
    }

    _string &arg(int num) { return arg(std::to_string(num)); }
    _string &arg(float num) { return arg(std::to_string(num)); }
    _string &arg(size_t num) { return arg(std::to_string(num)); }
    _string &arg(double num) { return arg(std::to_string(num)); }
    _string &arg(long long num) { return arg(std::to_string(num)); }
    _string &arg(const _string &str) {
        replaceAll("{0}", str);
        for(size_t i{}; i < size() - 3; i++) {
            if(at(i) == '{' && at(i+2) == '}') {
                (*this)[i+1] = at(i+1) - 1;
            }
        }
        return *this;
    }
};

struct _any: public std::any {
    template<typename Type>
    _any(const Type& t): std::any(t) {}

    template<typename Type>
    std::optional<Type> cast() const {
        if(type() == typeid(Type)) {
            return std::any_cast<Type>(*this);
        } else {
            return std::optional<Type>{};
        }
    }

    std::optional<int>    toInt()   const { return cast<int>(); }
    std::optional<bool>   toBool()  const { return cast<bool>(); }
    std::optional<char>   toChar()  const { return cast<char>(); }
    std::optional<long>   toLong()  const { return cast<long>(); }
    std::optional<float>  toFloat() const { return cast<float>(); }
    std::optional<char *> toStr()   const { return cast<char *>(); }
    std::optional<double> toDouble() const { return cast<double>(); }
    std::optional<_string> toStdStr() const { return cast<_string>(); }
};
}
