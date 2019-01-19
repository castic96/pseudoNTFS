/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavièkový soubor: functions.h
    Detailní informace v souboru functions.c
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

/* ____________________________________________________________________________

    Definované symboly preprocesoru
   ____________________________________________________________________________
*/
#define MFT_FRAGMENTS_COUNT		32		/* poèet fragmentù v jednom MFT */
#define BUFF_SIZE				1024	/* velikost pomocného bufferu */

/* ____________________________________________________________________________

    Struktury a datové typy
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________

    Hlavièky funkcí
   ____________________________________________________________________________
*/
void func_ls(int32_t dir_uid, char *file_name);
void func_pwd(int32_t local_pwd, char *file_name);

#endif
