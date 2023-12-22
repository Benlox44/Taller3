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
            //Creamos conexiones bidireccionales

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
            //devuelve el nombre y velocidad de la conexion 0 del Nodo, y como siempre es Cliente
            //siempre sera la del router (tiene una conexion solamente)
            //y si el bool es true, modifica la velocidad con +300
            Nodo* nodo = buscarNodo(id);
            string nombre = nodo->getConexiones()[0].first->getNombre();
            int velocidad = nodo->getConexiones()[0].second.first;
            pair<string, int> nombreVelocidad = make_pair(nombre, velocidad);
            if (modificar) { nodo->getConexiones()[0].second.first += 300; }
            return nombreVelocidad;
        }

        vector<pair<string, int>> getNombre(int userId)
        {
            //Devuelve el nombre y peso al cual pertenece la ID
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

        bool printRoute(Nodo* inicio, Nodo* destino)
        {
            //Printea la ruta desde el nodo destino hasta el incio, a traves de los Padres
            //A la vez que printea el tiempo acumulado de cada conexion
            bool found = false;
            if (destino->getTiempo() != DBL_MAX)
            {
                found = true;
                cout << "Fastest route from " << inicio->getNombre() 
                    << " to " << destino->getNombre() << endl;

                Nodo* actual = destino;
                Nodo* padre = actual->getPadre(); // Guardar el padre para la primera iteración
                while (actual != nullptr && padre != nullptr)
                {
                    cout << actual->getNombre() << " <- ";
                    cout << padre->getNombre() << " time: " << actual->getTiempo() << " [s]" << endl;
                    
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
            return found;
        }

        void sentFile(int weight, int userId, int destinationId)
        {
            //Agrega al nodo que pertenece destinationId y le agrega el peso
            //y quien el nodo de quien se lo envio a inbox.
            Nodo* inicio = buscarNodo(userId);
            Nodo* destino = buscarNodo(destinationId);

            pair<Nodo*,int> inbox = make_pair(inicio, weight);
            if (destino->getTiempo() != DBL_MAX) destino->getInbox().push_back(inbox);
        }
        
        bool bellmanFord(int weight, int userId, int destinationId)
        {
            //Logica de BellmanFord para encontrar la ruta mas rapida del inicio hacia todos los nodos
            if (weight == -1 || userId == -1 || destinationId == -1)
            {
                cout << "Missing data" << endl;
                return false;
            }

            Nodo* inicio = buscarNodo(userId);
            Nodo* destino = buscarNodo(destinationId);

            for (Nodo* nodo : nodos) //Inicializar tiempos en infinito y 0 el inicio
            {
                nodo->setTiempo(DBL_MAX);
                nodo->setPadre(nullptr);
            }
            inicio->setTiempo(0);

            //Relajar aristas
            for (int i = 1; i < nodos.size(); i++) //Recorremos V-1 veces
            {
                for (Nodo* nodo : nodos) //Recorremos todos los nodos
                {
                    for (auto conexion : nodo->getConexiones()) //Para cada nodo recorremos todas sus conexiones
                    {
                        Nodo* vecino = conexion.first;
                        if (vecino->isCliente() && vecino != destino) continue; //Si es cliente y ese cliente no es vecino no nos sirve (solo router)

                        int velocidad = conexion.second.first;
                        int distancia = conexion.second.second;
                        double tiempo = (static_cast<double>(weight) / static_cast<double>(velocidad)) * distancia; //Calculamos el tiempo de la conexion

                        if (nodo->getTiempo() != INT_MAX && //Si el tiempo es infinito, no sabemos como llegar aun, por lo que no sirve
                        nodo->getTiempo() + tiempo < vecino->getTiempo()) //Si el tiempo acumulado es menor que el del vecino, cambiar tiempo y padre
                        {
                            vecino->setTiempo(nodo->getTiempo() + tiempo);
                            vecino->setPadre(nodo);
                        }
                    }
                }
            }
            //padres listos, print padres de destino a inicio
            bool found = printRoute(inicio, destino);
            return found;
        }
};