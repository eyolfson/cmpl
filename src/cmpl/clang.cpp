/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include "clang.hpp"

#include <sys/wait.h>
#include <unistd.h>

using namespace cmpl;

pid_t cmpl::clang_compile(const Path& input, const Path& output,
                         const std::vector<CString>& cxx_flags) {
    pid_t pid = fork();
    if (pid == -1) return pid;
    if (pid == 0) {
        std::vector<char *> argv;
        for (const auto& flag : cxx_flags) {
            argv.push_back(const_cast<char *>(flag.c_str()));
        }
        argv.push_back(const_cast<char *>("-c"));
        argv.push_back(const_cast<char *>("-o"));
        argv.push_back(const_cast<char *>(output.c_str()));
        argv.push_back(const_cast<char *>(input.c_str()));
        argv.push_back(nullptr);
        execvp(argv[0],
               argv.data());
        _exit(-1);
    }
    return pid;
}

bool cmpl::clang_link_binary(std::vector<Path>& inputs, const Path& output,
                             const std::vector<CString>& linker_flags) {
    pid_t pid = fork();
    if (pid == -1) return false;
    if (pid == 0) {
        std::vector<char *> argv;
        for (const auto& flag : linker_flags) {
            argv.push_back(const_cast<char *>(flag.c_str()));
        }
        argv.push_back(const_cast<char *>("-o"));
        argv.push_back(const_cast<char *>(output.c_str()));
        for (const auto& input : inputs) {
            argv.push_back(const_cast<char *>(input.c_str()));
        }
        argv.push_back(nullptr);
        execvp(argv[0],
               argv.data());
        _exit(-1);
    }
    else {
        int stat_val;
        wait(&stat_val);
        if (WIFEXITED(stat_val) == 0) return false;
        if (WEXITSTATUS(stat_val) != 0) return false;
        return true;
    }
    return false;
}
