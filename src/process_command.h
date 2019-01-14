/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavièkový soubor: process_command.h
    Detailní informace v souboru process_command.c
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#ifndef _PROCESS_COMMAND_H
#define _PROCESS_COMMAND_H

#include "command.h"

/* ____________________________________________________________________________

    Definované symboly preprocesoru
   ____________________________________________________________________________
*/
//zde 

/* ____________________________________________________________________________

    Struktury a datové typy
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________

    Hlavièky funkcí
   ____________________________________________________________________________
*/
int process_requirement_before(command *current_command, char *file_name);
int process_command_format(command *current_command, char *file_name);

#endif
