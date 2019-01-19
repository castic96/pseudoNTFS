/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavi�kov� soubor: functions_helper.h
    Detailn� informace v souboru functions_helper.c
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#ifndef _FUNCTIONS_HELPER_H
#define _FUNCTIONS_HELPER_H

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
mft_item **get_mft_item_by_uid(int32_t uid);
int *load_dir_data(int32_t start_address, char *path_name);
int32_t get_uid_from_path(char *path, char *file_name);
int32_t get_uid_by_name_dir(char *dir_name, int32_t local_pwd, char *file_name);

#endif
