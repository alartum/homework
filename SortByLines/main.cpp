#define DEBUG

#include <stdio.h>
#include "mylib.h"
#include "files.h"
#include <string.h>
#include <errno.h>

const size_t MAX_FILENAME = 256;

int main()
{
    INIT_INFO ("SortByLines");

    char filename[MAX_FILENAME], savename[MAX_FILENAME];

    COMMENT ("File to read from:");
    printf (">");
    scanf ("%s", filename);
    COMMENT ("File to save to:");
    printf (">");
    scanf ("%s", savename);

    size_t length = 0;
    const char* buffer = ReadFileDynamic (filename, &length);
    if (!buffer)
    {
        perror ("Reading error");
        return 1;
    }

    size_t nLines = 0;
    char** linesMap = BuildLinesMap ((char*)buffer, length, &nLines);
    if (!linesMap)
    {
        perror ("Parsing error");
        return 1;
    }

    char** sortedLines = (char**)calloc (nLines, sizeof(char*));
    if (!sortedLines)
    {
        errno = ENOMEM;
        perror ("Memory");
        return 1;
    }

    memcpy (sortedLines, linesMap, sizeof (char*) * nLines);
    QSortLinesBack(sortedLines, nLines);
    SaveMapToFile(savename, sortedLines, nLines);
// FIXME: You have several memory leaks above.
    free ((char*)buffer);
    free (linesMap);
    free (sortedLines);
    COMMENT("Done!");
    printf ("#Data was successfully saved to %s.\n", savename);

    return 0;
}
