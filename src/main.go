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
	"fmt"
	"math"
	"strconv"
	"strings"

	"dheinemann.com/replicalc/calc"
	"dheinemann.com/replicalc/chartypes"
	"github.com/peterh/liner"
)

const versionNum = "0.2.0"

// Default variable to store evaluated result in
const defaultVarName = "ans"

func printCopyright() {
	fmt.Printf("Replicalc %v, Copyright (C) 2022 David Heinemann\n", versionNum)
	fmt.Printf("Replicalc comes with ABSOLUTELY NO WARRANTY.\n")
	fmt.Printf("This is free software, and you are welcome to redistribute it\n")
	fmt.Printf("under certain conditions. See the LICENSE file for details.\n")
}

func handleError(errorCode calc.ErrorCode, context string) {
	if errorCode == calc.ErrorSuccess {
		return
	} else if errorCode == calc.ErrorDivideByZero {
		fmt.Printf("Error: Division by zero\n")
	} else if errorCode == calc.ErrorUnbalancedParantheses {
		fmt.Printf("Error: Unbalanced parantheses\n")
	} else if errorCode == calc.ErrorUnknownVariable {
		fmt.Printf("Error: Unknown variable '%v'\n", context)
	} else {
		fmt.Printf("Unexpected error\n")
	}
}

func isExit(input string) bool {
	return input == "exit" || input == "quit"
}

// Format a float for printing
func formatFloat(value float64) string {
	// Omit decimal places when value is a whole number
	if math.Mod(value, 1) > 0 {
		return strconv.FormatFloat(value, 'f', 4, 64)
	} else {
		return strconv.FormatFloat(value, 'f', 0, 64)
	}
}

// Print a float
func printFloat(value float64) {
	formattedValue := formatFloat(value)
	fmt.Printf("%v\n", formattedValue)
}

// Print the value of a variable
func printVariable(varName string) {
	formattedValue := formatFloat(calc.GetVar(varName))
	fmt.Printf("%v = %v\n", varName, formattedValue)
}

// Get the target variable to store an expression in, and the adjusted version of an expression in the event that the
// user is storing it in a variable other than the default.
//
// For example:
//   "2 * 5"       -> ("ans", "2 * 5")
//   "foo = 2 * 5" -> ("foo", "2 * 5")
func getTargetAndExpression(expr string) (varName string, adjustedExpr string, ok bool) {
	eqIndex := strings.Index(expr, "=")
	if eqIndex >= 0 {
		// Valid syntax for setting variable is 'VARNAME = EXPR'
		varName = strings.TrimSpace(expr[:eqIndex])
		adjustedExpr = strings.TrimSpace(expr[eqIndex+1:])

		// Variable name must only consist of alphabetical characters
		for i := 0; i < len(varName); i++ {
			if !chartypes.IsLetter(varName[i]) {
				return varName, adjustedExpr, false
			}
		}

		// Variable name cannot be an empty string
		if len(varName) == 0 {
			return varName, adjustedExpr, false
		}
	} else {
		varName = defaultVarName
		adjustedExpr = expr
	}

	return varName, adjustedExpr, true
}

// Start the REPL.
func repl() {
	line := liner.NewLiner()
	defer line.Close()

	line.SetCtrlCAborts(true)

	for {
		expr, err := line.Prompt("> ")
		if err != nil {
			fmt.Printf("Fatal error: %v\n", err.Error())
			return
		} else {
			line.AppendHistory(expr)
		}

		expr = strings.Trim(expr, "\n\r ")
		if isExit(expr) {
			break
		}

		// User may print the value of a variable by writing its name. In this case, it shouldn't be evaluated as a math
		// expression.
		if calc.VarExists(expr) {
			printVariable(expr)
			continue
		}

		varName, expr, ok := getTargetAndExpression(expr)
		if !ok {
			fmt.Printf("Error: Invalid variable name '%v'\n", varName)
			continue
		}
		result, errorCode, context := calc.Calculate(expr)

		if errorCode != calc.ErrorSuccess {
			handleError(errorCode, context)
			continue
		}

		// Set the target variable ("ans" by default)
		calc.SetVar(varName, result)

		// Print formatted result
		if varName != defaultVarName {
			// User set non-default variable; show what was set
			printVariable(varName)
		} else {
			printFloat(result)
		}
	}
}

func main() {
	printCopyright()
	fmt.Println()
	calc.InitializeVariables()
	repl()
}
