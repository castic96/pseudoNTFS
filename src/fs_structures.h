/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavi�kov� soubor: fs_structures.h
    Detailn� informace v souboru fs_structures.c
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#ifndef _FS_STRUCTURES_H
#define _FS_STRUCTURES_H

/* ____________________________________________________________________________

    Definovan� symboly preprocesoru
   ____________________________________________________________________________
*/
#define UID_ITEM_FREE			0		/* hodnota UID voln�ho mft z�znamu */
#define BITMAP_FREE				0		/* hodnota v bitmap� - volno */
#define MFT_FRAGMENTS_COUNT		32		/* po�et fragment� v jednom MFT */			
#define MB_TO_B_CONST			1048576	/* konstanta pro p�evod z MB na B */
#define CLUSTER_SIZE			10		/* velikost clusteru */

/* ____________________________________________________________________________

    Struktury a datov� typy
   ____________________________________________________________________________
*/
typedef struct Boot_record {
    char signature[9];              	/* login autora FS */
    char volume_descriptor[251];    	/* popis vygenerovan�ho FS */
    int32_t disk_size;              	/* celkov� velikost FS */
    int32_t cluster_size;           	/* velikost clusteru */
    int32_t cluster_count;          	/* po�et cluster� */
    int32_t mft_start_address;      	/* adresa po��tku MFT */
    int32_t bitmap_start_address;   	/* adresa po��tku bitmapy */
    int32_t data_start_address;     	/* adresa po��tku datov�ch blok� */
    int32_t mft_max_fragment_count;		/* max. po�et fragment� v jednom MFT */
} boot_record;

typedef struct Mft_fragment {
    int32_t fragment_start_address;     /* start adresa fragmentu */
    int32_t fragment_count;             /* po�et cluster� ve fragmentu */
} mft_fragment;

typedef struct Mft_item {
    int32_t uid;				/* UID polo�ky */
    int8_t type;				/* soubor-0, adres��-1, nebo symbol. link-2 */
    int8_t item_order;			/* po�ad� v MFT p�i v�ce souborech, jinak 1 */
    int8_t item_order_total;	/* celkov� po�et polo�ek v MFT */
    char item_name[12];			/* 8+3 + /0 ukon�ovac� znak */
    int32_t item_size;			/* velikost souboru v bytech */
    mft_fragment fragments[MFT_FRAGMENTS_COUNT]; /* fragmenty souboru */
} mft_item;

/* ____________________________________________________________________________

    Hlavi�ky funkc�
   ____________________________________________________________________________
*/
boot_record *init_boot_record(int size);
mft_item **init_mft_item_array(int mft_item_count, int32_t data_start_address);
mft_item *init_mft_item_root(int32_t data_start_address);
mft_item *init_mft_item_free();
void delete_mft_item_array(mft_item **curr_mft_item_arr, int mft_item_count);
int8_t *init_bitmap(int cluster_count);

#endif
