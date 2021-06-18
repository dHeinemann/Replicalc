# Replicalc

Replicalc is a simple keyboard-driven calculator for Linux and MS-DOS.
Expressions are evaluated inside a REPL, similar to the classic TI-83
calculator.

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

Before proceeding, initialize all submodules if you haven't already done so:

```
git submodule init
git submodule update
```

### Linux

1. Install the ncurses development library. For example (Fedora): `sudo dnf install ncurses-devel`
2. Navigate to the repository root.
2. Run `make`.

### MS-DOS

1. Install the [Open Watcom compiler](http://openwatcom.org).
2. Navigate to the `dos` directory.
2. Run `wmake`.

## License

Replicalc is published under the GNU General Public License, version 2. See the
LICENSE file for more information.