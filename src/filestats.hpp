/**
 * regex move: filestats class.
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */


#ifndef FILESTATS_HPP
#define FILESTATS_HPP


class FileStats {
public:
    int success;
    int failed;
    int skipped;

    FileStats() {
        success = 0;
        failed = 0;
        skipped = 0;
    }
};


#endif
