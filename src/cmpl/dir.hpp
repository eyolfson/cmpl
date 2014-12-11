/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#ifndef __CMPL_DIR_HPP__
#define __CMPL_DIR_HPP__

#include "c_string.hpp"
#include "path.hpp"

#include <dirent.h>

#include <vector>

namespace cmpl {

class Dir {
    DIR * _data;
    const Path& _path;
public:
    Dir(const Path& path);
    ~Dir();
    // Dir(Dir & other) = delete;
    // Dir(Dir && other) {
    //     data = other.data;
    //     other.data = nullptr;
    // }
    bool is_valid() const { return _data != nullptr; }
    bool is_invalid() const { return !is_valid(); }
    void read() const;
    
    std::vector<Path> compile_files(const Path& project_path,
                                    const Path& target_path) const;
    std::vector<CString> read_dir_c_strings() const;
};

}

#endif
