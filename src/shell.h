/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavi�kov� soubor: shell.h
    Detailn� informace v souboru shell.c
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#ifndef _SHELL_H
#define _SHELL_H

/* ____________________________________________________________________________

    Definovan� symboly preprocesoru
   ____________________________________________________________________________
*/
#define BUFF_SIZE	256			/* velikost bufferu pro p�ij�m�n� vstup� */

/* ____________________________________________________________________________

    Struktury a datov� typy
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________

    Hlavi�ky funkc�
   ____________________________________________________________________________
*/
void shell_before_format(char *file_name);
void shell_after_format(char *file_name);

#endif
