/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavièkový soubor: functions_helper.h
    Detailní informace v souboru functions_helper.c
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#ifndef _FUNCTIONS_HELPER_H
#define _FUNCTIONS_HELPER_H

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
mft_item **get_mft_item_by_uid(int32_t uid);
int *load_dir_data(int32_t start_address, char *path_name);
int32_t get_uid_from_path(char *path, char *file_name);
int32_t get_uid_by_name_dir(char *dir_name, int32_t local_pwd, char *file_name);

#endif
