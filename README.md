# OsShell

Para lograr compilar y correr correctamente nuestro codigo de la shell se debe compilar con el comando clasico de  
**gcc shell.c -o (nombre archivo de salida)**, 
y para correrlo se debe escribir en la terminal **./(nombre archivo de salida)**

De ahí en adelante la shell funciona casi igual que la terminal clasica de linux, pero mejorada, ya que tenemos los comandos de tipo favs, que son los creados por nosotros, y los comandos de los recordatorios, 
los cuales son utiles en caso de necesitar poner una alarma.

Para el uso de los comandos de tipo favorito se tiene que escribir la palabra favs, seguido del comando que se quiere ejecutar. Los comandos que hay son mostrar, cargar, borrar, guardar y guardar (comando), crear,
y ejecutar; pero para poder ejecutar dichos comandos se requiere tener un archivo de favoritos ya creados y estar en la carpeta donde se creo el archivo de favoritos, salvo para el comando mostrar, 
ya que este trabaja sobre nuestro archivo "log.txt", el cual hace de cache para los comando ingresados por el usuario. En el de querer setear un recordatorio lo que se tiene que escribir **set recordatorio (tiempo) (mensaje)**, 
y este se creara y luego de dicho tiempo desplegara el mensaje en la terminal. Cabe destacar que se puede tener mas de un recordatorio activo a la vez y que imprimen hace cuanto tiempo fue que se solicito crearlo. 
Y por ultimo, en caso de querer cerrar la terminal se puede hacer con el comando exit.
