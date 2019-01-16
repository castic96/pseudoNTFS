/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavi�kov� soubor: global_vars.h
    Detailn� informace v souboru global_vars.c
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#ifndef _GLOBAL_VARS_H
#define _GLOBAL_VARS_H

/* ____________________________________________________________________________

    Definovan� symboly preprocesoru
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________

    Struktury a datov� typy
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________

    Hlavi�ky funkc�
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________
 
    Glob�ln� prom�nn� modulu
   ____________________________________________________________________________
*/
extern boot_record *global_boot_record;
extern mft_item **global_mft_item_array;
extern int8_t *global_bitmap;

#endif
