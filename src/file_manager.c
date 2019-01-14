/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: file_manager.c
    Tento modul se star� o pr�ci s datov�m souborem pro ulo�en�
    souborov�ho syst�mu pseudoNTFS. Star� se o jeho vytvo�en�, ulo�en�
    struktur, form�tov�n� atd.
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "file_manager.h"

/* ____________________________________________________________________________

    int file_exists(char *path_name)
    
    Zjis�uje, zda soubor se zadanou cestou `path_name' existuje.
    Pokud existuje, vrac� 1, v opa�n�m p��pad� vrac� 0.
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
    
    Zform�tuje soubor na po��te�n� hodnoty. Pokud soubor neexistuje, vytvo�� 
	jej. P�i �sp�chu vrac� 1, v opa�n�m p��pad� vrac� 0.
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


