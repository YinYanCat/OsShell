#include "header.h"

void never_lyrics() {
	printf("Never gonna give you up\nNever gonna let you down\nNever gonna run around and desert you\n");
	printf("Never gonna make you cry\nNever gonna say goodbye\nNever gonna tell a lie and hurt you\n");
}

int set_recordatorio(char **cmd, int size) {

    int status;
    if(size>=3) {
    	if(strcmp(cmd[1],"recordatorio")==0) {
	  int seconds = atoi(cmd[2]);
	  pid_t pid = fork();
	  if (pid == 0) {
	    pid_t TermPid = fork();
	    if(TermPid==0){
	      char *shell[] = {program_name,0};
	      execvp(shell[0],shell);
	    }else{
	      sleep(seconds);
	      kill(TermPid, SIGKILL);
	      printf("\nALARMA: Recordatorio de hace %d segundos\n", seconds);
	      for(int i=3; i<size; i++)
		printf("%s ",cmd[i]);
	      printf("\n");
	    
	      exit(0);
	    }
	  } else {
	    printf("Recordatorio establecido para %d segundos\n", seconds);
	    wait(&status);
	  }
	  return(0);
    	}
    }
    for(int i=0; i<size-1; i++)
      printf("%s ",cmd[i]);
    
    printf("%s: No such file or directory\n", cmd[size-1]);
    return(0);
}
