/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include "memory_map.hpp"

#include <sys/mman.h>

using namespace cmpl;

MemoryMap::MemoryMap(FileDesc& file_desc) {
    size_t length = file_desc.size();
    void * ptr = mmap(nullptr, length, PROT_READ, MAP_PRIVATE, file_desc.fd(),
                      0);
    if (ptr != MAP_FAILED) {
        _data = ptr;
        _length = length;
    }
}

MemoryMap::~MemoryMap() {
    if (_data != nullptr) {
        munmap(_data, _length);
        _data = nullptr;
        _length = 0;
    }
}
