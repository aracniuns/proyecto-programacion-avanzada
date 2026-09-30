Juego de la mazmorra
Proyecto 1 - Programacion Avanzada 2026-30

Es un juego de estrategia por consola hecho en C++. La idea es que cada
vez que se ejecuta el programa se le da UN solo comando (moverse, atacar,
abrir un cofre...) y la partida queda guardada en un archivo binario, asi
que en la siguiente ejecucion se sigue justo donde se quedo.

Una aclaracion sobre el codigo: usamos nombres de variables cortos
(pv, ph, ene, cof, trn...) para que no quedaran lineas tan largas. En
estructuras.h esta comentado que significa cada uno.


QUE HAY EN LA CARPETA

  main.cpp        Lee el comando que se escribe en la consola y llama a
                  la funcion que toca.
  estructuras.h   Las constantes del juego y los structs (Pers, Ene, Cof,
                  Juego y Cab).
  juego.hxx       Las declaraciones de las funciones.
  juego.cpp       Toda la logica: el tablero, las acciones y el guardado
                  en el .bin.

  Tambien dejamos los archivos que genera el programa, como prueba de que
  funciona:
  partida.bin     La partida guardada (personaje, enemigos y tablero).
  reporte.txt     El historial con el nombre y el puntaje de cada partida
                  que se termina.


COMO COMPILARLO

Nosotros lo compilamos con g++ (MinGW-w64):

    g++ -Wall -Wextra -O2 -o juego.exe main.cpp juego.cpp


COMO SE JUEGA

Primero se crea la partida y despues se va jugando comando por comando.
No hay que guardar nada a mano, eso lo hace el programa solo.

    juego.exe start       Crea una partida nueva. Pide el nombre, el
                          tamano N del tablero, cuantos goblins, cuantos
                          arqueros y cuantos cofres. El jefe se agrega
                          solo.
    juego.exe board       Muestra el tablero.
    juego.exe stats       Muestra la vida, la habilidad y el oro del
                          personaje.
    juego.exe move up     Mueve al personaje una casilla. Tambien sirve
                          con down, left y right.
    juego.exe seek        Abre el cofre de la casilla donde esta el
                          personaje.
    juego.exe attack      Ataca al enemigo de la casilla donde esta el
                          personaje.

Asi se ve cada cosa en el tablero:

    -        casilla vacia
    @        el personaje
    #        la salida
    ?        cofre sin abrir (cuando se abre pasa a O)
    E        enemigo que todavia no se ha descubierto
    G A J    goblin, arquero y jefe
    X        enemigo vencido

Los enemigos aparecen como E hasta que uno se les acerca; ahi se ve cual
es.


REGLAS

  - El personaje empieza con 100 PV (vida) y 20 PH (habilidad), en alguna
    casilla del centro del tablero.
  - La salida queda en un punto al azar del borde.
  - Al abrir un cofre puede salir:
      oro     (50%)  -> +10 de oro
      pocion  (30%)  -> +50 PV
      trampa  (20%)  -> -30 PV
  - Enemigos:
      Goblin:  30 PV. Hace 10 de dano si uno esta en su casilla.
               Al vencerlo se ganan 2 PH.
      Arquero: 20 PV. Hace 10 de dano desde las casillas de al lado.
               Al vencerlo se ganan 3 PH.
      Jefe:    60 PV. Hace 40 de dano en su casilla y cada 2 turnos se
               mueve una casilla hacia la salida. Solo hay uno.
               Al vencerlo se ganan 5 PH.
  - Cuando el personaje ataca le quita al enemigo tantos PV como PH
    tenga.
  - La partida se acaba cuando se llega a la salida (se gana) o cuando
    la vida llega a 0 (se pierde).
  - El puntaje final es:

        B + (oro * 2) + (PV * 2) + (PH * 5)

    donde B vale 100 si se gano y 0 si se perdio.