
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
	seeFAV(filename, fileroute);
        }
      if (strcmp(st[1], "ejecutar") == 0 && st[2] != NULL) { // para leer un favorito se usa "f"
            char *resultado = getFAV(st, filename, fileroute);
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
