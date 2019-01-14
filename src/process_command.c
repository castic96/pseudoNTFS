/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: process_command.c
    Tento modul slouží ke zpracování jednotlivých pøíkazù 
    a volání pøíslušných funkcí pro vykonání požadované
    funkcionality zadaného pøíkazu.
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "process_command.h"
#include "command.h"

/* ____________________________________________________________________________

	int process_requirement_before(command *current_command, char *file_name)
    
    Zpracování pøíkazu pøed prvním naformátováním souboru. Vrací 1, pokud
    je pøíkaz validní, v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_requirement_before(command *current_command, char *file_name) {
	
	if (current_command->function == NULL) {
		printf("Neznamy prikaz!\n");
		return 0;
	}
	
	if (strcmp("format", current_command->function) == 0) {
		if (process_command_format(current_command, file_name) != 1) {
			printf("Chyba pri vykonavani funkce process_command_format()\n");
			return 0;
		}
	} else {
		printf("Neznamy prikaz!\n");
		return 0;
	}
	
	return 1;
}


/* ____________________________________________________________________________

	int process_command_format(command *current_command, char *file_name)
    
    Zpracování pøíkazu format. Vrací 1, pokud vše probìhlo v poøádku, 
	v opaèném pøípadì vrací 0.
   ____________________________________________________________________________
*/
int process_command_format(command *current_command, char *file_name) {
	
	if ((current_command->first_param != NULL) && (current_command->second_param == NULL)) {
		int param_length = strlen(current_command->first_param);
		char *size_str = NULL;
		int size_int = 0;
		
		size_str = calloc(1, (strlen(current_command->first_param) + 1) * sizeof(char));
		strcpy(size_str, current_command->first_param);
		
		if ((size_str[param_length - 1] == 'B')  
			&& (size_str[param_length - 2] == 'M')) {
			size_str[strlen(size_str) - 1] = '\0';
			size_str[strlen(size_str) - 1] = '\0';
		
			size_int = atoi(size_str);
		
			format_file(file_name, size_int);
		
			if (size_str != NULL) {
				free(size_str);
				size_str = NULL;
			}
		
			return 1;
		}
		else {
			return 0;
		}	
	}
	else {
		return 0;	
	}
}
