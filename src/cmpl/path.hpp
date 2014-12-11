/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#ifndef __CMPL_PATH_HPP__
#define __CMPL_PATH_HPP__

#include "c_string.hpp"

#include <utility>

namespace cmpl {

class Path {
    CString c_string;
public:   
    Path() {}
    Path(const Path& path) : c_string(path.c_string) {}
    Path(Path && path) : c_string(std::move(path.c_string)) {}
    ~Path() {}
    bool is_valid() const { return c_string.is_valid(); }
    bool is_invalid() const { return !is_valid(); }
    bool is_root() const;

    Path& append(const char * c_str);
    Path append(const char * c_str) const;
    Path& dirname();
    Path dirname() const;
    bool make() const;
    bool is_extension(const char * c_str) const;
    Path& remove_ending(const char * c_str) {
        c_string.remove_ending(c_str);
        return *this;
    }

    auto length() const { return c_string.length(); }
    const char * c_str() const { return c_string.c_str(); }

    static Path find_file(const char * filename);
private:
};

}

#endif
