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

bool copySEARCH(FILE *archivo, char **comando) {

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
    return save;
}

void savelogFAV(FILE *archivolog, FILE *archivofavs){

    if( archivofavs == NULL || archivolog == NULL){
        printf("archivo nulo, no se pudo guardar");
        return;
    } 

    char linealog[MAXLINE], lineafavs[MAXLINE];
    while(fgets(linealog, MAXLINE, archivolog)){
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

void setFAV(char **input, int largo, FILE *archivo) {
    if (!copySEARCH(archivo, input)) {
      long original_position = ftell(archivo);
      // Escribir el comando en el archivo
      fseek(archivo, 0, SEEK_END);
      for (int i = 2; i < largo; i++) {
	fprintf(archivo, "%s", input[i]);
	if (i < largo - 1) {
	  fprintf(archivo, " ");
	}
      }
      fprintf(archivo, "\n");

      fseek(archivo, original_position, SEEK_SET);
      
    } else {
      printf("Comando ya guardado\n");
    }
}

void seeFAV(FILE *archivo){

    int numlinea = 1;
    char linea[MAXLINE];
    while(fgets(linea, MAXLINE, archivo)){
        printf("%d: %s\n", numlinea, linea);
        numlinea++;
    }
}

void delFAV(FILE *archivo){ //borra todos los comandos de los favoritos
    int fd = fileno(archivo);
    if (ftruncate(fd, 0) != 0) {
        perror("Error al eliminar contenido");
    }
}

char *getFAV(char **input, FILE *archivo) {
    
    int num_comando = atoi(input[2]); // Convertir la cadena a un número entero
    char *linea = malloc(MAXLINE);
    if (linea == NULL) {
        printf("Error al asignar memoria.\n");
        fclose(archivo);
        return NULL;
    }

    int contador = 1;
    while (fgets(linea, MAXLINE, archivo) != NULL) {
        if (contador == num_comando) {
            // Eliminar el salto de línea al final, si existe
            linea[strcspn(linea, "\n")] = 0;
            fclose(archivo);
            return linea;
        }
        contador++;
    }

    printf("Usted posee menos de %d comando favoritos.\n", num_comando);
    free(linea);
    return NULL;
}

int favourites(char *inputcmd, FILE *archivo, FILE *log) {
    //strdup
    char input[MAXCMDSIZE];
    strncpy(input, inputcmd, sizeof(input) - 1);
    input[strcspn(input, "\n")] = 0;  // Elimina el salto de línea al final de la entrada

    int esp = getTokenNum(input, " ");
    char **st = listSTR(input, " ");


    if (strcmp(st[0], "favs") == 0 && st[1] != NULL){
      
      if(strcmp(st[1], "guardar") == 0) { // para guardar en favoritos se usa "favs guardar"
	if(st[2] != NULL){
	  setFAV(st, esp, archivo);
	}
	else{
	  savelogFAV(archivo, log);
	}
      }
      if(strcmp(st[1], "cargar") == 0){// para imprimir los favoritos guardados en terminal
            seeFAV(archivo);
        } 
      if(strcmp(st[1], "borrar") == 0){
            delFAV(archivo);
        }
      if(strcmp(st[1], "mostrar") == 0){
            seeFAV(log);
        }
      if (strcmp(st[1], "ejecutar") == 0 && st[2] != NULL) { // para leer un favorito se usa "f"
            char *resultado = getFAV(st, archivo);
            if (resultado != NULL) {

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
