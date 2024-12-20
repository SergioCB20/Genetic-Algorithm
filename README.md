# Descripción del problema

El problema consiste en optimizar el costo de transporte de productos desde diferentes nodos de origen hasta destinos específicos, teniendo en cuenta las demandas y los inventarios en cada nodo. El objetivo es distribuir los productos de la manera más eficiente en términos de costos, utilizando un algoritmo genético para encontrar la mejor solución.

Cada nodo de origen tiene un inventario limitado, y cada nodo de destino tiene una demanda específica. El costo de transporte entre los nodos de origen y destino está dado por una matriz de costos. El algoritmo genético se emplea para encontrar la mejor asignación de productos desde los nodos de origen a los de destino, de modo que se minimicen los costos totales de transporte, asegurando que se satisfagan todas las demandas y que no se excedan los inventarios disponibles.

## Enfoque:
1. **Representación cromosómica**: Cada solución posible es representada por un cromosoma, que describe la asignación de productos desde los nodos de origen hacia los destinos.
2. **Selección**: Se seleccionan los cromosomas con mayor fitness (es decir, los que tienen menor costo de transporte) para ser reproducidos.
3. **Crossover (recombinación)**: Los cromosomas seleccionados se combinan para generar nuevos cromosomas con características de ambos padres.
4. **Mutación**: Se realizan pequeñas modificaciones en los cromosomas generados para evitar que el algoritmo se estanque en soluciones subóptimas.
5. **Evaluación**: El fitness de cada cromosoma se evalúa utilizando la función `calcularFitness`, que determina el costo de transporte total para una solución dada.

El algoritmo continúa iterando hasta alcanzar un número máximo de generaciones o hasta que no se observe mejora en las soluciones, logrando así una distribución eficiente de productos con el menor costo de transporte posible.

## Conclusión

Este enfoque permite resolver problemas complejos de optimización de manera eficiente, utilizando la fuerza de la selección natural para encontrar soluciones de calidad en un espacio de soluciones grande y complicado.

