/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavièkový soubor: command.h
    Detailní informace v souboru command.c
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#ifndef _COMMAND_H
#define _COMMAND_H

/* ____________________________________________________________________________

    Struktury a datové typy
   ____________________________________________________________________________
*/
typedef struct Command {
	char *function;
	char *first_param;
	char *second_param;
} command;

/* ____________________________________________________________________________

    Hlavièky funkcí
   ____________________________________________________________________________
*/
command *new_command(char *buffer);
void remove_spaces(char *buffer);

#endif
