# Diseño
 
## Diagrama de la estructura de datos

El siguiente diagrama UML muestra el diseño de la estructura de datos realizada:

![img1](/design/diagrama_uml.png)

## Descripción general
Pasos para resolver el problema:
1. Abrir y leer el archivo de entrada.
2. Cargar el identificador, la profundidad, el número de filas, el número de columnas, la matriz inicial, la cantidad de figuras siguientes y la secuencia de figuras.
3. Para la primera figura obtener sus posibles rotaciones y para cada columna intentar colocar la pieza en su primera rotación y luego continuar con las otras posibles rotaciones. Una vez colocada, hacer lo mismo para la siguiente figura según la secuencia.
4. Una vez colocadas todas las figuras, calcular el puntaje en el nivel más bajo del árbol. Comparar el puntaje calculado contra el puntaje más alto hasta el momento y si es mayor entonces almacenarlo como el nuevo mejor puntaje y también la matriz.
5. Al terminar de recorrer todas las posibilidades y tener la mejor jugada posible, se debe crear un archivo de salida para cada nivel. Para esto se debe guardar la pieza insertada y la rotación, así como su posición en la matriz.

## Pseudocódigo
A continuación se muestra el pseudocódigo del algoritmo principal utilizado para resolver el problema:
```
Algoritmo Resolver_Tetris_DFS (tetris, secuencia_piezas, indice_pieza)
Variables
    entero cant_rotaciones, num_rotacion, num_columna, num_fila, puntaje_actual
    figura_t figura

Inicio
    Si indice_pieza = tetris.profundidad + 1 Entonces
        puntaje_actual -> Obtener_Puntaje (tetris)
        
        Si puntaje_actual < puntaje_minimo Entonces
            puntaje_minimo <- puntaje_actual
            tetris_final = tetris
        Fin Si
    Fin Si

    cant_rotaciones <- Obtener_Num_Rotaciones (secuencia_piezas[indice_pieza])

    Para num_rotacion <- 0 Hasta cant_rotaciones Hacer
        figura <- Obtener_Figura (secuencia_piezas[indice_pieza], num_rotacion)

        Para num_columna <- 0 Hasta tetris.columnas Hacer
            Si Posicion_Valida (tetris, figura, num_columna) Entonces
                num_fila <- Colocar_Figura (tetris, pieza, num_columna)
                Resolver_Tetris_DFS (tetris, secuencia de piezas, indice_pieza + 1)                
                Quitar_Figura (tetris, figura, num_fila, num_columna)
            Fin Si   
        Fin Para
    Fin Para

Fin Algoritmo
```