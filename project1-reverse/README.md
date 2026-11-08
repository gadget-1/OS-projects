Project 1: reverse

Prints the lines of a file in reverse order.

    gcc -o reverse reverse.c -Wall -Werror

Usage:

    ./reverse                       reads stdin, writes stdout
    ./reverse input.txt             reads input.txt, writes stdout
    ./reverse input.txt output.txt  reads input.txt, writes output.txt

Lines are read with getline() and stored in a linked list, newest first, so printing the list gives the reversed order. There is no limit on line or file length.

Errors are printed to stderr and the program exits with code 1:
 too many arguments
 input or output file cannot be opened
 input and output are the same file
 malloc fails

Additional assumptions:
 A last line without a newline gets one in the output.
 The same-file check uses stat(), so "a.txt" and "./a.txt" are detected too.
 An empty input gives an empty output.

Sources: https://github.com/remzi-arpacidusseau/ostep-projects


