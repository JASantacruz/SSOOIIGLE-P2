DIRSRC := src/
DIREXE := exec/
DIRLIB := libros/
DIRINC := include/

LDLIBS := -pthread -std=c++11 -Iinclude/
PP := g++


all: dirs buscador

dirs: 
	mkdir -p $(DIREXE)

buscador: $(DIRSRC)manager.cpp
	$(PP) $^ -o $(DIREXE)manager $(LDLIBS)

prueba:
	./$(DIREXE)manager "$(DIRLIB)prueba.txt" que 3

oro-ceniza:
	./$(DIREXE)manager "$(DIRLIB)El-oro-y-la-ceniza.txt" capítulo 10

ultima-sirena:
	./$(DIREXE)manager "$(DIRLIB)La-última-sirena.txt" capítulo 10

leyes-equipo:
	./$(DIREXE)manager "$(DIRLIB)17-LEYES-DEL-TRABJO-EN-EQUIPO.txt" equipo 5

clear: 
	rm -r $(DIREXE)