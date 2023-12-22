#pragma once

#include <iostream>

#include <string>
#include <vector>

using namespace std;

class Nodo
{
    private:
        int id;
        string nombre;
        bool cliente; //false = router
        vector<pair<Nodo*, pair<int, int>>> conexiones; //Vecino, velocidad y distancia
        double tiempo; //Atributo para el algoritmo BellmanFord
        Nodo* padre; // Atributo para el algoritmo BellmanFord
        vector<pair<Nodo*,int>> inbox; //inicio y peso

    public:
        Nodo(int id, string nombre, bool cliente)
        : id(id), nombre(nombre), cliente(cliente) {}

        int getId() { return id; }
        string getNombre() { return nombre; }
        bool isCliente() { return cliente; }
        vector<pair<Nodo*, pair<int, int>>>& getConexiones() { return conexiones; }
        double getTiempo() { return tiempo; }
        Nodo*& getPadre() { return padre; }
        vector<pair<Nodo*,int>>& getInbox() { return inbox; }
        void setTiempo(double nuevoTiempo) { tiempo = nuevoTiempo; }
        void setPadre(Nodo* nuevoPadre) { padre = nuevoPadre; }
        
};