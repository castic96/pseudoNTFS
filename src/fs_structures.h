/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavièkový soubor: fs_structures.h
    Detailní informace v souboru fs_structures.c
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#ifndef _FS_STRUCTURES_H
#define _FS_STRUCTURES_H

/* ____________________________________________________________________________

    Definované symboly preprocesoru
   ____________________________________________________________________________
*/
#define UID_ITEM_FREE			0		/* hodnota UID volného mft záznamu */
#define BITMAP_FREE				0		/* hodnota v bitmapì - volno */
#define MFT_FRAGMENTS_COUNT		32		/* poèet fragmentù v jednom MFT */			
#define MB_TO_B_CONST			1048576	/* konstanta pro pøevod z MB na B */
#define CLUSTER_SIZE			10		/* velikost clusteru */

/* ____________________________________________________________________________

    Struktury a datové typy
   ____________________________________________________________________________
*/
typedef struct Boot_record {
    char signature[9];              	/* login autora FS */
    char volume_descriptor[251];    	/* popis vygenerovaného FS */
    int32_t disk_size;              	/* celková velikost FS */
    int32_t cluster_size;           	/* velikost clusteru */
    int32_t cluster_count;          	/* poèet clusterù */
    int32_t mft_start_address;      	/* adresa poèátku MFT */
    int32_t bitmap_start_address;   	/* adresa poèátku bitmapy */
    int32_t data_start_address;     	/* adresa poèátku datových blokù */
    int32_t mft_max_fragment_count;		/* max. poèet fragmentù v jednom MFT */
} boot_record;

typedef struct Mft_fragment {
    int32_t fragment_start_address;     /* start adresa fragmentu */
    int32_t fragment_count;             /* poèet clusterù ve fragmentu */
} mft_fragment;

typedef struct Mft_item {
    int32_t uid;				/* UID položky */
    int8_t type;				/* soubor-0, adresáø-1, nebo symbol. link-2 */
    int8_t item_order;			/* poøadí v MFT pøi více souborech, jinak 1 */
    int8_t item_order_total;	/* celkový poèet položek v MFT */
    char item_name[12];			/* 8+3 + /0 ukonèovací znak */
    int32_t item_size;			/* velikost souboru v bytech */
    mft_fragment fragments[MFT_FRAGMENTS_COUNT]; /* fragmenty souboru */
} mft_item;

/* ____________________________________________________________________________

    Hlavièky funkcí
   ____________________________________________________________________________
*/
boot_record *init_boot_record(int size);
mft_item **init_mft_item_array(int mft_item_count, int32_t data_start_address);
mft_item *init_mft_item_root(int32_t data_start_address);
mft_item *init_mft_item_free();
void delete_mft_item_array(mft_item **curr_mft_item_arr, int mft_item_count);
int8_t *init_bitmap(int cluster_count);

#endif
