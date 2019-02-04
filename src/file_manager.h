/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavièkový soubor: file_manager.h
    Detailní informace v souboru file_manager.c
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#ifndef _FILE_MANAGER_H
#define _FILE_MANAGER_H

/* ____________________________________________________________________________

    Definované symboly preprocesoru
   ____________________________________________________________________________
*/
#define		UID_ITEM_ROOT			1		/* hodnota UID root adresáøe */

/* ____________________________________________________________________________

    Hlavièky funkcí
   ____________________________________________________________________________
*/
int file_exists(char *path_name);
int format_file(char *path_name, int size);
int load_file(char *path_name);

#endif