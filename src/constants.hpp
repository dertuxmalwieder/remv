/**
 * regex move: constants.
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the MIT No Attribution license. See the COPYING file for more
 * details.
 */


#ifndef REMV_CONST
#define REMV_CONST

#include <string>

// remv version (tries to follow the SemVer recommendations).
// history:
//   1.4.1  [2021-01-20]
//          we use modern C++ now (auto ->)
//          added include guards and rearranged some includes
//
//   1.4.0  [2020-10-28]
//          relicensed as MIT-0 for the No Warranty clause
//
//   1.3.1  [2019-09-26]
//          checking for "-" in argv, just in case...
//
//   1.3.0  [2019-01-14]
//          the -r flag now won't try to rename directories unless -d is
//          specified as well
//
//   1.2.2  [2019-01-14]
//          fixed the -E flag (see 1.2.1)
//
//   1.2.1  [2019-01-11]
//          fixed too many newlines in the output ;-)
//          trying to create missing directories before renaming now
//          KNOWN PROBLEM: the -E flag denies renaming whatsoever yet
//
//   1.2.0  [2019-01-09]
//          fixed missing newline in the output
//          removed double path in the target file name
//          renaming in reverse order now to respect directory renames
//
//   1.1.1  [2019-01-07]
//          fixed handling of relative directories
//
//   1.1.0  [2019-01-07]
//          added a flag to skip file extensions
//          fixed a few, err, surprise features
//
//   1.0.1  [2018-01-14]
//          remv deleted files. Oops.
//
//   1.0.0  [2018-01-13]
//          initial release


const std::string VERSION = "1.4.1";


#endif
