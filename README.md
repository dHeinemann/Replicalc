# Replicalc

Replicalc is a lightweight, keyboard-driven calculator. Expressions are
evaluated inside a REPL, similar to the classic TI-83 calculator.

Replicalc supports Linux and Windows. The BSDs and MacOS should also work, but are untested.
Windows binaries are provided under [Releases](https://github.com/dHeinemann/Replicalc/releases).

![Replicalc Screenshot](screenshot.png?raw=true)

## Usage

To evaluate an expression, type it into the prompt:

```
> 9 * 3
27
```

The result is automatically stored inside the `ans` variable. This can be used
in further expressions:

```
> 9 * 3
27

> ans
27

> ans + 4
31
```

Results can be stored inside an alternative variable using the assignment
operator (`=`):

```
> foo = 10
foo = 10
```

Use the `vars` command to print a list of variables and their respective
values:

```
> vars
Variables:
 ans  31
 foo  10
```

Fractions are supported, up to four decimal places. The decimal places are
automatically omitted for whole numbers:

```
> 9 / 3
3

> 9 / 3.0001
2.9999
```

## Features

**Operators**

* Addition (`+`), Subtraction (`-`), Multiplication (`*`), Division (`/`), Modulo (`%`)
* Variable Assignment (`=`)
* Exponents (`^`)
* Parenthesis (`(`, `)`)

**Commands**

* `help`: Print help, and list of available commands
* `version`: Print Replicalc version information
* `vars`: Print list of variables and their values
* `exit`: Quit Replicalc

## Compiling

Replicalc requires [Go](https://go.dev) 1.16 (or newer).

To compile:

1. Open the terminal and navigate to the **src** directory.
2. Run `go build`.

## Contributions

Contributions that align with the Replicalc project goals are welcome. Please see [CONTRIBUTING.md](CONTRIBUTING.md) for
more information.

## License

Replicalc is published under the GNU General Public License, version 2. See
[LICENSE](LICENSE) for more information.
