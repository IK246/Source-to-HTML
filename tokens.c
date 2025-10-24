#include "main.h"

// getting the external variables from other files
extern parser event;
extern char buffer[100];

// function to get the preprocessor directives
status preprocessor(char *ch, file_info *info)
{
    int i=0;
    do
    {
        buffer[i++] = *ch;
        *ch = fgetc(info->src_fptr);
    }
    while(*ch != ' ' && *ch != '<' && *ch != 34 && *ch != '\n');

    // move the function pointer one step back
    fseek(info->src_fptr, -1, SEEK_CUR);
    buffer[i] = '\0';
    return e_success;
}

// function to get the string literals and header files
status literal(char *ch, file_info *info)
{
    char pre = *ch;
    int i=0;
    do
    {
        buffer[i++] = *ch;
        *ch = fgetc(info->src_fptr);
    }
    while(*ch != 34 && *ch != EOF && *ch != '\n' && !(pre == 60 && *ch == 62));

    // if the literal is a header file, add '>' at the end
    if (pre == 60)
        buffer[i++] = 62;
    // if not add " at the end
    else
        buffer[i++] = 34;
    buffer[i] = '\0';
    *ch = -1;
    return e_success;
}

// function to check for keywords and identifiers
status keyword(char *ch, file_info *info)
{
    int i=0;
    do
    {
        buffer[i++] = *ch;
        *ch = fgetc(info->src_fptr);
    }
    while((isalnum(*ch) || *ch == '_') && *ch != EOF);
    buffer[i] = '\0';

    // move the function pointer one step back
    fseek(info->src_fptr, -1, SEEK_CUR);
    *ch = -1;

    // checking if the buffer matches any keyword or not
    if(check_keyword(buffer) == e_failure)
    {
        // if the buffer does not matches with any keyword, it is identifier
        event = e_others;
    }
    return e_success;
}

// list of keywords in C language
char *data_key[] = {"char", "int", "float", "long", "short", "double", "struct", "union", "typedef", "signed", "unsigned", "void", "auto", "extern", "registor", "static", "const", "volatile", "enum"};

char *control_key[] = {"if", "else", "switch", "case", "default", "for", "while", "do", "goto", "break", "continue", "return", "sizeof"};

// function to check if the buffer matches any keyword or not
status check_keyword(char *buffer) // Checking for keywords
{
    for(int i=0; i<18; i++) // loop to check if the buffer matches any keyword or not
        if(!strcmp(buffer, data_key[i])) // Comparing the buffer with each keyword
        {
            event = e_data_key;
            return e_success;
        }

    for(int i=0; i<12; i++) // loop to check if the buffer matches any keyword or not
        if(!strcmp(buffer, control_key[i])) // Comparing the buffer with each keyword
        {
            event = e_control_key;
            return e_success;
        }
    return e_failure;
}

// function to check for numerical constants
status num_const(char *ch, file_info *info)
{
    int i=0;
    do
    {
        buffer[i++] = *ch;
        *ch = fgetc(info->src_fptr);
    }
    while(isalnum(*ch) || *ch == '.');

    // move the function pointer one step back
    fseek(info->src_fptr, -1, SEEK_CUR);
    *ch = -1;
    buffer[i] = '\0';
    return e_success;
}

// function to check for character constants
status char_const(char *ch, file_info *info)
{
    int i=0;
    do
    {
        buffer[i++] = *ch;
        *ch = fgetc(info->src_fptr);
    }
    while(*ch != 39 && *ch != '\n');

    // adding the ' at the end
    buffer[i++] = 39;
    buffer[i] = '\0';
    return e_success;
}

// list of multi-character operators in C language
char *multi_operator[] = {"->", "++", "--", "<<", ">>", "<=", ">=", "==", "!=", "&&", "||", "*=", "/=", "%=", "+=", "-=", "&=", "|=", "^=", "<<=", ">>=", "**"};

// function to check for operators
status check_operator(char *ch, file_info *info)
{
    int i=0;
    do
    {
        buffer[i++] = *ch;
        *ch = fgetc(info->src_fptr);
    }
    while(single_operator(*ch) == e_success);

    // move the function pointer one step back
    fseek(info->src_fptr, -1, SEEK_CUR);
    *ch = -1;
    buffer[i] = '\0';

    if(i != 1)
    {
        int j;
        // loop to check if the buffer matches any multi-character operator or not
        for(j=0; j<22; j++)
        {
            if(!strcmp(buffer, multi_operator[j]))
                return e_success;
        }
        return e_failure;
    }
    return e_success;
}

// function to check for single character operators
status single_operator(char ch) // function to check for single character operators
{
    // checks if the character matches with any of the operators below
    if(ch == '-' || ch == '.' || ch == '>' || ch == '+' || ch == '&' || ch == '|' || ch == '~' || ch == '*' || ch == '%' || ch == '/' || ch == '<' || ch == '=' || ch == '?' || ch == '!' || ch == ',')
        return e_success;
    else
        return e_failure;
}

// Checking for special characters
status check_special(char ch)
{
    if(ch == ':' || ch == ';' || ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']')
    {
        buffer[0] = ch;
        buffer[1] = '\0';
        return e_success;
    }
    else
        return e_failure;
}
