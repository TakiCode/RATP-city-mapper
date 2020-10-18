#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iterator>  
#include <queue>        
#include <utility> 
#include "Ratp_Dijkstra.h"

using namespace std;

int main(int argc, char* argv[])
{
    try{
    //  une gestion des exeptions d'erreurs si on a 5 argument ou plus  a l'entrer donc forcément c'est une erreur 
    if(argc != 5)
    {
        // on sort du porgramme;
        cerr << "Nombre de paramètres est incorrectes il faut qu'il siot égale à 4" << endl;
        exit(1);
    }

    // creation de l'objet

    Ratp_Dijkstra Obj_Ratp;

    // recuperation des paarametres
    string fichier_stations = argv[1];
    string fichier_connections = argv[2];

    // lire le fichier station et connections 
    Obj_Ratp.read_stations(fichier_stations);         
    Obj_Ratp.read_connections(fichier_connections);


    char *str=argv[3];

    if ( isdigit(str[0])){
    //cout<< "integer"<<endl;
   
      uint64_t _start = stoull(argv[3]);

      uint64_t _end = stoull(argv[4]);

      vector<pair <uint64_t, uint64_t> > Chemin_Court = Obj_Ratp.compute_and_display_travel(_start,_end);
    }else{

        vector< pair<uint64_t,uint64_t> > Chemin_Court = Obj_Ratp. compute_travel(argv[3],argv[4]);
    
    }
    

      

    // le vecteur qui contient les paire des neuds et cout
    //vector<pair <uint64_t, uint64_t> > Chemin_Court = Obj_Ratp.compute_and_display_travel(_start,_end);

    }catch ( const int& ERROR){
        if (ERROR==1)
            std::cerr<<" la station et la méme vous avez pas besoin de prendre un métro "<<std::endl;
        if ( ERROR==2)
            std::cerr<<" la Station "<<argv[3]<<" n'existe pas  "<<std::endl;
        if (ERROR==4)
            std::cerr<<" la Station "<<argv[4]<<" n'existe pas  "<<std::endl;
        if (ERROR==3)
            std::cerr<<" la Station "<<argv[3]<<" n'existe pas  "<<std::endl;
        if (ERROR==5)
            std::cerr<<" la  Station "<<argv[4]<<" n'existe pas  "<<std::endl;
            

    }


    return 0;

}



