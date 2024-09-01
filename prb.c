#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCMDSIZE 1024
#define MAXLINE 1024

int getTokenNum(char *input, char *check) {
    char *str = strdup(input);
    char *token = strtok(str, check);
    int TKNnum = 0;
    while (token != NULL) {
        TKNnum++;
        token = strtok(NULL, check);
    }
    free(str);
    return TKNnum;
}

char **listSTR(char *input, char *check) {
    char *str = strdup(input);
    int numTKN = getTokenNum(input, check);
    int index = 0;
    char **output = malloc((numTKN + 1) * sizeof(char *));
    char *token = strtok(str, check);
    while (token != NULL) {
        while (*token == ' ') {
            token++;
        }

        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ') {
            end--;
        }
        *(end + 1) = '\0';
        output[index++] = strdup(token);
        token = strtok(NULL, check);
    }
    output[numTKN] = NULL;
    free(str);
    return output;
}

void setFAV(char **input, int largo, const char *archivotexto) {
    FILE *archivo = fopen(archivotexto, "a");

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo :(\n");
        return;
    }
    for (int i = 1; i < largo; i++) {
        fprintf(archivo, "%s", input[i]);
        if (i < largo - 1) {
            fprintf(archivo, " ");
        }
    }
    fprintf(archivo, "\n");
    fclose(archivo);
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

int main() {
    char input[MAXCMDSIZE];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;  // Elimina el salto de línea al final de la entrada
    printf("%s\n", input);

    int esp = getTokenNum(input, " ");
    char **st = listSTR(input, " ");
    const char *nombreArchivo = "archivo.txt";

    if (strcmp(st[0], "fav") == 0) { // para guardar en favoritos se usa "fav"
        setFAV(st, esp, nombreArchivo);
    } else if (strcmp(st[0], "f") == 0) { // para leer un favorito se usa "f"
        char *resultado = getFAV(st, nombreArchivo);
        if (resultado != NULL) {
            printf("Resultado: %s\n", resultado);
            free(resultado);
        }
    }

    // Liberar la memoria asignada para la lista de cadenas
    for (int i = 0; i < esp; i++) {
        free(st[i]);
    }
    free(st);

    return 0;
}