/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include "c_string.hpp"

#include <cstdlib>
#include <cstring>

using namespace cmpl;

void CString::init_from_c_str(const char * c_str, size_t length) {
    size_t required_capacity = length + 1;
    reserve(required_capacity);
    if (_capacity >= required_capacity) {
        std::strcpy(_data, c_str);
        _length = length;
    }
}

CString::CString(const CString& c_string) {
    _data = static_cast<char *>(std::malloc(c_string._capacity));
    if (_data == nullptr) {
        _length = 0;
        _capacity = 0;
    }
    else {
        std::strcpy(_data, c_string._data);
        _length = c_string._length;
        _capacity = c_string._capacity;
    }
}

CString::CString(CString&& c_string) {
    _data = c_string._data;
    c_string._data = nullptr;
    _length = c_string._length;
    c_string._length = 0;
    _capacity = c_string._capacity;
    c_string._capacity = 0;
}

CString::CString(char const * c_str) {
    init_from_c_str(c_str, std::strlen(c_str));
}

CString::CString(char const * c_str, size_t length) {
    init_from_c_str(c_str, length);
}

CString::~CString() {
    std::free(_data);
    _data = nullptr;
    _length = 0;
    _capacity = 0;
}

bool CString::starts_with(const char * c_str) const {
    size_t c_str_len = std::strlen(c_str);
    if (c_str_len == 0) return true;
    if (_length < c_str_len) return false;
    for (size_t index = 0; index < c_str_len; ++index) {
        if (_data[index] != c_str[index]) return false;
    }
    return true;
}

bool CString::ends_with(const char * c_str) const {
    size_t c_str_len = std::strlen(c_str);
    if (c_str_len == 0) return true;
    if (_length < c_str_len) return false;
    size_t data_index = _length - 1;
    size_t c_str_index = c_str_len - 1;
    while (true) {
        if (_data[data_index] != c_str[c_str_index]) return false;
        if (c_str_index == 0) return true;
        --data_index;
        --c_str_index;
    }
}

CString& CString::remove_ending(const char * c_str) {
    /* TODO: better implementation */
    if (ends_with(c_str)) {
        size_t c_str_len = std::strlen(c_str);
        size_t new_length = _length - c_str_len;
        _data[new_length] = '\0';
        _length = new_length;
    }
    return *this;
}

CString& CString::append(const char * c_str) {
    size_t c_str_len = std::strlen(c_str);
    size_t required_capacity = _length + c_str_len + 1;
    reserve(required_capacity);
    if (_capacity >= required_capacity) {
        std::strcat(_data, c_str);
        _length += c_str_len;
    }
    return *this;
}

CString& CString::append(const StringRef& string_ref) {
    size_t required_capacity = _length + string_ref.length() + 1;
    reserve(required_capacity);
    if (_capacity >= required_capacity) {
        std::strncat(_data, string_ref.data(), string_ref.length());
        _length += string_ref.length();
    }
    return *this;
}

void CString::reserve(size_t capacity) {
    if (capacity > _capacity) {
        char * realloc = static_cast<char *>(std::realloc(_data, capacity));
        if (realloc != nullptr) {
            _data = realloc;
            _capacity = capacity;
        }
    }
}

void CString::recalculate_length() {
    bool found_null = false;
    for (size_t index = 0; index < _capacity; ++index) {
        if (_data[index] == '\0') {
            _length = index;
            found_null = true;
            break;
        }
    }
    if (!found_null) {
        size_t last_index = _capacity - 1;
        _data[last_index] = '\0';
        _length = last_index;
    }   
}

bool cmpl::operator==(const CString& c_string, const char * c_str) {
    return std::strcmp(c_string.c_str(), c_str) == 0;
}
