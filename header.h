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

void setFAV(char **input, int largo, FILE *archivo);

char *getFAV(char **input, FILE *archivo);

void seeFAV(FILE *archivo);

void delFAV(FILE *archivo);

int favourites(char *inputcmd, FILE *archivo, FILE *log);

int set_recordatorio(char **cmd, int size);

void never_lyrics();

bool copySEARCH(FILE *archivo, char **comando);

void savelogFAV(FILE *archivolog, FILE *archivofavs);
