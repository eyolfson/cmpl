/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include "string_ref.hpp"

bool cmpl::operator==(const StringRef& string_ref, const char * c_str) {
    for (size_t i = 0; i < string_ref._length; ++i) {
        if (c_str[i] == '\0') return false;
        if (string_ref._data[i] != c_str[i]) return false;
    }
    return true;
}
