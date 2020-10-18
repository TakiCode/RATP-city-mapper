
# les variables 
CPP=g++ 
LINKER= $(CPP) -o
OBJS=main.o Ratp_Dijkstra.o
PROG= Ratp_Dijkstra 

# les flgas 
CPPFLAGS= -c -std=c++11 -Wall -Wextra -Werror -pedantic -pedantic-errors -O3

all : $(PROG)
	

$(PROG) : $(OBJS)
	
	@echo "   "
	@echo "*****************************Exécutable $(PROG) a été  généré correctement*****************************."
	@echo "   "
	$(LINKER) $(PROG) main.o Ratp_Dijkstra.o

main.o : main.cpp Ratp_Dijkstra.cpp Ratp_Dijkstra.h Generic_mapper.hpp Generic_connection_parser.hpp Generic_station_parser.h 
	$(CPP) $(CPPFLAGS) $<
Ratp_Dijkstra.o  : Ratp_Dijkstra.cpp Ratp_Dijkstra.h Generic_mapper.hpp Generic_connection_parser.hpp Generic_station_parser.h 
	
	$(CPP) $(CPPFLAGS) $<
clean :
	@rm -rf $(PROG) $(OBJS)
	@echo " "
	@echo "*****************les fichiers objets et de l'exécutable $(PROG) ont été supprimeé avec succés***************** "
	@echo " "


