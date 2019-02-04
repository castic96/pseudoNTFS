/*

    PseudoNTFS file system
    Verze: 1.0

    Hlavi�kov� soubor: process_command.h
    Detailn� informace v souboru process_command.c
    
    Dialekt: C11
    P�eklada�: ka�d� kompatibiln� s normou C11

    Copyright (c) Zden�k �astor�l, 2019
    Semestr�ln� pr�ce z p�edm�tu KIV/ZOS

*/

#ifndef _PROCESS_COMMAND_H
#define _PROCESS_COMMAND_H

/* ____________________________________________________________________________

    Definovan� symboly preprocesoru
   ____________________________________________________________________________
*/
#define	PATH_LEN	512		/* maxim�ln� d�lka cesty ve znac�ch */
#define	NAME_LEN	12		/* d�lka jm�na ve znac�ch */

/* ____________________________________________________________________________

    Hlavi�ky funkc�
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