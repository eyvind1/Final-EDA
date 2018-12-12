#include <iostream>
#include "SSTree.h"
#include <fstream>
#include <sstream>
#include <string>   
#include <LexRisLogic/StringSplitter.h>
#include <LexRisLogic/Convert.h>
#include <LexRisLogic/FileStream.h>
using namespace std;
#define DIM 91
#define TAM 515345

int main(int argc, char const *argv[])

{
	
    
	SSTree SStree(DIM,TAM);
    cout<<"Insertando"<<endl;
	ifstream archivo_entrada; 
    string linea;
    int contador=0;
    float con[100];

    archivo_entrada.open("YearPredictionMSD.txt");    
    getline(archivo_entrada, linea);
    while(getline(archivo_entrada,linea))
    {
    	stringstream  lineStream(linea);
    	string cell;
    	int cont=0;
    	float dato[DIM];
    	while(getline(lineStream,cell,','))
            {   
               std::string::size_type sz;    
               dato[cont]=std::stof(cell,&sz);
               cont+=1;
            }
        
        SStree.insert(dato);

    if(contador==TAM-1)
        {
        	break;
        }
       contador+=1;
    }
    SStree.query(DIM,TAM);
    cout<<"finished"<<endl;

		return 0;




}
