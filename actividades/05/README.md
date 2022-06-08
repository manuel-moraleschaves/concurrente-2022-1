
# CI0117 - Actividad de Clase 5

### Instrucciones generales

Las soluciones a los ejercicios de esta actividad deberán ser subidas a su repositorio personal de Github, siguiendo la siguiente estructura de folders:

```
acividades/[número de actividad]/[nombre de ejercicio]/
```
Agregue un archivo *Makefile* para cada uno de los ejercicios con los comandos necesarios para compilar el codigo fuente y cree un archivo .gitignore para evitar subir archivos ejecutables.

## Ejercicio 1 [position_race_condvar]

Comprenda el programa [position_race_barrier](https://github.com/jocan3/CI0117-2022-S1/tree/main/ejemplos/11_pthreads/position_race_barrier) visto en clase, y modifíquelo de manera que en lugar de utilizar una barrera utilice una **variable de condición** para controlar que todos los hilos *corredores* salgan al mismo tiempo de la línea de salida.

Cree las variables adicionales necesarias para que se logre la misma funcionalidad: Cualquiera que sea la cantidad de hilos con los que se ejecute el programa, éste debería imprimir de forma correcta que los hilos están esperando antes de que empiecen a *correr*.

**Sintaxis:**

```c
    pthread_cond_t condvar;
```
```c
    pthread_cond_init(&condvar, NULL);
```
```c
    pthread_cond_destroy(&condvar);
```
```c
    pthread_cond_wait(&condvar, &mutex);
```
```c
    pthread_cond_signal(&condvar);
```
```c
    pthread_cond_broadcast(&condvar);
```

## Ejercicio 2 [prod_cons_unbounded_rwlock]

Haga una copia del programa [producer_consumer_unbound](https://github.com/jocan3/CI0117-2022-S1/tree/main/ejemplos/10_pthreads/producer_consumer_unbound) visto en clase y asegúrese de comprenderlo, compilarlo y ejecutarlo. 

Modifique los archivos **Queue.h** y **Queue.c** de forma que en lugar de un *mutex* utilicen candados de lectura y escritura en todos los lugares donde sea requerido y de manera correcta.

Luego de hacer el cambio, ejecute el programa y asegúrese de que se mantiene la misma funcionalidad. De igual forma verifique que no haya condiciones de carrera ni problemas de memoria utilizando los sanitizers y *memcheck*.

**Sintaxis:**

```c
    pthread_rwlock_t rwlock;
```
```c
    pthread_rwlock_init(&rwlock, NULL);
```
```c
    pthread_rwlock_destroy(&rwlock);
```
```c
    pthread_rwlock_rdlock(&rwlock);
```
```c
    pthread_rwlock_wrlock(&rwlock);
```
```c
    pthread_rwlock_unlock(&rwlock);
```
