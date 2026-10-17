# UNIVERSIDAD CENTRAL DE VENEZUELA
## FACULTAD DE CIENCIAS
### ESCUELA DE COMPUTACIÓN
**Matemáticas Discretas III**

---

# Tarea 2: Máquina de Turing Universal

Dada la codificación de una Máquina de Turing (MT) y una cadena de entrada, se desea que simule una MT Universal que verifique si la cadena es **Aceptada** o **Rechazada**.

---

## 1. Entrada
La entrada será leída por consola estándar y tendrá la siguiente forma:
* La primera línea tendrá un entero $N$ indicando el número de casos de prueba, es decir, el número de máquinas de Turing a simular.
* Posteriormente se recibirán $N$ líneas (una por cada caso), donde cada línea representa una Máquina de Turing codificada con su respectiva cadena a evaluar en la forma **`M0w`** donde $M$ es la Máquina de Turing codificada y $w$ es una lista de cadenas a ser evaluadas por dicha máquina.

---

## 2. Salida
La salida debe ser escrita por consola estándar. Cada caso de prueba debe comenzar con la línea **`Caso N:`** donde $N$ representa el número del caso de prueba (empezando en 1), seguido de la descripción formal de la máquina en formato de tuplas y conjuntos, y finalmente si las cadenas son **Aceptadas** o **Rechazadas**, de la siguiente manera:

```text
Caso 1:
T = (S, I, f, S0)
S = {s1, s2, s3}
I = {a, b}
S0 = s1
f = {
(s1, a, s3, a, R),
(s3, a, s3, a, R),
(s3, b, s2, b, R)
}
Aceptada
Rechazada
```

---

## 3. Consideraciones Técnicas y Reglas de Codificación

1. **Estados:** Se representan en notación unaria. El estado inicial siempre será el estado 1 (`1`), y el estado final de una máquina de Turing $T$ es un estado que no es el primer estado de ninguna 5-tupla de las que aparecen en la descripción de $T$.
2. **Símbolo Blanco:** El símbolo blanco será codificado con la cadena `1` y se mostrará en la salida con el símbolo **`B`** mayúscula.
3. **Alfabeto de Cinta:** Los demás símbolos del alfabeto de cinta se representan en notación unaria y se enumeran como `11`, `111`, `1111`, y así sucesivamente en orden lexicográfico, siendo `'a'` el primer símbolo (`11`) y considerando solo letras minúsculas. El alfabeto de entrada $I$ no incluye al símbolo blanco `B`.
4. **Directrices de Desplazamiento:** Las directrices de desplazamiento hacia la derecha ($\rightarrow$), izquierda ($\leftarrow$) y estacionario ($\uparrow$) se codifican con `1`, `11` y `111`, respectivamente, imprimiéndose en la salida como **`R`**, **`L`** y **`-`**.
5. **Codificación de Transiciones:** Una transición $f(q, a) = (p, b, D)$ se codifica usando ceros como separadores para los estados, los símbolos del alfabeto de cinta y la directriz de desplazamiento $D$. 
   * *Ejemplo:* La transición $f(s3, a) = (s5, b, \leftarrow)$ se codifica como:
     `01110110111110111010`
     *(Donde `111` es $s3$, `11` es $a$, `11111` es $s5$, `111` es $b$, y `11` es $\leftarrow$)*
6. **Estructura de las Cadenas a Evaluar:** Las cadenas a evaluar estarán formadas por símbolos de cinta, separados por ceros (`0`). Distintas cadenas entre sí estarán separadas por doble cero (`00`).
7. **Separación de la Máquina y las Cadenas (`M0w`):** La máquina codificada $M$ y la lista de cadenas $w$ están separadas exactamente por **tres ceros consecutivos (`000`)**.

---

## 4. Condiciones de Entrega
* **Lenguajes permitidos:** C, C++ y Python 3.
* **Nomenclatura del archivo:** El archivo entregado debe nombrarse estrictamente con la convención: `Apellido_Nombre.ext` (ej. `Perez_Juan.cpp`).
* **Modalidad:** Entrega individual al correo `tareasmdiii@gmail.com` antes de la fecha límite.