/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include "c_string.hpp"
#include "clang.hpp"
#include "dir.hpp"
#include "parser.hpp"
#include "path.hpp"
#include "version.hpp"

#include <cstdio>
#include <cstring>

using namespace cmpl;

int main(int argc, char const * const * argv)
{
    /* Poor handling of command line arguments */
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--version") == 0) {
            VERSION.print();
            return 0;
        }
    }

    /* Look for project file */
    const Path project_file_path = Path::find_file("project.cmpl");
    if (project_file_path.is_invalid()) return 1;
    Parser parser(project_file_path);
    if (parser.is_invalid()) return 2;

    /* Compile the files in post order, at the root, link */
    const Path project_path = project_file_path.dirname();
    const Path project_src_path = project_path.append("src");
    const Path project_cmpl_path = project_path.append("cmpl");

    /* Symbolic links should be ignored in the src and cmpl directories */
    if (!project_cmpl_path.make()) return 1;
    std::vector<CString> targets;
    {
        Dir dir(project_src_path);
        targets = dir.read_dir_c_strings();
    }
    for (const auto& target : targets) {
        const Path target_src_path = project_src_path.append(target.c_str());
        const Path target_cmpl_path = project_cmpl_path.append(target.c_str());
        if (!target_cmpl_path.make()) return 2;

        std::vector<Path> object_paths;
        {
            Dir dir(target_src_path);
            object_paths = dir.compile_files(project_cmpl_path,
                                             target_cmpl_path,
                                             parser.cxx_flags());
        }
        if (!target.starts_with("lib")) {
            const Path project_bin_path = project_path.append("bin");
            if (!project_bin_path.make()) return 1;
            const Path target_path = project_bin_path.append(target.c_str());
            printf("\e[36;1m[Link]\e[0m    \e[36m%s\e[0m\n",
                   target_path.c_str() + project_path.length() + 1);
            clang_link_binary(object_paths, target_path, parser.linker_flags());
        }
    }
    return 0;
}
