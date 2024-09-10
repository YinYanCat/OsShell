#include "header.h"

#define MAXCMDSIZE 1024
#define MAXLINE 1024


// Función auxiliar para contar elementos en un arreglo de punteros (asumiendo terminación con NULL)
int contarElementos(char **arr) {
    int count = 0;
    while (arr[count] != NULL) {
        count++;
    }
    return count;
}


bool copySEARCH(const char *archivotexto, char* fileroute, char **comando) {
    
    char lastdir[PATH_MAX];
    getcwd(lastdir,sizeof(lastdir));
    change_directory(fileroute);
    FILE *archivo = fopen(archivotexto, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return false;
    }

    bool save = false;
    char linea[MAXLINE];  // Corregido: Es un arreglo de char, no un puntero a puntero
    char comandostr[MAXCMDSIZE];  // Corregido: Usar tamaño fijo para el string concatenado
    
    // Inicializar comandostr
    comandostr[0] = '\0'; // Asegurarse de que está vacío
    for (int i = 0; i < contarElementos(comando); i++) {
        strcat(comandostr, comando[i]);
        if (i < contarElementos(comando) - 1) {
            strcat(comandostr, " ");
        }
    }

    while (fgets(linea, MAXLINE, archivo)) {
        // Eliminar el salto de línea al final de la línea leída
        linea[strcspn(linea, "\n")] = '\0';

        if (strcmp(linea, comandostr) == 0) {
            save = true;
            break;
        }
    }

    fclose(archivo);
    change_directory(lastdir);
    return save;
}

void savelogFAV(char *filename, char* fileroute, FILE *archivolog){
  
  char lastdir[PATH_MAX];
  getcwd(lastdir,sizeof(lastdir));
  change_directory(fileroute);
  FILE *archivofavs = fopen(filename, "a");
  rewind(archivolog);
  
  if( archivofavs == NULL || archivolog == NULL){
    printf("archivo nulo, no se pudo guardar");
    return;
  } 

  char linealog[MAXLINE], lineafavs[MAXLINE];
  while(fgets(linealog, MAXLINE, archivolog)){
    if(strcmp(listSTR(linealog, " ")[0], "favs") == 0 || strcmp(linealog, "")){
      continue;
    }
    else{
      int secopia = 0;
      rewind(archivofavs);
      while(fgets(lineafavs, MAXLINE, archivofavs)){
        if(strcmp(linealog, lineafavs) == 0){
          secopia = 1;
          break;
        }
      }
      if(secopia == 0){
        fprintf(archivofavs, "%s", linealog);
      }
    }
  }
  fclose(archivofavs);
  rewind(archivolog);
  change_directory(lastdir);
}

void setFAV(char **input, int largo, const char *archivotexto, char* fileroute) {
  if (!copySEARCH(archivotexto,fileroute, input)) {
      char lastdir[PATH_MAX];
      getcwd(lastdir,sizeof(lastdir));
      change_directory(fileroute);
      FILE *archivo = fopen(archivotexto, "a");
      
      if (archivo == NULL) {
	printf("No se pudo abrir el archivo :(\n");
	return;
      }
      
      // Escribir el comando en el archivo
      for (int i = 2; i < largo; i++) {
	fprintf(archivo, "%s", input[i]);
	if (i < largo - 1) {
	  fprintf(archivo, " ");
	}
      }
      fprintf(archivo, "\n");
      fclose(archivo);
      change_directory(lastdir);
      
    } else {
      printf("Comando ya guardado\n");
    }
}

void seeFAV(const char *archivotexto, char* fileroute){
  char lastdir[PATH_MAX];
  getcwd(lastdir,sizeof(lastdir));
  change_directory(fileroute);
  FILE *archivo = fopen( archivotexto, "r");
  
  if (archivo == NULL) {
    printf("No se pudo abrir el archivo :(\n");
    return;
  }
  int numlinea = 1;
  char linea[MAXLINE];
  while(fgets(linea, MAXLINE, archivo)){
    printf("%d: %s\n", numlinea, linea);
        numlinea++;
  }
  fclose(archivo);
  change_directory(lastdir);
}

void seeLOG(FILE *log){  
  if (log == NULL) {
    printf("Archivo nulo:(\n");
    return;
  }
  rewind(log);
  int numlinea = 1;
  char linea[MAXLINE];
  while(fgets(linea, MAXLINE, log)){
    printf("%d: %s\n", numlinea, linea);
      numlinea++;
  }
  rewind(log);
}

void delFAV(const char *archivotexto, char* fileroute){ //borra todos los comandos de los favoritos
  char lastdir[PATH_MAX];
  getcwd(lastdir,sizeof(lastdir));
  change_directory(fileroute);
  
  if (remove(archivotexto) != 0){
    printf("No se pudo borrar el archivo\n");
  }
  change_directory(lastdir);
}


char *getFAV(char **input, const char *archivotexto, char* fileroute) {
  char lastdir[PATH_MAX];
  getcwd(lastdir,sizeof(lastdir));
  change_directory(fileroute);
  FILE *archivo = fopen(archivotexto, "r");
  if (archivo == NULL) {
    printf("No se pudo leer el archivo :(\n");
    change_directory(lastdir);
    return NULL;
  }
    
  int num_comando = atoi(input[2]); // Convertir la cadena a un número entero
  char *linea = malloc(MAXLINE);
  if (linea == NULL) {
    printf("Error al asignar memoria.\n");
    fclose(archivo);
    change_directory(lastdir);
    return NULL;
  }
  
  int contador = 1;
    while (fgets(linea, MAXLINE, archivo) != NULL) {
      if (contador == num_comando) {
	// Eliminar el salto de línea al final, si existe
	linea[strcspn(linea, "\n")] = 0;
	fclose(archivo);
	change_directory(lastdir);
	return linea;
      }
      contador++;
    }
    
    printf("Usted posee menos de %d comando favoritos.\n", num_comando);
    free(linea);
    fclose(archivo);
    change_directory(lastdir);
    return NULL;
}

int favourites(char *inputcmd, char* filename, char* fileroute, FILE *log) {
  //strdup
  char input[MAXCMDSIZE];
    strncpy(input, inputcmd, sizeof(input) - 1);
    input[strcspn(input, "\n")] = 0;  // Elimina el salto de línea al final de la entrada
    
    int esp = getTokenNum(input, " ");
    char **st = listSTR(input, " ");


    if (strcmp(st[0], "favs") == 0 && st[1] != NULL){
      
      if(strcmp(st[1], "guardar") == 0) { // para guardar en favoritos se usa "favs guardar"
	if(st[2] != NULL){
	  setFAV(st, esp, filename, fileroute);
	}
	else{
	  savelogFAV(filename, fileroute, log);
	}
      }
      if(strcmp(st[1], "cargar") == 0){// para imprimir los favoritos guardados en terminal
            seeFAV(filename, fileroute);
        } 
      if(strcmp(st[1], "borrar") == 0){
            delFAV(filename, fileroute);
        }
      if(strcmp(st[1], "mostrar") == 0){
        seeLOG(log);
        }
      if (strcmp(st[1], "ejecutar") == 0 && st[2] != NULL) { // para leer un favorito se usa "f"
            char *resultado = getFAV(st, filename, fileroute);
            if (resultado != NULL) {
              char ***cmd2;
              int size = saveCMDs(resultado,&cmd2);
              exec(cmd2, size);
              for (int i = 0; i < size; i++) {
                for (int j = 0; cmd2[i][j] != NULL; j++) {
                  free(cmd2[i][j]);
                }
                free(cmd2[i]);
              }
              free(cmd2);
            }
        }
    }
    else if(st[1] == NULL){
        printf("Instrucción mal entregada, intente otra vez\n");
    }
    
    // Liberar la memoria asignada para la lista de cadenas
    for (int i = 0; i < esp; i++) {
      free(st[i]);
    }
    free(st);
    
    return 0;
}


//Documents/SO/OsShell 
