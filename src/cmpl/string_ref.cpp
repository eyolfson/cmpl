/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include "string_ref.hpp"

#include <cstring>

using namespace cmpl;

StringRef::StringRef(const char * c_str) : _data(c_str) {
    size_t c_str_len = std::strlen(c_str);
    _length = c_str_len;
}

bool cmpl::operator==(const StringRef& lhs, const StringRef& rhs) {
    if (lhs._length != rhs._length) return false;
    for (size_t i = 0; i < lhs._length; ++i) {
        if (lhs._data[i] != rhs._data[i]) return false;
    }
    return true;
}

bool cmpl::operator==(const StringRef& string_ref, const char * c_str) {
    for (size_t i = 0; i < string_ref._length; ++i) {
        if (c_str[i] == '\0') return false;
        if (string_ref._data[i] != c_str[i]) return false;
    }
    return true;
}
