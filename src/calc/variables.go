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

// Default variable to store evaluated result in
const DefaultVarName = "ans"

type VariableDb struct {
	variables map[string]float64
}

func newVariableDb() VariableDb {
	return VariableDb{
		variables: make(map[string]float64),
	}
}

func (db VariableDb) Exists(key string) bool {
	_, ok := db.variables[key]
	return ok
}

func (db VariableDb) Get(key string) float64 {
	return db.variables[key]
}

func (db *VariableDb) Set(key string, value float64) {
	db.variables[key] = value
}

func (db VariableDb) GetAll() []string {
	vars := []string{}
	for k := range db.variables {
		vars = append(vars, k)
	}

	return vars
}
