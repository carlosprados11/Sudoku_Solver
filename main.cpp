/* Algoritmo para la lectura de un grafo desde un fichero,
busqueda de solución mediante ramificación y poda, y representación
de la solución*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>

#define num_datos 30
#define num_ite 5
#define fichero_sudoku "info_sudoku.txt"
#define fichero_grafo "info_grafo.txt"
#define fichero_dimacs "info_dimacs.cql"

using namespace std;

class Vertice
{
    public:
        int id; // Identificador del vertice
        vector <int> adj; // Vertices adjuntos
        int color;
        int linea;
        int columna;
        int cuadro;
        int contador;
        vector <int> posibles;
        int padre;
        bool testeado;

        void Muestra_Datos() // Muestra valores de un vertice
        {
            cout << "\nid: " << id << "\tCont: ";
            if(color == 0)
                cout << "( )";
            else
                cout << "(" << color << ")";

            cout << "\tAdj: ";
            for (unsigned int j=0; j<adj.size(); j++)
                cout << adj[j] << " ";
        }
};

vector <Vertice> Mi_Grafo; // Grafo del sudoku
vector <vector <int>> Mi_Sudoku; // Contenido del sudoku
void Profundidad(int);
vector <Vertice> Costructor;

void Genera_Grafo()
{
    ofstream grafo(fichero_grafo, ios::out);

    Vertice Aux;

    if(grafo.good())
    {
        cout << endl << "Generando grafo" << endl;

        for(int i=1; i<=81; i++)
        {
            Aux.linea = 1 + (i-1)/9;
            Aux.columna = 1 + (i-1)%9;

            int var_aux = (Aux.linea-1)/3;
            Aux.cuadro = 1 + (3*var_aux + (Aux.columna-1)/3);
            Costructor.push_back(Aux);
        }

        cout << endl << "Escribiendo fichero \"" << fichero_grafo << "\"." << endl;

        for(int i=1; i<=81; i++)
        {
            grafo << " n " << i;
           // grafo << " " << Costructor[i-1].linea << " " << Costructor[i-1].columna << " " << Costructor[i-1].cuadro;
            for(int j=1; j<=81; j++)
            {
                if(((Costructor[i-1].linea == Costructor[j-1].linea) || (Costructor[i-1].columna == Costructor[j-1].columna) || (Costructor[i-1].cuadro == Costructor[j-1].cuadro)) && (i != j))
                {
                    //grafo << "";
                    grafo << " " << j;
                }

            }
            grafo << endl;
        }

        grafo.close();
    }
    else
        cerr << endl << "Error al crear el fichero \"" << fichero_grafo << "\"" << endl;
}

void Lee_Grafo()
{
    ifstream grafo; // Fichero contenedor del grafo
    string lectura;
    bool det_id = true; // Determina que nos viene un nuevo vertice
    bool volcar = false; // Determina momento de volcar la información a la memoria

    grafo.open(fichero_grafo, ios::in);
    if(grafo.good())
    {

        cout << endl << "Fichero \"" << fichero_grafo << "\" abierto con exito." << endl;
        Vertice Nuevo_Vertice;

        while(!grafo.eof())
        {
            grafo >> lectura;

            if(!grafo.fail())
            {
                stringstream str;
                str << lectura;

                if(lectura == "n") // Nuevo vertice
                {
                    det_id = true;

                    if (volcar==true)
                    {
                        Mi_Grafo.push_back(Nuevo_Vertice); // Guardo nuevo vertice
                        Nuevo_Vertice.adj.clear(); // Vacio la variable
                    }
                    else
                        volcar = true;

                }
                else
                {
                    if(det_id == true)
                    {
                        str >> Nuevo_Vertice.id;
                        det_id = false;
                    }
                    else
                    {
                        int aux;
                        str >> aux;
                        Nuevo_Vertice.adj.push_back(aux);
                    }
                }
            }
        }

        Mi_Grafo.push_back(Nuevo_Vertice);

        cout << "Fichero \"" << fichero_grafo << "\" leido y cerrado con exito.";
        grafo.close();
    }
    else
        cerr << endl << "Error en la apertura del fichero \"" << fichero_grafo << "\"" << endl;
}

void Muestra_Grafo()
{
    // Mostramos el contenido del grafo
    cout << endl << "Vertices y sus adyacentes: "<< endl;
    for(unsigned int i=0; i<Mi_Grafo.size(); i++)
        Mi_Grafo[i].Muestra_Datos();

    cout << endl;
}

void Lee_Sudoku()
{
    ifstream sudoku; // Fichero contenedor del grafo
    int lectura;
    //string nombre_fichero = "info_sudoku.txt";

    int linea = 0;
    int columna = 0;
    vector <int> Fila;

    sudoku.open(fichero_sudoku, ios::in);
    if(sudoku.good())
    {

        cout << endl << endl << "Fichero \"" << fichero_sudoku << "\" abierto con exito." << endl;

        while(!sudoku.eof())
        {
            sudoku >> lectura;

            if(!sudoku.fail())
            {
                columna++;
                Fila.push_back(lectura);

                if(columna>=9)
                {
                    columna = 0;
                    linea++;
                    Mi_Sudoku.push_back(Fila);
                    Fila.clear();
                }
            }
        }

        cout << "Fichero \"" << fichero_sudoku << "\" leido y cerrado con exito" << endl;
        sudoku.close();
    }
    else
        cerr << endl << "Error en la apertura del fichero \"" << fichero_sudoku << "\"";
}

void Muestra_Sudoku()
{
    // Mostramos el contenido del grafo.
    cout << endl;
    for(unsigned int i=0; i<9; i++)
    {
        if(i%3 == 0)
            cout << " --------- --------- ---------" << endl;

        for(unsigned int j=0; j<9; j++)
        {
            if(j%3 == 0)
                cout << "|";

            if (Mi_Sudoku[i][j] == 0)
                cout << "   ";
            else
                cout << " " << Mi_Sudoku[i][j] << " ";

        }
        cout << "|" << endl;
    }
    cout << " --------- --------- ---------" << endl;

}

void Compruebo_Datos()
{
    // Funcion necesaria para indicar que valores tenemos
    for(unsigned int i=0; i<9; i++)
    {
        for(unsigned int j=0; j<9; j++)
        {
            Mi_Grafo[9*i+j].color = Mi_Sudoku[i][j];
        }
    }
}

bool Busqueda()
{
    // Algoritmo de busqueda heuristica
    bool finish = false;
    int it = 0;
    int inicio = -1;
    int nuevo_vertice;

    do // Busco el vertice de inicio (primero vacio)
    {
        if(Mi_Grafo[it].color==0)
            inicio = it;
        else
            it++;

    }while(inicio == -1);

    nuevo_vertice = inicio;
    vector <int> imposible;
    int anterior = inicio;

    // El vertice de inicio no tiene padre
    Mi_Grafo[inicio].padre = - 1;

    do
    {
        if(Mi_Grafo[nuevo_vertice].testeado==false)
        {
            // Busco colores posibles (numero posible)
            imposible.clear();

            // Busco numeros que no puedo poner
            for(unsigned int i=0; i<Mi_Grafo[nuevo_vertice].adj.size(); i++)
            {
                if(Mi_Grafo[Mi_Grafo[nuevo_vertice].adj[i]-1].color != 0)
                    imposible.push_back(Mi_Grafo[Mi_Grafo[nuevo_vertice].adj[i]-1].color);
            }

            // Hago el complemento y busco los que se pueden poner
            Mi_Grafo[nuevo_vertice].posibles.clear();
            for(int j=1; j<=9; j++)
            {
                unsigned int counter = 0;
                for(unsigned int i=0; i<imposible.size(); i++)
                {
                    if(imposible[i]!=j)
                        counter++;
                }

                if(counter==imposible.size())
                {
                    Mi_Grafo[nuevo_vertice].posibles.push_back(j);
                }
            }

            // Ya tengo los numeros posibles de ese cuadrado
            Mi_Grafo[nuevo_vertice].testeado=true;
        }

        // No hay datos posibles que poner. Entoces marcha atras
        if(Mi_Grafo[nuevo_vertice].posibles.size()==0 || Mi_Grafo[nuevo_vertice].posibles.size()<=Mi_Grafo[nuevo_vertice].contador)
        {
            //cout << endl << "\nVUELTO ATRAS\n";
            // Borro los valores preestablecidos
            Mi_Grafo[nuevo_vertice].color = 0;
            Mi_Grafo[nuevo_vertice].posibles.clear();
            int linea = nuevo_vertice/9;
            int columna = nuevo_vertice%9;
            Mi_Sudoku[linea][columna] = 0;
            Mi_Grafo[nuevo_vertice].testeado = false;
            Mi_Grafo[nuevo_vertice].contador=0;

            // Vuelvo hacia atras
            nuevo_vertice = Mi_Grafo[nuevo_vertice].padre;
            Mi_Grafo[nuevo_vertice].contador++;
        }
        else if(Mi_Grafo[nuevo_vertice].posibles[Mi_Grafo[nuevo_vertice].contador]<10 && Mi_Grafo[nuevo_vertice].posibles[Mi_Grafo[nuevo_vertice].contador]>0) // Podemos rellenar datos del sudoku y buscamos el siguiente vertice
        {
            // Rellenamos sudoku
            Mi_Grafo[nuevo_vertice].color = Mi_Grafo[nuevo_vertice].posibles[Mi_Grafo[nuevo_vertice].contador];
            int nuevo_color = Mi_Grafo[nuevo_vertice].color;
            int linea = nuevo_vertice/9;
            int columna = nuevo_vertice%9;
            Mi_Sudoku[linea][columna] = nuevo_color;

            bool salir = false;
            anterior = nuevo_vertice;

            // Busco el siguiente vertice vacio
            if(nuevo_vertice<80)
            {
                nuevo_vertice++;
                do
                {
                    if(Mi_Grafo[nuevo_vertice].color==0)
                    {
                        Mi_Grafo[nuevo_vertice].padre = Mi_Grafo[anterior].id - 1;
                        salir=true;
                    }
                    else
                    {
                        nuevo_vertice++;
                        if(nuevo_vertice==80)
                            salir = true;
                    }


                }while(salir==false);
            }
            else
                finish=true;

        }

        //Muestra_Sudoku();

    }while(finish==false && nuevo_vertice!=-1);

    return finish;
}

void Genera_Dimacs()
{
    ofstream dimacs(fichero_dimacs, ios::out);

    if(dimacs.good())
    {
        cout << endl << "Generando fichero \"" << fichero_dimacs "\"" << endl;

        dimacs << "c File: " << fichero_dimacs;
        dimacs << endl << "c" << endl << "c Source: Carlos Prados, Andrea Fontalvo, Yao Hu";
        dimacs << endl << "c c.prados@alumnos.upm.es" << endl;
        dimacs << endl << "p edge " << Mi_Grafo.size() << " ";
        dimacs << Mi_Grafo.size()*Mi_Grafo[0].adj.size()/2;

        // Cuento el numero de aristas
        for(unsigned int i=0; i<Mi_Grafo.size(); i++)
        {
            for(unsigned int j=0; j<Mi_Grafo[i].adj.size(); j++)
            {
                if(Mi_Grafo[i].id<Mi_Grafo[i].adj[j])
                    dimacs << endl << "e " << Mi_Grafo[i].id << " " << Mi_Grafo[i].adj[j];
            }
        }

        dimacs.close();
    }
    else
        cerr << endl << "Error al crear el fichero \"" << fichero_dimacs << "\"" << endl;
}

int main()
{
    // Genera el grafo
    Genera_Grafo();

    // Tomo información del grafo
    Lee_Grafo();

    // Tomo información del sudoku
    Lee_Sudoku();

    // Compruebo que valores están disponibles
    Compruebo_Datos();

    // Muestro los datos tomados
    //Muestra_Grafo();
    Muestra_Sudoku();

    // Genero el archivo DIMACS
    Genera_Dimacs();

    if(Busqueda())
    {
        cout << endl << endl << "Sudoku resuelto con exito.";
        cout << endl << "Resultado obtenido:" << endl << endl;
        Muestra_Sudoku();
    }
    else
        cout << endl << endl << "Solución no encontrada.";

    cout << endl;

    return 0;
}
