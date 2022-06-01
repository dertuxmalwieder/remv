/**
 * regex move: main header file.
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the MIT No Attribution license. See the COPYING file for more
 * details.
 */

#ifndef REMV_HPP
#define REMV_HPP

#if defined(__has_include)
#  if __has_include(<filesystem>)
#    include <filesystem>
#    define NAMESPACE_FILESYSTEM std::filesystem
#  elif __has_include(<experimental/filesystem>)
//   Some compilers are not really fast.
#    include <experimental/filesystem>
#    define NAMESPACE_FILESYSTEM std::experimental::filesystem
#  endif
#endif
#ifndef NAMESPACE_FILESYSTEM
// Fallback to the default:
#  include <filesystem>
#  define NAMESPACE_FILESYSTEM std::filesystem
#endif

#include <regex>
#include <string>

#include "filestats.hpp"

#ifdef _WIN32
#define DIR_SEPARATOR '\\'
#else
#define DIR_SEPARATOR '/'
#endif

namespace fs = NAMESPACE_FILESYSTEM;

struct params
{
    int iLogLevel;
    bool bDryRun;
    bool bHasRecursive;
    bool bSkipExtensions;
    bool bRenameDirectories;
    bool bSimulate;
    bool bFirstOnly;
};

auto perform_renames(fs::directory_entry iterator, std::regex needle, std::string replacements, params *renameParams)
    -> FileStats;
auto traverse_haystack(std::string startpath, std::regex needle, std::string replacements, params *renameParams)
    -> FileStats;
auto rename_file(std::string in_path, std::string out_path, params *renameParams) -> int;

#endif
