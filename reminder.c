#include "header.h"

#define MAXCMDSIZE 1024

void never_lyrics() {
	printf("Never gonna give you up\nNever gonna let you down\nNever gonna run around and desert you\n");
	printf("Never gonna make you cry\nNever gonna say goodbye\nNever gonna tell a lie and hurt you\n");
}

void set_recordatorio(char *input, char **cmd) {
    char *token = strtok(input, " ");
    int value = 0;
    while (token != NULL) {
        value++;
        token = strtok(NULL, " ");
    }
    if(value>=3) {
    	if(strcmp(cmd[1],"recordatorio")==0) {
    		int seconds = atoi(cmd[2]);
    		if (fork() == 0) {
				sleep(seconds);
				printf("\nbseconds: %d\n", seconds);
        		exit(0);  // Terminate child
    		}
		    printf("done\n");
    	}
    }
}
