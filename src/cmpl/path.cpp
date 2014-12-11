/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include "path.hpp"

#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

#include <utility>

#include "file_desc.hpp"

using namespace cmpl;

bool Path::is_root() const {
    return c_string == "/";
}

Path& Path::append(const char * c_str) {
    c_string.append("/");
    c_string.append(c_str);
    return *this;
}

Path Path::append(const char * c_str) const {
    Path path(*this);
    path.append(c_str);
    return path;
}

Path& Path::dirname() {
    ::dirname(c_string.mut_c_str());
    c_string.recalculate_length();
    return *this;
}

Path Path::dirname() const {
    Path path(*this);
    path.dirname();
    return path;
}

bool Path::make() const {
    if (mkdir(c_string.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
        if (errno == EEXIST) {
            return true;
        }
        else {
            return false;
        }
    }
    return true;
}

bool Path::is_extension(char const * c_str) const {
    return c_string.ends_with(c_str);
}

Path Path::find_file(const char * filename)
{
    Path path;
    path.c_string.reserve(128);
    if (getcwd(path.c_string.mut_c_str(),
               path.c_string.capacity()) == nullptr) {
        return path;
    }
    path.c_string.recalculate_length();

    path.append(filename);
    FileDesc fd(path);
    while (fd.is_invalid()) {
        path.dirname();
        path.dirname();
        if (path.is_root()) {
            return Path();
        }
        path.append(filename);
        fd = FileDesc(path);
    }

    return std::move(path);
}
