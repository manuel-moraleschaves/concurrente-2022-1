# CI0117 - Actividad de clase 3

### Instrucciones generales

Las soluciones al ejercicio de esta actividad deberán ser subidas a su repositorio personal de Github, siguiendo la siguiente estructura de folders:

```
acividades/[número de actividad]/[nombre de ejercicio]/
```
Agregue un archivo *Makefile* para cada uno de los ejercicios con los comandos necesarios para compilar el codigo fuente y cree un archivo .gitignore para evitar subir archivos ejecutables.

## Ejercicio 1 [parking_sem]

Escriba un programa en C que simule un parqueo donde la capacidad máxima de vehículos es 3. Cada hilo va a representar un vehículo el cual imprimirá 3 mensajes diferentes:
- Cuando esté esperando entrar al parqueo.
- Cuando haya entrado al parqueo y se estacione.
- Cuando salga del parqueo.

Asegúrese de que el segundo mensaje (cuando el vehículo esté estacionado) se imprima únicamente si hay espacio en el parqueo y para ello utilice la biblioteca *<semaphore.h>* y las diferentes funciones vistas en clase para simular el ingreso al parqueo.

Opcionalmente, puede utilizar algún mecanismo de C para colocar un pequeño *sleep* aleatorio luego de que el vehículo estacione y simular la permanencia del vehículo en el parqueo. Por ejemplo, el siguiente código realiza un "sleep" de 0 a 10 segundos:

```c
    int duration = rand() % 10;
    sleep(duration);
```

Ejemplo de salida esperada:
```
car 0/4: waiting to enter the parking
car 0/4: entered the parking
car 1/4: waiting to enter the parking
car 1/4: entered the parking
car 3/4: waiting to enter the parking
car 3/4: entered the parking
car 2/4: waiting to enter the parking
car 0/4: left the parking
car 2/4: entered the parking
car 1/4: left the parking
car 3/4: left the parking
car 2/4: left the parking
```
**Sugerencia:** Puede basarse en los ejemplos de semáforos vistos en las últimas clases.
