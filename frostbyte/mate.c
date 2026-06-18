#define MATE_IMPLEMENTATION
#include "mate.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("usage: %s [RLIMGUI_PATH] [--headless]\n", argc ? argv[0] : "mate");
        return 1;
    }

    bool is_headless = false;
    const char* headless = "";
    const char* arg1 = argv[1];
    if (strcmp(arg1, "--headless") == 0) {
        is_headless = true;
        headless = " -DFROSTBYTE_HEADLESS";
    }

    printf("headless: %d\n", is_headless);

    char* path_buffer = NULL;
    int path_buffer_size = strlen(arg1) + 30;

    if (!is_headless) {
        path_buffer = (char*) malloc(path_buffer_size);
        memset(path_buffer, 0, path_buffer_size);
    }

    char lib_flags[120];
    memset(lib_flags, 0, 120);
    snprintf(lib_flags, 120, "-std=c++17 -Wall -Werror -g -march=native -static-libstdc++ -static-libgcc -fPIC%s", headless);

    char build_dir[30];
    memset(build_dir, 0, 30);
    if (is_headless)
        snprintf(build_dir, 30, "./build-headless");
    else
        snprintf(build_dir, 30, "./build");

    MateOptions mate_options = {
        .buildDirectory = build_dir
    };
    CreateConfig(mate_options);

    StartBuild();

    char lib_output[30];
    memset(lib_output, 0, 30);
    if (is_headless)
        snprintf(lib_output, 30, "libfrostbyte-headless");
    else
        snprintf(lib_output, 30, "libfrostbyte");

    StaticLibOptions lib_options = {
        .output = lib_output,
        .flags = lib_flags
    };
    StaticLib lib = CreateStaticLib(lib_options);

    AddIncludePaths(lib, "./include", "./dependencies/json/include", "./dependencies/curl/include", "./dependencies/uuid_v4");

    AddFile(lib, "./src/*.cpp");
    AddFile(lib, "./src/libraries/*.cpp");
    AddFile(lib, "./src/ui/*.cpp");
    AddFile(lib, "./src/engine/classes/*.cpp");
    AddFile(lib, "./src/engine/classes/frostbyte/*.cpp");
    AddFile(lib, "./src/engine/datatypes/*.cpp");

    AddIncludePaths(lib, "./dependencies/luau/Analysis/include");
    AddIncludePaths(lib, "./dependencies/luau/Ast/include");
    AddIncludePaths(lib, "./dependencies/luau/Common/include");
    AddIncludePaths(lib, "./dependencies/luau/Compiler/include");
    AddIncludePaths(lib, "./dependencies/luau/Config/include");
    AddIncludePaths(lib, "./dependencies/luau/VM/include");
    AddIncludePaths(lib, "./dependencies/luau/VM/src");

    if (!is_headless) {
        snprintf(path_buffer, path_buffer_size, "%s/raylib-master/src", arg1);
        AddIncludePaths(lib, path_buffer);
        memset(path_buffer, 0, path_buffer_size);
        snprintf(path_buffer, path_buffer_size, "%s/imgui-master", arg1);
        AddIncludePaths(lib, path_buffer);

        memset(path_buffer, 0, path_buffer_size);
        snprintf(path_buffer, path_buffer_size, "%s/bin/Release", arg1);
        AddLibraryPaths(lib, path_buffer);
    }

    InstallStaticLib(lib);

    EndBuild();

    if (path_buffer)
        free(path_buffer);
    return 0;
}

