#include "main.h"

parser event;
uint8_t newline[] = {'<', 'b', 'r', '>', '\n', '\0'};
uint8_t space[] = {'&', 'n', 'b', 's', 'p', ';', '\0'}, buffer[100];
uint32_t flag = 0; 

status open_files(file_info *info)
{
    // opening the source file in read mode
    if((info->src_fptr = fopen(info->src_fname, "r")) == NULL)
    {
        puts(R "Info: " Re "Error while opening the source file!" );
        return e_failure;
    }

    // opening the destination file in write mode
    if((info->dest_fptr = fopen(info->dest_fname, "w")) == NULL)
    {
        puts(R "Info: " Re "Error while opening the destination file!" );
        return e_failure;
    }
    return e_success;
}

status html_begin(file_info *info)
{
    // writing the initial html tags to the destination file
    fprintf(info->dest_fptr, "<!DOCTYPE html>\n");
    fprintf(info->dest_fptr, "<html lang=%cen-US%c>\n", 34, 34);
    fprintf(info->dest_fptr, "<head>\n");
    fprintf(info->dest_fptr, "<title>%s</title>\n", "code2html");
    fprintf(info->dest_fptr, "<meta charset=%cUTF-8%c>\n", 34, 34);
    fprintf(info->dest_fptr, "<link rel=%cstylesheet%c href=%cstyle.css%c>\n", 34, 34, 34, 34);
    fprintf(info->dest_fptr, "<body style=%cbackground-color: black%c>\n", 34, 34);
    fprintf(info->dest_fptr, "<div>\n");
    return e_success;
}

void loading()
{
    char str[20] = "loading";
    int k = strlen(str);

    for(int i=0; i<4; i++)
    {
        printf(Y "\r%s" Re, str);
        fflush(stdout);
        str[k++] = '.';
        str[k] = '\0';
        usleep(500000);
    }
}

status html_end(file_info *info)
{
    // writing the closing html tags to the destination file
    fprintf(info->dest_fptr, "</div>\n");
    fprintf(info->dest_fptr, "</body>\n");
    fprintf(info->dest_fptr, "</head>\n");
    return e_success;
}

status file_close(file_info *info)
{
    // closing the opened files
    fclose(info->src_fptr);
    fclose(info->dest_fptr);

    return e_success;
}

status s2html(file_info *info)
{
    // open all files
    if(open_files(info) == e_failure)
        return e_failure;
    usleep(300000);
    puts(G "Info: " Re "Files Opened Successfully!");

    // write the header of the html file
    if(html_begin(info) == e_failure)
        return e_failure;
    usleep(300000);
    puts(G "Info: " Re "Header of html File Added!");

    // setting the initial event to idle
    event = e_idle;
    char ch;

    // reading the source file character by character
    while((ch = fgetc(info->src_fptr)) != EOF)
    {
        if(check_special(ch) == e_success)
            event = e_others;

        if(ch == '\n')
        {
            fprintf(info->dest_fptr, "%s", newline);
            continue;
        }
        else if(ch == ' ')
        {
            fprintf(info->dest_fptr, "%s", space);
            continue;
        }
        else if(ch == '/')
        {
            int i=0;
            buffer[i++] = ch;
            ch = fgetc(info->src_fptr);
            if(ch == '/')
            {
                do
                {
                    buffer[i++] = ch;
                    ch = fgetc(info->src_fptr);
                }
                while(ch != '\n' && ch != EOF);
                fseek(info->src_fptr, -1, SEEK_CUR);
                buffer[i] = '\0';
                event = e_comment;
                check_event(info);
                continue;
            }
            else if(ch == '*')
            {
                char pre;
                do
                {
                    buffer[i++] = ch;
                    pre = ch;
                    ch = fgetc(info->src_fptr);
                }
                while(!(ch == '/' && pre == '*') && ch != EOF);
                buffer[i++] = '/';
                buffer[i] = '\0';
                event = e_comment;
                check_event(info);
                continue;
            }
            else
            {
                fseek(info->src_fptr, -1, SEEK_CUR);
                ch = '/';
            }
        }
        if(ch == 34 || (ch == 60 && flag == 1))
        {
            flag = 0;
            if(literal(&ch, info) == e_success)
                event = e_string;
        }
        else if(single_operator(ch) == e_success)
        {
            if(check_operator(&ch, info) == e_success)
                event = e_others;
        }
        else if(isdigit(ch))
        {
            if(num_const(&ch, info) == e_success)
                event = e_num_const;
        }
        else if(ch == 39)
        {
            if(char_const(&ch, info) == e_success)
                event = e_ascii_const;
        }
        else if(isalpha(ch) || ch == '_')
        {
            keyword(&ch, info);
        }
        else if(ch == '#')
        {
            if(preprocessor(&ch, info) == e_success)
            {
                event = e_comment;
                flag = 1;
            }
        }
        check_event(info);
    }
    loading();
    puts(G "\rInfo: " Re "All tokens are added to the html file!!");

    // function to add the closing tags of html file
    if(html_end(info) == e_failure)
        return e_failure;
    usleep(300000);
    puts(G "Info: " Re "Closing tags of the html files are added!!");

    // function to close all the opened files
    if(file_close(info) == e_failure)
        return e_failure;
    usleep(300000);
    puts(G "Info: " Re "All the files closed!!");
    return e_success;
}

// function to write the tokens to the destination file
status check_event(file_info *info)
{
    switch(event)
    {
        case e_comment:
            fprintf(info->dest_fptr, "<span class=%ccomment%c>%s</span>", 34, 34, buffer);
            break;
        case e_data_key:
            fprintf(info->dest_fptr, "<span class=%creversed_key1%c>%s</span>", 34, 34, buffer);
            break;
        case e_control_key:
            fprintf(info->dest_fptr, "<span class=%creversed_key2%c>%s</span>", 34, 34, buffer);
            break;
        case e_string:
            {
                int i=0;
                while(buffer[i])
                {
                    if(((buffer[i] == '%' || buffer[i] == 92) && (isalnum(buffer[i+1]) || buffer[i+1] == 92 || buffer[i+1] == 34)) || ((buffer[i-1] == '%' || buffer[i-1] == 92) && (buffer[i] == 34 || buffer[i] == 39 || isalnum(buffer[i]))))
                        fprintf(info->dest_fptr, "<span class=%cascii_char%c>%c</span>", 34, 34, buffer[i]);
                    else
                        fprintf(info->dest_fptr, "<span class=%cstring%c>%c</span>", 34, 34, buffer[i]);
                    i++;
                }
            }
            break;
        case e_num_const:
            fprintf(info->dest_fptr, "<span class=%cnumberic_constant%c>%s</span>", 34, 34, buffer);
            break;
        case e_ascii_const:
            fprintf(info->dest_fptr, "<span class=%cascii_char%c>%s</span>", 34, 34, buffer);
            break;
        case e_others:
            fprintf(info->dest_fptr, "<span class=%cothers%c>%s</span>", 34, 34, buffer);
            break;
    }
    event = e_idle;
    return e_success;
}
