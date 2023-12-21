#include <iostream>
#include <cctype>
#include <fstream>
#include <sstream>

#include "Grafo.h"

void setNodos(Grafo& grafo)
{
    ifstream file("servidores.csv");

    string line;
    getline(file, line);

    while (getline(file, line))
    {
        istringstream ss(line);

        string part;
        string parts[3];
        int i = 0;
        bool isCliente = false;

        while (getline(ss, part, ','))
        {
            parts[i] = part;
            i++;
        }

        if (parts[2] == "cliente") isCliente = true;

        grafo.agregarNodo(stoi(parts[0]), parts[1], isCliente);
    }
}

void setConexiones(Grafo& grafo)
{
    ifstream file("conexiones.csv");

    string line;
    getline(file, line);

    while (getline(file, line))
    {
        istringstream ss(line);

        string part;
        string parts[4];
        int i = 0;

        while (getline(ss, part, ','))
        {
            parts[i] = part;
            i++;
        }

        grafo.agregarConexion(stoi(parts[0]), stoi(parts[1]), stoi(parts[2]), stoi(parts[3]));
    }
}

void cinError(istream& cin)
{
    cin.clear();
    cin.ignore(INT_MAX, '\n');
    cout << "Invalid value" << endl;
}

int setWeight()
{
    int weight;

    while (true)
    {
        cout<<"Enter file's weight (Mb): ";
        cin >> weight;

        if (!cin || weight <= 0) cinError(cin);
        else break;
    }
    return weight;
}

int setId(Grafo& grafo)
{
    int id;

    while (true)
    {
        cout << "Enter ID: ";
        cin >> id;

        if (!cin || grafo.buscarNodo(id) == nullptr || !grafo.esCliente(id))
        {
            cinError(cin);
        }
        else
        {
            break;
        }
    }
    return id;
}

void data(Grafo& grafo, int userId)
{
    if (userId == -1)
    {
        cout << "Not logged in" << endl;
        return;
    }

    pair<string, int> nombreVelocidad = grafo.velocidad(userId, false);
    string nombre = nombreVelocidad.first;
    int velocidad = nombreVelocidad.second;

    cout << "You have a connection to " << nombre << endl;
    cout << "with a speed of " << velocidad << " [mb/s]" << endl;

    char option;

    while (true)
    {
        if (velocidad == 300 || velocidad == 600)
        {
            cout << "Upgrade your speed by 300 [mb/s] for 20$ [Y/N]" << endl;
            cin >> option;

            if (!cin || (tolower(option) != 'y' && tolower(option) != 'n')) cinError(cin);
            else { break; }
        }
        else if (velocidad == 900)
        {
            cout << "You alredy have the max speed!" << endl;
            return;
        }
    }
    grafo.velocidad(userId, true);
}

void inbox(Grafo grafo, int userId)
{
    if (userId == -1)
    {
        cout << "Not logged in" << endl;
        return;
    }

    vector<pair<string, int>> nombresPesos = grafo.getNombre(userId);

    if (nombresPesos.size() == 0)
    {
        cout << "You haven't any inbox" << endl;
        return;
    }

    for (auto nombrePeso : nombresPesos)
    {
        string nombre = nombrePeso.first;
        int peso = nombrePeso.second;

        cout << nombre << " has sent you a file of " << peso << " [m/b]" << endl;
    }
}

void sent(Grafo& grafo, int weight, int userId, int destinationId)
{
    bool found = grafo.bellmanFord(weight, userId, destinationId);
    if (!found) return;

    cout << endl;
    char option;

    while (true)
    {
        cout << "Sent file? [Y/N]: ";
        cin >> option;
        if (!cin || (tolower(option) != 'y' && tolower(option) != 'n')) cinError(cin);
        else { break; }
    }

    if (option == 'n') return;
    grafo.sentFile(weight, userId, destinationId);
}

int main()
{
    Grafo grafo;

    setNodos(grafo);
    setConexiones(grafo);

    int userId = -1;
    int destinationId = 1;
    int weight = -1;

    while (true)
    {
        int option;

        cout << "==Menu==" << endl;
        cout << "1. Set user's ID" << endl;
        cout << "2. Set destination ID" << endl;
        cout << "3. Set file's weight" << endl;
        cout << "4. Sent file" << endl;
        cout << "5. Change profile data" << endl;
        cout << "6. Inbox" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter an option: ";
        cin >> option;
        cout << endl;

        if (!cin) cinError(cin);

        switch (option)
        {
            case 1:
                userId = setId(grafo);
                break;

            case 2:
                destinationId = setId(grafo);
                break;

            case 3:
                weight = setWeight();
                break;

            case 4:
                sent(grafo, weight, userId, destinationId);
                break;

            case 5:
                data(grafo, userId);
                break;

            case 6:
                inbox(grafo, userId);
                break;

            case 7:
                cout << "Closing program";
                return 0;

            default:
                cout << "Invalid option" << endl;
        }
        cout << endl;
    }
}