#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "juego.hxx"

using namespace std;

int main(int argc, char *argv[])
{
    srand((unsigned)time(0));

    int N, gob, arq, cof;

    if (argc < 2)
    {
        cout << "error faltan argumentos se debe iniciar de la siguiente manera: .\\juego.exe COMANDO" << endl;
        return 1;
    }
    char *arg = argv[1];
    bool start = (arg[0] == 's' && arg[1] == 't' && arg[2] == 'a' && arg[3] == 'r' && arg[4] == 't' && arg[5] == '\0');
    bool board = (arg[0] == 'b' && arg[1] == 'o' && arg[2] == 'a' && arg[3] == 'r' && arg[4] == 'd' && arg[5] == '\0');
    bool stats = (arg[0] == 's' && arg[1] == 't' && arg[2] == 'a' && arg[3] == 't' && arg[4] == 's' && arg[5] == '\0');
    bool move = (arg[0] == 'm' && arg[1] == 'o' && arg[2] == 'v' && arg[3] == 'e' && arg[4] == '\0');
    bool seek = (arg[0] == 's' && arg[1] == 'e' && arg[2] == 'e' && arg[3] == 'k' && arg[4] == '\0');
    bool attack = (arg[0] == 'a' && arg[1] == 't' && arg[2] == 't' && arg[3] == 'a' && arg[4] == 'c' && arg[5] == 'k' && arg[6] == '\0');

    Juego j;

    if (start)
    {
        char nom[TAM_NOM];
        cout << "Nombre = ";
        cin.getline(nom, TAM_NOM);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cout << "N = ";
        cin >> N;
        cout << "Goblins = ";
        cin >> gob;
        cout << "Arqueros = ";
        cin >> arq;
        cout << "Cofres = ";
        cin >> cof;

        /* Validar cosas basicas */
        if (!cin || N < 5 || N > 30)
        {
            cout << "Datos invalidos. N debe estar entre 5 y 30." << endl;
            return 1;
        }
        if (gob < 0 || arq < 0 || cof < 0)
        {
            cout << "Las cantidades no pueden ser negativas." << endl;
            return 1;
        }
        int ocu = cof + gob + arq + 1 + 2; /* +jefe +jugador +salida */
        if (ocu > N * N)
        {
            cout << "No caben tantos elementos en un tablero de " << (N * N) << " casillas. Reduce las cantidades." << endl;
            return 1;
        }

        crear(j, nom, N, cof, gob, arq);
        guardar(j);
        cout << "\nPartida creada (se agrego 1 jefe automaticamente)." << endl;
        cout << "Enemigos: " << (gob + arq + 1) << " | Cofres: " << cof << endl;
        render(j);
        liberar(j);
    }
    else if (board)
    {
        if (!cargar(j))
        {
            cout << "No hay partida iniciada. Usa 'start'." << endl;
            return 0;
        }
        render(j);
        liberar(j);
    }
    else if (stats)
    {
        if (!cargar(j))
        {
            cout << "No hay partida iniciada. Usa 'start'." << endl;
            return 0;
        }
        verStats(j);
        liberar(j);
    }
    else if (move && argc >= 3)
    {
        char *arg2 = argv[2];
        if (!cargar(j))
        {
            cout << "No hay partida iniciada. Usa 'start'." << endl;
            return 0;
        }
        mover(j, arg2);
        liberar(j);
    }
    else if (move)
    {
        cout << "Uso: .\\juego.exe move [up|down|left|right]" << endl;
    }
    else if (seek)
    {
        if (!cargar(j))
        {
            cout << "No hay partida iniciada. Usa 'start'." << endl;
            return 0;
        }
        explorar(j);
        liberar(j);
    }
    else if (attack)
    {
        if (!cargar(j))
        {
            cout << "No hay partida iniciada. Usa 'start'." << endl;
            return 0;
        }
        atacar(j);
        liberar(j);
    }
    else
    {
        cout << "comando no reconocido si no ha iniciado partida pruebe con 'start'" << endl;
    }

    return 0;
}
