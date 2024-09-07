#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>

char *program_name;

int exec(char ***cmd, int num_cmds);

int getTokenNum(char *input, char *check);

char **listSTR(char *input, char *check);

int saveCMDs(char *input, char ****cmd);

int setFAV(char **input, int largo, const char *archivotexto);

char *getFAV(char **input, const char *archivotexto);

void seeFAV(const char *archivotexto);

void delFAV(const char *archivotexto);

int favourites(char *inputcmd);

int set_recordatorio(char **cmd, int size);

void never_lyrics();
