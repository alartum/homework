#define DEBUG

#include <stdio.h>
#include "mylib.h"
#include "files.h"
#include <string.h>
#include "stringlist.h"
#include <errno.h>


const size_t MAX_FILENAME = 256;

int main()
{
    INIT_INFO ("SortByLines");

    char filename[MAX_FILENAME] = "Onegin.txt";//{};
    char savename[MAX_FILENAME] = "NewOnegin.txt";//{};

    /*COMMENT ("File to read from:");
    printf (">");
    scanf ("%s", filename);
    COMMENT ("File to save to:");
    printf (">");
    scanf ("%s", savename); */

    StringList strings = {};
    stringlist_init(&strings);
    if (stringlist_construct (&strings, filename) == ERROR)
    {
        perror("General error");
        return 1;
    }

//        stringlist_dump(&strings);
    StringList sorted = NOINIT; // = { NOINIT_POISON };
    stringlist_init(&sorted);
    if (stringlist_construct (&sorted, &strings) == ERROR)
    {
        perror("Other error");
        return 2;
    }

  //  stringlist_sort (&sorted);
    stringlist_back_sort (&sorted);
    if (stringslist_to_file (&sorted, savename) == ERROR)
    {
        perror ("One more error");
        return 3;
    }
<<<<<<< HEAD
    stringlist_destruct (&sorted);
    stringlist_destruct (&strings);
=======

    memcpy (sortedLines, linesMap, sizeof (char*) * nLines);
    QSortLinesBack(sortedLines, nLines);
    SaveMapToFile(savename, sortedLines, nLines);
// FIXME: You have several memory leaks above.
    free ((char*)buffer);
    free (linesMap);
    free (sortedLines);
>>>>>>> 6df2d3a8bcc05ff0adbc5d30619f5894f5c7a42e
    COMMENT("Done!");
    printf ("#Data was successfully saved to %s\n", savename);

    return 0;
}
