/**
 * regex move: the help screens
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the MIT No Attribution license. See the COPYING file for more
 * details.
 */


#include <iostream>
#include "constants.hpp"


inline void show_usage() {
    // Displays the usage.
    std::cout << std::endl;
    std::cout << " This is remv - move files using regular expressions." << std::endl;
    std::cout << std::endl;
    std::cout << " USAGE:" << std::endl;
    std::cout << "\tremv [COMMANDS]" << std::endl;
    std::cout << "\tremv [OPTIONS] <regex> <replacement> [<startpath>]" << std::endl;
    std::cout << std::endl;
    std::cout << " OPTIONS (any combination):" << std::endl;
    std::cout << "\t-r\tRecurse into subdirectories." << std::endl;
    std::cout << "\t-E\tSkip file extensions while renaming." << std::endl;
    std::cout << "\t-d\tAlso rename directories on the way." << std::endl;
    std::cout << "\t-s\tSets the start directory to the last parameter;" << std::endl;
    std::cout << "\t\telse, remv will start in '.'." << std::endl;
    std::cout << "\t-v\tVerbose logging." << std::endl;
    std::cout << "\t-vv\tVery verbose logging." << std::endl;
    std::cout << "\t-n\tDry run - don't modify anything just yet." << std::endl;
    std::cout << std::endl;
    std::cout << " COMMANDS:" << std::endl;
    std::cout << "\t-V\tDisplay the remv version and exit." << std::endl;
    std::cout << "\t-h\tDisplay this help screen and exit." << std::endl;
    std::cout << std::endl;
    std::cout << " You can use $1, $2 etc. in your replacement strings for back-" << std::endl;
    std::cout << " references." << std::endl;
    std::cout << std::endl;
}


inline void show_version() {
    // Displays the current remv version.
    std::cout << std::endl << "This is remv version " << VERSION << "." << std::endl;
}
