/*
 * Replicalc, a REPL-based calculator for programmers
 * Copyright (C) 2022 David Heinemann
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

package cmd

import (
	"errors"
	"fmt"
	"os"
	"sort"
	"strings"

	"dheinemann.com/replicalc/calc"
	"dheinemann.com/replicalc/meta"
	"github.com/jedib0t/go-pretty/v6/table"
)

type command struct {
	name        string
	description string
	printable   bool
	execute     func(args []string, calculator calc.Calculator) error
}

var ErrExitCommand = errors.New("User sent 'exit' command.")

func exit(args []string, calculator calc.Calculator) error {
	if len(args) > 1 {
		return errors.New(fmt.Sprintf("Error: command '%v' has no arguments.", args[0]))
	}
	return ErrExitCommand
}

func help(args []string, calculator calc.Calculator) error {
	if len(args) > 1 {
		return errors.New(fmt.Sprintf("Error: command '%v' has no arguments.", args[0]))
	}

	fmt.Println("Commands:")

	t := table.NewWriter()
	t.SetOutputMirror(os.Stdout)
	t.Style().Options.DrawBorder = false
	t.Style().Options.SeparateColumns = false

	for _, v := range getCommands() {
		if !v.printable {
			continue
		}
		t.AppendRow([]interface{}{v.name, v.description})
	}
	t.Render()

	return nil
}

func getCommands() []command {
	commands := []command{
		{
			name:        "exit",
			description: "Exit Replicalc",
			printable:   true,
			execute:     exit,
		},
		{
			name:      "quit",
			printable: false,
			execute:   exit,
		},
		{
			name:        "vars",
			description: "Print list of variables and their values",
			printable:   true,
			execute: func(args []string, calculator calc.Calculator) error {
				fmt.Println("Variables:")

				vars := calculator.Variables().GetAll()
				if len(vars) == 0 {
					fmt.Println(" (no variables)")
				}

				t := table.NewWriter()
				t.SetOutputMirror(os.Stdout)
				t.Style().Options.DrawBorder = false
				t.Style().Options.SeparateColumns = false

				sort.Strings(vars)
				for _, k := range vars {
					t.AppendRow([]interface{}{k, calculator.Variables().Get(k)})
				}

				t.Render()

				return nil
			},
		},
		{
			name:        "help",
			description: "Print help text",
			printable:   true,
			execute:     help,
		},
		{
			name:      "?",
			printable: false,
			execute:   help,
		},
		{
			name:        "version",
			description: "Print version information",
			printable:   true,
			execute: func(args []string, calculator calc.Calculator) error {
				fmt.Println(meta.TitleText)
				return nil
			},
		},
	}

	return commands
}

func newCommandMap() map[string]command {
	commands := getCommands()

	commandMap := map[string]command{}
	for i, v := range commands {
		commandMap[v.name] = commands[i]
	}

	return commandMap
}

var commands map[string]command = newCommandMap()

func IsCommand(input string) bool {
	commandName := strings.Split(input, " ")[0]
	_, ok := commands[commandName]
	return ok
}

func ExecCommand(input string, calculator calc.Calculator) error {
	args := strings.Split(input, " ")
	commandName := args[0]
	command, ok := commands[commandName]
	if !ok {
		return errors.New(fmt.Sprintf("Error: command '%v' does not exist.", commandName))
	}

	return command.execute(args, calculator)
}
