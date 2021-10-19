/*
 * Replicalc, a REPL-based calculator for programmers
 * Copyright (C) 2021 David Heinemann
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

char* trim(char* str)
{
    int i;

    /* Trim start */
    for (i = 0; (size_t) i < strlen(str); i++)
    {
        if (!isspace(str[i])) {
            break;
        }
        str++;
    }

    /* Trim end */
    for (i = (int) strlen(str) - 1; i >= 0; i--)
    {
        if (!isspace(str[i])) {
            break;
        }
        str[i] = '\0';
    }

    return str;
}

void format(double value, char* output)
{
    int i;

    sprintf(output, "%f", value);
    for (i = (int) strlen(output) - 1; i >= 0; i--)
    {
        if (output[i] == '0') {
            output[i] = '\0';
        }
        else
        {
            if (output[i] == '.')
            {
                output[i] = '\0';
            }
            break;
        }
    }
}

int isQuit(char* expr)
{
    if (strcmp(trim(expr), "exit") == 0) { return 1; }
    if (strcmp(trim(expr), "quit") == 0) { return 1; }
    if (strcmp(trim(expr), "q")    == 0) { return 1; }
    return 0;
}

/**
 * Safely copy the first num characters of source to destination, up to the first NULL character. If source is longer
 * than num, the final character of destination will be set to NULL (guaranteeing a NULL-terminated C string).
 * 
 * @param destination Destination buffer.
 * @param source Source buffer.
 * @param num Maximum number of characters to copy.
 */
void copyString(char destination[], char source[], int num)
{
    int i;

    for (i = 0; i < num; i++)
    {
        if (i < num - 1)
            destination[i] = source[i];
        else
            destination[i] = '\0'; /* guarantee string is NULL-terminated */

        if (source[i] == '\0')
            break; /* stop after NULL */
    }
}
