/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include "file_desc.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace cmpl;

FileDesc::FileDesc(const Path& path) {
    _fd = open(path.c_str(), O_RDONLY);
}

FileDesc::~FileDesc() {
    if (is_valid()) {
        close(_fd);
    }
}

size_t FileDesc::size() {
    struct stat sb;
    if (fstat(_fd, &sb) == -1) return 0;
    return sb.st_size;
}
