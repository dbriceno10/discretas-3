**Universidad Central de Venezuela**
**Facultad de Ciencias.**
**Escuela de Computación.**
**Matemáticas Discretas III**

# El Validador de Identificadores Multi-Lenguaje

El laboratorio de compiladores de la Escuela de Computación requiere desarrollar un sistema de análisis léxico preliminar capaz de verificar si un conjunto de cadenas corresponde a identificadores válidos en diferentes lenguajes de programación (por ejemplo: C-Style, Python y COBOL).

Cada lenguaje cuenta con su propio léxico regular reconocido por un Autómata Finito Determinista (AFD) independiente. Las reglas para cada uno son:

### 1. C-Style:
* Debe comenzar obligatoriamente con una letra minúscula (a-z).
* A partir del segundo carácter puede contener letras minúsculas, dígitos (0-9) o guiones bajos (`_`).
* No puede terminar con un guión bajo (`_`).

### 2. Python:
* Debe comenzar con una letra (minúscula o mayúscula) o un guión bajo (`_`).
* A partir del segundo carácter puede contener letras, dígitos o guiones bajos.
* Puede terminar en guión bajo o letras/dígitos.

### 3. COBOL:
* Debe comenzar obligatoriamente con una letra mayúscula (A-Z).
* A partir del segundo carácter puede contener letras mayúsculas, dígitos o guiones (`-`).
* No puede terminar con un guión (`-`).

## Objetivo
Desarrolle un programa que simule el comportamiento de los autómatas de cada lenguaje y determine, para cada cadena de entrada, si es ACEPTADA o RECHAZADA en cada uno de los paradigmas evaluados.

## Entrada
La entrada se recibe por la entrada estándar.
* La primera línea contiene un entero `K`, que representa la cantidad de cadenas a evaluar.
* Las siguientes `K` líneas contienen, cada una, una cadena `S`.

## Salida
Para cada cadena evaluada, el programa debe mostrar el resultado de su evaluación frente a cada uno de los lenguajes soportados, manteniendo un formato claro y estructurado.

## Ejemplo

| Entrada | Salida |
| :--- | :--- |
| `3`<br>`mi_variable1`<br>`VALOR_MAX`<br>`9variable` | **Cadena:** `mi_variable1`<br>**C-Style:** ACEPTADA<br>**Python:** ACEPTADA<br>**COBOL:** RECHAZADA<br><br>**Cadena:** `VALOR_MAX`<br>**C-Style:** RECHAZADA<br>**Python:** ACEPTADA<br>**COBOL:** RECHAZADA<br><br>**Cadena:** `9variable`<br>**C-Style:** RECHAZADA<br>**Python:** RECHAZADA<br>**COBOL:** RECHAZADA |

## Consideraciones
* La solución debe modelar explícitamente los estados y las transiciones de los Autómatas Finitos Determinísticos (AFD) para cada lenguaje.
* No se permite utilizar expresiones regulares para validar las cadenas; la aceptación o el rechazo debe determinarse estrictamente mediante la simulación de los autómatas.
* Una cadena vacía debe clasificarse como RECHAZADA en todos los lenguajes.
* Si una cadena contiene caracteres no válidos para el alfabeto del lenguaje evaluado, debe clasificarse automáticamente como RECHAZADA.
* El programa debe leer los datos desde la entrada estándar y escribir los resultados en la salida estándar, respetando estrictamente el formato especificado.
* Lenguajes permitidos: C, C++ y Python 3.
* El archivo entregado debe nombrarse con la convención: `Apellido_Nombre.ext`
* La entrega es individual y debe ser enviada al correo tareasmdiii@gmail.com antes de la fecha límite **01/10/2026 hasta las 11:59 p.m.**
* Las copias serán severamente penalizadas según lo establecido en la Ley de Universidades. Se anima a la discusión, pero se prohíbe la copia de proyectos. Cualquier proyecto entregado debe ser fruto de su propio trabajo.

*GDMDIII/Sem 1-2026*