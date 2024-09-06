#include "header.h"

void never_lyrics() {
	printf("Never gonna give you up\nNever gonna let you down\nNever gonna run around and desert you\n");
	printf("Never gonna make you cry\nNever gonna say goodbye\nNever gonna tell a lie and hurt you\n");
}
int set_recordatorio(char *input, char **cmd) {
    int value = getTokenNum(input, " ");
    int status;
    if(value>=3) {
    	if(strcmp(cmd[1],"recordatorio")==0) {
    		int seconds = atoi(cmd[2]);
            pid_t pid = fork();
    		if (pid == 0) {
                sleep(seconds);
                printf("\nALARMA: Recordatorio de hace %d segundos\n", seconds);
                for(int i=3; i<value; i++)
                    printf("%s ",cmd[i]);
                printf("\n");
                exit(0);
            } else {
                printf("Recordatorio establecido para %d segundos\n", seconds);
                wait(&status);
            }
            return(0);
    	}
    }
    for(int i=0; i<value-1; i++)
        printf("%s ",cmd[i]);
    printf("%s: No such file or directory\n", cmd[value-1]);
    return(0);
}
