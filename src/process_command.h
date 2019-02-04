/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavièkový soubor: process_command.h
    Detailní informace v souboru process_command.c
    
    Dialekt: C11
    Pøekladaè: každý kompatibilní s normou C11

    Copyright (c) Zdenìk Èastorál, 2019
    Semestrální práce z pøedmìtu KIV/ZOS

*/

#ifndef _PROCESS_COMMAND_H
#define _PROCESS_COMMAND_H

/* ____________________________________________________________________________

    Definované symboly preprocesoru
   ____________________________________________________________________________
*/
#define	PATH_LEN	512		/* maximální délka cesty ve znacích */
#define	NAME_LEN	12		/* délka jména ve znacích */

/* ____________________________________________________________________________

    Hlavièky funkcí
   ____________________________________________________________________________
*/
int process_requirement_before(command *current_command, char *file_name);
int process_requirement_after(command *current_command, char *file_name);
int process_command_format(command *current_command, char *file_name);
int process_command_ls(command *current_command, char *file_name);
int process_command_pwd(command *current_command, char *file_name);
int process_command_cd(command *current_command, char *file_name);
int process_command_mkdir(command *current_command, char *file_name);
int process_command_incp(command *current_command, char *file_name);
int process_command_outcp(command *current_command, char *file_name);
int process_command_cat(command *current_command, char *file_name);
int process_command_cp(command *current_command, char *file_name);
int process_command_mv(command *current_command, char *file_name);
int process_command_rm(command *current_command, char *file_name);
int process_command_rmdir(command *current_command, char *file_name);
int process_command_info(command *current_command, char *file_name);
int process_command_load(command *current_command, char *file_name);
int process_command_slink(command *current_command, char *file_name);
int process_command_exit(command *current_command, char *file_name);

#endif