#include <stdio.h>
#include "share.h"

FILE *get_stdout()
{
    return stdout;
}

FILE *get_stderr()
{
    return stderr;
}

int get_fbf()
{
    return _IOFBF;
}

int get_lbf()
{
    return _IOLBF;
}

int get_nbf()
{
    return _IONBF;
}