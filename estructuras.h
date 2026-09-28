#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

/*Proyecto 1 - Programacion Avanzada*/

/*Archivos*/
#define ARCH_PART "partida.bin" /* estado del juego binarip */
#define ARCH_REP "reporte.txt"  /* reporte general de puntajes */

#define TAM_NOM 32 /* longitud maxima del nombre */

/* Simbolos del tablero */
#define SB_VAC '-'  /* casilla vacia */
#define SB_JUG '@'  /* jugador */
#define SB_ENE 'E'  /* enemigo sin descubrir */
#define SB_GOB 'G'  /* goblin */
#define SB_ARQ 'A'  /* arquero */
#define SB_JEF 'J'  /* jefe */
#define SB_VEN 'X'  /* enemigo vencido */
#define SB_COF '?'  /* cofre sin abrir */
#define SB_COFA 'O' /* cofre abierto */
#define SB_SAL '#'  /* salida */

/* Estadisticas iniciales */
#define PV_INI 100 /* puntos de vida iniciales*/
#define PH_INI 20  /* puntos de habilidad*/

/*Efecto de los cofres */
#define POC_PV 50 /* pocion*/
#define ORO_B 10  /* bolsa */
#define TRA_D 30  /* trampa */

/* Probabilidades*/
#define PROB_ORO 50
#define PROB_POC 80 /* 50 + 30 */

/* Estadisticas de enemigos */
#define GOB_PV 30
#define GOB_AT 10
#define ARQ_PV 20
#define ARQ_AT 10
#define JEF_PV 60
#define JEF_AT 40

/* Bonos de PH al vencer a cada enemigo */
#define BON_GOB 2
#define BON_ARQ 3
#define BON_JEF 5

#define TRN_JEF 2 /* el jefe avanza cada 2 turnos */

/* datos pal Puntaje*/
#define BASE_GAN 100
#define P_ORO 2
#define P_PV 2
#define P_PH 5

/*Tipos de enemigo */
#define TP_GOB 0
#define TP_ARQ 1
#define TP_JEF 2

/* Contenido de cofre */
#define CT_ORO 0
#define CT_POC 1
#define CT_TRA 2

/*  Personaje */
struct Pers
{
    char nom[TAM_NOM]; /* nombre */
    int fil;           /* fila */
    int col;           /* columna */
    int pv;            /* puntos de vida */
    int ph;            /* puntos de habilidad/ataque */
    int oro;           /* piezas de oro  */
};

/* Enemigo */
struct Ene
{
    int tipo; /* TP_GOB, TP_ARQ o TP_JEFe */
    int fil;
    int col;
    int pv;  /* vida del enemigo */
    int atq; /* dano que inflige */
    bool vivo;
    bool desc; /* descubierto por el jugador */
    int ctj;   /* contador de turnos */
};

/*  Cofre  */
struct Cof
{
    int fil;
    int col;
    int cont;  /* CT_ORO, CT_POC o CT_TRA*/
    bool abto; /* abierto */
};

/* estado completo usa memoria dinamicaapuntadores */
struct Juego
{
    int N;     /* dimension del tablero */
    char *tab; /* tablero N*N lineal */
    int sf;    /* salida: fila */
    int sc;    /* salida: columna */
    Pers jug;  /* jugador */
    Ene *ene;  /* arreglo dinamico de enemigos */
    int nEne;  /* cantidad de enemigos */
    Cof *cof;  /* arreglo dinamico de cofres */
    int nCof;  /* cantidad de cofres */
    int trn;   /* turno */
    bool fin;  /* partida finalizada */
    bool gan;  /* gano */
};

/* que se escribe al inicio del archivo binario */
struct Cab
{
    int N;
    int sf;
    int sc;
    int nEne;
    int nCof;
    int trn;
    int fin;
    int gan;
    Pers jug;
};

#endif
