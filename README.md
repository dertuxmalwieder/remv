# remv

## Rename files with regular expressions.

This application aims to solve the problem that renaming files with regular expressions sucks dicks in hell.

### The current approach to the problem:

    ls * | sed -E 's/(.*)text(.*)to(.*)remove(.*)/mv & "$1$2$3$4"/' | sh

If you think this is handy, please seek help.

### The `remv` approach to the problem:

    remv "(.*)text(.*)to(.*)remove(.*)" "$1$2$3$4"

### How to compile:

Get `remv` from the [Fossil repository](https://code.rosaelefanten.org/remv) or on whichever mirror you read this.

Grab `clang` for your OS (we expect C++20 or newer), then:

    clang -std=c++20 src/remv.cpp

Or, on Windows:

    .\build.cmd

Enjoy.

### How to use it:

     USAGE:
            remv [COMMANDS]
            remv [OPTIONS] <regex> <replacement> [<startpath>]
    
     OPTIONS (any combination):
            -r      Recurse into subdirectories.
            -E      Skip file extensions while renaming.
            -d      Also rename directories on the way.
            -s      Sets the start directory to the last parameter;
                    else, remv will start in '.'.
            -f      Only replaces the first occurrence in each name.
            -v      Verbose logging.
            -vv     Very verbose logging.
            -n      Dry run - don't modify anything just yet.
    
     COMMANDS:
            -V      Display the remv version and exit.
            -h      Display this help screen and exit.

### Comments?

* [@tux0r](https://twitter.com/tux0r) on Twitter.
* [Tuxman](https://www.donationcoder.com/forum/index.php?action=profile;u=49822) on DonationCoder.
