/*

    PseudoNTFS file system
    Verze: 1.0

    Modul: global_vars.c
    Tento modul reprezentuje p�epravku pro glob�ln� prom�nn�
	cel� aplikace.
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "fs_structures.h"
#include "global_vars.h"

/* ____________________________________________________________________________
 
    Glob�ln� prom�nn� modulu
   ____________________________________________________________________________
*/
boot_record *global_boot_record = NULL;
mft_item **global_mft_item_array = NULL;
int8_t *global_bitmap = NULL;

int32_t pwd = -1;
int run_shell = 1;