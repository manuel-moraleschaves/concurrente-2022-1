# Diseño
 
## Diagrama de la estructura de datos
El siguiente diagrama UML muestra el diseño de la estructura de datos realizada:

![img1](./diagrama_uml.png)

Como se puede apreciar, existen 3 estructuras de datos básicas que representan las figuras, el estado del tetris y una cola para manejar los niveles. La estructura de figuras no se relaciona directamente con el estado del tetris ni con los niveles, pero el estado del tetris sí tiene relación directa con la estructura de niveles mediante un puntero al nivel inicial o base cargado desde el archivo de entrada. Además, la estructura de niveles tiene una relación a sí misma ya que al funcionar como una cola cada nodo (nivel) se relaciona con el nodo que le sigue (siguiente nivel).

Por otro lado, existen otras 3 estructuras de datos que son utilizadas para el manejo de la concurrencia:
*   Datos privados de cada hilo. Tiene una relación directa hacia la estructura de datos compartidos.
*   Datos compartidos entre todos los hilos. Tiene una relación directa hacia la estructura del estado global del tetris y también hacia la estructura de posibles jugadas o movimientos.
*   Posibles movimientos. Representa las distintas combinaciones en cuanto a la rotación y la columna de la figura del primer nivel, en otras palabras, representa las posibles jugadas de la primer figura que serán repartidas entre los hilos de ejecución. 


## Diagrama de repartición de trabajo entre hilos
La repartición del trabajo entre los hilos de ejecución se realiza mediante un mapeo cíclico, donde cada hilo efectuará el cálculo recursivo para un movimiento o jugada específica de la primera figura. Cada unidad de trabajo corresponde a una combinación específica de la rotación y la columna de la figura del primer nivel; y de forma cíclica, cada hilo tomará la siguiente unidad de trabajo en el arreglo de todas las posibles jugadas para la primera figura calculadas en el main.

Lo anterior se resume en el siguiente diagrama:

![img1](./diagrama_hilos.png)


## Descripción general
Esta solución se basa principalmente en la ejecución recursiva de una rutina que intenta encontrar una posición válida donde pueda colocar cada una de las piezas que "van cayendo", según la profundidad dada. Esto se realiza mediante fuerza bruta y a través de un algoritmo de Búsqueda en Profundidad (Depth First Search) que permite colocar todas las piezas de la secuencia en sus diferentes rotaciones y en diferentes posiciones para calcular el puntaje de la jugada en el nivel más abajo (hoja del árbol) con el fin de poder comparar dicho puntaje con el mejor puntaje que se tenga hasta el momento y poder así determinar si una jugada es mejor que la otra. En caso de haber encontrado una mejor jugada, se almacena en memoria la información del nivel bajo la estructura de una cola al insertar el nuevo nodo como un hijo o como el nodo siguiente del nivel actual, esto permite llevar el rastreo o tracking de la pieza colocada en cada nivel. El puntaje de las jugadas está basado en el cálculo de la altura mínima que tiene el tablero del tetris después de colocar una pieza.

De forma general, estos son los pasos que ejecuta el programa principal:
1. Abre y lee el archivo de entrada.
2. Carga el estado inicial del tetris: el identificador, la profundidad, el número de filas, el número de columnas, la matriz inicial, la cantidad de figuras siguientes y la secuencia de figuras.
3. Crea el nivel inicial o base, como se quiera llamar.
4. Empieza el conteo del tiempo de ejecución.
5. Inicia la ejecución recursiva del algoritmo principal (ver pseudocódigo) para solucionar el tetris.
6. Genera los archivos resultantes para cada uno de los niveles almacenados en la cola después de la ejecución.
7. Libera toda la memoria correspondiente.
8. Calcula y muestra el tiempo tardado en la ejecución.


## Pseudocódigo
A continuación se muestra el pseudocódigo de los algoritmos principales utilizados para resolver el problema:
```
Algoritmo Solve_Tetris (data)
Variables
    entero num_rotacion, num_fila
    figura_t figura
    nivel_t nuevo_nivel, nivel_actual
    datos_privados_t datos_privados
    datos_compartidos_t datos_compartidos

Inicio
    datos_privados <- data
    datos_compartidos <- datos_privados.datos_compartidos
    
    Para i <- datos_privados.num_hilo Hasta datos_compartidos.cant_jugadas Aumentar i + datos_compartidos.cant_hilos Hacer
        tetris = Clonar_Tetris(datos_compartidos.tetris)

        figura <- Obtener_Figura(tetris.secuencia_piezas[0], datos_compartidos.posibles_jugadas.num_rotacion)

        Si Columna_Valida(tetris, figura, datos_compartidos.posibles_jugadas.num_columna) Entonces
            num_fila <- Colocar_Figura(tetris, figura, datos_compartidos.posibles_jugadas.num_columna)

            Si num_fila <> -1 Entonces
                nuevo_nivel = Crear_Nivel(tetris.secuencia_piezas[indice_pieza], datos_compartidos.posibles_jugadas.num_rotacion,
                                            tetris.filas, tetris.columnas, tetris.matriz)
                
                nivel_actual <- nivel_base

                nivel_actual.siguiente <- nuevo_nivel

                Solve_Tetris_DFS(tetris, 1, nivel_base, datos_compartidos)
            Fin Si
        Fin Si
        Destruir_Tetris(tetris)
    Fin Para

Fin Algoritmo
```

```
Algoritmo Solve_Tetris_DFS (tetris, indice_pieza, nivel_base, datos_compartidos)
Variables
    entero cant_rotaciones, num_rotacion, num_columna, num_fila, altura_actual
    figura_t figura
    nivel_t nuevo_nivel, nivel_actual

Inicio
    Si indice_pieza = tetris.profundidad + 1 Entonces
        altura_actual -> Obtener_Altura(tetris)
        
        mutex_altura.bloquear
        Si altura_actual < datos_compartidos.tetris.altura_minima Entonces
            tetris.altura_minima <- altura_actual
            mutex_niveles.bloquear
            Clonar_Nivel(nivel_base, datos_compartidos.tetris.niveles, tetris.filas, tetris.columnas)
            mutex_niveles.desbloquear
        Fin Si
        mutex_altura.desbloquear
    Fin Si

    cant_rotaciones <- Obtener_Num_Rotaciones(tetris.secuencia_piezas[indice_pieza])

    Para num_rotacion <- 0 Hasta cant_rotaciones Hacer
        figura <- Obtener_Figura(tetris.secuencia_piezas[indice_pieza], num_rotacion)

        Para num_columna <- 0 Hasta tetris.columnas Hacer
            Si Columna_Valida(tetris, figura, num_columna) Entonces
                num_fila <- Colocar_Figura(tetris, figura, num_columna)

                Si num_fila <> -1 Entonces
                    nuevo_nivel = Crear_Nivel(tetris.secuencia_piezas[indice_pieza], num_rotacion,
                                              tetris.filas, tetris.columnas, tetris.matriz)
                    
                    nivel_actual <- nivel_base
                    Para i <- 0 Hasta indice_pieza Hacer
                        nivel_actual <- nivel_actual.siguiente
                    Fin Para

                    Si nivel_actual.siguiente Entonces
                        Destruir_Niveles(nivel_actual.siguiente, tetris.filas)
                    Fin Si

                    nivel_actual.siguiente <- nuevo_nivel

                    Solve_Tetris_DFS(tetris, indice_pieza + 1, nivel_base, datos_compartidos)

                    Quitar_Figura (tetris, figura, num_fila, num_columna)
                Fin Si
            Fin Si   
        Fin Para
    Fin Para

Fin Algoritmo
```