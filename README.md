# Replicalc

Replicalc is a simple keyboard-driven calculator. Expressions are evaluated
inside a REPL, similar to the classic TI-83 calculator.

**"Screenshot":**

```
Replicalc, Copyright (C) 2021 David Heinemann
Replicalc comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions. See the LICENSE file for details.

> 10 * (2 + 3) / 2
25
>
```

Replicalc currently uses an ncurses-based interface. Additional interfaces for
other platforms are planned:

* conio.h for DOS/Windows
* Win16 for Windows
* GTK for Linux

## Status

This is a hobby project to learn the C programming language. The code is bad
and will contain bugs; it can be considered an early alpha. Run at your own
risk.

## Supported Operators

* Addition (`+`), Subtraction (`-`), Multiplication (`*`), Division (`/`)
* Exponents (`^`)
* Parenthesis (`(`, `)`)

Additional operators are forthcoming.

## Compiling

First, install the ncurses development library:

* **Fedora:** `sudo dnf install ncurses-devel`

Next, run `make` in the repository root.

## License

Replicalc is published under the GNU General Public License, version 2. See the
LICENSE file for more information.