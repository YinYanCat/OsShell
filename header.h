#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>

int exec(char ***cmd, int num_cmds);

int getTokenNum(char *input, char *check);

char **listSTR(char *input, char *check);

int saveCMDs(char *input, char ****cmd);

int setFAV(char **input, int largo, const char *archivotexto);

char *getFAV(char **input, const char *archivotexto);

int favourites();
