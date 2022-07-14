#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

char removeExtraSpace(char *string)
{
    int i, j;
    int len = strlen(string);
    for (i = 0; i < len; i++)
    {
        if (string[0] == ' ')
        {
            for (i = 0; i < (len - 1); i++)
                string[i] = string[i + 1];
            string[i] = '\0';
            len--;
            i = -1;
            continue;
        }
        if (string[i] == ' ' && string[i + 1] == ' ')
        {
            for (j = i; j < (len - 1); j++)
            {
                string[j] = string[j + 1];
            }
            string[j] = '\0';
            len--;
            i--;
        }
    }
}

char *strtoke(char *str, const char *delim)
{
    static char *start = NULL;
    char *token = NULL;
    if (str)
        start = str;
    if (!start)
        return NULL;
    token = start;
    start = strpbrk(start, delim);
    if (start)
        *start++ = '\0';
    return token;
}

int trigger = 1;
void perror(const char *s);

void wordWrap(int width, FILE *file, char *name)
{
    int counter = 0;
    FILE *destF;
    if (name == NULL)
    {
        destF = stdout;
    }
    else
    {
        destF = fopen(name, "w+");
    }
    int lastLineWasNotNewLine = 1;

    FILE *fp = file;
    char *temp;
    char *line=(char*)malloc(INT_MAX);
    if (fp)
    {
        while (1)
        {
            char *c = NULL;
            while ((c = fgets(line, INT_MAX, fp)) != NULL)
            {
                line=strncpy(line,line,strlen(line-1));
                removeExtraSpace(line);

                if (line[0] == '\n')
                {
                    if (lastLineWasNotNewLine == 1)
                    {
                        fprintf(destF, "\n\n");
                        lastLineWasNotNewLine = 0;
                    }
                    counter = 0;
                    break;
                }
                else
                {
                    lastLineWasNotNewLine = 1;
                }

                line[strcspn(line, "\n")] = 0;

                char *ed2;
                char *word = strtok_r(line, " ", &ed2);
                while (word != NULL)
                {
                    if (strlen(word) > width)
                    {
                        trigger = 0;
                    }
                    if (counter + strlen(word) + 1 > width)
                    {
                        fprintf(destF, "\n");
                        counter = 0;
                    }
                    if (counter != 0)
                    {
                        fprintf(destF, " ");
                        counter++;
                    }
                    counter += strlen(word);
                    fprintf(destF, "%s", word);

                    word = strtok_r(NULL, " ", &ed2);

                    // if(strlen(word)==0){
                    //     return;
                    // }
                }
            }

            if (c == NULL)
                break;
        }

        fclose(fp);
    }
    else
    {
        printf("Error opening file...");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {

        return EXIT_FAILURE;
    }
    int width = atoi(argv[1]);
    if (argc == 2)
    {

        if (feof(stdin))
            printf("stdin reached eof\n");

        void *content = (char*)calloc(1000,sizeof(char));
        FILE *fp = fopen("a.txt","w+");
        int read;
        while ((read = fread(content, 1, 1, stdin)))
        {   
            fwrite(content, read, 1, fp);
        }
        fclose(fp);
        fp=fopen("a.txt","r");
        printf("\n");
        wordWrap(width, fp, NULL);
        printf("\n");
        remove("a.txt");
        if (trigger == 0)
        {
            return EXIT_FAILURE;
        }
        else
        {
            return EXIT_SUCCESS;
        }
    }
    struct stat bufr;
    stat(argv[2], &bufr);
    int exist = stat(argv[2], &bufr);
    if (exist == -1)
    {
        perror("Error");
        return EXIT_FAILURE;
    }

    FILE *file;

    if (S_ISREG(bufr.st_mode))
    {
        file = fopen(argv[2], "r");
        char fName[strlen(argv[2]) + 6];
        sprintf(fName, "wrap.%s", argv[2]);
        // printf("%s\n", fName);
        wordWrap(width, file, NULL);
        printf("\n");
    }
    else if (S_ISDIR(bufr.st_mode))
    {

        DIR *directory;
        struct dirent *file;
        FILE *a;
        char ch;

        directory = opendir(argv[2]);
        if (directory == NULL)
        {
            perror("Error");
            return EXIT_FAILURE;
        }

        while ((file = readdir(directory)) != NULL)
        {
            if (strncmp(file->d_name, "wrap.", 5) != 0 && strncmp(file->d_name, ".", 1) != 0)
            {
                // printf("%s - %ld\n", file->d_name, strlen(file->d_name));
                char fName[strlen(argv[2]) + strlen(file->d_name) + 2];
                char newFile[strlen(argv[2]) + strlen(file->d_name) + 7];
                sprintf(fName, "%s/%s", argv[2], file->d_name);
                sprintf(newFile, "%s/wrap.%s", argv[2], file->d_name);

                stat(fName, &bufr);
                // printf("works");
                if (S_ISREG(bufr.st_mode))
                {

                    FILE *p = fopen(fName, "r");
                    if (p == NULL)
                    {
                        perror("Error");
                        return EXIT_FAILURE;
                    }
                    else
                    {
                        wordWrap(width, p, newFile);
                    }
                    // printf("%s\n", file->d_name);
                }
            }
        }

        closedir(directory);
    }
    if (trigger == 0)
    {
        return EXIT_FAILURE;
    }
    exit(1);
}