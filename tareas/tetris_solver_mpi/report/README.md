# Documento de reporte
.
.
.
.

 
## Comparación de rendimiento
Para cada Tarea se efectuaron 3 corridas y en cada una de ellas se capturó la medición del tiempo que duró su ejecución y se registró en la siguiente [hoja de cálculo](./comparacion_rendimiento.xlsx).

Una vez que se capturaron esos datos, se realizó el cálculo de las siguientes métricas:

* **Speedup *S* (incremento de velocidad)**: se calcula como la relación entre el tiempo que tarda una computación previa a la optimización (*Tbefore*), contra el tiempo que tarda la misma computación posterior a la optimización (*Tafter*). En este caso corresponde al tiempo de ejecución serial (antes) respecto al tiempo de ejecución posterior a la paralización (después).

![img01](./formula_speedup.png)

* **Eficiencia *E***: es una relación entre el incremento de velocidad y la cantidad de trabajadores (*w*) que tuvieron que involucrarse para conseguir el incremento del desempeño.

![img02](./formula_eficiencia.png)

\
