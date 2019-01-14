/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavi�kov� soubor: process_command.h
    Detailn� informace v souboru process_command.c
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#ifndef _PROCESS_COMMAND_H
#define _PROCESS_COMMAND_H

#include "command.h"

/* ____________________________________________________________________________

    Definovan� symboly preprocesoru
   ____________________________________________________________________________
*/
//zde 

/* ____________________________________________________________________________

    Struktury a datov� typy
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________

    Hlavi�ky funkc�
   ____________________________________________________________________________
*/
int process_requirement_before(command *current_command, char *file_name);
int process_command_format(command *current_command, char *file_name);

#endif
