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

package main

import (
	"errors"
	"fmt"
	"math"
	"strconv"
	"strings"

	"dheinemann.com/replicalc/calc"
	"dheinemann.com/replicalc/chartypes"
	"dheinemann.com/replicalc/cmd"
	"dheinemann.com/replicalc/meta"
	"github.com/peterh/liner"
)

// Print Replicalc copyright and license details
func printCopyright() {
	fmt.Println(meta.TitleText)
	fmt.Println(meta.LicenseText)
}

// Format a float for printing
func formatFloat(value float64) string {
	// Omit decimal places when value is a whole number
	if math.Mod(value, 1) == 0 {
		return strconv.FormatFloat(value, 'f', 0, 64)
	} else {
		return strconv.FormatFloat(value, 'f', 4, 64)
	}
}

// Print a float
func printFloat(value float64) {
	formattedValue := formatFloat(value)
	fmt.Printf("%v\n", formattedValue)
}

// Print the value of a variable
func printVariable(varName string, calculator calc.Calculator) {
	formattedValue := formatFloat(calculator.Variables().Get(varName))
	fmt.Printf("%v = %v\n", varName, formattedValue)
}

// Get the target variable to store an expression in, and the adjusted version of an expression in the event that the
// user is storing it in a variable other than the default.
//
// For example:
//   "2 * 5"       -> ("ans", "2 * 5")
//   "foo = 2 * 5" -> ("foo", "2 * 5")
func getTargetAndExpression(expr string) (varName string, adjustedExpr string, err error) {
	eqIndex := strings.Index(expr, "=")
	if eqIndex >= 0 {
		// Valid syntax for setting variable is 'VARNAME = EXPR'
		varName = strings.TrimSpace(expr[:eqIndex])
		adjustedExpr = strings.TrimSpace(expr[eqIndex+1:])

		for i := 0; i < len(varName); i++ {
			if !chartypes.IsLetter(varName[i]) {
				return varName, adjustedExpr, errors.New("Variable name must only consist of alphabetical characters")
			}
		}

		if len(varName) == 0 {
			return varName, adjustedExpr, errors.New("Must specify a variable name when using assignment operator (=)")
		}
	} else {
		varName = calc.DefaultVarName
		adjustedExpr = expr
	}

	return varName, adjustedExpr, nil
}

// Start the REPL.
func repl() {
	line := liner.NewLiner()
	defer line.Close()

	line.SetCtrlCAborts(true)

	calculator := calc.NewCalculator()

	for {
		fmt.Println()
		input, err := line.Prompt("> ")

		if err != nil {
			if err != liner.ErrPromptAborted {
				// ErrPromptAborted gets thrown by Ctrl+C
				fmt.Printf("Fatal error: %v\n", err.Error())
			}
			return
		} else {
			line.AppendHistory(input)
		}

		input = strings.Trim(input, "\n\r ")
		if len(input) == 0 {
			continue
		}

		if cmd.IsCommand(input) {
			if err := cmd.ExecCommand(input, calculator); err != nil {
				if err == cmd.ErrExitCommand {
					return
				}
				fmt.Printf("Error: %v\n", err.Error())
			}
			continue
		}

		// User may print the value of a variable by writing its name. In this case, it shouldn't be evaluated as a math
		// expression.
		if calculator.Variables().Exists(input) {
			printVariable(input, calculator)
			continue
		}

		varName, input, err := getTargetAndExpression(input)
		if err != nil {
			fmt.Printf(err.Error())
			continue
		}

		result, err := calculator.Evaluate(input)
		if err != nil {
			fmt.Println(err.Error())
			continue
		}

		// Set the target variable ("ans" by default)
		varDb := calculator.Variables()
		varDb.Set(varName, result)

		// Print formatted result
		if varName != calc.DefaultVarName {
			// User set non-default variable; show what was set
			printVariable(varName, calculator)
		} else {
			printFloat(result)
		}
	}
}

func main() {
	printCopyright()
	repl()
}
