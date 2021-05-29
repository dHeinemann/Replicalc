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
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#include "calc.h"
#include "chartype.h"

#define EXPR_LEN 128 /* Maximum buffer size for expression input */

/*
 * Trim whitespace surrounding a string.
 */
char* trim(char* str) {
    int i;

    /* Trim start */
    for (i = 0; (size_t) i < strlen(str); i++) {
        if (!isspace(str[i])) {
            break;
        }
        str++;
    }

    /* Trim end */
    for (i = (int) strlen(str) - 1; i >= 0; i--) {
        if (!isspace(str[i])) {
            break;
        }
        str[i] = '\0';
    }

    return str;
}

/*
 * Trim trailing zeros and period from decimal numbers.
 */
void format(double value, char* output) {
    int i;

    sprintf(output, "%f", value);
    for (i = (int) strlen(output) - 1; i >= 0; i--) {
        if (output[i] == '0') {
            output[i] = '\0';
        } else {
            if (output[i] == '.') {
                output[i] = '\0';
            }
            break;
        }
    }
}

/*
 * Detect exit attempt.
 */
int isQuit(char* expr) {
    if (strcmp(trim(expr), "exit") == 0) { return 1; }
    if (strcmp(trim(expr), "quit") == 0) { return 1; }
    if (strcmp(trim(expr), "q")    == 0) { return 1; }
    return 0;
}

/*
 * Display an input prompt.
 */
int prompt() {
    addstr("> ");
    return getcurx(stdscr);
}

/*
 * Scroll the window upward to free up the given number of lines.
 */
void scrolllines(int num) {
    int i;
    int origY = getcury(stdscr);
    if (getcury(stdscr) + num <= LINES) {
        return;
    }

    move(0, getcurx(stdscr));
    for (i = 0; i < num; i++) {
        deleteln();
    }
    move(origY - num, getcurx(stdscr));
}

/*
 * Handle ncurses keyboard input.
 *
 * ch: Button pressed.
 * startIndex: Column at which text input begins.
 * length: Maximum length of text input.
 */
int handle_input(int ch, int startIndex, int* length) {
    double result;
    int errorCode;
    char expr[EXPR_LEN];
    char resultStr[EXPR_LEN];

    if (ch == KEY_LEFT) {
        if (getcurx(stdscr) > startIndex) {
            move(getcury(stdscr), getcurx(stdscr) - 1);
        }
    } else if (ch == KEY_RIGHT) {
        if (getcurx(stdscr) <= *length + 1) {
            move(getcury(stdscr), getcurx(stdscr) + 1);
        }
    } else if (ch == KEY_BACKSPACE || ch == 127 || ch == '\b') {
        if (getcurx(stdscr) > startIndex) {
            mvdelch(getcury(stdscr), getcurx(stdscr) - 1);
            (*length)--;
        }
    } else if (ch == KEY_DC) {
        if (getcurx(stdscr) <= *length + 1) {
            delch();
            (*length)--;
        }
    } else if (ch == KEY_ENTER || ch == '\n') {
        mvinnstr(getcury(stdscr), startIndex, expr, EXPR_LEN);
        if (isQuit(expr)) {
            return 0;
        }

        scrolllines(2);
        errorCode = Error_Success;
        result = calculate(trim(expr), &errorCode);
        move(getcury(stdscr) + 1, 0);
        switch (errorCode) {
            case Error_Success:
                format(result, resultStr);
                insstr(resultStr);
                break;
            case Error_UnbalParens:
                insstr("Error: Unbalanced parentheses");
                break;
            case Error_DivideByZero:
                insstr("Error: Division by zero");
                break;
        }
        move(getcury(stdscr) + 1, 0);
        prompt();
    } else if (is_numeric(ch) || is_alpha(ch) || is_symbol(ch) || ch == ' ') {
        if (getcurx(stdscr) <= *length + 1) {
            insch(ch);
            move(getcury(stdscr), getcurx(stdscr) + 1);
        } else {
            addch(ch);
        }
        (*length)++;
    }

    return 1;
}

/*
 * Show copyright disclaimer.
 */
void copyright() {
    mvinsstr(0, 0, "Replicalc, Copyright (C) 2021 David Heinemann");
    mvinsstr(1, 0, "Replicalc comes with ABSOLUTELY NO WARRANTY.");
    mvinsstr(2, 0, "This is free software, and you are welcome to redistribute it");
    mvinsstr(3, 0, "under certain conditions. See the LICENSE file for details.");
    move(5, 0);
}

int main() {
    int startIndex;
    int length = 0;
    int running = 1;

    /* Init ncurses */
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, 1);

    copyright();
    startIndex = prompt();

    /* Main loop */
    while (running) {
        running = handle_input(getch(), startIndex, &length);
    }

    endwin();
    return 0;
}