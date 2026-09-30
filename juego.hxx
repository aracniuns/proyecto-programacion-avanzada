#ifndef JUEGO_HXX
#define JUEGO_HXX

#include "estructuras.h"

/* Memoria */
void crear(Juego &j, const char *nom, int N, int nCof, int nGob, int nArq);
void liberar(Juego &j);

/* Persistencia en el archivo binario*/
bool guardar(const Juego &j);
bool cargar(Juego &j);

/*Tablero  */
void actTab(Juego &j);       /* recalcula el tablero y revela lo del lado el adyacente*/
void render(const Juego &j); /* imprime el tablero*/
void verStats(const Juego &j);

/* Acciones*/
void mover(Juego &j, const char *dir); /* direciones: "up"/"down"/"left"/"right" */
void explorar(Juego &j);
void atacar(Juego &j);

/* Turno y fin del juego*/
void actualizar(Juego &j); /* que pasa despues del turno */
int puntaje(const Juego &j);
void finalizar(Juego &j, bool gan);

#endif
