#ifndef GENERIC_STATION_PARSER_H_INCLUDED
#define GENERIC_STATION_PARSER_H_INCLUDED


#include <string>
#include <iostream>
#include <unordered_map>
#include <map>

namespace travel{
  typedef struct Station Station;
  struct Station{
    std::string name;
    std::string line_id;
    std::string address;
    std::string line_name;
    friend std::ostream& operator<<(std::ostream& _os, const Station& _stop){
      _os << "Station: " << _stop.name << " (line " << _stop.line_id << ")";
      return _os;
    }
  };

  class Generic_station_parser{
  public:

    const std::unordered_map<uint64_t, Station>& get_stations_hashmap() const{
      return this->stations_hashmap;
    }

  protected:
    virtual void read_stations(const std::string& _filename) = 0;

  protected:
    std::unordered_map<uint64_t, Station> stations_hashmap;
  };
}



#endif // GENERIC_STATION_PARSER_H_INCLUDED
