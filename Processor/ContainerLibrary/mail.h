#ifndef MAIL_H_INCLUDED
#define MAIL_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int send_email (const char header_file[], const char message_file[], const char attach_file[], const char attach_name[])//const char whom[], const char attachment[])
{
    Buffer header_text = {};
    buffer_construct(&header_text, header_file);
    open_file (temp_message, "./Mail/temp_message", "w", "Can't open file!");
    char to_whom[NAME_MAX];
    sscanf (header_text.chars, "To:%s", to_whom);
    fprintf (temp_message, "%sMime-Version: 1.0\nContent-Type: multipart/mixed; boundary=\"newpart\"\n--newpart\n", header_text.chars);
    fprintf (temp_message, "Content-Type: text/plain; charset=utf-8\n\n");
    Buffer message_text = {};
    buffer_construct(&message_text, message_file);
    if (attach_file)
    {
        fprintf (temp_message, "%s\n\n--newpart\n", message_text.chars);
        fprintf (temp_message, "Content-Type: application/octet-stream\n");
        fprintf (temp_message, "Content-Transfer-Encoding: base64\n");
        fprintf (temp_message, "Content-Disposition: attachment; filename=\"%s\"\n", attach_name);
    }
    close_file(temp_message);
    char command[100] = {};
    strcat (command, "base64 ");
    strcat (command, attach_file);
    strcat (command, " >> ./Mail/temp_message");
    system (command);
    command[0] = '\0';
    strcat (command, "ssmtp ");
    strcat (command, to_whom);
    strcat (command, " < ./Mail/temp_message");
    system (command);
    system ("rm ./Mail/temp_message");

    return 0;
}



#endif // MAIL_H_INCLUDED
