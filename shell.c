#include "header.h"
#include "prb.c"
#include "reminder.c"

#define MAXCMDSIZE 1024

int exec(char ***cmd, int num_cmds){ //Función que toma arreglos de comandos que contienen una lista de argumentos y la cantidad de comandos y los ejecuta

  int status;
  
  int pipes[(num_cmds-1)*2];

  //Código para ver los comandos que se ejecutarán
  
  /* for (int i = 0; i < num_cmds; i++) { */
  /*   int j = 0; */
  /*   while (cmd[i][j] != NULL) { */
  /*     printf("%s ", cmd[i][j]); */
  /*     j++; */
  /*   } */
  /*   printf("\n"); */
  /* } */

  
  for (int i = 0; i < num_cmds - 1; i++) { //Creación de pipes
        if (pipe(pipes + i * 2) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }


  // Cada pipe tiene entrada y salida para un comando
  // Ejemplo
  // pipes[0] : entrada de 1er comando
  // pipes[1] : salida de 1er comando
  // pipes[2] : entrada de 2do comando
  // pipes[3] : salida de 2do comando ...
  
  
  for(int x = 0; x < num_cmds; x++){
    pid_t pid = fork(); // Creación de hijo que ejecutara el comando en la posición x
    if (pid == 0) {
      
      if(x>0){  //Duplica entradas de pipes
	dup2(pipes[(x-1)*2],STDIN_FILENO);
      }
      if(x<num_cmds-1){ //Duplica salida de pipes
	dup2(pipes[2*x+1],STDOUT_FILENO);
      }
      

      for (int i=0; i<(num_cmds-1)*2; i++){ //Cerrar pipes en hijo
	close(pipes[i]);
      }
      
      execvp(cmd[x][0],cmd[x]); //Ejecución
      perror(cmd[x][0]);	
      exit(1);
    }  
  }
  for(int i = 0; i < (num_cmds-1)*2;i++) { //Cerrar pipies en padre
    close(pipes[i]);
  }
  for (int x = 0; x < num_cmds; x++) { //Esperar hijos
    wait(&status);
  }

  return 0;
  
}

int getTokenNum(char *input, char *check){ //Función que recibe un string y cuenta los elementos separados por el string check
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

char **listSTR(char *input, char *check){ //Función que devuelve un array con strings separados por el string check

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

int saveCMDs(char *input, char ****cmd){ //Función que guarda los array comandos que contienen sus array de argumentos

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

void change_directory(char *dir) {
  if(strcmp(dir, "~")==0) {
    chdir(getenv("HOME"));
  }
  else {
    if(chdir(dir)!=0)
      perror(dir);  
  }
}


int main(int argc, char *argv[]) {
  program_name = strdup(argv[0]);
  char input[MAXCMDSIZE];
  char cwd[MAXCMDSIZE];
  char user[100];
  getlogin_r(user,100);
  char ***cmd;
  FILE *log = fopen("log.txt","a");
  chdir(getenv("HOME"));

  while(1){
    getcwd(cwd, sizeof(cwd));
    printf("\e[1;32m%s\e[0m:\e[1;35m%s\e[0m> ", user,cwd);
    fgets(input,sizeof(input),stdin);
    input[strcspn(input, "\n")] = '\0';
    fputs(input,log);
    fputs("\0",log);
    fputs("\n",log);
    int cmdsq = saveCMDs(input,&cmd);

    if(strlen(input) != 0){
      if(strcmp(cmd[0][0], "cd")==0){
	      if(cmd[0][1]!=NULL){
	        change_directory(cmd[0][1]);
	      } 
        else {
	        printf("Directorio no especificado\n");
	      }
      }
      else if(strcmp(cmd[0][0], "favs")==0){
        favourites(input);
      }
      else if(strcmp(cmd[0][0],"set")==0){
        set_recordatorio(cmd[0],getTokenNum(input," favs "));
      }
      else if(strcmp(input,"never")==0){
        never_lyrics();
      }
      else if(strcmp(input,"exit")==0){
        break;
      }
      else
        exec(cmd,cmdsq);
    }

    for (int i = 0; i < cmdsq; i++) {
      for (int j = 0; cmd[i][j] != NULL; j++) {
	      free(cmd[i][j]);
      }
      free(cmd[i]);
    }
    free(cmd);
  }
  fclose(log);
}
