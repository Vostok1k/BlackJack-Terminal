#include <iostream>
#include <windows.h> // libreria para reproducir sonido
#include <mmsystem.h>
#include <string>
#include <time.h>

using namespace std;

string name;


int palo = 0;
int carta = 0;
int PuntosJugador = 0;
int PuntosCroupier = 0;
boolean repeat;


//1 corazon
//2 diamantes
//3 treboles
//4 picas

//   1-AS    2      3      4      5      6      7      8      9     10     11     12      13
bool baraja[4][13] = {
  { false, false, false, false, false, false, false, false, false, false, false, false, false },
  { false, false, false, false, false, false, false, false, false, false, false, false, false },
  { false, false, false, false, false, false, false, false, false, false, false, false, false },
  { false, false, false, false, false, false, false, false, false, false, false, false, false },
};

string nombres[] = { "corazones","diamantes","treboles","picas" };

void EscogerCarta() {
    palo = rand() % 4;
    carta = 0;
    //rand() % 13
}


void TurnoJugador() {
    int as = 0;
    for (size_t i = 0; i < 2; i++) {

        EscogerCarta();
        while (baraja[palo][carta] == true){
            EscogerCarta();
        }
        cout << "Ha salido un " << carta + 1 << " de " << nombres[palo];
        if (carta == 0) {
            while (as!=11 && as!=1)
            {
                cout << "\nQue valor quieres que tenga el AS, 1 o 11?\n";
                cout << "Valor: ";              
                cin >> as;
            }
        }
        baraja[palo][carta] = true; 
        
        if (carta >= 9){
            PuntosJugador += 10;
            cout << " + 10 pntos.\n";
        }
        else if (carta < 9 && carta != 0) {
            PuntosJugador += carta + 1;
            cout << " + " << carta+1 <<" pntos.\n";
        }
        else {
            PuntosJugador += as;
        }
    }
    cout << "Puntos: " << PuntosJugador;

    cout << "\nQuieres otra carta? (S/N)";

    cin >> repeat;
    
}


int main()
{
    srand(time(NULL));
    cout << "Welcome to Black Jack hsagdkj\n";
    cout << "What is your name? ";
    cin >> name;
    while (PuntosJugador <= 21 && repeat == true)
    {

    }
    TurnoJugador();
    return 0;

}


