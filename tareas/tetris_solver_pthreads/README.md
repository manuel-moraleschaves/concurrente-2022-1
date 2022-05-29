# Tarea01: [tetris_solver_serial]
 
## Descripción del problema

Tetris es un video juego tradicional en el cual se tiene un área de juego que consiste en una cuadrícula de tamaño NxM (comúnmente se utiliza 20x10). Hay siete figuras geométricas diferentes, conocidas como *tetriminós* que están compuestas por cuatro bloques cuadrados unidos entre sí. Durante una partida de tetris, se van generando figuras de forma aleatoria, una por una, desde la parte superior del área de juego y éstas van cayendo hacia la parte inferior. La siguiente figura no se genera hasta que la actual figura no haya caído completamente.

Cada uno de los tetriminós se representa con una letra en mayúscula y posee cierta cantidad de rotaciones posibles:

![figures](http://jocan3.com/misc_images/tetris_figures.png)

Los jugadores pueden decidir la rotación de la pieza (0°, 90°, 180°, 270°) y en qué lugar debe caer. Cuando una línea horizontal se completa, esa línea desaparece y todas las piezas que están por encima descienden una posición, liberando espacio de juego y por tanto facilitando la tarea de situar nuevas piezas. Cada línea completa le da cierta cantidad de puntos al jugador, dependiendo del nivel y cuántas líneas se completaron en *combo*. El jugador también puede ver cuáles serán las siguientes K figuras (comúnmente seis figuras) y planear la posición basándose en las mismas.

Cuando las piezas se amontonan y colisionan con la parte superior del tablero, el juego se termina. Puede [jugarlo online](https://tetris.com/play-tetris) o ver un [gamepley](https://www.youtube.com/watch?v=QQ5U-rN7Veg) para entender mejor la dinámica.

Esta tarea consiste en crear un solucionador de tetris utilizando el lenguage de programación C que determinará la mejor posición y la rotación de una figura de tetris en un momento dado. El programa recibe un estado de juego de tetris en un archivo con el siguiente formato:

- La línea 1 contiene un número que representa un identificador del estado del juego.
- La línea 2 contiene un número entero que representa la **profundidad** a calcular (explicación más adelante).
- La línea 3 contiene un número entero N que representa la cantidad de filas del área de juego.
- La línea 4 contiene un número entero M que representa la cantidad de columnas del área de juego.
- Las siguientes N líneas contienen cadenas de M caracteres que representan el área de juego. En el área de juego, un '0' significa un cuadro vacío, mientras que cualquier otro dígito representa un bloque ocupado.
- La siguiente línea contiene un número entero K que representa la cantidad de figuras "siguientes".
- Las últimas K líneas contienen un caracter representando alguno de las siete figuras diferentes. El primer caracter corresponde a la figura que cayendo en ese momento y a la cual se debe calcular la posición.

Ejemplo de entrada:

```
1650259796944
1
20
10
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0001111000
0000040000
0004440000
0000700000
0007770000
0002200000
0020220000
0220066000
0777742240
7
I
Z
I
J
S
L
S
```

Con respecto a la **profundidad**, ésta determina la cantidad de piezas a considerar para la jugada. Por ejemplo, si la profundidad es 0 entonces solo se toma en cuenta la primera figura. Si la profundidad es 1, entonces se toman en cuenta la primera y segunda figura, y así sucesivamente.

El cálculo de la jugada se realiza mediante *fuerza bruta* con un algoritmo de [búsqueda en profundidad](https://es.wikipedia.org/wiki/B%C3%BAsqueda_en_profundidad). Dependiendo del nivel, para cada rotación y posición de una figura, se calculan también todas las combinaciones posibles de la siguiente. Por ejemplo, para la entrada mostrada anteriormente, dado que el nivel es 1 se consideran las primeras 2.

No se implementan jugadas "inteligentes" donde una figura se gira a último momento para que quepa en un determinado espacio, sino que se asume que la figura se rota desde el inicio. Tampoco se calculan ni *limpian* las líneas que se completan.

Cada vez que se coloca una pieza, se calcula un puntaje en los niveles más bajos (hojas del árbol) que indica "qué tan bueno" es el estado del juego en ese momento. Dicho cálculo se realiza según lo explicado en: [Tetris: A Heuristic Study](http://kth.diva-portal.org/smash/get/diva2:815662/FULLTEXT01.pdf) o [Heuristic AI for playing tetris](https://github.com/takado8/Tetris).

Una vez que se han recorrido todas las combinaciones posibles y calculado un puntaje para cada una, se guarda la mejor posición y rotación para cada figura.

Por cada nivel de profundidad, se genera un archivo "tetris_play_{nivel}.txt" que contiene:
- En la primera linea el identificador del estado del juego (el mismo recibido en el archivo de entrada).
- En la segunda línea la letra que representa la figura colocada en dicho nível.
- En la tercera línea el número que representa la rotación aplicada (0, 1, 2, ó 3).
- En la cuarta línea, el número de filas N del área de juego.
- En la quinta línea, el número de columnas M del área de juego.
- Y en las siguientes N líneas, hileras de M caracteres representando cada una de las filas del área de juego del estado original más las figuras del nivel actual y los anteriores colocadas en la mejor posición encontrada.

Ejemplo de salida esperada (tetris_play_0.txt):

```
1650259796944
I
1
20
10
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0001111000
0000040000
0004440000
0000700000
0007770000
1002200000
1020220000
1220066000
1777742240
```
Ejemplo de salida esperada (tetris_play_1.txt):
```
1650259796944
Z
0
20
10
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0001111000
0000040000
0004440000
0000700000
0007770000
1002200000
1020220220
1220066022
1777742240
```
Si para un estado dado, el programa no pudo encontrar del todo una posición valida para colocar la figura correspondiente al nivel, entonces **no** se genera ningún archivo de salida para dicho nivel ni para los siguientes.

El cálculo del mejor puntaje o jugada se realiza de forma concurrente utilizando hilos de ejecución que se reparten el trabajo. Dicha repartición de trabajo se realiza mediante un mapeo cíclico.

## Manual de uso
Para compilar el programa se puede utilizar el makefile incluido de la siguiente manera:

- Compilar normal
```
make
```

- Compilar asan
```
make asan
```

- Compilar msan
```
make msan
```

- Compilar tsan
```
make tsan
```

- Compilar ubsan
```
make ubsan
```

Para ejecutar el programa se debe enviar por parámetro la ruta del archivo de pruebas que se quiere utilizar y la cantidad de hilos que se desean crear. Se debe respetar la siguiente estructura:
```
./bin/tetris_solver_pthreads ruta_archivo cantidad_hilos
```

Ejemplo:
```
./bin/tetris_solver_pthreads ./test/Test3.txt 2
```

En caso de ejecutar el programa sin enviar el último parámetro se utilizará una cantidad de hilos igual a la cantidad de cores que posea la máquina donde se está ejecutando y en caso de no enviar ningún parámetro el programa tomará por default el archivo ./test/Test1.txt.
\
\
También, se pueden utilizar los siguientes comandos adicionales:

- Linter
```
make
make lint
```

- Memchek
```
make
make memcheck
```

- Helgrind
```
make
make helgrind
```

Finalmente, el programa creará los archivos de salida correspondientes según lo mencionado anteriormente para cada uno de los niveles en los que se colocó una figura.


## Créditos
Desarrollado por **Manuel Morales Chaves**.  
<manuel.moraleschaves@ucr.ac.cr>
\
\
Recursos consultados/utilizados:
- [File Input and Output in C](https://www.cs.utah.edu/~germain/PPS/Topics/C_Language/file_IO.html).

- [Depth First Search or DFS for a Graph](https://www.geeksforgeeks.org/depth-first-search-or-dfs-for-a-graph/).

- [Depth First Search](https://iq.opengenus.org/depth-first-search/).
