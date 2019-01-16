/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: command.c
    Tento modul slou�� pro rozd�len� vstupu na funkci
    a jej� parametry. Slou�� jako p�epravka t�chto hodnot.
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "command.h"

/* ____________________________________________________________________________

	command *new_command(char *buffer)
    
    Na�te data z parametru `buffer' a rozd�l� je podle mezery na funkci
	a jej� parametry. Vrac� strukturu `command', kter� slou�� jako p�epravka
	v��e zm�n�n�ch hodnot.
   ____________________________________________________________________________
*/
command *new_command(char *buffer) {
	
	command *new_comm = (command *) calloc(1, sizeof(command));
	
	new_comm->function = strtok(buffer, " ");
	new_comm->first_param = strtok(NULL, " ");
	new_comm->second_param = strtok(NULL, " ");
	
	remove_spaces(new_comm->function);
	remove_spaces(new_comm->first_param);
	remove_spaces(new_comm->second_param);

	return new_comm;
}


/* ____________________________________________________________________________

	void remove_spaces(char *buffer)
    
    Na�te data z parametru `buffer' a odstran� b�l� znaky.
   ____________________________________________________________________________
*/
void remove_spaces(char *buffer) {
	
	if (buffer == NULL) {
		return;
	}
	
	char *i = buffer;
	char *j = buffer;
	
	while (*j != '\0') {
		
		*i = *j++;
		
		if (!isspace(*i)) {
			i++;
		}
	}
	*i = '\0';
}
