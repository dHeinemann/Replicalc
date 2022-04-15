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
	"bufio"
	"fmt"
	"math"
	"os"
	"strconv"
	"strings"

	"dheinemann.com/replicalc/calculator"
)

func printCopyright() {
	fmt.Println("Replicalc 0.2.0, Copyright (C) 2022 David Heinemann")
	fmt.Println("Replicalc comes with ABSOLUTELY NO WARRANTY.")
	fmt.Println("This is free software, and you are welcome to redistribute it")
	fmt.Println("under certain conditions. See the LICENSE file for details.")
}

func handleError(errorCode calculator.ErrorCode) {
	if errorCode == calculator.ErrorSuccess {
		return
	} else if errorCode == calculator.ErrorDivideByZero {
		fmt.Println("Error: Division by zero")
	} else if errorCode == calculator.ErrorUnbalancedParantheses {
		fmt.Println("Error: Unbalanced parantheses")
	} else {
		fmt.Println("Unexpected error")
	}
}

func isExit(input string) bool {
	return input == "exit" || input == "quit"
}

func repl() {
	for {
		fmt.Printf("\n> ")
		reader := bufio.NewReader(os.Stdin)
		expr, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Error: invalid input")
			continue
		}

		expr = strings.Trim(expr, "\n\r ")
		if isExit(expr) {
			break
		}

		result, errorCode := calculator.Calculate(expr)
		if errorCode != calculator.ErrorSuccess {
			handleError(errorCode)
			continue
		}

		var formattedResult string
		if math.Mod(result, 1) > 0 {
			formattedResult = strconv.FormatFloat(result, 'f', 4, 64)
		} else {
			// Omit decimal places when printing whole numbers
			formattedResult = strconv.FormatFloat(result, 'f', 0, 64)
		}

		fmt.Printf("%v\n", formattedResult)
	}
}

func main() {
	printCopyright()
	repl()
}
