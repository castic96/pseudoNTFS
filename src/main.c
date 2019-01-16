/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: main.c
    Tento modul sjednocuje v�echny komponenty programu a obsahuje spustiteln�
    bod programu.
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_manager.h"
#include "shell.h"

/* ____________________________________________________________________________

    Definovan� symboly preprocesoru
   ____________________________________________________________________________
*/
#define NUM_OF_ARGS		2		/* po�et nutn�ch argument� p��kazov� ��dky */

/* ____________________________________________________________________________
 
    Glob�ln� prom�nn� modulu
   ____________________________________________________________________________
*/
char *file_name = NULL; 			/* n�zev souboru se strukturou FS */
//command *current_command = NULL;	/* aktu�ln� p��kaz zadan� u�ivatelem */


/* ____________________________________________________________________________

    void showhelp()
    
    Vytiskne na konzoli kr�tkou n�pov�du, jak pou��vat program.
   ____________________________________________________________________________
*/
void showhelp() {
	
	printf("\nNapoveda:\n");
	printf("\nPouziti:\n   pseudoNTFS.exe <souborovy_system>\n\n");
	printf("   <souborovy_system>");
	printf(" - nazev souboroveho systemu,");
	printf(" do ktereho se ukladaji informace o souborech.\n");
	printf("Priklad:\n   pseudoNTFS.exe myFS \n\n");
}


/* ____________________________________________________________________________

    void init(int argc, char *argv[])
    
    Zpracuje parametry p��kazov� ��dky `argv'. Parametr `argc' je po�et 
	parametr� na p��kazov� ��dce.
   ____________________________________________________________________________
*/
void init(int argc, char *argv[]) {
		
	if (argc != NUM_OF_ARGS) {
		printf("Nespravny pocet argumentu.\n");
		showhelp();
		exit(EXIT_FAILURE);
	}
	
	/* alokov�n� pam�ti pro n�zev souboru -
	o jedna v�t�� kv�li ukon�ovac�mu znaku '\000' */ 
	file_name = calloc(1, (strlen(argv[1]) + 1) * sizeof(char));
	strcpy(file_name, argv[1]);
}


/* ____________________________________________________________________________

    void run()
    
    V�konn� funkce programu. Vykon�v� ve�ker� v�po�et programu a d�v�
    dohromady v�echny jeho ��sti.
   ____________________________________________________________________________
*/
void run() {	
	
	if (!file_exists(file_name)) {
		shell_before_format(file_name);
	}
	else {
		if (load_file(file_name) != 1) {
			exit(EXIT_FAILURE);
		}
	}
	    
	
	
	
}


/* ____________________________________________________________________________

    void shutdown() 
    
    Dealokuje pam�, referen�n� prom�nn� nastav� na NULL, p�iprav�
    na ukon�en� programu.
   ____________________________________________________________________________
*/
void shutdown() {
	
	/* uvoln�n� pam�ti po `file_name' */
	if (file_name != NULL) {
		free(file_name);
		file_name = NULL;
	}
	
	/* uzav�en� proud� */
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
}


/* ____________________________________________________________________________
 
    HLAVN� FUNKCE PROGRAMU
   ____________________________________________________________________________
*/
int main(int argc, char *argv[]) {
	
	init(argc, argv);
	run();
	shutdown();
	
	return EXIT_SUCCESS;
}
