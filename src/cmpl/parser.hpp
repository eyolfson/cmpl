/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#ifndef __CMPL_PARSER_HPP__
#define __CMPL_PARSER_HPP__

#include "path.hpp"
#include "string_ref.hpp"

#include <vector>

namespace cmpl {

class Parser {
    std::vector<CString> _cxx_flags;
    std::vector<CString> _linker_flags;
    bool _valid{ false };
public:
    Parser(const Path&);

    const std::vector<CString>& cxx_flags() { return _cxx_flags; }
    const std::vector<CString>& linker_flags() { return _linker_flags; }
    bool is_valid() const { return _valid; }
    bool is_invalid() const { return !is_valid(); }
};

}

#endif
