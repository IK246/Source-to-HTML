#include "main.h"

// Global Variables
int line = 0, i=0, round_bracket = 0, square_bracket = 0, curly_bracket = 0;
char arr[200], newline[] = {'<', 'b', 'r', '>', '\n', '\0'}, space[] = {'&', 'n', 'b', 's', 'p', ';', '\0'};

/*
 * Name: Karthick
 * Title: Source to HTML
 * Data: 11/09/25
 */

status do_scan(file_info *info) // Scanning the file for tokens
{
    char ch;
    while((ch = fgetc(info->src_fptr)) != EOF) // Reading character by character until EOF
    {
#if 1
        if(check_special(ch) == e_success) // Checking for special characters
            fprintf(info->dest_fptr, "<span class=%cothers%c>%c</span>", 34, 34, ch);
        if(ch == '\n') // checks for new line
        {
            line++; // Incrementing line count
            fprintf(info->dest_fptr, "%s", newline);
            continue; // Continuing to next iteration to avoid further checks
        }
        if(ch == ' ')
        {
            fprintf(info->dest_fptr, "%s", space);
            continue;
        }

        if(ch == '/') // Checking for comments
        {
            int i=0;
            arr[i++] = ch;
            ch = fgetc(info->src_fptr);
            if(ch == '/') // Checking for single line comment
            {
                do
                {
                    arr[i++] = ch;
                    ch = fgetc(info->src_fptr);
                }
                while(ch != '\n' && ch != EOF);
                arr[i] = '\0';
                fprintf(info->dest_fptr, "<span class=%ccomment%c>%s</span>", 34, 34, arr);
                if(ch == '\n')
                {
                    line++;
                    fprintf(info->dest_fptr, "%s", newline);
                }
                continue;
            }
            else if(ch == '*') // Checking for multi line comment
            {
                char pre;
                do
                {
                    arr[i++] = ch;
                    pre = ch;
                    ch = fgetc(info->src_fptr);
                    if(ch == '\n')
                        line++;
                }
                while(!(ch == '/' && pre == '*') && ch != EOF);
                if(ch == EOF)
                {
                    printf(R "Line %d Error:" Re " Unterminated comment\n", line);
                    fclose(info->src_fptr);
                    return e_failure;
                }
                arr[i++] = '/';
                arr[i] = '\0';
                fprintf(info->dest_fptr, "<span class=%ccomment%c>%s</span>", 34, 34, arr);
                continue;
            }
            else // if not a comment, then it is a division operator
            {
                fseek(info->src_fptr, -1, SEEK_CUR); // Moving the file pointer one step back
                ch = '/'; // Assigning ch to '/' to check for operator
            }
        }

        if(ch == '#') // Checking for preprocessor directives
            preprocessor(&ch, info); // funcion call to ignore preprocessor directives

        if(ch == 34) // Checking for string literals
            if(literal(&ch, info) == e_failure) // function call to check string literal
                return e_failure;
#endif
        if(single_operator_check(ch) == e_success) // Checking for single character operators
            if(check_operator(&ch, info) == e_failure) // function call to check for operators
                return e_failure;

        if(ch >= '0' && ch <= '9') // Checking for numerical constants
            if(num_const(&ch, info) == e_failure) // function call to check numerical constants
                return e_failure;

        if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' || (ch >= '0' && ch <= '9')) // Checking for keywords and identifiers
            if(keyword(&ch, info) == e_failure) // function call to check for keywords and identifiers
                return e_failure;
#if 1
        if(ch == 39) // Checking for character constants
            if(char_const(&ch, info) == e_failure) // function call to check for character constants
                return e_failure;

        if(ch == '$' || ch == '@') // Checking for invalid characters
        {
            printf(R "Line %d Error:" Re " stray %c%c%c in program\n", line, 39, ch, 39); // Printing error message
            return e_failure;
        }
#endif
    }

    // Checking for unbalanced brackets
    if(round_bracket != 0)
    {
        printf(R "Error:" Re " expected %c)%c\n", 39, 39);
        return e_failure;
    }
    if(square_bracket != 0)
    {
        printf(R "Error:" Re " expected %c]%c\n", 39, 39);
        return e_failure;
    }
    if(curly_bracket != 0)
    {
        printf(R "Error:" Re " expected %c}%c\n", 39, 39);
        return e_failure;
    }

    return e_success;
}

char *data_key[] = {"char", "int", "float", "long", "short", "double", "struct", "union", "typedef", "signed", "unsigned", "void", "auto", "extern", "registor", "static", "const", "volatile", "enum"};

char *control_key[] = {"if", "else", "switch", "case", "default", "for", "while", "do", "goto", "break", "continue", "return", "sizeof"};

status check_keyword(char *buffer) // Checking for keywords
{
    for(int i=0; i<18; i++) // loop to check if the buffer matches any keyword or not
        if(!strcmp(buffer, data_key[i])) // Comparing the buffer with each keyword
            return e_data_key;

    for(int i=0; i<12; i++) // loop to check if the buffer matches any keyword or not
        if(!strcmp(buffer, control_key[i])) // Comparing the buffer with each keyword
            return e_control_key;
    return e_failure;
}

status check_special(char ch) // Checking for special characters
{
    // checks for the brackets
    if(ch == '(')
        round_bracket++;
    else if(ch == ')')
        round_bracket--;
    else if(ch == '[')
        square_bracket++;
    else if(ch == ']')
        square_bracket--;
    else if(ch == '{')
        curly_bracket++;
    else if(ch == '}')
        curly_bracket--;
    // checks for other special characters
    else if(ch == ':' || ch == ';')
        return e_success;
    else
        return e_failure;
    return e_success;
}

status preprocessor(char *ch, file_info *info) // function to ignore preprocessor directives
{
    int i=0;
    do
    {
        arr[i++] = *ch;
        *ch = fgetc(info->src_fptr);
    }
    while(*ch != '\n'); // loop till new line character
                        //arr[i++] = *ch;
    arr[i] = '\0';
    fprintf(info->dest_fptr, "<span class=%cheader_file%c>%s</span>", 34, 34, arr);
    line++; // incrementing the line count
    return e_success;
}

status literal(char *ch, file_info *info)
{
    do
    {
        int i=0;
        if(*ch == '%')
        {
            arr[i++] = *ch;
            *ch = fgetc(info->src_fptr);
            if(isalpha(*ch) || *ch == '%')
            {
                arr[i++] = *ch;
                arr[i] = '\0';
                fprintf(info->dest_fptr, "<span class=%cascii_char%c>%s</span>", 34, 34, arr);
            }
            else
            {
                fseek(info->src_fptr, -1, SEEK_CUR);
                *ch = arr[0];
            }
        }
        else if(*ch == 92)
        {
            arr[i++] = *ch;
            *ch = fgetc(info->src_fptr);
            if(isalnum(*ch) || *ch == 39 || *ch == 34)
            {
                arr[i++] = *ch;
                arr[i] = '\0';
                fprintf(info->dest_fptr, "<span class=%cascii_char%c>%s</span>", 34, 34, arr);
            }
            else
            {
                fseek(info->src_fptr, -1, SEEK_CUR);
                *ch = arr[0];
            }
        }
        else    
            fprintf(info->dest_fptr, "<span class=%cstring%c>%c</span>", 34, 34, *ch);
        *ch = fgetc(info->src_fptr);
    }
    while(*ch != 34 && *ch != EOF && *ch != '\n');
    if(*ch == EOF || *ch == '\n')
    {
        if(*ch == '\n')
        {
            line++;
        }
        printf(R "Line %d Error:" Re " Missing terminating %c character\n", line, 34);
        return e_failure;
    }
    fprintf(info->dest_fptr, "<span class=%cstring%c>%c</span>", 34, 34, 34);
    return e_success;
}

status keyword(char *ch, file_info *info) // function to check for keywords and identifiers
{
    int i=0;
    do
    {
        arr[i++] = *ch;
        *ch = fgetc(info->src_fptr);
    }
    while(isalnum(*ch) || *ch == '_'); // loop till alphanumeric or underscore
    arr[i] = '\0'; // assgining null character at the end of the string
    fseek(info->src_fptr, -1, SEEK_CUR); // moving the file pointer one step back

    if(check_keyword(arr) == e_data_key) // checking if the string is a keyword
        fprintf(info->dest_fptr, "<span class=%creversed_key1%c>%s</span>", 34, 34, arr);
    else if(check_keyword(arr) == e_control_key) // checking if the string is a keyword
        fprintf(info->dest_fptr, "<span class=%creversed_key2%c>%s</span>", 34, 34, arr);
    else if(isalpha(arr[0]) || arr[0] == '_') // checking if the string is a valid identifier
        fprintf(info->dest_fptr, "<span class=%cothers%c>%s</span>", 34, 34, arr);
    else
    {
        if(strlen(arr) == 1) // if the length of the string is 1, then it is a number constant
        {
            *ch = arr[0];
            return e_success;
        }
        printf(R "Line %d Error:" Re " invalid identifier %s \n", line, arr);
        return e_failure;
    }
    return e_success;
}
