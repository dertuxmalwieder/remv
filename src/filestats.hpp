/**
 * regex move: filestats class.
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the MIT No Attribution license. See the COPYING file for more
 * details.
 */

#ifndef FILESTATS_HPP
#define FILESTATS_HPP

class FileStats
{
  public:
    int success;
    int failed;
    int skipped;

    FileStats()
    {
        success = 0;
        failed = 0;
        skipped = 0;
    }
};

#endif
