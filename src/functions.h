/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavi�kov� soubor: functions.h
    Detailn� informace v souboru functions.c
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

/* ____________________________________________________________________________

    Definovan� symboly preprocesoru
   ____________________________________________________________________________
*/
#define MFT_FRAGMENTS_COUNT		32		/* po�et fragment� v jednom MFT */
#define BUFF_SIZE				1024	/* velikost pomocn�ho bufferu */

/* ____________________________________________________________________________

    Struktury a datov� typy
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________

    Hlavi�ky funkc�
   ____________________________________________________________________________
*/
void func_ls(int32_t dir_uid, char *file_name);
void func_pwd(int32_t local_pwd, char *file_name);

#endif
