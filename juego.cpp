#include <iostream>
#include <cstdlib>
#include <fstream>
#include "juego.hxx"

using namespace std;

int signo(int x);
int difAbs(int a, int b);
bool dentro(int N, int f, int c);
int idx(int N, int f, int c);
void copiarNom(char *dst, const char *src, int mx);
bool igual(const char *a, const char *b);
void ubSalida(int N, int &sf, int &sc);
void ubCentro(int N, int &pf, int &pc);
void celdaLibre(char *occ, int N, int &f, int &c);
int sortCont();
void statEne(int tp, int &pv, int &at);
const char *nomTipo(int tp);
char simbEne(int tp);
bool revFin(Juego &j);

int signo(int x)
{
    return (x > 0) - (x < 0);
}

int difAbs(int a, int b) /* mide la distancia entre un jugador y un enemigo*/
{
    int d = a - b;
    return d < 0 ? -d : d;
}

bool dentro(int N, int f, int c)
{
    return f >= 0 && f < N && c >= 0 && c < N;
}

int idx(int N, int f, int c) /* convierte una cordenada del tablero en la posicion del arreglo dinamico*/
{
    return f * N + c;
}

void copiarNom(char *dst, const char *src, int mx)
{
    int i = 0;
    while (src[i] != '\0' && i < mx - 1)
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

bool igual(const char *a, const char *b)
{
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
            return false;
        i++;
    }
    return a[i] == b[i];
}

void ubSalida(int N, int &sf, int &sc)
{
    int lado = rand() % 4;
    int pos = rand() % N;
    if (lado == 0)
    {
        sf = 0;
        sc = pos;
    }
    else if (lado == 1)
    {
        sf = N - 1;
        sc = pos;
    }
    else if (lado == 2)
    {
        sf = pos;
        sc = 0;
    }
    else
    {
        sf = pos;
        sc = N - 1;
    }
}

/* Jugador en la zona central del tablero. */
void ubCentro(int N, int &pf, int &pc)
{
    int lo = N / 4;
    int hi = N - N / 4;
    if (hi <= lo)
    {
        lo = N / 2;
        hi = lo + 1;
    }
    pf = lo + rand() % (hi - lo);
    pc = lo + rand() % (hi - lo);
}

void celdaLibre(char *occ, int N, int &f, int &c)
{
    do
    {
        f = rand() % N;
        c = rand() % N;
    } while (occ[idx(N, f, c)] != SB_VAC);
}

/* Contenido de un cofre segun las probabilidades. */
int sortCont()
{
    int r = rand() % 100 + 1;
    if (r <= PROB_ORO)
        return CT_ORO;
    if (r <= PROB_POC)
        return CT_POC;
    return CT_TRA;
}

/* Estadisticas base de un enemigo. */
void statEne(int tp, int &pv, int &at)
{
    if (tp == TP_GOB)
    {
        pv = GOB_PV;
        at = GOB_AT;
    }
    else if (tp == TP_ARQ)
    {
        pv = ARQ_PV;
        at = ARQ_AT;
    }
    else
    {
        pv = JEF_PV;
        at = JEF_AT;
    }
}

const char *nomTipo(int tp)
{
    if (tp == TP_GOB)
        return "Goblin";
    if (tp == TP_ARQ)
        return "Arquero";
    return "Jefe";
}

char simbEne(int tp)
{
    if (tp == TP_GOB)
        return SB_GOB;
    if (tp == TP_ARQ)
        return SB_ARQ;
    return SB_JEF;
}

/*Memoria*/

void crear(Juego &j, const char *nom, int N, int nCof, int nGob, int nArq)
{
    j.N = N;
    j.nCof = nCof;
    j.nEne = nGob + nArq + 1; /* +1 por el jefe */

    j.tab = new char[N * N];
    j.ene = new Ene[j.nEne];
    j.cof = (nCof > 0) ? new Cof[nCof] : nullptr;

    char *occ = new char[N * N];
    for (int i = 0; i < N * N; i++)
        occ[i] = SB_VAC;

    ubSalida(N, j.sf, j.sc);
    occ[idx(N, j.sf, j.sc)] = SB_SAL;

    int pf, pc;
    ubCentro(N, pf, pc);
    occ[idx(N, pf, pc)] = SB_JUG;

    copiarNom(j.jug.nom, nom, TAM_NOM);
    j.jug.fil = pf;
    j.jug.col = pc;
    j.jug.pv = PV_INI;
    j.jug.ph = PH_INI;
    j.jug.oro = 0;

    for (int i = 0; i < nCof; i++)
    {
        int f, c;
        celdaLibre(occ, N, f, c);
        occ[idx(N, f, c)] = SB_COF;
        j.cof[i].fil = f;
        j.cof[i].col = c;
        j.cof[i].cont = sortCont();
        j.cof[i].abto = false;
    }

    int e = 0;
    for (int g = 0; g < nGob; g++, e++)
        j.ene[e].tipo = TP_GOB;
    for (int a = 0; a < nArq; a++, e++)
        j.ene[e].tipo = TP_ARQ;
    j.ene[e].tipo = TP_JEF;

    for (int i = 0; i < j.nEne; i++)
    {
        int f, c;
        celdaLibre(occ, N, f, c);
        occ[idx(N, f, c)] = SB_ENE;
        j.ene[i].fil = f;
        j.ene[i].col = c;
        statEne(j.ene[i].tipo, j.ene[i].pv, j.ene[i].atq);
        j.ene[i].vivo = true;
        j.ene[i].desc = false;
        j.ene[i].ctj = 0;
    }

    delete[] occ;

    j.trn = 0;
    j.fin = false;
    j.gan = false;

    actTab(j);
}

void liberar(Juego &j)
{
    delete[] j.tab;
    delete[] j.ene;
    delete[] j.cof;
    j.tab = nullptr;
    j.ene = nullptr;
    j.cof = nullptr;
}

/* guardar y cargar binario */

bool guardar(const Juego &j)
{
    ofstream f(ARCH_PART, ios::binary);
    if (!f)
    {
        cout << "Error: no se pudo escribir " << ARCH_PART << "\n";
        return false;
    }

    Cab c;
    c.N = j.N;
    c.sf = j.sf;
    c.sc = j.sc;
    c.nEne = j.nEne;
    c.nCof = j.nCof;
    c.trn = j.trn;
    c.fin = j.fin ? 1 : 0;
    c.gan = j.gan ? 1 : 0;
    c.jug = j.jug;

    f.write((char *)&c, sizeof(Cab));
    f.write(j.tab, (streamsize)j.N * j.N);
    if (j.nEne > 0)
        f.write((char *)j.ene, (streamsize)j.nEne * sizeof(Ene));
    if (j.nCof > 0)
        f.write((char *)j.cof, (streamsize)j.nCof * sizeof(Cof));

    f.close();
    return true;
}

bool cargar(Juego &j)
{
    ifstream f(ARCH_PART, ios::binary);
    if (!f)
        return false;

    Cab c;
    f.read((char *)&c, sizeof(Cab));
    if (!f)
        return false;

    j.N = c.N;
    j.sf = c.sf;
    j.sc = c.sc;
    j.nEne = c.nEne;
    j.nCof = c.nCof;
    j.trn = c.trn;
    j.fin = (c.fin != 0);
    j.gan = (c.gan != 0);
    j.jug = c.jug;

    j.tab = new char[j.N * j.N];
    j.ene = (j.nEne > 0) ? new Ene[j.nEne] : nullptr;
    j.cof = (j.nCof > 0) ? new Cof[j.nCof] : nullptr;

    f.read(j.tab, (streamsize)j.N * j.N);
    if (j.nEne > 0)
        f.read((char *)j.ene, (streamsize)j.nEne * sizeof(Ene));
    if (j.nCof > 0)
        f.read((char *)j.cof, (streamsize)j.nCof * sizeof(Cof));

    f.close();
    return true;
}

void actTab(Juego &j)
{
    for (int i = 0; i < j.N * j.N; i++)
        j.tab[i] = SB_VAC;

    j.tab[idx(j.N, j.sf, j.sc)] = SB_SAL;

    for (int i = 0; i < j.nCof; i++)
    {
        char s = j.cof[i].abto ? SB_COFA : SB_COF;
        j.tab[idx(j.N, j.cof[i].fil, j.cof[i].col)] = s;
    }

    for (int i = 0; i < j.nEne; i++)
    {
        Ene &en = j.ene[i];
        if (en.vivo && difAbs(en.fil, j.jug.fil) <= 1 && difAbs(en.col, j.jug.col) <= 1)
        {
            en.desc = true;
        }
        char s;
        if (!en.vivo)
            s = SB_VEN;
        else if (en.desc)
            s = simbEne(en.tipo);
        else
            s = SB_ENE;
        j.tab[idx(j.N, en.fil, en.col)] = s;
    }

    j.tab[idx(j.N, j.jug.fil, j.jug.col)] = SB_JUG;
}

void render(const Juego &j)
{
    cout << "\n  Mazmorra (" << j.N << "x" << j.N << ")   Turno: " << j.trn << "\n\n";

    cout << "     ";
    for (int c = 0; c < j.N; c++)
        cout << (c < 10 ? " " : "") << c << " ";
    cout << "\n";

    for (int r = 0; r < j.N; r++)
    {
        cout << "  " << (r < 10 ? " " : "") << r << " ";
        for (int c = 0; c < j.N; c++)
            cout << " " << j.tab[idx(j.N, r, c)] << " ";
        cout << "\n";
    }

    cout << "\n  Leyenda: @ jugador  # salida  ? cofre  O abierto\n";
    cout << "         E enemigo  G goblin  A arquero  J jefe  X vencido\n\n";
}

void verStats(const Juego &j)
{
    cout << "\n  === Estadisticas del personaje ===\n";
    cout << "  Nombre : " << j.jug.nom << "\n";
    cout << "  Vida (PV) : " << j.jug.pv << "\n";
    cout << "  Habil (PH) : " << j.jug.ph << "\n";
    cout << "  Oro : " << j.jug.oro << "\n";
    cout << "  Posicion : (" << j.jug.fil << ", " << j.jug.col << ")\n";
    cout << "  Turno : " << j.trn << "\n";
    if (j.fin)
        cout << "  Estado    : FINALIZADA (" << (j.gan ? "GANO" : "PERDIO") << ")\n\n";
    else
        cout << "  Estado    : en curso\n\n";
}

/* Turno*/

void actualizar(Juego &j)
{
    /* a) Ataques de los enemigos. */
    for (int i = 0; i < j.nEne; i++)
    {
        Ene &en = j.ene[i];
        if (!en.vivo)
            continue;

        int df = difAbs(en.fil, j.jug.fil);
        int dc = difAbs(en.col, j.jug.col);

        if (en.tipo == TP_ARQ)
        {
            /* Arquero */
            if (df <= 1 && dc <= 1)
            {
                j.jug.pv -= en.atq;
                en.desc = true;
                cout << "  Un arquero te ataca desde (" << en.fil << "," << en.col << ") y te resta " << en.atq << " PV.\n";
            }
        }
        else
        {
            /* Goblin y jefe */
            if (df == 0 && dc == 0)
            {
                j.jug.pv -= en.atq;
                en.desc = true;
                cout << "  Un " << nomTipo(en.tipo) << " te ataca y te resta " << en.atq << " PV.\n";
            }
        }
    }

    for (int i = 0; i < j.nEne; i++)
    {
        Ene &en = j.ene[i];
        if (en.tipo != TP_JEF || !en.vivo)
            continue;

        en.ctj++;
        if (en.ctj < TRN_JEF)
            continue;
        en.ctj = 0;

        if (en.fil == j.jug.fil && en.col == j.jug.col)
            continue;

        int dfS = j.sf - j.jug.fil; /* distancia en filas jugador/salida */
        int dcS = j.sc - j.jug.col; /* distancia en columnas jugador/salida */
        int tf = j.jug.fil;
        int tc = j.jug.col;
        if (difAbs(dfS, 0) >= difAbs(dcS, 0))
            tf += signo(dfS);
        else
            tc += signo(dcS);

        if (tf == j.sf && tc == j.sc)
        {
            tf = j.jug.fil;
            tc = j.jug.col;
        }

        if (en.fil == tf && en.col == tc)
        {
            cout << "  El jefe te bloquea el paso en (" << en.fil << "," << en.col << ").\n";
            continue;
        }

        int nf = en.fil + signo(tf - en.fil);
        int nc = en.col + signo(tc - en.col);

        if (!dentro(j.N, nf, nc) || (nf == j.sf && nc == j.sc))
            continue;

        en.fil = nf;
        en.col = nc;
        cout << "  El jefe se mueve para cortarte el camino, ahora en (" << en.fil << "," << en.col << ").\n";
    }
}

int puntaje(const Juego &j)
{
    int base = j.gan ? BASE_GAN : 0;
    int pv = j.jug.pv < 0 ? 0 : j.jug.pv;
    return base + j.jug.oro * P_ORO + pv * P_PV + j.jug.ph * P_PH;
}

void finalizar(Juego &j, bool gan)
{
    j.fin = true;
    j.gan = gan;
    if (j.jug.pv < 0)
        j.jug.pv = 0;

    int pt = puntaje(j);

    ofstream f(ARCH_REP, ios::app);
    if (f)
    {
        f << "Personaje: " << j.jug.nom << " | Puntaje: " << pt << " | Resultado: " << (gan ? "GANO" : "PERDIO") << "\n";
        f.close();
    }

    cout << "\n  ***************************************\n";
    if (gan)
        cout << "  Has alcanzado la salida. VICTORIA!\n";
    else
        cout << "  Te has quedado sin vida. DERROTA.\n";
    cout << "  Puntaje final: " << pt << "\n";
    cout << "  (registrado en " << ARCH_REP << ")\n";
    cout << "  ***************************************\n\n";
}

bool revFin(Juego &j)
{
    if (j.jug.pv <= 0)
    {
        finalizar(j, false);
        return true;
    }
    return false;
}

void mover(Juego &j, const char *dir)
{
    if (j.fin)
    {
        cout << "  La partida ya finalizo. Usa 'start' para jugar de nuevo.\n";
        return;
    }

    int df = 0, dc = 0;
    if (igual(dir, "up"))
        df = -1;
    else if (igual(dir, "down"))
        df = 1;
    else if (igual(dir, "left"))
        dc = -1;
    else if (igual(dir, "right"))
        dc = 1;
    else
    {
        cout << "  Direccion invalida. Usa: up | down | left | right\n";
        return;
    }

    int nf = j.jug.fil + df;
    int nc = j.jug.col + dc;

    if (!dentro(j.N, nf, nc))
    {
        cout << "  No puedes salir de los limites de la mazmorra.\n";
        return; /* movimiento invalido que no consume turno */
    }

    j.jug.fil = nf;
    j.jug.col = nc;
    cout << "  Te mueves a (" << nf << ", " << nc << ").\n";

    /* Victoria: llega a la salida. */
    if (nf == j.sf && nc == j.sc)
    {
        j.trn++;
        actTab(j);
        finalizar(j, true);
        guardar(j);
        return;
    }

    actualizar(j);
    j.trn++;

    revFin(j);
    actTab(j);
    guardar(j);
}

void explorar(Juego &j)
{
    if (j.fin)
    {
        cout << "  La partida ya finalizo. Usa 'start' para jugar de nuevo.\n";
        return;
    }

    Cof *c = nullptr;
    for (int i = 0; i < j.nCof; i++)
    {
        if (!j.cof[i].abto && j.cof[i].fil == j.jug.fil && j.cof[i].col == j.jug.col)
        {
            c = &j.cof[i];
            break;
        }
    }

    if (c == nullptr)
    {
        cout << "  No hay ningun cofre por explorar en esta casilla.\n";
        return;
    }

    c->abto = true;
    if (c->cont == CT_ORO)
    {
        j.jug.oro += ORO_B;
        cout << "  Cofre con ORO: +" << ORO_B << " piezas de oro.\n";
    }
    else if (c->cont == CT_POC)
    {
        j.jug.pv += POC_PV;
        cout << "  Cofre con POCION: +" << POC_PV << " PV.\n";
    }
    else
    {
        j.jug.pv -= TRA_D;
        cout << "  Cofre TRAMPA! Te ataca y resta " << TRA_D << " PV.\n";
    }

    actualizar(j);
    j.trn++;

    revFin(j);
    actTab(j);
    guardar(j);
}

void atacar(Juego &j)
{
    if (j.fin)
    {
        cout << "  La partida ya finalizo. Usa 'start' para jugar de nuevo.\n";
        return;
    }

    Ene *o = nullptr;
    for (int i = 0; i < j.nEne; i++)
    {
        if (j.ene[i].vivo && j.ene[i].fil == j.jug.fil && j.ene[i].col == j.jug.col)
        {
            o = &j.ene[i];
            break;
        }
    }

    if (o == nullptr)
    {
        cout << "  No hay ningun enemigo en esta casilla para atacar.\n";
        return;
    }

    o->desc = true;
    o->pv -= j.jug.ph; /* el jugador inflige dano igual a sus PH */
    cout << "  Atacas al " << nomTipo(o->tipo) << " (dano " << j.jug.ph << ").\n";

    if (o->pv <= 0)
    {
        o->vivo = false;
        int bon = (o->tipo == TP_GOB)   ? BON_GOB
                  : (o->tipo == TP_ARQ) ? BON_ARQ
                                        : BON_JEF;
        j.jug.ph += bon;
        cout << "  Has vencido al " << nomTipo(o->tipo) << "! (+" << bon << " PH)\n";
    }
    else
    {
        cout << "  El " << nomTipo(o->tipo) << " resiste con " << o->pv << " PV restantes.\n";
    }

    actualizar(j);
    j.trn++;

    revFin(j);
    actTab(j);
    guardar(j);
}
