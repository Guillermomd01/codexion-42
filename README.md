*Este proyecto ha sido creado como parte del currículo de 42 por gumunoz.*
# Codexion - 42 Project

## Descripción
**Codexion** es una simulación de concurrencia multihilo inspirada en el clásico "Problema de los Filósofos". El objetivo es gestionar un ecosistema donde múltiples programadores compiten por recursos limitados (dongles) para realizar tareas de compilación antes de que expire su tiempo de vida (*burnout*).

El proyecto implementa un planificador de tareas con dos modos de prioridad: **FIFO** (First In, First Out) y **EDF** (Earliest Deadline First), utilizando una estructura de montículo binario (*binary heap*) para garantizar que los programadores más críticos reciban los recursos primero.

## Instrucciones

### Compilación
El proyecto incluye un Makefile con las reglas habituales. Se requiere `gcc` y la librería `lpthread`.
```bash
make        # Compila el ejecutable codexion
make clean  # Elimina objetos
make fclean # Elimina objetos y ejecutable
make re     # Recompila todo
```

### Ejecución
El programa recibe 8 argumentos numéricos y el tipo de scheduler:
```bash
./codexion [n_coders] [t_burnout] [t_compile] [t_debug] [t_refactor] [n_compiles] [t_cooldown] [scheduler]
```
**Ejemplo:**
`./codexion 5 800 200 200 200 7 100 edf`

## Blocking cases handled
Para garantizar la estabilidad de la simulación, se han gestionado los siguientes problemas de concurrencia:
* **Prevención de Interbloqueos (Deadlocks):** Se rompe la condición de "espera circular" mediante la adquisición jerárquica de recursos. Los programadores siempre solicitan primero el dongle con el ID numérico más bajo.
* **Inanición (Starvation):** El uso del scheduler **EDF** asegura que los hilos con un tiempo de vida restante menor tengan prioridad en la cola del montículo, evitando que hilos con IDs bajos acaparen recursos indefinidamente.
* **Gestión de Cooldown:** Se implementó una espera activa controlada para respetar el `t_cooldown` de los dongles sin bloquear el hilo de manera innecesaria.
* **Detección de Burnout:** Un hilo monitor independiente supervisa en tiempo real el tiempo transcurrido desde la última compilación de cada programador.
* **Serialización del Log:** Todas las impresiones por consola están protegidas por un mutex de escritura (`write_lock`) para evitar el solapamiento de mensajes.

## Thread synchronization mechanisms
La implementación utiliza las primitivas de la librería `pthread` para coordinar el acceso a los dongles y el estado de la simulación:
* **`pthread_mutex_t`:** Utilizados para proteger el acceso a las estructuras de los dongles, el montículo de prioridad y las variables de estado global.
* **`pthread_cond_t`:** Implementado en cada dongle para permitir que los hilos esperen eficientemente hasta que un recurso sea liberado.
* **Comunicación Thread-Safe:** El monitor utiliza `pthread_cond_broadcast` para despertar simultáneamente a todos los hilos en espera cuando ocurre un burnout.

## Recursos
* Documentación oficial de POSIX Threads (`man pthreads`).
* Tutoriales sobre Binary Heaps y algoritmos de prioridad.

### Uso de IA
Se ha utilizado inteligencia artificial generativa como herramienta de apoyo en las siguientes tareas:
* **Depuración de Deadlocks:** Análisis de trazas de Helgrind para identificar bloqueos circulares.
* **Ayuda en README:** Ayuda en la creación y optimizacion del README.
* **Refactorización:** Organización modular del código.