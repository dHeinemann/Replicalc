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

#if defined(__DOS__) || defined(__NT__)
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include <float.h>
#include <stdlib.h>
#include <string.h>

#include "calc.h"
#include "chartype.h"
#include "history.h"
#include "limits.h"
#include "strfun.h"

#define MAX_DBL_CHARS (3 + DBL_MANT_DIG - DBL_MIN_EXP) /* Maximum chars needed to represent a double */

/*
 * Display an input prompt.
 */
void prompt()
{
    printw("> ");
}

/*
 * Restore text from a buffer to the console.
 *
 * @param historyBuffer Buffer to restore from.
 * @param startIndex Console index to begin writing from.
 * @param length Length of current input on the console.
 */
void restoreFromBuffer(char historyBuffer[], int startIndex, int* length)
{
    int difference;
    int i;

    difference = *length > (int) strlen(historyBuffer)
        ? *length - strlen(historyBuffer)
        : 0;

    move(getcury(stdscr), startIndex);
    printw(historyBuffer);
    *length = (int) strlen(historyBuffer);

    if (difference > 0)
        for (i = 0; i < difference; i++)
            printw(" ");

    move(getcury(stdscr), startIndex + strlen(historyBuffer));
}

static struct History* history;

/**
 * Handle ncurses keyboard input.
 *
 * @param ch Button pressed.
 * @param startIndex Column at which text input begins.
 * @param length Maximum length of text input.
 * @param isExtended Indicates whether button pressed was an Extended button.
 */
int handleInput(int ch, int startIndex, int* length)
{
    double result;
    int errorCode;
    char expr[EXPR_LEN];
    char resultStr[MAX_DBL_CHARS];
    char historyBuffer[EXPR_LEN];

    if (ch == KEY_LEFT)
    {
        if (getcurx(stdscr) > startIndex)
        {
            move(getcury(stdscr), getcurx(stdscr) - 1);
        }
    }
    else if (ch == KEY_RIGHT)
    {
        if (getcurx(stdscr) <= *length + 1)
        {
            move(getcury(stdscr), getcurx(stdscr) + 1);
        }
    }
    else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b')
    {
        if (getcurx(stdscr) > startIndex)
        {
            mvdelch(getcury(stdscr), getcurx(stdscr) - 1);
            (*length)--;
        }
    }
    else if (ch == KEY_UP)
    {
        if (previousExpression(history, historyBuffer))
        {
            restoreFromBuffer(historyBuffer, startIndex, length);
        }
    }
    else if (ch == KEY_DOWN)
    {
        if (nextExpression(history, historyBuffer))
        {
            restoreFromBuffer(historyBuffer, startIndex, length);
        }
    }
    else if (ch == KEY_DC)
    {
        if (getcurx(stdscr) <= *length + 1)
        {
            delch();
            (*length)--;
        }
    }
    else if (ch == KEY_ENTER || ch == '\n')
    {
        /* Read user's input from startIndex, then move back to the end of the input. */
        mvinnstr(getcury(stdscr), startIndex, expr, EXPR_LEN);
        move(getcury(stdscr), startIndex + *length);

        printw("\n");
        if (isQuit(expr))
        {
            return 0;
        }

        errorCode = Error_Success;
        result = calculate(trim(expr), &errorCode);
        switch (errorCode)
        {
            case Error_Success:
                format(result, resultStr);
                printw("%s", resultStr);
                break;
            case Error_UnbalParens:
                printw("Error: Unbalanced parentheses");
                break;
            case Error_DivideByZero:
                printw("Error: Division by zero");
                break;
        }

        addToHistory(history, expr);

        *length = 0;
        printw("\n");
        prompt();
    }
    else if (isNumeric(ch) || isLetter(ch) || isSymbol(ch) || ch == ' ')
    {
        if (getcurx(stdscr) <= *length + 1)
        {
            insch(ch);
            move(getcury(stdscr), getcurx(stdscr) + 1);
        }
        else
        {
            addch(ch);
        }
        (*length)++;
    }

    return 1;
}

/*
 * Show copyright disclaimer.
 */
void showCopyright(char** copytext, int length)
{
    int i;

    for (i = 0; i < length; i++)
    {
        printw("%s\n", copytext[i]);
    }

    printw("\n");
}

void startUserInterface(char** copytext, int copylen)
{
    int startIndex;  /* Index to begin reading input from (i.e. after the prompt) */
    int length = 0;  /* Actual length of input. */
    int running = 1; /* Indicates whether to continue processing user input, or exit. */

    history = createHistory(history);

    /* Init ncurses */
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, 1);
    keypad(stdscr, 1);

    /* Print initial screen */
    showCopyright(copytext, copylen);
    prompt();
    startIndex = getcurx(stdscr);

    /* Main loop */
    while (running)
    {
        running = handleInput(getch(), startIndex, &length);
    }

    endwin();

    destroyHistory(history);
}
