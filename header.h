#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<stdbool.h>
#include<ctype.h>
#include <linux/limits.h>

char *program_name;

int exec(char ***cmd, int num_cmds);

int getTokenNum(char *input, char *check);

char **listSTR(char *input, char *check);

int saveCMDs(char *input, char ****cmd);

void setFAV(char **input, int largo, const char *archivotexto, char* fileroute);

char *getFAV(char **input, const char *archivotexto, char* fileroute);

void seeFAV(const char *archivotexto, char* fileroute);

void delFAV(const char *archivotexto, char* fileroute);

int favourites(char *inputcmd, char* filename, char* fileroute, FILE *log);

void change_directory(char *dir);

int set_recordatorio(char **cmd, int size);

void never_lyrics();

bool copySEARCH(const char *archivotexto, char* fileroute, char **comando);

void savelogFAV(char *filename, char* fileroute, FILE *archivolog);
