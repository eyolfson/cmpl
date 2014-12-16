/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#ifndef __CMPL_STRING_REF_HPP__
#define __CMPL_STRING_REF_HPP__

#include <cstddef>

namespace cmpl {

class StringRef {
    const char * _data;
    size_t _length;
public:
    StringRef() : _data(nullptr), _length(0) {}
    StringRef(const char * c_str);
    StringRef(const char * c_str, size_t length)
        : _data(c_str), _length(length) {}
    template<size_t L> StringRef(const char (& static_c_str) [L])
        : _data(static_c_str), _length(L - 1) {}

    const char * data() const { return _data; }
    size_t length() const { return _length; }

    friend bool operator==(const StringRef& lhs, const StringRef& rhs);
    friend bool operator==(const StringRef& string_ref, const char * c_str);
};

}

#endif
