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

package calc

var variables map[string]float64

var variablesInitialized bool = false

func InitializeVariables() {
	variables = make(map[string]float64)
	variablesInitialized = true
}

func VarExists(key string) bool {
	_, ok := variables[key]
	return ok
}

func GetVar(key string) float64 {
	return variables[key]
}

func SetVar(key string, value float64) {
	variables[key] = value
}

func GetVars() []string {
	vars := []string{}
	for k := range variables {
		vars = append(vars, k)
	}

	return vars
}
