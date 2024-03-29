/**
 * regex move: main file.
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the MIT No Attribution license. See the COPYING file for more
 * details.
 */

#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <system_error>
#include <vector>

#include "constants.hpp"
#include "filestats.hpp"
#include "help.hpp"
#include "remv.hpp"

auto main(int argc, char **argv) -> int
{
    if (argc == 1 || argc > 5)
    {
        // This is guaranteed to have the wrong number of parameters...
        show_usage();
        return 1;
    }

    // We have the following possibilities here:
    //   * remv -V
    //   * remv -h
    //   * remv [in] [out]
    //   * remv -r/E/f/v/vv/d/n [in] [out]
    //   * remv -s/E/f/r/v/vv/d/n [in] [out] [startdir]
    //
    // Everything else is a syntax error. That means
    // that the last two parameters - if argc is 4 or
    // 5 - are <in> and <out> unless -s is specified
    // which adds another parameter.
    //
    // So, if argc is 3, check for -V and -h, else
    // check for any combination of -[srvvdn].

    bool bHasStartpath = false;
    std::string startpath = "";

    FileStats results;
    params renameParams;

    // Look which options the argv[1] has for us ...
    if (argc > 2 && argv[1][0] == '-')
    {
        for (size_t i = 1; i < strlen(argv[1]); i++)
        {
            if (argv[1][i] == 'E')
            {
                // Found "E".
                renameParams.bSkipExtensions = true;
            }
            else if (argv[1][i] == 'r')
            {
                // Found "r".
                renameParams.bHasRecursive = true;
            }
            else if (argv[1][i] == 'f')
            {
                // Found "f".
                renameParams.bFirstOnly = true;
            }
            else if (argv[1][i] == 'v')
            {
                // Add one verbose level.
                renameParams.iLogLevel++;
            }
            else if (argv[1][i] == 'd')
            {
                // Found "d".
                renameParams.bRenameDirectories = true;
            }
            else if (argv[1][i] == 'n')
            {
                // Found "n".
                renameParams.bDryRun = true;
            }
            else if (argv[1][i] == 's' && argc == 5)
            {
                // Found "s" and has a startpath parameter.
                bHasStartpath = true;
            }
            else
            {
                // Invalid.
                show_usage();
                return 1;
            }
        }
    }

    if (argc == 2)
    {
        if (strncmp(argv[1], "-V", strlen(argv[1])) == 0)
        {
            // remv -V
            show_version();
            return 0;
        }
        if (strncmp(argv[1], "-h", strlen(argv[1])) == 0)
        {
            // remv -h
            show_usage();
            return 0;
        }

        // remv <wrong param>
        show_usage();
        return 1;
    }
    else if (argc == 3)
    {
        // remv <in> <out>
        try
        {
            std::regex needle(argv[1]);
            std::string replacements = argv[2];
            results = traverse_haystack(startpath, needle, replacements, &renameParams);
        }
        catch (std::regex_error &)
        {
            std::cerr << "remv could not parse your input regex." << std::endl;
            std::cerr << "Aborted." << std::endl;
            return 1;
        }
    }
    else if (argc >= 4)
    {
        // remv <in> <out> <options...>
        if (argv[1][0] != '-')
        {
            // no hyphen ...
            show_usage();
            return 1;
        }

        // Validity checks:
        try
        {
            std::regex needle(argv[2]);
            std::string replacements = argv[3];
            if (bHasStartpath)
            {
                if (!fs::is_directory(fs::status(argv[4])))
                {
                    // The path that was determined to be the start path is not
                    // a directory according to what we see. Abort.
                    std::cerr << argv[4] << " does not seem to be a directory. Aborting." << std::endl;
                    return 1;
                }
                startpath = argv[4];
            }
            else
            {
                startpath = ".";
            }

            // Do it!
            results = traverse_haystack(startpath, needle, replacements, &renameParams);
        }
        catch (std::regex_error &)
        {
            std::cerr << "remv could not parse your input regex: " << argv[1] << std::endl;
            std::cerr << "Aborted." << std::endl;
            return 1;
        }
    }

    // Print stats:
    if (renameParams.iLogLevel > 0)
    {
        std::cout << " [STATS] ";
        if (renameParams.bDryRun)
        {
            std::cout << "The simulation suggested " << results.success
                      << " renames. Note that some of them might fail.";
        }
        else
        {
            std::cout << "Renamed " << results.success << " files "
                      << (renameParams.bRenameDirectories ? "and directories " : "") << "(" << results.failed
                      << " failed, " << results.skipped << " skipped).";
        }
        std::cout << std::endl;
    }

    return 0;
}

auto traverse_haystack(std::string haystack, std::regex needle, std::string replacements, params *renameParams)
    -> FileStats
{
    // Traverses through <haystack>, replacing <needle> by <replacements>.
    FileStats ret;
    FileStats this_round; // One per directory_item.

    std::vector<fs::directory_entry> filePaths;

    if (renameParams->bHasRecursive)
    {
        // Perform a recursive search under the current directory.
        for (auto &p : fs::recursive_directory_iterator(haystack))
        {
            filePaths.emplace_back(p);
        }
    }
    else
    {
        // Perform a non-recursive search.
        for (auto &p : fs::directory_iterator(haystack))
        {
            filePaths.emplace_back(p);
        }
    }

    // Reverse order:
    std::reverse(std::begin(filePaths), std::end(filePaths));

    for (auto p : filePaths)
    {
        this_round = perform_renames(p, needle, replacements, renameParams);
        ret.skipped += this_round.skipped;
        ret.success += this_round.success;
        ret.failed += this_round.failed;
    }

    return ret;
}

auto perform_renames(fs::directory_entry iterator, std::regex needle, std::string replacements, params *renameParams)
    -> FileStats
{
    FileStats ret;
    if (fs::is_regular_file(iterator) || (renameParams->bRenameDirectories && fs::is_directory(iterator)))
    {
        // <iterator> is either a regular file or a directory and we want to
        // process it according to our <in_regex>.
        std::smatch match;
        std::string old_extension = "";

        std::stringstream parent_path_stream;
        std::stringstream out_path_stream;

        // Store the parent directory:
        parent_path_stream << iterator.path().parent_path().string();
        parent_path_stream << DIR_SEPARATOR;

        if (renameParams->bRenameDirectories)
        {
            // If the user has decided to not rename directories, why would we
            // want to rename paths on the way?
            out_path_stream << parent_path_stream.str();
        }

        // Regardless of what bSkipExtensions says, the original file can only
        // be found by its full path. bSkipExtensions will only control which
        // part of the file name should be regex_replace()d below.
        std::string orig_file_path = iterator.path().string();

        if (renameParams->bSkipExtensions)
        {
            // Don't rename extensions.
            out_path_stream << iterator.path().stem().string();
            old_extension = iterator.path().extension().string();
        }
        else
        {
            // Rename extensions as well.
            out_path_stream << iterator.path().filename().string();
        }

        // The in_path should have:
        //  1. the file path without an extension
        //  2. the file path with an extension
        // depending on the bSkipExtensions parameter...
        std::string in_path = out_path_stream.str();

        if (std::regex_search(in_path, match, needle))
        {
            // Found a match here.
            std::string new_name =
                renameParams->bFirstOnly
                    ? std::regex_replace(in_path, needle, replacements, std::regex_constants::format_first_only)
                    : std::regex_replace(in_path, needle, replacements);
            std::stringstream new_file_path;

            new_file_path << new_name;

            if (renameParams->bSkipExtensions && !old_extension.empty())
            {
                // Add the extension again.
                new_file_path << old_extension;
            }

            new_name = new_file_path.str();

            // Target check:
            fs::path new_path(new_name);
            if (fs::exists(new_path))
            {
                // Oops.
                if (renameParams->iLogLevel > 0)
                {
                    if (fs::is_directory(iterator))
                    {
                        // This has been created already because we call
                        // create_directories() during the renaming. Assume
                        // no failure here. :-)
                        if (renameParams->iLogLevel > 1)
                        {
                            // Only log non-renaming tasks for "very verbose" because why spam the user?
                            std::cout << " Processed        '" << iterator << "' ..." << std::endl;
                        }
                        ret.skipped++;
                    }
                    else
                    {
                        std::cout << " Can't rename " << iterator << "' to '" << new_name << "' - file exists."
                                  << std::endl;
                        ret.failed++;
                    }
                    return ret;
                }
            }

            if (renameParams->iLogLevel > 0)
            {
                std::cout << (renameParams->bSimulate ? " Would rename" : " Renaming    ")
                          << (renameParams->iLogLevel > 1 && !renameParams->bSimulate ? "     " : " ") << "  '"
                          << iterator << "' to '" << new_name << "' ..." << std::endl;
            }
            if (!renameParams->bSimulate)
            {
                if (rename_file(orig_file_path, new_name, renameParams) != 0)
                {
                    ret.failed++;
                    return ret;
                }
            }

            ret.success++;
        }
        else
        {
            if (renameParams->iLogLevel > 1)
            {
                // Only log non-renaming tasks for "very verbose" because why spam the user?
                std::cout << " Skipping         '" << iterator << "' ..." << std::endl;
            }
            ret.skipped++;
        }
    }
    else
    {
        // Skip.
        if (renameParams->iLogLevel > 1)
        {
            // Highly verbose.
            std::cout << " Skipping     '" << iterator << "'." << std::endl;
        }
        ret.skipped++;
    }
    return ret;
}

auto rename_file(std::string in_path, std::string out_path, params *renameParams) -> int
{
    // Does the actual renaming.
    std::error_code err;
    fs::path in(in_path);
    fs::path out(out_path);

    // Anything missing?
    fs::create_directories(out.parent_path().string(), err);
    if (err)
    {
        // Tried to create an invalid path or something.
        std::cerr << "   error [cd]: " << err.message() << std::endl;
        return 1;
    }

    fs::rename(in, out, err);
    if (err)
    {
        // Tried to rename into an invalid path or something.
        std::cerr << "   error [rn]: " << err.message() << std::endl;
        return 1;
    }

    if (renameParams->iLogLevel > 1)
    {
        std::cout << "Successfully renamed " << in_path << " to " << out_path << "." << std::endl;
    }
    return 0;
}
