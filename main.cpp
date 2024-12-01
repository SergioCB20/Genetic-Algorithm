
/* 
 * File:   main.cpp
 * Author: Grupo 7 - Algoritmos Avanzados
 *
 * Created on 28 de noviembre de 2024, 04:06 PM
 */

#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#define Tcasamiento 0.5
#define Pcasamiento 0.5
#define Ttorneo 0.3
#define Tmutacion 0.2
#define IND 5
#define O 3
#define D 4
#define K 7
using namespace std;

struct strnodo{
    int idNodo;
    int demanda;//cantidad de bienes necesaria del nodo
    int inventario;//inventario actual del nodo
};

bool aberracion(vector<int> &cromo){
    bool bandera=true;
    for(int i=0;i<cromo.size();i++){
        for(int j=i+1;j<cromo.size();j++){
            if(cromo[i]==cromo[j])
                return true;
        }
    }
    return false;
}

int obtenerMenorCosto(double costoTransp[][D], int indDestino, vector<strnodo> 
    &nodosCopia){
    int menorCosto=99, indNodo;
    for(int i=0;i<O;i++){
        if(menorCosto>costoTransp[i][indDestino-O]&&nodosCopia[i].inventario){
            menorCosto=costoTransp[i][indDestino-O];
            indNodo=i;
        }
    }
    return indNodo;
}

int obtenerNodoPrio(vector<int> cromo, int prio){
    for(int i=0;i<cromo.size();i++){
        if(cromo[i]==prio)return i;
    }
}

// Función para procesar el transporte entre origen y destino
int procesarTransporte(int indOrigen, int indDestino, vector<strnodo>& nodosCopia, double costoTransp[][D]) {
    int costo = 0;
    if (nodosCopia[indOrigen].inventario >= nodosCopia[indDestino].demanda) {
        costo = costoTransp[indOrigen][indDestino - O] * nodosCopia[indDestino].demanda;
        nodosCopia[indOrigen].inventario -= nodosCopia[indDestino].demanda;
        nodosCopia[indDestino].demanda = 0;
    } else {
        costo = costoTransp[indOrigen][indDestino - O] * nodosCopia[indOrigen].inventario;
        nodosCopia[indDestino].demanda -= nodosCopia[indOrigen].inventario;
        nodosCopia[indOrigen].inventario = 0;
    }
    return costo;
}

// Función para copiar los nodos
vector<strnodo> copiarNodos(strnodo nodos[]) {
    vector<strnodo> nodosCopia;
    for (int i = 0; i < K; i++) {
        nodosCopia.push_back(nodos[i]);
    }
    return nodosCopia;
}

// Función para satisfacer la demanda de un nodo destino
int satisfacerDemanda(int indDestino, vector<strnodo>& nodosCopia, double costoTransp[][D]) {
    int costo = 0;

    while (nodosCopia[indDestino].demanda > 0) {
        int indOrigen = obtenerMenorCosto(costoTransp, indDestino, nodosCopia);
        costo += procesarTransporte(indOrigen, indDestino, nodosCopia, costoTransp);
    }
    
    return costo;
}


int calculafitness(vector<int> cromo, strnodo nodos[], double costoTransp[][D]) {
    int costo = 0;
    vector<strnodo> nodosCopia = copiarNodos(nodos);

    // Procesar cada nodo destino
    for (int i = D; i > 0; i--) {
        int indDestino = obtenerNodoPrio(cromo, i) + O;
        costo += satisfacerDemanda(indDestino, nodosCopia, costoTransp);
    }
    
    return costo;
}

void muestrapoblacion(vector<vector<int>> poblacion,strnodo nodos[],
        double costoTransp[][D]){
    for(int i=0;i<poblacion.size();i++){
        for(int j=0;j<poblacion[i].size();j++){
            cout << poblacion[i][j] << "  ";
        }
        cout <<" fo="<< calculafitness(poblacion[i],nodos, costoTransp)<<endl;
    }
}

void generarCromo(vector<int> &cromo){
    vector<int> prioridades;
    int ind;
    for(int i=1;i<=D;i++){
        prioridades.push_back(i);
        //llena un vector de enteros con las prioridades
    }
    while(prioridades.size()){
        ind=rand()%prioridades.size();
        //decide de forma aleatoria la prioridad del siguiente elemento del
        //vector
        cromo.push_back(prioridades[ind]);
        //inserta la id que corresponde al indice obtenido, de esta forma
        //se consigue una permutacion aleatoria
        prioridades.erase(prioridades.begin()+ind);
    }
}


void generapoblacioninicial(vector<vector<int>> &poblacion){
    int cont=0;
    srand (time(NULL));
    while(cont<IND){
        vector<int> cromo;
        generarCromo(cromo);
        poblacion.push_back(cromo);
        cont++; 
    }
}

int seleccionPorTorneo(vector<vector<int>> poblacion,strnodo nodos[], 
        double costoTransp[][D] ){
    int indMejor=rand()%poblacion.size(), indIndividuo;
    int t=round(poblacion.size()*Ttorneo);
    for(int i=0;i<t;i++){
        indIndividuo=rand()%poblacion.size();
        if(calculafitness(poblacion[indIndividuo],nodos,costoTransp)<
                calculafitness(poblacion[indMejor],nodos,costoTransp)){
            indMejor=indIndividuo;
        }
    }
    return indMejor;
}

void calculasupervivencia(vector<vector<int>>poblacion,
    vector<int>&supervivencia, strnodo nodos[], double costoTransp[][D]){
    int suma=0;
    
    for(int i=0;i<poblacion.size();i++)
        suma+=calculafitness(poblacion[i],nodos,costoTransp);
    for(int i=0;i<poblacion.size();i++){
        int fit= round(100*(double)calculafitness(poblacion[i],nodos,costoTransp)/suma);
        supervivencia.push_back(fit);
    }

}

void cargaruleta(vector<int>supervivencia,int *ruleta){
    int ind=0;
    for(int i=0;i<supervivencia.size();i++)
        for(int j=0;j<supervivencia[i];j++)
            ruleta[ind++]=i;
}


void seleccion(vector<vector<int>> poblacion, vector<vector<int>> &padres,
        strnodo nodos[], double costoTransp[][D]){
    int npadres, indMejor, cont=0;
    int ruleta[100]{-1};
    vector<int>supervivencia;
    calculasupervivencia(poblacion,supervivencia,nodos, costoTransp);
    cargaruleta(supervivencia,ruleta);
    int nseleccionados= poblacion.size()*Tcasamiento;        
    for(int i=0;i<nseleccionados;i++){
        int ind=rand()%100;
        if(ruleta[ind]>-1)
            padres.push_back(poblacion[ruleta[ind]]);
            
    } 
}

void generahijo(vector<int>padre,vector<int>madre,vector<int>&hijo){
    int pos=round(padre.size()*Pcasamiento);
    
    for(int i=0;i<pos;i++)
        hijo.push_back(padre[i]);
    for(int i=pos;i<padre.size();i++)
        hijo.push_back(madre[i]);
    
}


void casamiento(vector<vector<int>> &poblacion,vector<vector<int>> &padres){
    for(int i=0;i<padres.size();i++)
        for(int j=0;j<padres.size();j++)
            if(i!=j){
                vector<int> cromo;
                generahijo(padres[i],padres[j],cromo);
                if(!aberracion(cromo)){
                    poblacion.push_back(cromo);
                }                
           }
}

void mutacion(vector<vector<int>> &poblacion,vector<vector<int>> &padres){
    int cont=0;
    int nmuta=round(padres[0].size()*Tmutacion);
    for(int i=0;i<padres.size();i++){
        while(cont<nmuta){
            int ind=rand()%padres[0].size();
            if(padres[i][ind]==0){
                padres[i][ind]=1;
            }
            else
                padres[i][ind]=0;
            cont++;
        }
        if(!aberracion(padres[i]))
            poblacion.push_back(padres[i]);     
    }
}

  

void muestracromo(vector<int>cromo){
    for(int i=0;i<cromo.size();i++){
         cout << cromo[i] << " ";
    }   
     cout << endl;
}



void eliminaberraciones(vector<vector<int>> &poblacion){
    for(int i=0;i<poblacion.size();i++){
        if(aberracion(poblacion[i])){
            poblacion.erase(poblacion.begin()+i); 
            i--;
        }
    }
}

void eliminaclones(vector<vector<int>> &poblacion){
    int bandera;
    for(int i=0;i<poblacion.size();i++){
        for(int j=i+1;j<poblacion.size();j++){
            bandera=1;
            for(int k=0;k<poblacion[i].size();k++){
                if(poblacion[i][k]!=poblacion[j][k]){
                    bandera=0;
                    break;
                } 
            }
            if(bandera){
                poblacion.erase(poblacion.begin()+j);
                j--;
            }
        }
    }
}

int muestramejor(vector<vector<int>> poblacion,
        strnodo nodos[],double costoTransp[][D]){
    int mejor=0;
    for(int i=0;i<poblacion.size();i++)
        if(calculafitness(poblacion[i],nodos,costoTransp)<
                calculafitness(poblacion[mejor],nodos,costoTransp))
            mejor=i;
    
    cout <<"La mejor solucion es:" << calculafitness(poblacion[mejor],
    nodos,costoTransp)<<endl;
    cout<<"Prioridades: "<<endl;
    for(int i=0;i<poblacion[mejor].size();i++){
        cout << poblacion[mejor][i] << "  ";        
    }
    cout << endl;
    return calculafitness(poblacion[mejor],nodos,costoTransp);
}

void imprimelinea(){
    for(int i=0;i<100;i++)
        cout<<"-";
    cout<<endl;
}

void transporteAG(strnodo nodos[],double costoTransp[][D]){
    vector<vector<int>> poblacion;
    int cont=0, mejorAnterior=-1, mejor, genSinMejora=0;
    generapoblacioninicial(poblacion);
    while(1){
        vector<vector<int>> padres;
        cout<<"Poblacion inicial: "<<endl;
        cout<<padres.size()<<endl;
        cout<<poblacion.size()<<endl;
        muestrapoblacion(poblacion,nodos,costoTransp);
        imprimelinea();
        
        seleccion(poblacion,padres,nodos,costoTransp);
        cout<<"Fase de seleccion:"<<endl;
        cout<<padres.size()<<endl;
        cout<<poblacion.size()<<endl;
        cout<<"Padres:"<<endl;
        muestrapoblacion(padres,nodos,costoTransp);
        cout<<"Poblacion:"<<endl;
        muestrapoblacion(poblacion,nodos,costoTransp);
        imprimelinea();
        
        casamiento(poblacion,padres);
        cout <<"Casamiento:"<<endl;
        cout<<poblacion.size()<<endl;
        cout<<"Poblacion:"<<endl;
        muestrapoblacion(poblacion,nodos,costoTransp);
        imprimelinea();
        
        mutacion(poblacion,padres);
        cout <<"Mutacion:"<<endl;
        cout<<poblacion.size()<<endl;
        cout<<"Poblacion:"<<endl;
        muestrapoblacion(poblacion,nodos,costoTransp);
        imprimelinea();
        
        eliminaberraciones(poblacion);
        eliminaclones(poblacion);
        cout <<endl<<"Luego de control de aberraciones:"<<endl;
        cout<<poblacion.size()<<endl;
        cout<<"Poblacion:"<<endl;
        muestrapoblacion(poblacion,nodos,costoTransp);
        imprimelinea();
        
        mejor=muestramejor(poblacion,nodos,costoTransp);
        if(mejor==mejorAnterior)
            genSinMejora++;
        else{
            mejorAnterior=mejor;
            genSinMejora=0;
        }   
        cont++;
        if(cont==100||genSinMejora==3) break;    
        
    }    
}

int main(int argc, char** argv) {
    strnodo nodos[K]={{1,0,550},
                      {2,0,300},
                      {3,0,450},
                      {4,300,0},
                      {5,350,0},
                      {6,300,0},
                      {7,350,0}};
//    int idNodo;
//    int demanda;
//    int inventario;
    double costoTransp[O][D]={{11,14,17,13},
                              {16,19,18,15},
                              {15,16,19,18}};
    transporteAG(nodos, costoTransp);
    return 0;
}

