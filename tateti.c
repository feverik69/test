/*
 ============================================================================
 Name        : tateti.c
 Author      : LP1
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "tateti.h"


/* Definiciones que converten la posición indicada con un número en
 * coordenadas de la matriz que representa el tablero.
 *
 *   1 | 2 | 3
 *  ---+---+---
 *   4 | 5 | 6
 *  ---+---+---
 *   7 | 8 | 9
 *
 * Entonces, 1 equivale a las coordenadas (0, 0),
 * 2 a (0, 1) y así sucesivamente.
 */
#define POS_A_FILA(x)		(x-1)/TAM_TABLERO
#define POS_A_COLUMNA(x)	(x-1)%TAM_TABLERO

/*
 * Tablero del TA - TE - TI
 * Estructura de datos principal del programa.
 */
static char tablero[TAM_TABLERO][TAM_TABLERO];
/*
 * Contador de jugadas realizadas. Se utiliza para detectar el final de la partida (empate)
 */
static int contJugadas;

/*
 * Si se detecta una vez el fin de la partida esta bandera se activa y en las verificaciones subsiguientes
 * informamos que la partida ya había terminado antes.
 */
static int bandFinPartida;

/*
 * Función que inicializa el tablero. Una casilla desocupada
 * está indicada con el caracter espacio (' ').
 * Parámetro:
 *  t -> el tablero (matriz de 3x3 de chars)
 * Retorno:
 * 	Ninguno, la matriz contendrá espacio en cada posición
 */
void inicializarTablero(void){
	int i, j;

	for(i = 0; i < TAM_TABLERO; i++)
		for(j = 0; j < TAM_TABLERO; j++)
			tablero[i][j] = CASILLA_LIBRE;

	// inicializamos el contador de jugadas
	contJugadas = 0;
	// inicializamos bandera de fin de partida
	bandFinPartida = 0;
}

/*
 * Función que verifica si, tomando es cuenta la jugada indicada, el juego termina.
 * Se verifican dos cosas:
 * 1. Si con esta jugada se gana la partida
 * 2. Si se han realizado 9 jugadas con lo que la partida es un empate
 * Parámetro:
 *  p -> posición de la jugada actual
 *  marca -> que marca se puso en la jugada actual
 * Retorno:
 * 	0 -> no termina la partida
 * 	1 -> el jugador que tiene esta marca gana la partida por un línea vertical
 * 	2 -> el jugador que tiene esta marca gana la partida por un línea horizontal
 * 	3 -> el jugador que tiene esta marca gana la partida por un línea diagonal de 1-9
 * 	4 -> el jugador que tiene esta marca gana la partida por un línea diagonal de 3-7
 * 	5 -> el empate
 * 	6 -> la partida ya finalizó, se detectó en una ejecución anterior de esta función
 */
int verifica_fin (int p, char marca){
	int i, j;
	int cont;

	if (bandFinPartida)
		return 6;	// la partida ya finalizó en una comprobación anterior

	// verifica vertical
	j = POS_A_COLUMNA(p);
	for (cont = 0, i = 0; i < TAM_TABLERO; i++)
		if (tablero[i][j] == marca)
			cont++;
	if (cont == 3){
		bandFinPartida = 1;
		return 1;	// "marca" ganó la partida con una vertical"
	}

	// verifica horizontal
	i = POS_A_FILA(p);
	for (cont = 0, j = 0; j < TAM_TABLERO; j++)
		if (tablero[i][j] == marca)
			cont++;
	if (cont == 3){
		bandFinPartida = 1;
		return 2;	// "marca" ganó la partida con una horizontal"
	}

	// verifica diagonales
	switch(p){
	case 1: case 9:
		if (tablero[0][0] == marca && tablero[1][1] == marca && tablero[2][2] == marca){
			bandFinPartida = 1;
			return 3;	// "marca" ganó la partida con una diagonal
		}
		break;
	case 3: case 7:
		if (tablero[0][2] == marca && tablero[1][1] == marca && tablero[2][0] == marca){
			bandFinPartida = 1;
			return 4;	// "marca" ganó la partida con la otra diagonal
		}
		break;
	case 5:
		if (tablero[0][2] == marca && tablero[1][1] == marca && tablero[2][0] == marca){
			bandFinPartida = 1;
			return 4;	// "marca" ganó la partida con la otra diagonal
		} else if (tablero[0][0] == marca && tablero[1][1] == marca && tablero[2][2] == marca){
			bandFinPartida = 1;
			return 3;	// "marca" ganó la partida con una diagonal
		}
		break;

	}

	// verificamos si el tablero está lleno pero no gana nadie
	if (contJugadas == 9)
		return 5;

	return 0;	// no termina la partida
}

/*
 * Función que retorna la marca contenida en una casilla.
 * Parámetros:
 * 	p -> posición en la que se pondría la marca
 * Retorno:
 * 	' ', 'X' o 'O' -> marca que la posición contiene
 * 	-1 -> error, la posición no es válida
 */
char marca (int p){
	if (p < 1 || p > 9)
		return -1;	// posición fuera de rango

	return tablero[POS_A_FILA(p)][POS_A_COLUMNA(p)];
}

/*
 * Función que coloca una marca en el tablero. Se verifica
 * si la posición está libre.
 * Parámetros:
 * 	p -> posición en la que se pondría la marca
 * 	marca -> caracter que se utilizará como marca del jugador
 * Retorno:
 * 	>= 0 -> indice el resultado de la comprobación de fin de juego
 * 	-1 -> si la casilla ya está ocupada
 * 	-2 -> si la casilla no existe
 */
int jugar (int p, char marca){
	if (p < 1 || p > 9)
		return -2;	// posición fuera de rango

	if (tablero[POS_A_FILA(p)][POS_A_COLUMNA(p)] == CASILLA_LIBRE){
		tablero[POS_A_FILA(p)][POS_A_COLUMNA(p)] = marca;
	} else {
		return -1;	// casilla ya ocupada
	}
	// incrementamos contador global de jugadas
	contJugadas++;

	return verifica_fin(p, marca);
}

/*
 * Función que coloca una marca en el tablero para el jugador O.
 * Parámetros:
 * 	p -> posición en la que se pondría la marca
 * Retorno:
 * 	0 -> no termina la partida, la casilla se pudo marcar
 * 	1 -> el jugador que tiene esta marca gana la partida por un línea vertical
 * 	2 -> el jugador que tiene esta marca gana la partida por un línea horizontal
 * 	3 -> el jugador que tiene esta marca gana la partida por un línea diagonal de 1-9
 * 	4 -> el jugador que tiene esta marca gana la partida por un línea diagonal de 3-7
 * 	5 -> el empate
 * 	6 -> la partida ya finalizó, se detectó en una ejecución anterior de esta función
 * 	-1 -> si la casilla ya está ocupada
 * 	-2 -> si la casilla no existe
 */
int jugarO (int p){
	return jugar (p, MARCA_O);
}

/*
 * Función que coloca una marca en el tablero para el jugador X.
 * Parámetros:
 * 	p -> posición en la que se pondría la marca
 * Retorno:
 * 	0 -> no termina la partida, la casilla se pudo marcar
 * 	1 -> el jugador que tiene esta marca gana la partida por un línea vertical
 * 	2 -> el jugador que tiene esta marca gana la partida por un línea horizontal
 * 	3 -> el jugador que tiene esta marca gana la partida por un línea diagonal de 1-9
 * 	4 -> el jugador que tiene esta marca gana la partida por un línea diagonal de 3-7
 * 	5 -> el empate
 * 	6 -> la partida ya finalizó, se detectó en una ejecución anterior de esta función
 * 	-1 -> si la casilla ya está ocupada
 * 	-2 -> si la casilla no existe
 */
int jugarX (int p){
	return jugar (p, MARCA_X);
}

/*
 * Copia el contenido del tablero actual.
 * Parámetros:
 * 	t -> matriz donde se copia el tablero actual
 * Retorno:
 * 	ninguno. El tablero copiado
 */
void retornaTablero (char t[TAM_TABLERO][TAM_TABLERO]){
	int i, j;

	// Copiamos el tablero que usamos para el juego
	// al parámetro de esta función
	for(i = 0; i < TAM_TABLERO; i++)
		for(j = 0; j < TAM_TABLERO; j++)
			t[i][j] = tablero[i][j];
}

/*
 * Mira si el tablero está vacío
 * Parámetro:
 *    Ninguno
 * Retorno:
 *   0 -> si no está vacío
 *   1 -> si está vacío
 */
int tableroVacio(void){
	int i, j;

	// Copiamos el tablero que usamos para el juego
	// al parámetro de esta función
	for(i = 0; i < TAM_TABLERO; i++)
		for(j = 0; j < TAM_TABLERO; j++)
			if (tablero[i][j] != CASILLA_LIBRE)
				return 0;	// retorna falso

	return 1;	// retorna verdadero
}
