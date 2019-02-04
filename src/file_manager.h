/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavi�kov� soubor: file_manager.h
    Detailn� informace v souboru file_manager.c
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#ifndef _FILE_MANAGER_H
#define _FILE_MANAGER_H

/* ____________________________________________________________________________

    Definovan� symboly preprocesoru
   ____________________________________________________________________________
*/
#define		UID_ITEM_ROOT			1		/* hodnota UID root adres��e */

/* ____________________________________________________________________________

    Hlavi�ky funkc�
   ____________________________________________________________________________
*/
int file_exists(char *path_name);
int format_file(char *path_name, int size);
int load_file(char *path_name);

#endif