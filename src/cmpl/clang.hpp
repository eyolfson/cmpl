/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#ifndef __CMPL_CLANG_HPP__
#define __CMPL_CLANG_HPP__

#include "c_string.hpp"
#include "path.hpp"

#include <vector>

#include <unistd.h>

namespace cmpl {

pid_t clang_compile(const Path& input, const Path& output,
                   const std::vector<CString>& cxx_flags);
bool clang_link_binary(std::vector<Path>& inputs, const Path& output,
                       const std::vector<CString>& linker_flags);

}

#endif
