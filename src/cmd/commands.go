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
	"github.com/jedib0t/go-pretty/v6/table"
)

func exit(args []string) error {
	if len(args) > 1 {
		return errors.New(fmt.Sprintf("Error: command '%v' has no arguments.", args[0]))
	}
	os.Exit(0)
	return nil
}

var commands = map[string]func(args []string) error{
	"exit": exit,
	"quit": exit,
	"vars": func(args []string) error {
		t := table.NewWriter()
		t.SetOutputMirror(os.Stdout)
		t.AppendHeader(table.Row{"Name", "Value"})

		vars := calc.GetVars()
		sort.Strings(vars)
		for _, k := range vars {
			t.AppendRow([]interface{}{k, calc.GetVar(k)})
		}

		t.Render()

		return nil
	},
}

func IsCommand(input string) bool {
	commandName := strings.Split(input, " ")[0]
	_, ok := commands[commandName]
	return ok
}

func ExecCommand(input string) error {
	args := strings.Split(input, " ")
	commandName := args[0]
	command, ok := commands[commandName]
	if !ok {
		return errors.New(fmt.Sprintf("Error: command '%v' does not exist.", commandName))
	}

	return command(args)
}
