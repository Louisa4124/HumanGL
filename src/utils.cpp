#include <iostream>

namespace utils
{

void err(const char* msg) { std::cerr << msg << std::endl; }

void ouin(const char* msg)
{
    static int count = 0;

    std::cerr << "(" << count << ") OUIN: " << msg << std::endl;
}
}   // namespace utils
