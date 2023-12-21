# Taller3
Participantes: Benjamín Gilberto y Jairo Vergara.

Documentación

Clase Nodo:
En la clase Nodo, utilizamos 5 atributos
int id, string nombre, bool cliente. Son atributos que todo servidor tiene (Router/Cliente)
conexiones. es un vector de todas las conexiones que tiene cada Nodo, guardando el nodo del destino y su velocidad/distancia.
int tiempo, Nodo padre. Son atributos que se utilizan para el algoritmo de BellmanFord
En los métodos son funciones simples como getId().

Main.cpp:
void setNodos(Grafo& grafo): Lee "servidores.csv" y entrega a Grafo sus componentes.
void setConexiones(Grafo& grafo): Lee "conexiones.csv" y entrega a Grafo sus componentes.
void cinError(istream& cin): limpia la bandeja cin y printea un mensaje de valor inválido.
int setWeight(): Le pide al usuario ingresar un peso, que sea mayor a 0.
int setId(Grafo& grafo): Le pide al usuario un id y llama al grafo con esCliente(id), si lo es, sigue el programa.
void data(Grafo& grafo, int userId): consulta la velocidad del router con el cual el usuario esta conectado y pregunta si quiere comprar mas mejora.
main(): despliega un menú con para eligir usuario, destino, peso, enviar archivo, cambiar datos del usuario, salir.

Clase Grafo:
Tiene como atributo un vector de nodos, el cual almacena todos los nodos de "servidores.csv"
void agregarNodo(int id, string nombre, bool cliente): recibe datos para crear un Nodo, lo crea y lo agrega a nodos.
Nodo* buscarNodo(int id): con una id, busca el Nodo que la tenga y lo devuelve.
void agregarConexion(int id1, int id2, int velocidad, int distancia): recibe los datos necesarios para crear una conexion, la crea con velocidad y distancia, y crea 2 conexiones para cada Nodo, siendo bidereccional.
bool esCliente(int id): recibe un id y busca el Nodo al que pertenece y verifica si es cliente o no.
pair<string, int> velocidad(int id, bool modificar): busca el Nodo con el id, y devuelve su nombre y velocidad, y si se quiere modificar, tambien le cambia la velocidad a +300.




