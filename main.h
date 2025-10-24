#ifndef MAIN_H
#define MAIN_H

// header files
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <unistd.h>

// color codes
#define R "\x1b[31m"
#define G "\x1b[32m"
#define Y "\x1b[33m"
#define C "\x1b[36m"
#define Re "\x1b[0m"

// enum for return status of the functions
typedef enum status
{

    e_failure,
    e_success

} status;

// enum for parser events
typedef enum parser
{

    e_comment,
    e_data_key,
    e_control_key,
    e_string,
    e_num_const,
    e_ascii_const,
    e_others,
    e_idle,

} parser;

// structure to store the file info
typedef struct file_info
{

    char *src_fname;
    FILE *src_fptr;

    char dest_fname[30];
    FILE *dest_fptr;

} file_info;

// function for file operations
status open_files(file_info *);
status html_begin(file_info *);
status html_end(file_info *);
status s2html(file_info *);
status close_file(file_info *);
status check_event(file_info *);

// functions to get the tokens
status preprocessor(char *, file_info *);
status literal(char *, file_info *);
status keyword(char *, file_info *);
status check_keyword(char *);
status num_const(char *, file_info *);
status char_const(char *, file_info *);
status check_operator(char *, file_info *);
status single_operator(char);
status check_special(char);

// function to show loading animation
void loading();
#endif
