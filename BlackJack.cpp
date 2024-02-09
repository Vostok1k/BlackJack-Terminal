#include <iostream>
#include <windows.h> // Librería para reproducir sonido
#include <mmsystem.h>
#include <string>
#include <cstdlib> // Añadido para rand()
#include <time.h>
#include <chrono> //sleep
#include <thread>

using namespace std;

int suit = 0;
int card = 0;
char repeat;
int ace = 0;
int timer = 0;
char repeatGameChar;

// 1 corazón
// 2 diamantes
// 3 treboles
// 4 picas

//   1-AS    2      3      4      5      6      7      8      9     10     11     12      13
bool deck[4][13] = { // para saber si ya hemos utilizado la carta o no
    {false, false, false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false, false, false},
};
const char* names[] = { u8"\u2666", u8"\u2666", u8"\u2663", u8"\u2660" };

void printSlowly(const string& message, unsigned int millisecondsPerCharacter) { //función para imprimir el texto lentamente
    for (char c : message) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(millisecondsPerCharacter));
    }
}

class Account {
public:
    string name; //donde se guarda el nombre de los jugadores
    int balance = 0; //donde se guardan los puntos

    void ChooseCard() { //función para escoger carta
        suit = rand() % 4; //elige palo aleatoriamente
        card = rand() % 13; //elige carta aleatoriamente
    }

    void Points() { //función para la suma de puntos
        if (card >= 9) { //si la carta es 10 o mayor se suman 10 puntos
            balance += 10;
        }
        else if (card < 9 && card != 0) { //si la carta tiene un valor menor a 10 y diferente a 0 (AS) se suma el valor de la carta
            balance += card + 1;
        }
        else { //se suma el valor de as escogido
            balance += ace;
        }

        cout << "Puntos de " << name << ": " << balance << "\n\n"; //imprime los puntos actuales de jugador/croupier
    }

    void Turn(bool isPlayerTurn) { //se encarga de imprimir el valor y palo de la carta y de quien es el turno
        ChooseCard(); //se ejecuta la función escoger carta
        while (deck[suit][card] == true) {
            ChooseCard();
        }
        if (isPlayerTurn) { //imprime de quien es el turno
            cout << "\n-----------------\n";
            cout << "| Turno Jugador |\n";
            cout << "-----------------\n\n";
        }

        else {
            cout << "\n------------------\n";
            cout << "| Turno Croupier |\n";
            cout << "------------------\n\n";
        }
        timer = rand() % (1000 - 500 + 1) + 500;
        this_thread::sleep_for(chrono::milliseconds(timer)); //tiempo de espera para que sea más "realista"
        switch (card) //imprime el valor de la carta en ASCII. Valores de 10
        {
        case 9:
            cout << " _______\n| 10    |\n|       |\n|   " << names[suit] << "   |\n|       |\n|_______|\n\n";
            break;
        case 10:
            cout << " _______\n| J     |\n|       |\n|   " << names[suit] << "   |\n|       |\n|_______|\n\n";
            break;
        case 11:
            cout << " _______\n| Q     |\n|       |\n|   " << names[suit] << "   |\n|       |\n|_______|\n\n";
            break;
        case 12:
            cout << " _______\n| K     |\n|       |\n|   " << names[suit] << "   |\n|       |\n|_______|\n\n";
            break;

        }
        if (card == 0) { //Valor AS
            cout << " _______\n| A     |\n|       |\n|   " << names[suit] << "   |\n|       |\n|_______|\n\n";
        }
        else if (card > 0 && card < 9) { //Valores del 2-9
            cout << " _______\n| " << card + 1 << "     |\n|       |\n|   " << names[suit] << "   |\n|       |\n|_______|\n\n";
        }
        PlaySound(TEXT("repartir3.wav"), SND_SYNC, SND_FILENAME);
        if (card == 0 && isPlayerTurn) { //Para escoger el valor que le quieres dar a AS, 1 o 11 (jugador)
            ace = 0;
            while (ace != 11 && ace != 1) {
                cout << "\nQue valor quieres que tenga el AS, 1 o 11?\n";
                cout << "Valor: ";
                cin >> ace;
            }
        }
        if (card == 0 && !isPlayerTurn) { //Valor que le da a AS el croupier
            if (balance <= 10) //si los puntos de croupier es menor o igual a 10 escoge 11.
            {
                ace = 11;
            }
            else { // sino (mayor a 10) AS es igual a 1.
                ace = 1;
            }
        }
        deck[suit][card] = true; //el valor de la carta se igual a true al ser utilizado
        Points(); //Se ejecuta la función Point para hacer la suma de puntos
    }
};

int main() {
    // Mensajes a imprimir
    string message1 = "\nQuieres otra carta? (S/N)";
    string message2 = "\t\t\t\t\t\tBienvenido al Casino de Barcelona";
    string message3 = "Escribe tu nombre: ";

    int speed = 30; // Velocidad de impresión (milisegundos por caracter)

    SetConsoleOutputCP(CP_UTF8); //consigue que se impriman valores que no están en la tabla ASCII

    Account player;
    Account dealer;

    dealer.name = "croupier";
    srand(time(NULL));
    printSlowly(message2, speed); // Imprimir cada mensaje lentamente
    cout << endl;
    printSlowly(message3, speed); //pedimos el nombre al jugador
    cin >> player.name;

    bool playAgain = true;
    while (playAgain) {
        for (size_t i = 0; i < 2; i++) { //ejecuta los dos turnos principales del jugador 
            player.Turn(true);
        }

        dealer.Turn(false); //ejecuta el primer turno principal del croupier
        while (repeat != 'S' && repeat != 'N') //solo acepta un S o N como respuesta sino te lo vuelve a pedir
        {
            printSlowly(message1, speed); //pide si quieres repetir tu turno
            cin >> repeat;
            repeat = toupper(repeat);
        }

        while (player.balance < 21 && repeat == 'S') { //mientras tus puntos sean menores a 21 y quieras repetir el bucle continua
            player.Turn(true); //se ejecuta el turno de el jugador

            if (player.balance > 21) { //te pasas de 21
                cout << "\n\t\t\t\t\t\t-------------------\n";
                cout << "\t\t\t\t\t\t| Has perdido. :( |\n";
                cout << "\t\t\t\t\t\t-------------------\n\n";
                break;
            }

            repeat = 'J'; //cambiar el valor de repeat para que no se repita el bucle sin querer
            while (repeat != 'S' && repeat != 'N') //solo acepta un S o N como respuesta sino te lo vuelve a pedir
            {
                printSlowly(message1, speed);
                cin >> repeat;
                repeat = toupper(repeat);
            }
        }

        if (repeat == 'N') { //decides acabar tu turno y empieza el del croupier
            while (dealer.balance < 21 && dealer.balance <= player.balance) { //se sigue ejecutando mientras el croupier tenga menos de 21 puntos 																   
                dealer.Turn(false);											   //y mientras tenga menos puntos o iguales que el jugador. En el caso de que los 2 tengan 21 sale del bucle y se imprime empate.
            }

            if (dealer.balance > 21 || player.balance > dealer.balance) { //si el croupier se pasa de 21 o croupier tiene menos puntos. Jugador gana
                cout << "\n\t\t\t\t\t\t------------------\n";
                cout << "\t\t\t\t\t\t| Has ganado! :) |\n";
                cout << "\t\t\t\t\t\t------------------\n\n";
            }
            else if (dealer.balance > player.balance) //Croupier no se pasan de 21 + croupier tiene mas puntos. Croupier gana
            {
                cout << "\n\t\t\t\t\t\t-------------------\n";
                cout << "\t\t\t\t\t\t| Has perdido. :( |\n";
                cout << "\t\t\t\t\t\t-------------------\n\n";
            }
            else { // solo se puede empatar en el caso de que tu tengas 21 y el croupier tenga 21
                cout << "\n\t\t\t\t\t\t-------------------\n";
                cout << "\t\t\t\t\t\t| Habeis empatado |\n";
                cout << "\t\t\t\t\t\t-------------------\n\n";
            }
        }

        // Preguntar al jugador si quiere jugar de nuevo
        printSlowly("\nQuieres jugar de nuevo? (S/N)", 30);
        cin >> repeatGameChar;
        repeatGameChar = toupper(repeatGameChar);
        playAgain = (repeatGameChar == 'S');
        // Reiniciar el estado del juego si el jugador decide jugar de nuevo
        if (playAgain) {
            system("cls"); // Limpiar la terminal
            player.balance = 0;
            dealer.balance = 0;
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 13; ++j) {
                    deck[i][j] = false;
                }
            }
            repeat = ' '; // Reiniciar la variable repeat para que vuelva a preguntar si se desea otra carta
        }
        else {
            printSlowly("\n\t\t\t\t\t\t\tHasta luego!\n", speed);
        }
    }

    return 0;
}
