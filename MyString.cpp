#include "MyString.h"

MyString::MyString(const char *str) {
    if (str == nullptr) {
        _data = nullptr;
        return;
    }

    _data = new char[strlen(str) + 1];
    strcpy(_data, str);
    _data[strlen(str)] = '\0';
}

MyString::MyString(const MyString &other) {
    if (other._data == nullptr) {
        _data = nullptr;
        return;
    }

    _data = new char[strlen(other._data) + 1];
    strcpy(_data, other._data);
    _data[strlen(other._data)] = '\0';
}

MyString &MyString::operator = (const MyString &other) {
    if (this == &other) return *this;
    if (other._data == nullptr) {
        _data = nullptr;
        return *this;
    }

    delete[] _data;
    _data = new char[strlen(other._data) + 1];
    strcpy(_data, other._data);
    _data[strlen(other._data)] = '\0';
    return *this;
}

MyString::MyString(MyString &&other) {
    if (other._data == nullptr) {
        _data = nullptr;
        return;
    }

    _data = new char[strlen(other._data) + 1];
    strcpy(_data, other._data);
    _data[strlen(other._data)] = '\0';
}

MyString &MyString::operator = (MyString &&other) {
    if (this == &other) return *this;
    if (other._data == nullptr) {
        _data = nullptr;
        return *this;
    }

    delete[] _data;
    _data = new char[strlen(other._data) + 1];
    strcpy(_data, other._data);
    _data[strlen(other._data)] = '\0';
    return *this;
}

MyString &MyString::operator + (const MyString &other) {
    if (other._data == nullptr) {
        return *this;
    }

    char *temp = new char[strlen(_data) + strlen(other._data) + 1];
    strcpy(temp, _data);
    strcat(temp, other._data);
    delete[] _data;
    _data = temp;
    return *this;
}

bool MyString::operator == (const MyString &other) {
    if (other._data == nullptr) {
        return _data == nullptr;
    }

    return strcmp(_data, other._data) == 0;
}

std::ostream &operator << (std::ostream &out, const MyString &other) {
    out << other._data;
    return out;
}

MyString::~MyString() {
    if (_data == nullptr) return;
    delete[] _data;
    _data = nullptr;
}