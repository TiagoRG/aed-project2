#include <stdio.h>
#include "Graph.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Pass the file name as an argument.\n");
        return 1;
    }

    char* filename = argv[1];
    char* extension = filename;

    while (*extension != '\0')
        extension++;

    if (extension - filename < 4)
    {
        printf("Please use a valid txt file.\n");
        return 1;
    }

    extension -= 4;
    if (extension[0] != '.' || extension[1] != 't' || extension[2] != 'x' || extension[3] != 't')
    {
        printf("Please use a valid txt file.\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");

    if (f == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    Graph *g = GraphFromFile(f);

    fclose(f);

    GraphDisplay(g);

    return 0;
}
