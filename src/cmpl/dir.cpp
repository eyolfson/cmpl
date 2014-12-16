/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include "dir.hpp"

#include <cstdio>
#include <cstring>

#include "clang.hpp"

using namespace cmpl;

Dir::Dir(Path const & path) : _path(path) {
    _data = opendir(_path.c_str());
}

Dir::~Dir() {
    /* Ignore any errors if they occur */
    closedir(_data);
    _data = nullptr;
}

void Dir::read() const {
    if (is_invalid()) return;

    struct dirent * entry;
    while (true) {
        entry = readdir(_data);
        if (entry == nullptr) break;
        if (entry->d_name[0] == '.') continue;
    }    
}

std::vector<Path> Dir::compile_files(
    const Path& project_path,
    const Path& target_path,
    const std::vector<CString>& cxx_flags) const {
    std::vector<Path> vector;

    if (is_invalid()) return std::move(vector);

    struct dirent * entry;
    while (true) {
        entry = readdir(_data);
        if (entry == nullptr) break;
        if (entry->d_name[0] == '.') continue;
        /* TODO: ensure they're actually files */
        if (entry->d_type == DT_REG) {
            CString name(entry->d_name);
            if (name.ends_with(".cpp")) {
                Path input = _path.append(name.c_str());
                name.remove_ending(".cpp");
                name.append(".o");
                Path output = target_path.append(name.c_str());
                /* Compile */
                printf("\e[34;1m[Compile]\e[0m \e[34m%s\e[0m\n",
                       output.c_str() + (project_path.length() + 1));
                clang_compile(input, output, cxx_flags);
                vector.push_back(std::move(output));
            }
        }
        else if (entry->d_type == DT_DIR) {
            printf("TODO: Directory nested in target directory");
        }
    }

    return std::move(vector);
}

std::vector<CString> Dir::read_dir_c_strings() const {
    std::vector<CString> vector;

    struct dirent * entry = nullptr;
    while (true) {
        entry = readdir(_data);
        if (entry == nullptr) break;
        if (entry->d_name[0] == '.') continue;
        /* TODO: ensure they're actually directories */
        if (entry->d_type == DT_DIR) {
            vector.push_back(CString(entry->d_name));
        }
    }

    return std::move(vector);
}
