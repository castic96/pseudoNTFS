/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavièkový soubor: global_vars.h
    Detailní informace v souboru global_vars.c
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#ifndef _GLOBAL_VARS_H
#define _GLOBAL_VARS_H

/* ____________________________________________________________________________

    Definované symboly preprocesoru
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________

    Struktury a datové typy
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________

    Hlavièky funkcí
   ____________________________________________________________________________
*/
//

/* ____________________________________________________________________________
 
    Globální promìnné modulu
   ____________________________________________________________________________
*/
extern boot_record *global_boot_record;
extern mft_item **global_mft_item_array;
extern int8_t *global_bitmap;

#endif
