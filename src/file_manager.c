/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: file_manager.c
    Tento modul se stará o práci s datovım souborem pro uloení
    souborového systému pseudoNTFS. Stará se o jeho vytvoøení, uloení
    struktur, formátování atd.
    
    Dialekt: C11
    Pøekladaè: kadı kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "file_manager.h"

/* ____________________________________________________________________________

    int file_exists(char *path_name)
    
    Zjisuje, zda soubor se zadanou cestou `path_name' existuje.
    Pokud existuje, vrací 1, v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int file_exists(char *path_name) {
	
	if (access(path_name, F_OK) != -1) {
		return 1;
	} 
	else {
		return 0;
	}
}


/* ____________________________________________________________________________

    int format_file(char *path_name, int size)
    
    Zformátuje soubor na poèáteèní hodnoty. Pokud soubor neexistuje, vytvoøí 
	jej. Pøi úspìchu vrací 1, v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int format_file(char *path_name, int size) {
	//TODO
	
}


/* ____________________________________________________________________________

  
   ____________________________________________________________________________
*/
int load_file() {
	
}


