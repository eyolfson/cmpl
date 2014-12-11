/*******************************************************************************
Copyright 2014 Jon Eyolfson

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.
*******************************************************************************/

#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <libgen.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <string>
#include <unordered_map>
#include <vector>

static char const * const CONFIG_NAME = "project.cmpl";
static size_t src_prefix_len;
static std::unordered_map<std::string, std::vector<std::string>> map;

static int process(char const * path,
                   struct stat const * status,
                   int type,
                   struct FTW * info)
{
    /* Ensure stat is valid */
    if (type == FTW_NS) {
        return -1;
    }

    size_t path_len = strlen(path);
    /* Buffer must be able to hold: path length, extra char, and null char */
    if (path_len + 2 >= PATH_MAX) {
        return -1;
    }

    char buffer[PATH_MAX];
    char const * src_path = &path[src_prefix_len + 4];
    strncpy(buffer, path, src_prefix_len);
    buffer[src_prefix_len] = '\0';
    strcat(buffer, "cmpl");
    if (strlen(src_path) != 0) {
        strcat(buffer, "/");
        strcat(buffer, src_path);
    }

    if (S_ISDIR(status->st_mode)) {
        mkdir(buffer, S_IRWXU | S_IRWXG | S_IRWXO);
        return 0;
    }

    /* Only care about regular files */
    if (!S_ISREG(status->st_mode)) return 0;

    /* Ensure the file belongs to a binary */
    char const * target_end = strchr(src_path, '/');
    if (target_end == nullptr) {
        return -1;
    }
    if (strncmp(src_path, "lib", 3) == 0) {
        return 0;
    }

    /* Check that the path is long enough for an extension */
    if (path_len < 5) {
        return 0;
    }

    /* Compile the file if it's extension is cpp */
    char const * ext = &path[path_len - 3];
    if (strcmp(ext, "cpp") == 0) {
        size_t buffer_len = strlen(buffer);
        buffer[buffer_len - 3] = 'o';
        buffer[buffer_len - 2] = '\0';
        printf("\e[34;1m[Compile]\e[0m \e[34m%s\e[0m\n",
               &buffer[src_prefix_len + 5]);
        pid_t pid = fork();
        if (pid == -1) return -1;
        if (pid == 0) {
            execlp("clang++",
                   "clang++", "-c", "-std=c++14", "-o", buffer, path, nullptr);
            exit(-1);
        }
        else {
            int stat_val;
            wait(&stat_val);
            if (WIFEXITED(stat_val) == 0) return -1;
            if (WEXITSTATUS(stat_val) != 0) return -1;
            auto & value = map[std::string(src_path, target_end)];
            value.push_back(std::string(buffer));
        }
    }
    return 0;
}

static inline void cat_path_config_name(char * s)
{
    strcat(s, "/");
    strcat(s, CONFIG_NAME);
}

int main(int argc, char const * const * argv)
{
    char buffer[PATH_MAX];
    char project_dir[PATH_MAX];

    /* Returns null pointer on failure */
    if (getcwd(buffer, PATH_MAX) == nullptr) {
        return 1;
    }

    /* Buffer must be able to hold: string lengths, / char, and null char */
    if ((strlen(buffer) + strlen(CONFIG_NAME) + 2) > PATH_MAX) {
        return 2;
    }

    cat_path_config_name(buffer);
    int fd = open(buffer, O_RDONLY);
    while (fd < 0) {
        dirname(dirname(buffer));
        if (strcmp(buffer, "/") == 0) {
            return 3;
        }
        cat_path_config_name(buffer);
        fd = open(buffer, O_RDONLY);
    }

    dirname(buffer);
    strcpy(project_dir, buffer);

    strcat(buffer, "/src");
    src_prefix_len = strlen(buffer) - 3;
    if (nftw(buffer, process, 10, 0) == -1) {
        return 4;
    }

    if (!map.empty()) {
        strcpy(buffer, project_dir);
        strcat(buffer, "/bin");
        mkdir(buffer, S_IRWXU | S_IRWXG | S_IRWXO);
    }
    for(auto & i : map) {
        printf("\e[36;1m[Binary]\e[0m  \e[36m%s\e[0m\n", i.first.c_str());
        std::vector<char *> link_argv;
        link_argv.push_back(const_cast<char *>("clang++"));
        for(auto & j : i.second) {
            link_argv.push_back(const_cast<char *>(j.c_str()));
        }
        link_argv.push_back(const_cast<char *>("-o"));
        strcpy(buffer, project_dir);
        strcat(buffer, "/bin/");
        strcat(buffer, i.first.c_str());
        link_argv.push_back(buffer);
        link_argv.push_back(nullptr);
        pid_t pid = fork();
        if (pid == -1) return 5;
        if (pid == 0) {
            execvp("clang++",
                   link_argv.data());
            exit(-1);
        }
        else {
            int stat_val;
            wait(&stat_val);
            if (WIFEXITED(stat_val) == 0) return 6;
            if (WEXITSTATUS(stat_val) != 0) return 6;
        }
    }
    return 0;
}
