#include <iostream>
#include "MyString.h"

int main() {
    MyString s1("Hello");
    std::cout << "s1 is " << s1 << std::endl;

    MyString s2(s1);
    std::cout << "s2 is " << s2 << std::endl;

    auto s3 = s1;
    std::cout << "s3 is " << s3 << std::endl;

    auto s4 = std::move(s1);
    std::cout << "s4 is " << s4 << std::endl;

    auto s5 = s1 + s2;
    std::cout << "s5 is " << s5 << std::endl;

    return 0;
}