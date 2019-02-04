/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: global_vars.c
    Tento modul reprezentuje pøepravku pro globální promìnné
	celé aplikace.
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "fs_structures.h"
#include "global_vars.h"

/* ____________________________________________________________________________
 
    Globální promìnné modulu
   ____________________________________________________________________________
*/
boot_record *global_boot_record = NULL;
mft_item **global_mft_item_array = NULL;
int8_t *global_bitmap = NULL;

int32_t pwd = -1;
int run_shell = 1;