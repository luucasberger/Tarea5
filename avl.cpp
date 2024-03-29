/* 5223744 - 4937759 */

 /* BIBLIOTECAS */
#include "../include/info.h"
#include "../include/avl.h"
#include "../include/cola_avls.h"
#include "../include/pila.h"
#include "../include/conjunto.h"
/* BIBLIOTECAS */

/* LIBRERIAS */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* LIBRERIAS */

// Representación de `avl_t'.
// Se debe definir en avl.cpp.
struct rep_avl {
  info_t dato;
	nat altura;
  nat cantidad;
  avl_t izq, der;
};

// Declaración del tipo `avl_t'.
typedef rep_avl *avl_t;

struct avl_ultimo { // PNG
  avl_t avl;
  int ultimo;
};

/* FUNCIONES AUXILIARES */

static nat maximo(nat n1, nat n2) { return (n1 >= n2) ? n1: n2; };

static void rotacion_simple_izquierda(avl_t &avl) {
  rep_avl *aux = avl->der;
  avl->der = aux->izq;
  aux->izq = avl;
  avl->altura = maximo(altura_de_avl(avl->izq), altura_de_avl(avl->der)) + 1; // a la rama izquierda.
  avl->cantidad = cantidad_en_avl(avl->izq) + cantidad_en_avl(avl->der) + 1;
  aux->altura = maximo(altura_de_avl(aux->izq), altura_de_avl(aux->der)) + 1;
  aux->cantidad = cantidad_en_avl(aux->izq) + cantidad_en_avl(aux->der) + 1;
  avl = aux;
};

static void rotacion_simple_derecha(avl_t &avl) {
  rep_avl *aux = avl->izq;
  avl->izq = aux->der;
  aux->der = avl;
  avl->altura = maximo(altura_de_avl(avl->izq), altura_de_avl(avl->der)) + 1; // a la rama izquierda.
  avl->cantidad = cantidad_en_avl(avl->izq) + cantidad_en_avl(avl->der) + 1;
  aux->altura = maximo(altura_de_avl(aux->izq), altura_de_avl(aux->der)) + 1;
  aux->cantidad = cantidad_en_avl(aux->izq) + cantidad_en_avl(aux->der) + 1;
  avl = aux;
};

static int FactorEquilibrio (avl_t avl) {
  int factor = 0;
  if (!es_vacio_avl(avl))
      factor = altura_de_avl(avl->der) - altura_de_avl(avl->izq);
  return factor;
};

static avl_t insertar_aux(info_t i, avl_t avl) {
  int factor = FactorEquilibrio(avl);
  if (factor < (-1) || factor > 1) {
  // Si entra a este 'if', entonces es necesario hacer algún tipo de rotación.
    if (factor < (-1) && (numero_info(i) < numero_info(avl->izq->dato))) {
    // Hay que balancear el lado izquierdo con solamente rotacion simple derecha.
      rotacion_simple_derecha(avl);
    } else if (factor > 1 && (numero_info(i) > numero_info(avl->der->dato))) {
    // Hay que balancear el lado derecho con solamente rotación simple izquierda.
      rotacion_simple_izquierda(avl);
    } else if (factor < (-1) && (numero_info(i) > numero_info(avl->izq->dato))) {
    //
      rotacion_simple_izquierda(avl->izq);
      rotacion_simple_derecha(avl);
    } else if (factor > 1 && (numero_info(i) < numero_info(avl->der->dato))) {
      rotacion_simple_derecha(avl->der);
      rotacion_simple_izquierda(avl);
    }
  } else {
    avl->altura = maximo(altura_de_avl(avl->izq), altura_de_avl(avl->der)) + 1;
  }
  return avl;
};

static nat Fibonacci(nat h) { // Para [avl_min_rec]
  if (h == 0)
    return 0;
  else if (h == 1)
    return 1;
  else
    return (1 + Fibonacci(h-1) + Fibonacci(h-2));
};

static info_t Info_sin_frase(nat DatoNumerico) { // Para [avl_min_rec]
  char *frase = new char[1];
  frase[0] = '\0';
  info_t info = crear_info(DatoNumerico, frase);
  return info;
};

static avl_ultimo avl_min_rec(nat h, nat primero) {
  avl_ultimo res;
  if (h == 0) {
    res.avl = NULL;
    res.ultimo = (primero - 1);
  } else if (h == 1) {
    rep_avl *nuevo = new rep_avl;
    nuevo->dato = Info_sin_frase(primero);
    nuevo->izq = NULL;
    nuevo->der = NULL;
    nuevo->altura = 1;
    nuevo->cantidad = 1;
    res.avl = nuevo;
    res.ultimo = primero;
  } else {
    avl_ultimo nuevo_izq = avl_min_rec(h-1, primero);
    rep_avl *avl_res = new rep_avl;
    avl_res->dato = Info_sin_frase(nuevo_izq.ultimo + 1);
    avl_res->izq = nuevo_izq.avl;
    avl_res->altura = h;
    avl_res->cantidad = Fibonacci(h);
    avl_ultimo nuevo_der = avl_min_rec(h-2, nuevo_izq.ultimo + 2);
    avl_res->der = nuevo_der.avl;
    res.avl = avl_res;
    res.ultimo = nuevo_der.ultimo;
  }
  return res;
};

static info_t *en_orden_rec(info_t *res, nat &tope, avl_t avl) {
  if (!es_vacio_avl(avl)) {
    res = en_orden_rec(res, tope, izq_avl(avl));
    res[tope] = raiz_avl(avl);
    tope++;
    res = en_orden_rec(res, tope, der_avl(avl));
  }
  return res;
};

static avl_t a2avl_rec(info_t *infos, int inf, int sup) {
  avl_t res;
  if (inf > sup)
    res = NULL;
  else {
    nat medio = (inf + sup) / 2;
    res = new rep_avl;
    res->dato = infos[medio];
    res->izq = a2avl_rec(infos, inf, medio - 1);
    res->der = a2avl_rec(infos, medio + 1, sup);
    res->altura = maximo(altura_de_avl(res->izq), altura_de_avl(res->der)) + 1;
    res->cantidad = cantidad_en_avl(res->izq) + cantidad_en_avl(res->der) + 1;
  }
  return res;
};

/* FUNCIONES AUXILIARES */

/*
  Devuelve un avl_t vacío (sin elementos).
  El tiempo de ejecución es O(1).
 */
avl_t crear_avl() {
  return NULL;
};

/*
  Devuelve `true' si y sólo si `avl' es vacío (no tiene elementos).
  El tiempo de ejecución es O(1).
 */
bool es_vacio_avl(avl_t avl) {
  return (avl == NULL);
};

/*
  Inserta `i' en `avl' respetando la propiedad de orden definida.
  Precondición: es_vacio_avl(buscar_en_avl(numero_info(i), avl).
  Precondición: numero_info(i) != INT_MAX.
  El tiempo de ejecución es O(log n), donde `n' es la cantidad de elementos
  de `avl'.
*/

void insertar_en_avl(info_t i, avl_t &avl) {
  if (es_vacio_avl(avl)) {
      rep_avl *nuevo = new rep_avl;
      nuevo->dato = i;
      nuevo->altura = 1;
      nuevo->cantidad = 1;
      nuevo->izq = NULL;
      nuevo->der = NULL;
      avl = nuevo;
  } else {
    avl->cantidad ++;
    if ((numero_info(i)) < (numero_info(avl->dato)))
      insertar_en_avl(i, avl->izq);
    else
      insertar_en_avl(i, avl->der);
  avl = insertar_aux(i, avl);
  }
};

/*
  Devuelve el subárbol que tiene como raíz al nodo con el elemento cuyo dato
  numérico es `clave'.
  Si ningún nodo cumple esa condición devuelve el árbol vacío.
  El tiempo de ejecución es O(log n), donde `n' es la cantidad de elementos de
  `avl'.
 */
avl_t buscar_en_avl(int clave, avl_t avl){
  avl_t res;
  if (!es_vacio_avl(avl)) {
    int comp = numero_info(avl->dato);
		if (clave < comp)
			res = buscar_en_avl(clave, avl->izq);
		else if (clave > comp)
			res = buscar_en_avl(clave, avl->der);
		else
			res = avl;
	} else
    res = crear_avl();
  return res;
};

/*
  Devuelve el elemento asociado a la raíz de `avl'.
  Precondición: ! es_vacio_avl(b).
  El tiempo de ejecución es O(1).
 */
info_t raiz_avl(avl_t avl){ return avl->dato; };

/*
  Devuelve el subárbol izquierdo de `avl'.
  Precondición: ! es_vacio_avl(avl).
  El tiempo de ejecución es O(1).
 */
avl_t izq_avl(avl_t avl){ return avl->izq; };

/*
  Devuelve el subárbol derecho de `avl'.
  Precondición: ! es_vacio_avl(avl).
  El tiempo de ejecución es O(1).
 */
avl_t der_avl(avl_t avl){ return avl->der; };

/*
  Devuelve la cantidad de elementos en `avl'.
  El tiempo de ejecución es O(1).
 */
nat cantidad_en_avl(avl_t avl) {
  nat cant = 0;
  if (!es_vacio_avl(avl))
    cant = avl->cantidad;
  return cant;
};

/*
  Devuelve la altura de  `avl'.
  La altura de un árbol vacío es 0.
  El tiempo de ejecución es O(1).
 */
nat altura_de_avl(avl_t avl){
  nat altura = 0;
  if (!es_vacio_avl(avl))
    altura = avl->altura;
  return altura;
};

/*
  Devuelve un arreglo con los elementos de `avl'.
  Los elementos en el arreglo deben estar en orden creciente según los datos
  numericos.
 */
info_t *en_orden_avl(avl_t avl){ // PNG
  info_t *res;
  if (es_vacio_avl(avl))
    res = NULL;
  else {
    res = new info_t[cantidad_en_avl(avl)];
    nat tope = 0;
    res = en_orden_rec(res, tope, avl);
  }
  return res;
};

/*
  Devuelve un avl_t con los `n' elementos que están en el rango [0 .. n - 1]
  del arreglo `infos'.
  Los elementos están ordenados de manera creciente estricto (creciente y sin
  repetidos) según los datos numércos.
  En cada nodo del árbol resultado la cantidad de elementos de su subárbol
  izquierdo es igual a, o 1 menos que, la cantidad de elementos de su subárbol
  derecho.
  El tiempo de ejecución es O(n).
 */
avl_t arreglo_a_avl(info_t *infos, nat n){
  return a2avl_rec(infos, 0, n - 1);
};

/*
  Devuelve un avl_t de altura `h' con `n' nodos, siendo `n' la mínima cantidad
  de elementos que puede tener un avl de altura `h'.
  Los datos numéricos de los elementos van desde 1 hasta `n'.
  El dato de texto de cada elemento es la cadena vacia.
  En ningún nodo puede ocurrir que el subárbol derecho tenga más nodos que el
  subárbol izquierdo.
  El tiempo de ejecución es O(n).
  Ver ejemplos en la letra y en el caso 408.
 */
avl_t avl_min(nat h) {
  avl_ultimo res = avl_min_rec(h, 1);
  return res.avl;
};

/*
  Imprime los datos numéricos de los nodos de cada nivel de `avl'.
  Imprime una línea por nivel, primero el más profundo y al final el que
  corresponde a la raíz.
  Cada nivel se imprime de izquierda a derecha. Se debe dejar un espacio en
  blanco después de cada número.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de nodos de `avl'.
  Ver ejemplos en la letra y en el caso 404.
 */
void imprimir_avl(avl_t avl){
  if(!es_vacio_avl(avl)) {
    pila_t p = crear_pila(cantidad_en_avl(avl) + altura_de_avl(avl) - 1);
    // Creo una pila 'p' de tamaño = cant. total de nodos del avl + altura del avl - 1, -1 porque al final imprimo
  // salto de línea al final de este 'if'.
    cola_avls_t c1 = crear_cola_avls();
    // 'c1' va a almacenar los datos del "nivel actual".
    encolar(avl, c1);
    cola_avls_t c2 = crear_cola_avls();
    // 'c2' va a almacenar los datos del "siguiente nivel".
    while(!es_vacia_cola_avls(c1)) {
    // Un 'while' para que se repitan las acciones HASTA que ya no queden más elementos por apilar en mi pila 'p'.
      while (!es_vacia_cola_avls(c1)) {
        if (frente(c1)->der != NULL)
          encolar(frente(c1)->der, c2);
        if (frente(c1)->izq != NULL)
          encolar(frente(c1)->izq, c2);
        apilar(numero_info(frente(c1)->dato), p);
        desencolar(c1);
        // Luego de apilar el número del frente de la "cola", lo saco de la misma, ya no lo necesito.
      }
      if (es_vacia_cola_avls(c1) && !es_vacia_cola_avls(c2))
        apilar(INT_MAX, p);
      // Si tengo elementos que faltan imprimir en 'c2' (elems del siguiente nivel) entonces apilo 'INT_MAX'.
      while (!es_vacia_cola_avls(c2)) {
        // Paso todos los elementos de 'c2' a 'c1'.
        encolar(frente(c2), c1);
        desencolar(c2);
      }
    }
    liberar_cola_avls(c1);
    liberar_cola_avls(c2);
    while(!es_vacia_pila(p)) {
      if(cima(p) == INT_MAX)
        printf("\n");
      else
        printf("%d%s", cima(p), " ");
      desapilar(p);
    }
    printf("\n");
    liberar_pila(p);
  }
};

/*
  Libera la memoria asignada a `avl' y la de sus elementos.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de
  `avl'.
 */
void liberar_avl(avl_t &avl){
  if (avl != NULL) {
		liberar_avl(avl->izq);
		liberar_avl(avl->der);
		liberar_info(avl->dato);
		delete(avl);
		avl = NULL;
	}
};
