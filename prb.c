#include "header.h"

#define MAXCMDSIZE 1024
#define MAXLINE 1024


int setFAV(char **input, int largo, const char *archivotexto) {
    FILE *archivo = fopen(archivotexto, "a");

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo :(\n");
        return 1;
    }
    for (int i = 2; i < largo; i++) {
        fprintf(archivo, "%s", input[i]);
        if (i < largo - 1) {
            fprintf(archivo, " ");
        }
    }
    fprintf(archivo, "\n");
    fclose(archivo);

    return 0;
}

void seeFAV(const char *archivotexto){
    FILE *archivo = fopen( archivotexto, "r");

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo :(\n");
        return;
    }
    int numlinea = 1;
    char linea[MAXLINE];
    while(fgets(linea, MAXLINE, archivo)){
        printf("%d: %s", numlinea, linea);
        numlinea++;
    }
    fclose(archivo);
}

void delFAV(const char *archivotexto){ //borra todos los comandos de los favoritos
    FILE *archivo = fopen( archivotexto, "w");

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo :(\n");
        return;
    }
    fprintf(archivo, "");
}

char *getFAV(char **input, const char *archivotexto) {
    FILE *archivo = fopen(archivotexto, "r");
    if (archivo == NULL) {
        printf("No se pudo leer el archivo :(\n");
        return NULL;
    }
    
    int num_comando = atoi(input[1]); // Convertir la cadena a un número entero

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
    fclose(archivo);
    return NULL;
}

int favourites(char *inputcmd) {
    //strdup
    char input[MAXCMDSIZE];
    strncpy(input, inputcmd, sizeof(input) - 1);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;  // Elimina el salto de línea al final de la entrada

    int esp = getTokenNum(input, " ");
    char **st = listSTR(input, " ");
    const char *nombreArchivo = "archivo.txt";

    if (strcmp(st[0], "favs") == 0 && st[1] != NULL){
        if(strcmp(st[1], "guardar") == 0) { // para guardar en favoritos se usa "favs guardar"
            setFAV(st, esp, nombreArchivo);
        }
        if(strcmp(st[1], "mostrar") == 0){// para imprimir los favoritos guardados en terminal
            seeFAV(nombreArchivo);
        } 
        if(strcmp(st[1], "borrar") == 0){
            delFAV(nombreArchivo);
        }
        if (st[2] != NULL && strcmp(st[2], "ejecutar") == 0) { // para leer un favorito se usa "f"
            char *resultado = getFAV(st, nombreArchivo);
            if (resultado != NULL) {
                printf("Resultado: %s\n", resultado);
                free(resultado);
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
