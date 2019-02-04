/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavi�kov� soubor: command.h
    Detailn� informace v souboru command.c
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#ifndef _COMMAND_H
#define _COMMAND_H

/* ____________________________________________________________________________

    Struktury a datov� typy
   ____________________________________________________________________________
*/
typedef struct Command {
	char *function;
	char *first_param;
	char *second_param;
} command;

/* ____________________________________________________________________________

    Hlavi�ky funkc�
   ____________________________________________________________________________
*/
command *new_command(char *buffer);
void remove_spaces(char *buffer);

#endif
