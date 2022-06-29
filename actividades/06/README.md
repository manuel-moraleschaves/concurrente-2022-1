# Actividad de Clase #6

## Instrucciones generales

Las soluciones a los ejercicios de esta actividad deberán ser subidas a su repositorio personal de Github, siguiendo la siguiente estructura de folders:

```
acividades/[número de actividad]/[nombre de ejercicio]/
```

Agregue un archivo Makefile para cada uno de los ejercicios con los comandos necesarios para compilar el codigo fuente y cree un archivo .gitignore para evitar subir archivos ejecutables.

## Ejercicio 1 [pi_omp]

Una manera de aproximar el valor del número pi (π) es utilizando la siguiente fórmula:

![Pi approx formla](http://jocan3.com/misc_images/pi.png)

En dicha sumatoria, conforme más grande sea el valor de *n*, mejor es la aproximación calculada. La fórmula anterior también puede verse en código de la siguiente manera:

```c

double factor = 1.0;
double sum = 0.0;
for (k = 0; k < n; ++k) {
  sum += factor/(2*k+1);
  factor = -factor;
}

pi = 4.0 * sum;

```
Escriba un programa en C o en C++ que paralelice el código anterior utilizando directivas de *OpenMP*, de forma que varios hilos puedan ejecutar cálculos de la sumatoria de forma concurrente. Su programa debe recibir como parámetro el valor de *n* (número de iteraciones). Utilice el número de hilos por defecto. Note que en el código anterior la manera en la que el *factor* es calculado implica una dependencia de valores entre iteraciones, por lo que debe cambiar la forma en la que éste se calcula. Utilice la directiva *default(none)* para especificar el *scope* de todas las variables involucradas. El programa debe imprimir el valor aproximado de pi (muestre al menos 16 decimales). Por ejemplo:

```c
  std::cout << "pi is approximately " << std::fixed << std::setprecision(16) << pi;
```

Ejemplo de entrada/salida:

```
$ ./pi_omp 99999
pi is approximately 3.1416026536897879
```