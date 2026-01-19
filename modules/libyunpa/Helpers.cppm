module;
#include <string>
export module libyunpa:Helpers;

namespace libyunpa {
  int ConvertAndTrim(std::string& input, char delim = ';') {
    auto count  = input.find(delim);
    auto result = std::stoi(input.substr(0, count));
    input       = input.substr(count + 1);
    return result;
  }
} // namespace libyunpa
