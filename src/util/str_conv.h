#include <string>

// Replace specified char (ch1) from the given string (str) to new char (ch2)
std::string replace_char(std::string str, const std::string ch1, const std::string ch2) {

    size_t start_pos = 0;
    while((start_pos = str.find(ch1, start_pos)) != std::string::npos) {
        str.replace(start_pos, 1, ch2);
        start_pos += ch2.length();
    }
    return str;
}