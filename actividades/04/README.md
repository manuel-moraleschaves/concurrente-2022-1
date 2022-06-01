
# CI0117 - Actividad de Clase 4

## Ejercicio 1 [prod_cons_unbounded_empty]

Comprenda el pseudocódigo visto en clase, el cual se encuentra en el archivo prod_cons_unbounded.pseudo. Modifique dicha solución para que los consumidores **no requieran de un contador de unidades consumidas**, sino que simplemente verifiquen si la cola está vacía para determinar si se deben dejar de consumir unidades.

```c
if empty(queue)
	break while
```
Realice todas las modificaciones necesarias en la lógica del main, del productor y del consumidor para lograrlo evitando que haya esperas indefinidas ni condiciones de carrera.


## Ejercicio 2 [prod_cons_unbounded_token]

Comprenda el pseudocódigo visto en clase, el cual se encuentra en el archivo prod_cons_unbounded.pseudo. Modifique dicha solución para que los consumidores **no requieran de un contador de unidades consumidas**, sino que verifiquen que haya un *token* (por ejemplo, un -1) en la cola para saber si deben de dejar de consumir unidades.
```c
consumed_unit := dequeue(queue)
if consumed_unit = -1 
	break while
```
Realice todas las modificaciones necesarias en la lógica del main, del productor y del consumidor para lograrlo evitando que haya esperas indefinidas ni condiciones de carrera.
