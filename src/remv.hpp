/**
 * regex move: main header file.
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */


#ifndef REMV_HPP
#define REMV_HPP

#if defined(__has_include)
#  if __has_include(<experimental/filesystem>)
//   Some compilers are not really fast.
#    include <experimental/filesystem>
#    define NAMESPACE_FILESYSTEM std::experimental::filesystem
#  endif
#endif
#ifndef NAMESPACE_FILESYSTEM
#  include <filesystem>
#  define NAMESPACE_FILESYSTEM std::filesystem
#endif

#include <string>
#include <regex>

#include "filestats.hpp"

#ifdef _WIN32
# define DIR_SEPARATOR '\\'
#else
# define DIR_SEPARATOR '/'
#endif


namespace fs = NAMESPACE_FILESYSTEM;


FileStats perform_renames(fs::directory_entry iterator, std::regex needle, std::string replacements, int iLogLevel, bool bSkipExtensions, bool bRenameDirectories, bool bSimulate);
FileStats traverse_haystack(std::string haystack, std::regex needle, std::string replacements, bool bSkipExtensions, bool bRecursive, int iLogLevel, bool bRenameDirectories, bool bSimulate);
int rename_file(std::string in_path, std::string out_path, int iLogLevel);



#endif
