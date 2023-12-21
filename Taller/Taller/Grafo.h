#include "Nodo.h"

#include <cfloat>

class Grafo
{
    private:
        vector<Nodo*> nodos;

    public:
        Grafo() {}

        void agregarNodo(int id, string nombre, bool cliente)
        {
            Nodo* nodo = new Nodo(id, nombre, cliente);
            nodos.push_back(nodo);
        }

        Nodo* buscarNodo(int id)
        {     
            for (Nodo* nodo : nodos) if (nodo->getId() == id) return nodo;
            return nullptr;
        }

        void agregarConexion(int id1, int id2, int velocidad, int distancia)
        {
            Nodo* nodo1 = buscarNodo(id1);
            Nodo* nodo2 = buscarNodo(id2);

            pair<int, int> velocidadDistancia = make_pair(velocidad, distancia);

            pair<Nodo*, pair<int, int>> conexion1 = make_pair(nodo2, velocidadDistancia);
            pair<Nodo*, pair<int, int>> conexion2 = make_pair(nodo1, velocidadDistancia);

            nodo1->getConexiones().push_back(conexion1);
            nodo2->getConexiones().push_back(conexion2);
        }

        bool esCliente(int id)
        {
            Nodo* nodo = buscarNodo(id);
            if (nodo->isCliente()) return true;
            return false;
        }

        pair<string, int> velocidad(int id, bool modificar)
        {
            Nodo* nodo = buscarNodo(id);
            string nombre = nodo->getConexiones()[0].first->getNombre();
            int velocidad = nodo->getConexiones()[0].second.first;
            pair<string, int> nombreVelocidad = make_pair(nombre, velocidad);
            if (modificar) { nodo->getConexiones()[0].second.first += 300; }
            return nombreVelocidad;
        }

        vector<pair<string, int>> getNombre(int userId)
        {
            Nodo* nodo = buscarNodo(userId);
            vector<pair<string, int>> nombresPesos;

            for (auto inbox : nodo->getInbox())
            {
                string nombre = inbox.first->getNombre();
                int weight = inbox.second;
                pair<string, int> nombrePeso = make_pair(nombre, weight);
                nombresPesos.push_back(nombrePeso);
            }
            return nombresPesos;
        }

        void printRoute(Nodo* inicio, Nodo* destino)
        {
            if (destino->getTiempo() != DBL_MAX)
            {
                cout << "Fastest route from " << inicio->getNombre() 
                    << " to " << destino->getNombre() << endl;

                Nodo* actual = destino;
                Nodo* padre = actual->getPadre(); // Guardar el padre para la primera iteración
                while (actual != nullptr && padre != nullptr)
                {
                    cout << actual->getNombre() << " <- ";
                    cout << padre->getNombre() << " tiempo: " << actual->getTiempo() << " [s]" << endl;
                    
                    actual = padre; // Mover al siguiente nivel
                    padre = actual->getPadre(); // Actualizar el padre
                }
                cout << endl;
                cout << "Total time: " << destino->getTiempo() << " [s]" << endl;
            }
            else
            {
                cout << "There isn't any route from " << inicio->getNombre() 
                    << " to " << destino->getNombre() << endl;
            }
        }
        
        void bellmanFord(int weight, int userId, int destinationId)
        {
            Nodo* inicio = buscarNodo(userId);
            Nodo* destino = buscarNodo(destinationId);

            //Inicializar tiempos en infinito y 0 el inicio
            for (Nodo* nodo : nodos)
            {
                nodo->setTiempo(DBL_MAX);
                nodo->setPadre(nullptr);
            }
            inicio->setTiempo(0);

            //Relajar aristas
            for (int i = 1; i < nodos.size(); i++)
            {
                for (Nodo* nodo : nodos)
                {
                    for (auto conexion : nodo->getConexiones())
                    {
                        Nodo* vecino = conexion.first;
                        if (vecino->isCliente() && vecino != destino) continue;

                        int velocidad = conexion.second.first;
                        int distancia = conexion.second.second;
                        double tiempo = (static_cast<double>(weight) / static_cast<double>(velocidad)) * distancia;

                        if (nodo->getTiempo() != INT_MAX && 
                        nodo->getTiempo() + tiempo < vecino->getTiempo())
                        {
                            vecino->setTiempo(nodo->getTiempo() + tiempo);
                            vecino->setPadre(nodo);
                        }
                    }
                }
            }
            //padres listos, print padres de destino a inicio
            printRoute(inicio, destino);
            
            pair<Nodo*,int> inbox = make_pair(inicio, weight);
            if (destino->getTiempo() != DBL_MAX) destino->getInbox().push_back(inbox);
        }
};