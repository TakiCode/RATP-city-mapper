#ifndef RATP_DIJKSTRA_H_INCLUDED
#define RATP_DIJKSTRA_H_INCLUDED

#include "Generic_mapper.hpp"
#include <fstream> 
#include <sstream>
#include <istream> 
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <stdexcept>
#include <iterator>   
#include <queue>
#include <algorithm>


class Ratp_Dijkstra : public travel::Generic_mapper
{
    public:
  
    std::vector<std::pair<uint64_t,uint64_t> > compute_travel(uint64_t _start, uint64_t _end) override;
    std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end) override;
    std::vector<std::pair<uint64_t,uint64_t> > compute_travel(const std::string&, const std::string&)override;
    bool noeud_existe(uint64_t noeud , std::map< uint64_t,std::pair<uint64_t,uint64_t> > visited);
    bool operator() (const std::pair<uint64_t,uint64_t>& a, const std::pair<uint64_t,uint64_t> &b) const;
    void read_stations(const std::string& _filename) override;
    void read_connections(const std::string& _filename) override;

};


#endif 

