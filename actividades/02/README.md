# CI0117 - Actividad de clase 2

Para esta actividad, suba las soluciones a su repositorio personal de Github, siguiendo la siguiente estructura de folders:

```
/acividades/[número de actividad]/[nombre de ejercicio]/
```

Utilice como base el ejemplo de [hola mundo múltiple con memoria compartida](https://github.com/jocan3/CI0117-2022-S1/tree/main/ejemplos/03_pthreads/hello_multiple_shared) visto durante la clase. Utilice también el archivo Makefile de ese mismo ejemplo.

## Ejercicio 1 [hello_multiple_message]

### Parte 1
- En un archivo hello_multiple_message.c cree un programa que realice lo siguiente:
  - Cada hilo tendrá en su memoria privada una cadena de caracteres de tamaño 6:
  ```c
   char message[6];
  ```
  - La subrutina de cada hilo deberá escribir un mensaje en su memoria privada dependiendo de si su número de hilo es par o impar. Si es par entonces escribe "hello". Si es impar, escribe "world". Puede utilizar la función *sscanf* para escribir el mensaje:
  ```c
   sscanf("hello", "%s", message);
  ```
  - Cada hilo imprime su número de hilo y mensaje:
  ```
   0: hello
   1: world
   2: hello
  ```
- Asegúrese de compilar y ejecutar el código en su máquina de forma individual utilizando el Makefile dado por el profesor. Su código debe cumplir con todas las validaciones del linter *cpplint*:
  ```
  make lint
  ```
- Ejecute su programa usando valgrind: 
  ```
  make memcheck
  ```
- Ejecute su programa utilizando los ejecutables de los *satinizers* generados al compilar: msan, asan y tsan.
 
- Discuta junto a su grupo de trabajo lo siguiente:
  - ¿Los hilos imprimen sus mensajes en orden?
    - R/ No, lo hace de forma desordenada debido al indeterminismo.
  - ¿Alguno de los sanitizers muestra algún mensaje de error o warning? ¿Por qué sí o por qué no?
    - R/ No, porque las escrituras se hacen a la data privada.
- Anote las respuestas a las preguntas anteriores, las cuales serán discutidas luego de la activdad.


### Parte 2
- Agrege al programa una variable mensaje que será compartido entre todos los hilos.
- Haga que cada hilo también copie el respectivo mensaje ("hello" o "world") en el mensaje compartido.
- Haga que el hilo principal imprima el mensaje compartido luego de que todos los hilos hayan terminado su ejecución. 
- Asegúrese de compilar el archivo, validar el *linter*, *memcheck* y los *sanitizers*.

- Discuta junto a su grupo de trabajo lo siguiente:
  - ¿Cuál es el valor del mensaje compartido? ¿Es siempre el mismo en cada ejecución?
    - R/ Se imprime el mensaje hello o world y debido al indeterminismo varía dependiendo de cuál hilo fue el último que realizó la escritura.
  - ¿Alguno de los sanitizers muestra algún mensaje de error o warning? ¿Por qué sí o por qué no?
    - R/ El msan y el asan no, pero el tsan sí: una condición de carrera porque se intenta escribir en la misma dirección de memoria.

- Anote las respuestas a las preguntas anteriores, las cuales serán discutidas luego de la activdad.

**Pregunta adicional**: ¿Qué se podría hacer para que las palabras "hello" y "world" siempre se impriman intercaladas?
  - Se podría utilizar algún método de control, de serialización, semáforos o algún sleep.
