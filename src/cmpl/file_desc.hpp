/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#ifndef __CMPL_FILE_DESC_HPP__
#define __CMPL_FILE_DESC_HPP__

#include "path.hpp"

namespace cmpl {

class FileDesc {
    int _fd;
public:
    explicit FileDesc(const Path& path);
    ~FileDesc();
    bool is_valid() { return _fd >= 0; }
    bool is_invalid() { return !is_valid(); }
    int fd() { return _fd; }
    size_t size();
};

}

#endif
