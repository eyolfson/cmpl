/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include "parser.hpp"

#include "file_desc.hpp"
#include "memory_map.hpp"

using namespace cmpl;

#include <cstdio>

static size_t expect(StringRef&& string_ref, const char * data, size_t remaining) {
    const size_t string_ref_length = string_ref.length();
    if (remaining < string_ref_length) {
        return 0;
    }
    if (StringRef{data, string_ref_length} == string_ref) {
        return string_ref_length;
    }
    return 0;
}

static size_t until_newline(const char * data, size_t remaining) {

    for (size_t i = 0; i < remaining; ++i) {
        if (data[i] == '\n') return i;
    }
    return 0;
}

Parser::Parser(const Path& path) {
    FileDesc fd{ path };
    MemoryMap memory_map(fd);
    if (memory_map.is_invalid()) return;

    const char *const data_begin{ memory_map.data<const char>() };
    const size_t length{ memory_map.length() };

    const char * data = data_begin;
    size_t offset{ 0 };

    size_t matched = expect("driver ", data, length - offset);
    if (matched == 0) {
        return;
    }
    offset += matched;
    data = &data_begin[offset];

    {
        matched = until_newline(data, length - offset);
        if (matched == 0) {
            return;
        }
        CString driver{data, matched};
        _cxx_flags.push_back(driver);
        _linker_flags.push_back(driver);
    }
    offset += matched;
    data = &data_begin[offset];

    matched = expect("\nstandard ", data, length - offset);
    if (matched == 0) {
        return;
    }
    offset += matched;
    data = &data_begin[offset];

    {
        matched = until_newline(data, length - offset);
        if (matched == 0) {
            return;
        }
        CString standard{"-std="};
        standard.append({data, matched});
        _cxx_flags.push_back(std::move(standard));
    }
    offset += matched;
    data = &data_begin[offset];

    matched = expect("\noptimization-level ", data, length - offset);
    if (matched == 0) {
        return;
    }
    offset += matched;
    data = &data_begin[offset];

    {
        matched = until_newline(data, length - offset);
        if (matched == 0) {
            return;
        }
        CString optimization_level{"-O"};
        optimization_level.append({data, matched});
        _cxx_flags.push_back(std::move(optimization_level));
    }
    offset += matched;
    data = &data_begin[offset];

    matched = expect("\nlibraries\n  + ", data, length - offset);
    if (matched == 0) {
        return;
    }
    offset += matched;
    data = &data_begin[offset];

    {
        matched = until_newline(data, length - offset);
        if (matched == 0) {
            return;
        }
        CString library{"-l"};
        library.append({data, matched});
        _linker_flags.push_back(std::move(library));
    }
    offset += matched;
    data = &data_begin[offset];

    matched = expect("\n", data, length - offset);
    if (matched == 0) {
        return;
    }
    offset += matched;
    if (offset == length) {
        _valid = true;
    }
}
