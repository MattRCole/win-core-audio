#pragma once
#include "json.hpp"
#include <string>
#include <sstream>

#define getString(json)       \
    std::stringstream __ss__; \
    __ss__ << json;           \
    return __ss__.str()

namespace conversion
{
   class string
   {
      std::string data;

      std::string getSkinny(std::wstring _data)
      {
         return std::string(_data.begin(), _data.end());
      }
      std::wstring getFat() const
      {
         return std::wstring(data.begin(), data.end());
      }

      bool isEqual(const std::string &other)
      {
         return data == other;
      }

      bool isEqual(const std::wstring &other)
      {
         return getFat() == other;
      }

      bool isGreater(const std::string &rhs)
      {
         return data > rhs;
      }
      bool isGreater(const std::wstring &rhs)
      {
         return getFat() > rhs;
      }

   public:
      //default constructor:
      string() : data("") {}
      string(const std::string &_data) : data(_data) {}
      string &operator=(const std::string _data)
      {
         data = _data;
         return *this;
      }
      operator std::string() const { return data; }
      string &operator=(const char c_str[])
      {
         data = c_str;
         return *this;
      }

      string(const std::wstring &_data) : data(getSkinny(_data)) {}
      string &operator=(const std::wstring _data)
      {
         data = getSkinny(_data);
         return *this;
      }
      operator std::wstring() const { return getFat(); }
      string &operator=(const wchar_t * wc_str)
      {
         data = getSkinny(wc_str);
         return *this;
      }

      //conparison operator overloading
      bool operator==(const std::string &other)
      {
         return isEqual(other);
      }
      bool operator==(const std::wstring &other)
      {
         return isEqual(other);
      }
      bool operator!=(const std::string &other)
      {
         return !isEqual(other);
      }
      bool operator!=(const std::wstring &other)
      {
         return !isEqual(other);
      }
      bool operator>(const std::string &rhs)
      {
         return isGreater(rhs);
      }
      bool operator>(const std::wstring &rhs)
      {
         return isGreater(rhs);
      }
      bool operator<(const std::string &rhs)
      {
         return !isGreater(rhs) && !isEqual(rhs);
      }
      bool operator<(const std::wstring &rhs)
      {
         return !isGreater(rhs) && !isEqual(rhs);
      }
      bool operator>=(const std::string &rhs)
      {
         return isGreater(rhs) || isEqual(rhs);
      }
      bool operator>=(const std::wstring &rhs)
      {
         return isGreater(rhs) || isEqual(rhs);
      }
      bool operator<=(const std::string &rhs)
      {
         return !isGreater(rhs);
      }
      bool operator<=(const std::wstring &rhs)
      {
         return !isGreater(rhs);
      }
   };
} // namespace conversion