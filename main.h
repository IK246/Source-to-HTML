#ifndef MAIN_H
#define MAIN_H

// header files
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// color codes
#define R "\x1b[31m"
#define G "\x1b[32m"
#define Y "\x1b[33m"
#define C "\x1b[36m"
#define Re "\x1b[0m"

// enum for return status of the functions
typedef enum status
{

    e_failure, // --> 0
    e_success, // --> 1
    e_data_key, // --> 2
    e_control_key // --> 3

} status;

// structure to store the file info
typedef struct file_info
{
    // stores source file info
    char *src_fname;
    FILE *src_fptr;

    // stores destination file info
    char dest_fname[100];
    FILE *dest_fptr;

} file_info;

// function for source2html.c
status open_files(file_info *);
status html_begin(file_info *);
status html_end(file_info *);
status s2html(file_info *);
void close_files(file_info *);

// function for token.c
status do_scan(file_info *);
status check_special(char);
status check_keyword(char *);
status literal(char *, file_info *);
status keyword(char *, file_info *);
status preprocessor(char *, file_info *);

// function for operator.c
status check_operator(char *, file_info *);
status single_operator_check(char);

// function for constant.c
status num_const(char *, file_info *);
status char_const(char *, file_info *);

#endif
