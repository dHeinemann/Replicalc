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

#include "cursesui.h"

int main() {
    int copylen = 4;
    char* copyright[4] = {
        "Replicalc, Copyright (C) 2021 David Heinemann",
        "Replicalc comes with ABSOLUTELY NO WARRANTY.",
        "This is free software, and you are welcome to redistribute it",
        "under certain conditions. See the LICENSE file for details."
    };

    startui(copyright, copylen);
    return 0;
}
