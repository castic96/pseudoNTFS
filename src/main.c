/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: main.c
    Tento modul sjednocuje všechny komponenty programu a obsahuje spustitelnı
    bod programu.
    
    Dialekt: C11
    Pøekladaè: kadı kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_manager.h"
#include "shell.h"

/* ____________________________________________________________________________

    Definované symboly preprocesoru
   ____________________________________________________________________________
*/
#define NUM_OF_ARGS		2		/* poèet nutnıch argumentù pøíkazové øádky */

/* ____________________________________________________________________________
 
    Globální promìnné modulu
   ____________________________________________________________________________
*/
char *file_name = NULL; 			/* název souboru se strukturou FS */
//command *current_command = NULL;	/* aktuální pøíkaz zadanı uivatelem */


/* ____________________________________________________________________________

    void showhelp()
    
    Vytiskne na konzoli krátkou nápovìdu, jak pouívat program.
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
    
    Zpracuje parametry pøíkazové øádky `argv'. Parametr `argc' je poèet 
	parametrù na pøíkazové øádce.
   ____________________________________________________________________________
*/
void init(int argc, char *argv[]) {
		
	if (argc != NUM_OF_ARGS) {
		printf("Nespravny pocet argumentu.\n");
		showhelp();
		exit(EXIT_FAILURE);
	}
	
	/* alokování pamìti pro název souboru -
	o jedna vìtší kvùli ukonèovacímu znaku '\000' */ 
	file_name = calloc(1, (strlen(argv[1]) + 1) * sizeof(char));
	strcpy(file_name, argv[1]);
}


/* ____________________________________________________________________________

    void run()
    
    Vıkonná funkce programu. Vykonává veškerı vıpoèet programu a dává
    dohromady všechny jeho èásti.
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
    
    Dealokuje pamì, referenèní promìnné nastaví na NULL, pøipraví
    na ukonèení programu.
   ____________________________________________________________________________
*/
void shutdown() {
	
	/* uvolnìní pamìti po `file_name' */
	if (file_name != NULL) {
		free(file_name);
		file_name = NULL;
	}
	
	/* uzavøení proudù */
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
}


/* ____________________________________________________________________________
 
    HLAVNÍ FUNKCE PROGRAMU
   ____________________________________________________________________________
*/
int main(int argc, char *argv[]) {
	
	init(argc, argv);
	run();
	shutdown();
	
	return EXIT_SUCCESS;
}
