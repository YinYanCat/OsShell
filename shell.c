#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<stdbool.h>

#define MAXCMDSIZE 1024


int exec(char ***cmd, int num_cmds){

  int status;
  
  int pipes[(num_cmds-1)*2];

  printf("\n");
  
  for(int i = 0; i < num_cmds; i++) {
    for(int j = 0; j<sizeof(cmd[i]); j++){
      printf("%s ",cmd[i][j]);
    }
    printf("\n");
  }
  
  for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipes + i * 2) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
  
  for(int x=0; x<num_cmds; x++){
    pid_t pid = fork();
    if (pid == 0) {
      
      if(x>0){
	dup2(pipes[(x-1)*2],STDIN_FILENO);
      }
      if(x<num_cmds-1){
	dup2(pipes[2*x+1],STDOUT_FILENO);
      }
      

      for (int i=0; i<(num_cmds-1)*2; i++){
	close(pipes[i]);
      }
      
      execvp(cmd[x][0],cmd[x]);
      
      
    }  
  }
  for(int i = 0; i < (num_cmds-1)*2;i++) {
    close(pipes[i]);
  }
  for (int x = 0; x < num_cmds; x++) {
    wait(&status);
  }

  return 0;
  
}


int saveCMDs(char *input, char ***cmd){
  
  int CMDnum = 0;
  char *token = strtok(input, "|");

  while(token!=NULL){
    CMDnum++;
    token = strtok(NULL, "|");
  }

  *cmd = malloc(CMDnum * sizeof(char **));

  int cmdIndex = 0;
  token = strtok(input,"|");
  while(token != NULL){
    
    while(*token == ' '){
      token++;
    }
    
    char *end = token + strlen(token) - 1;
    
    while(end>token && *end == ' '){
      end--;
    }
    *(end+1)='\0';
    
    char *currentCMD = strdup(token);

    (cmd)[cmdIndex] = malloc(100*sizeof(char*));
    int argnum=0;
    char *argtoken = strtok(currentCMD, " ");

    while(argtoken!=NULL){
      while(*argtoken==' '){
	argtoken++;
      }
      char *argend = argtoken + strlen(argtoken) -1;
      while(argend>argtoken && *argend == ' '){
	argend--;
      }
      *(argend+1)='\0';
      (cmd)[cmdIndex][argnum++] = strdup(argtoken);
      argtoken = strtok(NULL," ");
    }

    (cmd)[cmdIndex][argnum]=NULL;

    cmdIndex++;

    token = strtok(NULL,"|");
  }
  (cmd)[cmdIndex] = NULL;
  

  return CMDnum;
}


int main(){

  char input[MAXCMDSIZE];
  char ***cmd = malloc(MAXCMDSIZE*sizeof(char**));

  while(1){
    printf("shell > ");
    fgets(input,sizeof(input),stdin);
    input[strcspn(input, "\n")] = '\0';
    
    int cmdsq = saveCMDs(input,cmd);
    
    if(strcmp(input,"exit")==0){
      break;
    }
    exec(cmd,cmdsq);
  }
  
  

}
