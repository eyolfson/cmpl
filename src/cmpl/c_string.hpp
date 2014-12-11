/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#ifndef __CMPL_C_STRING_HPP__
#define __CMPL_C_STRING_HPP__

#include <cstddef>

namespace cmpl {

class CString {
    char * _data;
    size_t _length;
    size_t _capacity;
public:
    CString() : _data(nullptr), _length(0), _capacity(0) {}
    CString(const CString& c_string);
    CString(CString&& c_string);
    explicit CString(char const * c_str);
    ~CString();

    bool is_invalid() const { return _length == 0; } 
    bool is_valid() const { return !is_invalid(); }
    char * mut_c_str() { return _data; }
    const char * c_str() const { return _data; }
    size_t length() const { return _length; }
    size_t capacity() const { return _capacity; }

    bool starts_with(const char * c_str) const;
    bool ends_with(const char * c_str) const;
    CString& remove_ending(const char * c_str);
    CString& append(const char * c_str);
    void reserve(size_t capacity);
    void recalculate_length();
};
    
bool operator==(const CString& c_string, const char * c_str);

}

#endif
