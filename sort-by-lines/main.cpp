#undef DEBUG

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

    char filename[MAX_FILENAME] = {};//"Onegin.txt";
    char savename[MAX_FILENAME] = {};//"NewOnegin.txt";//{};

    COMMENT ("File to read from:");
    printf (">");
    scanf ("%s", filename);
    COMMENT ("File to save to:");
    printf (">");
    scanf ("%s", savename);

    StringList strings = {};
    stringlist_init(&strings);
    if (stringlist_construct (&strings, filename) == ERROR)
    {
        perror("General error");
        return 1;
    }

//        stringlist_dump(&strings);
    StringList sorted = {};//NOINIT; // = { NOINIT_POISON };
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
    stringlist_destruct (&sorted);
    stringlist_destruct (&strings);
    COMMENT("Done!");
    printf ("#Data was successfully saved to %s\n", savename);

    return 0;
}
