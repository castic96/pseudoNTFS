/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavièkový soubor: shell.h
    Detailní informace v souboru shell.c
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#ifndef _SHELL_H
#define _SHELL_H

/* ____________________________________________________________________________

    Definované symboly preprocesoru
   ____________________________________________________________________________
*/
#define BUFF_SIZE	256			/* velikost bufferu pro pøijímání vstupù */

/* ____________________________________________________________________________

    Struktury a datové typy
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________

    Hlavièky funkcí
   ____________________________________________________________________________
*/
void shell_before_format(char *file_name);
void shell_after_format(char *file_name);

#endif
