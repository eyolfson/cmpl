/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#ifndef __CMPL_VERSION_HPP__
#define __CMPL_VERSION_HPP__

#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>

namespace cmpl {

class Version {
public:
    uint8_t const _major;
    uint8_t const _minor;
    uint16_t const _patch;
    std::vector<std::string> _pre_release;
    std::vector<std::string> _build;
public:
    Version(uint8_t major, uint8_t minor, uint16_t patch)
        : _major(major), _minor(minor), _patch(patch) {}
    Version(uint8_t major, uint8_t minor, uint16_t patch,
            std::initializer_list<std::string> pre_release)
        : _major(major), _minor(minor), _patch(patch),
          _pre_release(pre_release) {}
    Version(uint8_t major, uint8_t minor, uint16_t patch,
            std::initializer_list<std::string> pre_release,
            std::initializer_list<std::string> build)
        : _major(major), _minor(minor), _patch(patch),
          _pre_release(pre_release), _build(build) {}
    void print() const;
};

extern Version const VERSION;

}

#endif
