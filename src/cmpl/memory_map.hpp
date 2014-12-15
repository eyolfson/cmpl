/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#ifndef __CMPL_MEMORY_MAP_HPP__
#define __CMPL_MEMORY_MAP_HPP__

#include "file_desc.hpp"

#include <sys/mman.h>

namespace cmpl {

class MemoryMap {
    void * _data{ nullptr };
    size_t _length{ 0 };
public:
    MemoryMap(FileDesc& file_desc);
    ~MemoryMap();

    bool is_invalid() { return _data == nullptr; }
    bool is_valid() { return !is_invalid(); }

    template <typename T> T * data() { return static_cast<T *>(_data); }
    size_t length() { return _length; }
};

}

#endif
