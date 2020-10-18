#include "Ratp_Dijkstra.h"

//--------------------------------------------------------------------------------------------------//
//                         ******* Sorbonne Université ******                                       //
//          Projet C++: Application RATP avec l'algorithme de recherche Dijkstra                    //
//                                                                                                  //   
//                         Nom: Bouchakour - Prénom: Lotfi                                          //                                                                                                          // 
//                         Professeur Encadrant :  Vincent Fraillon Maison                          //   
//                                                                                                  //
//                                                                                                  // 
//                                                                                                  //
//                                                                                                  //
//--------------------------------------------------------------------------------------------------//



//-------------------------------------méthode Read_Connection--------------------------------------//


void Ratp_Dijkstra :: read_connections(const std::string& _filename)
{
    // lire le fichier 
    std::ifstream ifs (_filename, std::ifstream::in);

    // on déclare une variable qui va contenir les lignes lues 
    std::string Line_R;

    // declaration d'une map qui va contenir les unorderd_map indexé par des unit64
    std::map< uint64_t,std::unordered_map<uint64_t,uint64_t>  > connection_station;
    
    // la 1er Line inutile 
    getline(ifs,Line_R);

    // une boucle qui lit les ligne du fichier
    while (std::getline(ifs, Line_R))
    {
        // on créé un objet istringstream pour transformer le string en un flux
        std::istringstream istrstrm(Line_R);

        // création des strings pour ranger les chains découpée 
        std::string id_line;
        std::string connection_line;
        std::string time ;

        getline(istrstrm,id_line,',');
        getline(istrstrm,connection_line,',');
        getline(istrstrm,time,',');
        
        // conversion des strings recupéré vers des unsigned 

            uint64_t int_id_line = std::stoull(id_line,nullptr,10);
            uint64_t int_connection_line = std::stoull(connection_line,nullptr,10);
            uint64_t int_time = std::stoull(time,nullptr,10);       

            // on remplit la unordered_map qui est a l'interiuer de la map 
            (connection_station[int_id_line]).insert(std::make_pair(int_connection_line,int_time));
       
        // remplisage de la haschmap 
         connections_hashmap[int_id_line] = connection_station[int_id_line];


    }


}



//-------------------------------------Méthode read_stations---------------------------------------


void Ratp_Dijkstra :: read_stations(const std::string& _filename)
{

  // ouverture du fichier station en lecture
   std::ifstream ifs (_filename, std::ifstream::in);

   // la variable des lignes lues 
   std::string line;

   // la variable pour enregistrer les id des stations
   uint64_t station_id ;
   
   // la 1er line inutile 
   getline(ifs,line);
// une boucle qui lit les ligne du fichier
   while (std::getline(ifs, line))
    {

      //création de la structure 
      travel::Station s;

        // conversion les string en un flux
        std::istringstream istrstrm (line);

       // devise chaque ligne suivant le separateur ","
        getline(istrstrm, s.name,',');
        
        // sta_id c'est un string qui va contenir les id des ligne 
        std::string sta_id;
        getline(istrstrm,sta_id,',');
        
        getline(istrstrm, s.line_id,','); 
    
        getline(istrstrm, s.address,',');

        getline(istrstrm, s.line_name,',');
      
        station_id = (std::stoull(sta_id,nullptr,10));
          
        //remplisage de la hashmap
        stations_hashmap[station_id] = s;

    }

}

//----------------------------------fonction Recontitution_chemin---------------------------------

//"Reconstruction_chemin" elle recuperer chaque noeud avec son cout et son noeud parent
// tant qu'on est pas arrivé au noued de debut 
    
std::vector<std::pair<uint64_t,uint64_t>> Recontitution_chemin(std::map< uint64_t,std::pair<uint64_t,uint64_t> > map_visited,
    std::map< uint64_t,std::pair<uint64_t,uint64_t> > map_ouvert,uint64_t Id_debut,uint64_t Id_courant){
    

    // un vector pour stocker le chemin trouvé 
   std::vector < std::pair <uint64_t, uint64_t> > chemin_trouve;

    do{

        // on récupère le cout pour arriver a ce noeud
        uint64_t cout_actuel = std::get<1>(map_visited[Id_courant]);

        uint64_t node_pere = std::get<0>(map_visited[Id_courant]);

        // on insère une pair {noeud:cout_correspodant}
        chemin_trouve.push_back(std::make_pair(Id_courant,cout_actuel)); 

        // le noeud courant devient le noeud pére
        Id_courant = node_pere;

     }while(Id_courant != Id_debut);

    chemin_trouve.push_back(map_ouvert[Id_debut]);
    return chemin_trouve;
 }

//////////////////////////



 std::vector<std::pair<uint64_t,uint64_t> > Ratp_Dijkstra::compute_travel(const std::string& Id_Start, const std::string& Id_End){

     uint64_t start_id=0;
     uint64_t fin_id=0;
    for (auto it : stations_hashmap){
        if(it.second.name==Id_Start){
            start_id=it.first;
            
        }
        if(it.second.name==Id_End){
            fin_id=it.first;
            
        }

    }

    if ( start_id== 0 )
        throw(3);

    if ( fin_id==0 )
        throw(4);
    
    return compute_and_display_travel(start_id,fin_id);
 }

//--------------------------------------Méthode compute_travel --------------------------------------//


// cette méthode elle permet de trouvé le plus cout chemin on se basant sur l'algorithme de dijkstra

 std::vector<std::pair<uint64_t,uint64_t> > Ratp_Dijkstra::compute_travel(uint64_t Id_Start, uint64_t Id_End)
 {

    bool debut(false),fin(false);
    if ( Id_Start== Id_End )
        throw(1);
    uint64_t current_node = Id_Start;

    for (auto it : this->stations_hashmap ){
        if ( it.first == Id_Start )
            debut =true;
        if( it.first == Id_End )
            fin=true;
    }
    if ( !(debut ) )
        throw(2);
    if(!(fin))
        throw(5);
   


    // noeuds déja visités et noeuds ouverts en va les ranger sous forme : { noeud "parent","cout"}
    
    // création d'une map qui va contenir les pair visiter 
    std::map< uint64_t,std::pair<uint64_t,uint64_t> > visited;

    // création d'un map qui va contenir les noeuds ouvert 
    std::map< uint64_t,std::pair<uint64_t,uint64_t> > ouvert;

    // création d'un vector qui va vontenir le meilleur chemin sous la forme de {  noeud : cout }
    std::vector < std::pair <uint64_t, uint64_t> > meilleur_chemin;

    //initialisation de la map avec le 1er noued (station de départ) 
    //donc on va pas revenir a cette station
    visited[current_node] = std::make_pair(current_node,0);

     // on insere le premier noeud avec un cout 0 dans la map ouvert pour l'analyser 
    ouvert[current_node] = std::make_pair(current_node,0);
    
    // utilisation de priority_queue pour stocker les noeuds ouverts avec ordre décroissant    
    std::priority_queue< std::pair<uint64_t,uint64_t> ,std::vector<std::pair<uint64_t,uint64_t>> ,Ratp_Dijkstra> Prio_queue;


    // tant qu'on a pas atteint de la station finale (Id_End) , on continue la recherche 
    while(current_node != Id_End)
    {


    // création d'une unordered_map pour contenir les  les connections possibles de current_node

    std::unordered_map< uint64_t,uint64_t > connections_station = connections_hashmap[current_node];

    // on recupere les noeuds fils du noeud courant

    for(std::unordered_map< uint64_t,uint64_t >::iterator i = connections_station.begin(); i != connections_station.end(); i++)
   
    {

        // si le noeud et dans la liste visited, on passe  a la station suivente (noeud suivant)
        
        if(noeud_existe(i->first,visited))
        {
            continue;
        }


        // on calcule le nouveau cout(temps) pour arriver au noeud fils (station suivent )a partir
        // du noeud current 
        uint64_t Somme_cout_fils = std::get<1>(ouvert[current_node]) + i->second ;
        
        // si le noud fils il existe dans la map ouvert on va l'analyser 
        if(noeud_existe(i->first,ouvert))
        {


           // on met a jour le cout s'il est inferieur au cout du chemin precedent
           // nv_cout_fils = cout du noueud depuis parant + le cout cumule pour
           // arriver au noeud actuel

            if(Somme_cout_fils < std::get<1>(ouvert[i->first]) )
            {
                // on met à jour le cout pour arriver au noeud fils
                // à travers le nouveau noeud pere

                std::get<1>(ouvert[i->first]) = Somme_cout_fils;
                // on met à jour le pere pour arriver au noeud fils
                // qui a generé le cout minimale 
                std::get<0>(ouvert[i->first]) = current_node;

                // on créé une nouvelle priority_queue
                // pour ranger les nouveaux éléments modifiés
                std::pair<uint64_t,uint64_t> pair_info = std::make_pair(i->first,Somme_cout_fils);
                Prio_queue.push(pair_info);
                
            }

        }

        else
        {

           // on met a jour le cout s'il est inferieur au cout du chemin precedent
           // nv_cout_fils = cout du noueud depuis le parent  + le cout cumule pour
           // arriver au noeud actuel


           std::pair<uint64_t,uint64_t> pair_info = std::make_pair(i->first,Somme_cout_fils);

           // on ajoute la pair au contenaire queue
           Prio_queue.push(pair_info);

           // on ajoute le noeud au contenaire ouvert
           ouvert[i->first] = std::make_pair(current_node,Somme_cout_fils);

        }

    }

    // comme priopity_queue range les elements selon le cout le plus bas donc on recupére 
    //le plus petit avec get<0>(Prio_queue.top())
    current_node = std::get<0>(Prio_queue.top());

     // on met le noeud visité dans la map visited pour ne pas revenir dans ce node
    visited[current_node ] = ouvert[current_node];

    // on supprime le noeud courant de la priority_queue 
    Prio_queue.pop();

    } // fin de la boucle while


    // on reconstitu le chemin 
    meilleur_chemin= Recontitution_chemin(visited,ouvert,Id_Start,current_node);
    return meilleur_chemin;
}

// -------------------------------Méthode noeud_existe --------------------------------------------


//cette mhéthode range les noeuds visité dans des map

 bool Ratp_Dijkstra::noeud_existe(uint64_t noeud , std::map< uint64_t,std::pair<uint64_t,uint64_t> > visited)
{       
        // chercher si le noeud il existe dans visited
        for( std::map< uint64_t,std::pair<uint64_t,uint64_t> >::iterator i = visited.begin() ; i != visited.end(); i++)
        {
            if(i->first == noeud)
                return true;
        }

        return false;
}

bool Ratp_Dijkstra::operator() (const std::pair<uint64_t,uint64_t>& a, const std::pair<uint64_t,uint64_t> & b) const
{
         return (std::get<1>(a) > std::get<1>(b));
}


//------------------------------------Fonction Convert_h_min_sec------------------------------------------//

std::string Convert_h_min_sec(uint64_t sec){


uint64_t Heurs;
Heurs=sec/3600;
uint64_t Minute;
Minute=sec/60;
uint64_t Secondes_Cn= sec - (Heurs*3600 + Minute*60) ;

std::string Convert(" ");

if(Heurs==0){
  if(Minute==0){
     Convert= std::to_string(Secondes_Cn)+"secondes";
  }

  else{ Convert= std::to_string(Minute)+" min: "+std::to_string(Secondes_Cn)+" secondes ";
  }

}

else {
  Convert= std::to_string (Heurs)+"h: "+std::to_string(Minute)+"min: "+std::to_string(Secondes_Cn)+"secondes";
}

return Convert;

}

//-------------------------------------Méthode compute_and_display_travel---------------------------------------

// cette méthode elle permet de trouvé le plus court chemin et d'afficher le trajet l'itinéraire emprunté

std::vector<std::pair<uint64_t,uint64_t> > Ratp_Dijkstra::compute_and_display_travel(uint64_t Id_Start, uint64_t Id_End)
{

        
    // on appel  a la fonction compte_travel pour recupérer le plus cour chemin 

    std::vector<std::pair<uint64_t,uint64_t> > Chemin_Court = compute_travel(Id_Start,Id_End);
    
    // tableau de Consigness a suivre
    std::string Consignes[4] = {"Marchez jusqu'à la station métro ","Prenez le métro( ligne ","-->Descendez à la station "};
    
    // difinition d'un string pour l'etuliser a l'affichage 

    std::string result("");

    // initialisation de la variable string pour l'afficher 
    result +='\n';
    result += "Meilleur chemin de " +
    stations_hashmap[Id_Start].name + " ( line " +stations_hashmap[Id_Start].line_id + " )"+ " vers "
    + stations_hashmap[Id_End].name + " ( line " +stations_hashmap[Id_End].line_id + " )" +'\n' ;

    // recupérer la taille de vector Chemin_Court retourné par comput_travel
    int size_chemin = Chemin_Court.size();

    // varialbe pour enregistrer le debut d'un trajet en métro
    int debut_trajet = 0;

    // cout pour un trajet dans la meme ligne 
    int cout_trajet = 0;
    // cout pour un deplacement a pied
    int Cout_Apied = 0;

    // on recupere toute les stations
    for(int _sta = 1 ; _sta < size_chemin; _sta++)
    {

        // on recupère la station
        travel::Station station = stations_hashmap[std::get<0>(Chemin_Court.at(size_chemin - _sta ) )];
        
        // on recupère la station suivente 
        travel::Station next_station = stations_hashmap[std::get<0>(Chemin_Court.at(size_chemin - (_sta+1) ))];

        // si les 2 lignes qui se suivent ne sont plus les memes
        // donc on a un changement de ligne
        // cout de depalcement entre chaque station
        Cout_Apied = std::get<1>(Chemin_Court.at(size_chemin - (_sta+1)) ) -
        std::get<1>(Chemin_Court.at(size_chemin - (_sta)) ) ;

        if( station.line_id != next_station.line_id ) 
        {

            // si on est en cours de trajet, et on a changé de ligne
            // forcement, le trajet s'est terminé
            if(debut_trajet){

                // on recupere la station du debut du trajet
                travel::Station station_debut_trajet = stations_hashmap[std::get<0>(Chemin_Court.at(size_chemin - debut_trajet))];

                // on recupere la station de fin du trajet
                travel::Station station_fin_trajet = stations_hashmap[std::get<0>(Chemin_Court.at(size_chemin - _sta))];

                // on recupere le cout du trajet em métro
                // qui est egale a  "point de debut -le point d'arret"

                cout_trajet = std::get<1>(Chemin_Court.at(size_chemin - (_sta)) ) -
                std::get<1>(Chemin_Court.at(size_chemin - (debut_trajet)) ) ;

                // mettre à jour les instructions
                 
                 result += "-->"+ Consignes[1] + station_debut_trajet.line_id+" ) " + " " +"Direction : "+
                 station_debut_trajet.line_name +'\n'+Consignes[2]+ station_fin_trajet.name +"."+ 
                 +"("+Convert_h_min_sec(cout_trajet)+")"+ '\n'; 
                 
                  //catch(const std::out_of_range& e){
                  //std::cerr<<"out_of_range : "<< e.what()<<'\n';

                  //}
                 // on recupere la station de fin du trajet
                travel::Station station = stations_hashmap[std::get<0>(Chemin_Court.at(size_chemin- _sta ) )];

                // on recupere la station qui correspond a l'indice _sta
                travel::Station next_station = stations_hashmap[std::get<0>(Chemin_Court.at(size_chemin- (_sta+1) ) )];


                if(Cout_Apied!=0){

                result +="-->"+ Consignes[0] + next_station.name + ",ligne " +
                next_station.line_id + " ( " + Convert_h_min_sec(Cout_Apied) +" )" + '\n';
                }

                // on a termine le trajet, on réinitialise le variable à 0
                debut_trajet = 0;
                // on passe a l'iteration suivante
                continue;


            }

            // si on a pas commence de trajet
            // on enregistre la valeur du de la station ou un
            // trajet va commencer
            if(Cout_Apied!=0){
            // mettre à jour les instructions
            result += "-->"+Consignes[0] + next_station.name + ",ligne " +
            next_station.line_id + " ( " + Convert_h_min_sec(Cout_Apied) + " )" + '\n';
            }


        }

        

        else
        {

            if(Cout_Apied == 0 )
            // cout entre station est égale à zéro
            // 2 lignes sont identiques
            // on a juste changé de direction sur la meme ligne
            {
                if(debut_trajet)
                {

                    // recuperer la station du debut du trajet
                     travel::Station station_debut_trajet = stations_hashmap[std::get<0>
                     (Chemin_Court.at(size_chemin- debut_trajet))];

                     // recuperer la station du fin du trajet
                     travel::Station station_fin_trajet = stations_hashmap[std::get<0>
                     (Chemin_Court.at(size_chemin - (_sta )))];

                     // on recuperer le cout total du trajet
                     cout_trajet = std::get<1>(Chemin_Court.at(size_chemin - (_sta)) ) -
                     std::get<1>(Chemin_Court.at(size_chemin - (debut_trajet)) ) ;

                     // on met a jour les instruction à suivre
                     result +=  "-->"+ Consignes[1] + station_debut_trajet.line_id+" ) " + " " +"Direction : "+
                     station.line_name + "\n"+Consignes[2] + station_fin_trajet.name +" ( "
                     + Convert_h_min_sec(cout_trajet)+" ) "+ "\n";

                     if(Cout_Apied!=0){
                     result += "-->" + Consignes[0] + next_station.name + ", ligne " +
                     next_station.line_id + " ( " + Convert_h_min_sec(Cout_Apied) +")" + '\n'        ;
                    }
                    // fin du trajet
                     debut_trajet = 0;



                }
                else
                {

                    // cout égale à zéro en juste changer de direction sur la meme ligne
                    if(Cout_Apied!=0){

                    result += "-->"+Consignes[0] + next_station.name + ", " +
                    next_station.line_name + " ( " + Convert_h_min_sec(Cout_Apied) +")"+'\n';
                    }

                }

            }else
            // le cout n'est pas égale à zéro, on a pris une ligne de métro
            {
                // si on est en trajet, on sort
                // on fait rien, on laisse le trajet suivre son cours
                if(debut_trajet )
                    continue;

                // sinon on commence un nouvel trajet
                debut_trajet = _sta;

            }

        }
    }

    // s'il reste un trajet en cours non traité
    if( debut_trajet)
    {
                     // on recupère la station
                    travel::Station station = stations_hashmap[std::get<0>(Chemin_Court.at(size_chemin - debut_trajet ) )];

                    travel::Station next_station = stations_hashmap[std::get<0>(Chemin_Court.at(size_chemin - 1) )];

                    Cout_Apied = std::get<1>(Chemin_Court.at(0) ) -
                    std::get<1>(Chemin_Court.at(size_chemin - (debut_trajet)) );

                     travel::Station station_debut_trajet = stations_hashmap[std::get<0>
                     (Chemin_Court.at(size_chemin - debut_trajet))];
                     travel::Station station_fin_trajet = stations_hashmap[std::get<0>
                     (Chemin_Court.at(0))];

                     result += "-->"+  Consignes[1] +station_debut_trajet.line_id+" ) " + " " +"Direction : "+
                     station.line_name +  '\n'+ Consignes[2] + station_fin_trajet.name +" ( " 
                     + Convert_h_min_sec(Cout_Apied)+" )"+ '\n';

                     
    }


    // on recupere le cout cumule pour arriver a destination
    result += '\n';
    int cout_cumule = std::get<1>(Chemin_Court.at(0));
    result += "*********Vous étes arrivé à distination Temps total estimé : " 
    + Convert_h_min_sec (cout_cumule) +"*********"+'\n';

    std::cout << result;
    return Chemin_Court;
}




