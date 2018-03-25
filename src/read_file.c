#include "read_file.h"

void readFile(const char *fileName, double **array, const char *separator)
{
    char buffer[1024];
    char *record, *line;
    int i = 0, j = 0;

    FILE *fstream = fopen(fileName, "r");
    if (fstream == NULL)
    {
        printf("\n file opening failed ");
        exit(0);
    }

    while ((line = fgets(buffer, sizeof(buffer), fstream)) != NULL)
    {
        record = strtok(line, separator);
        while (record != NULL)
        {
            array[i][j++] = atof(record);
            record = strtok(NULL, separator);
        }
        j = 0;
        i++;
    }
    fclose(fstream);
}

void countRowsCols(const char *fileName, int *rows, int *cols, const char *separator)
{
    char buffer[1024];
    char *record, *line;
    int row = 0, column = 0;

    FILE *fstream = fopen(fileName, "r");
    if (fstream == NULL)
    {
        printf("\n file opening failed ");
        exit(0);
    }

    while ((line = fgets(buffer, sizeof(buffer), fstream)) != NULL)
    {
        record = strtok(line, separator);
        while (record != NULL)
        {
            record = strtok(NULL, separator);
            column++;
        }
        *cols = column;
        column = 0;
        row++;
    }
    *rows = row;
    fclose(fstream);
}