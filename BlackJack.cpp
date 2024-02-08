#include <iostream>
#include <windows.h> // Librería para reproducir sonido
#include <mmsystem.h>
#include <string>
#include <cstdlib> // Added for rand()
#include <time.h>
#include <chrono> //sleep
#include <thread>

using namespace std;

int palo = 0;   
int carta = 0;
char repeat;
int as = 0;
int timer = 0;

// 1 corazon
// 2 diamantes
// 3 treboles
// 4 picas

//   1-AS    2      3      4      5      6      7      8      9     10     11     12      13
bool baraja[4][13] = {
    {false, false, false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false, false, false},
};
const char* nombres[] = { u8"\u2666", u8"\u2666", u8"\u2663", u8"\u2660" };

class Account {
public:
    string name;
    int balance = 0;

    void EscogerCarta() {
        palo = rand() % 4;
        carta = rand() % 13;
    }

    void Ponits() {
        if (carta >= 9) {
            balance += 10;
        }
        else if (carta < 9 && carta != 0) {
            balance += carta + 1;
        }
        else {
            balance += as;
        }

        cout << "Puntos de " << name << ": " << balance << "\n";
    }

    void Turno(bool esTurnoJugador) {
        EscogerCarta();
        while (baraja[palo][carta] == true) {
            EscogerCarta();
        }
        if (esTurnoJugador)
        {
            cout << "\n------------------\n";
            cout << "| Turno Jugador. |\n";
            cout << "------------------\n\n";
        }
        else {
            cout << "\n-------------------\n";
            cout << "| Turno Croupier. |\n";
            cout << "-------------------\n\n";
        }
        timer = rand() % (1200 - 800 + 1) + 800;
        this_thread::sleep_for(chrono::milliseconds(timer));
        switch (carta)
        {
        case 9:
            cout << " _______\n| 10    |\n|       |\n|   " << nombres[palo] << "   |\n|       |\n|_______|\n\n";
            break;
        case 10:
            cout << " _______\n| J     |\n|       |\n|   " << nombres[palo] << "   |\n|       |\n|_______|\n\n";
            break;
        case 11:
            cout << " _______\n| Q     |\n|       |\n|   " << nombres[palo] << "   |\n|       |\n|_______|\n\n";
            break;
        case 12:
            cout << " _______\n| K     |\n|       |\n|   " << nombres[palo] << "   |\n|       |\n|_______|\n\n";
            break;

        }       
        if(carta == 0){
            cout << " _______\n| A     |\n|       |\n|   " << nombres[palo] << "   |\n|       |\n|_______|\n\n";
        }
        else if(carta > 0 && carta < 9 ){
            cout << " _______\n| " << carta+1 << "     |\n|       |\n|   " << nombres[palo] << "   |\n|       |\n|_______|\n\n";
        }
        if (carta == 0 && esTurnoJugador) {
            as = 0;
            while (as != 11 && as != 1) {
                cout << "\nQue valor quieres que tenga el AS, 1 o 11?\n";
                cout << "Valor: ";
                cin >> as;
            }
        }
        if (carta == 0 && !esTurnoJugador) {
            if (balance <= 10)
            {
                as = 11;
            }
            else {
                as = 1;
            }
        }
        baraja[palo][carta] = true;
        Ponits();
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Account jugador;
    Account croupier;
    croupier.name = "croupier";       
    srand(time(NULL));
    cout << "\t\t\t\t\t\tWelcome to Black Jack hsagdkj\n";
    cout << "What is your name? ";
    cin >> jugador.name;
    
    for (size_t i = 0; i < 2; i++) {
        jugador.Turno(true);
    }

    croupier.Turno(false);
    while (repeat != 'S' && repeat != 'N')
    {
    cout << "Quieres otra carta? (S/N)";
    cin >> repeat;
    repeat = toupper(repeat);
    }
        while (jugador.balance < 21 && repeat == 'S') {
            jugador.Turno(true);

            if (jugador.balance > 21){
                cout << "\n\t\t\t\t\t\t-------------------\n";
                cout << "\t\t\t\t\t\t| Has perdido. :( |\n";
                cout << "\t\t\t\t\t\t-------------------\n\n";
                return 0;
            }
            repeat = 'J';
            while (repeat != 'S' && repeat != 'N')
            {
                cout << "Quieres otra carta? (S/N)";
                cin >> repeat;
                repeat = toupper(repeat);
            }
        }
        if (repeat == 'N') {
            while (croupier.balance < 21 && croupier.balance <= jugador.balance){
                croupier.Turno(false);
            }            
        }
        if (croupier.balance > 21 || jugador.balance > croupier.balance){ //si el croupier se pasa de 21 o croupier tiene menos puntos. jugador gana
            cout << "\n\t\t\t\t\t\t------------------\n";
            cout << "\t\t\t\t\t\t| Has ganado! :) |\n";
            cout << "\t\t\t\t\t\t------------------\n\n";
        }else if (croupier.balance > jugador.balance) //no se pasan de 21 + croupier tiene mas puntos. croupier gana
        {
            cout << "\n\t\t\t\t\t\t-------------------\n";
            cout << "\t\t\t\t\t\t| Has perdido. :( |\n";
            cout << "\t\t\t\t\t\t-------------------\n\n";
        }
        else { // empates
            cout << "\n\t\t\t\t\t\t-------------------\n";
            cout << "\t\t\t\t\t\t| Habeis empatado |\n";
            cout << "\t\t\t\t\t\t-------------------\n\n";
        }    
    return 0;
}