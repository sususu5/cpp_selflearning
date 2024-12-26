#ifndef MYSTRING_H
#define MYSTRING_H

#include <iostream>
#include <cstring>

class MyString {
public:
    MyString(): _data(nullptr) {}
    MyString(const char *str);
    MyString(const MyString &other);
    MyString &operator = (const MyString &other);

    MyString(MyString &&other);
    MyString &operator = (MyString &&other);

    MyString &operator + (const MyString &other);
    bool operator == (const MyString &other);
    // Not a part of this class
    friend std::ostream &operator << (std::ostream &out, const MyString &other);

    ~MyString();

private:
    char *_data;
};

#endif