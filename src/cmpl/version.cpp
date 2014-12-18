/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include "version.hpp"

#include <cstdio>

using namespace cmpl;

const Version cmpl::VERSION = {0, 0, 3, {"dev"}};

void Version::print() const {
    printf("Cmpl %d.%d.%d", _major, _minor, _patch);
    if (_pre_release.size() > 0) {
        printf("-");
        for (decltype(_pre_release.size()) i = 0;
             i < _pre_release.size() - 1; ++i) {
            printf("%s", _pre_release[i].c_str());
            printf(".");
        }
        printf("%s", _pre_release[_pre_release.size() - 1].c_str());
    }
    if (_build.size() > 0) {
        printf("+");
        for (decltype(_build.size()) i = 0;
             i < _build.size() - 1; ++i) {
            printf("%s", _build[i].c_str());
            printf(".");
        }
        printf("%s", _build[_build.size() - 1].c_str());
    }
    printf("\n");
}
