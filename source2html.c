#include "main.h"

extern int line;

status open_files(file_info *info)
{
    // opening the source file in read mode
    if((info->src_fptr = fopen(info->src_fname, "r")) == NULL)
    {
        puts(R "Info:" Re "Error while opeing the source file!" Re);
        return e_failure;
    }

    // opening the destination file in write mode
    if((info->dest_fptr = fopen(info->dest_fname, "w")) == NULL)
    {
        puts(R "Info:" Re "Error while openig the destination file!" Re);
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

status html_end(file_info *info)
{
    // writing the closing html tags to the destination file
    fprintf(info->dest_fptr, "</div>\n");
    fprintf(info->dest_fptr, "</body>\n");
    fprintf(info->dest_fptr, "</head>\n");
    return e_success;
}
void file_close(file_info *info)
{
    // closing the opened files
    fclose(info->src_fptr);
    fclose(info->dest_fptr);
}

status s2html(file_info *info)
{
    // open all the files
    if(open_files(info) == e_failure)
        return e_failure;
    puts(G "Info:" Re "Files Opened Successfully");

    // write the header of the html file
    if(html_begin(info) == e_failure)
        return e_failure;
    puts(G "Info:" Re "Header of html File Added");

    // extract the tokens from the soruce file and write them to the destination file 
    if(do_scan(info) == e_failure)
        return e_failure;
    printf(G "Info:" Re "%d lines of File Contents Added Successfully\n", line);

    // add the footer of the html file
    if(html_end(info) == e_failure)
        return e_failure;
    puts(G "Info:" Re "Footer of html File Added");

    file_close(info);
    puts(G "Info:" Re "All files closed");

    return e_success;
}
