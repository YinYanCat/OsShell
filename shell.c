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

  /* for (int i = 0; i < num_cmds; i++) { */
  /*   int j = 0; */
  /*   while (cmd[i][j] != NULL) { */
  /*     printf("%s ", cmd[i][j]); */
  /*     j++; */
  /*   } */
  /*   printf("\n"); */
  /* } */

  
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

int getTokenNum(char *input, char *check){
  char *str = strdup(input);
  char *token = strtok(str,check);
  int TKNnum = 0;
  while(token!=NULL){
    TKNnum++;
    token = strtok(NULL,check);
  }
  free(str);
  return TKNnum;
}

char **listSTR(char *input, char *check){

  char *str = strdup(input);
  int numTKN = getTokenNum(input,check);
  int index = 0;
  char **output = malloc((numTKN+1) * sizeof(char *));
  char *token = strtok(str,check);
  while(token != NULL){
    while(*token == ' '){
      token++;
    }
  
    char *end = token + strlen(token) -1;
    while(end > token && *end == ' '){
      end--;
    }
    *(end + 1)='\0';
    output[index++] = strdup(token);
    token = strtok(NULL,check);
  }
  output[numTKN]=NULL;
  free(str);
  return output;
}

int saveCMDs(char *input, char ****cmd){

  int numCMDs = getTokenNum(input,"|");
  char **CMDs = listSTR(input, "|");
  (*cmd) = malloc(numCMDs*sizeof(char***));
  for(int i = 0; i < numCMDs; i++){
    int numARGs = getTokenNum(CMDs[i]," ");
    char **ARGs = listSTR(CMDs[i], " ");
    (*cmd)[i] = malloc((numARGs+1) * sizeof(char**));
    for(int j = 0; j < numARGs; j++){
      (*cmd)[i][j] = strdup(ARGs[j]);
    }
    (*cmd)[i][numARGs] = NULL;
    
    for (int j = 0; j < numARGs; j++) {
      free(ARGs[j]);
    }
    free(ARGs);
  }

  for (int i = 0; i < numCMDs; i++) {
    free(CMDs[i]);
  }
  free(CMDs);
  return numCMDs;
}


int main(){

  char input[MAXCMDSIZE];
  char ***cmd;

  while(1){
    printf("shell > ");
    fgets(input,sizeof(input),stdin);
    input[strcspn(input, "\n")] = '\0';
    
    int cmdsq = saveCMDs(input,&cmd);
    
    if(strcmp(input,"exit")==0){
      break;
    }
    exec(cmd,cmdsq);

    for (int i = 0; i < cmdsq; i++) {
      for (int j = 0; cmd[i][j] != NULL; j++) {
	free(cmd[i][j]);
      }
      free(cmd[i]);
    }
    free(cmd);
  }
  
  

}
