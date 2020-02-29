#!/usr/bin/python3

# INFORMACION DEL SCRIPT
# ---------------------------------------------------------
# AUTOR   : Ezequiel Larrarte
# FECHA   : 27-02-2019
# VERSION : 01
#
# DESCRIPCION
# Este script busca ubicar las N reinas sin que se amenacen
# en un tablero de ajedrez de NxN
# https://es.wikipedia.org/wiki/Problema_de_las_ocho_reinas
# ---------------------------------------------------------

# LIBRERIAS
# ---------------------------------------------------------
import os
import sys
if "SCRIPTS" in os.environ.keys():
	sys.path.append(os.environ['SCRIPTS'] + '/include')
from posix_exitcodes import *
import copy
# ---------------------------------------------------------

# CONFIGURACION
# ---------------------------------------------------------
BOARD_SIZE = 8
FIELD_EMPTY = '-'
FIELD_QUEEN = 'Q'
# ---------------------------------------------------------

# FUNCIONES
# ---------------------------------------------------------

# Print a board
def print_solution(l_solution):
	board = new_board()

	for queen in l_solution:
		board[queen["y"]][queen["x"]] = FIELD_QUEEN

	for n_y in range(BOARD_SIZE):
		for n_x in range(BOARD_SIZE):
			print(board[n_y][n_x], end=" ")
		print()
	print()

# Tell if a given coordinate is threaten by other queens
def not_threaten(l_solution, n_queen_y, n_queen_x):
	for queen in l_solution:
		if queen["y"] == n_queen_y or queen["x"] == n_queen_x: return False
		if (queen["y"] - n_queen_y) == (queen["x"] - n_queen_x): return False
		if (queen["y"] - n_queen_y) == -(queen["x"] - n_queen_x): return False

	return True

# Return a new board
def new_board():
	return [[FIELD_EMPTY for i in range(BOARD_SIZE)] for i in range(BOARD_SIZE)]

# Try to add a not threatening queen to the board
def add_queens(l_solution=[], n_queens=BOARD_SIZE, n_offset_from=0):
	global l_solutions

	if n_queens:
		for n_offset in range(n_offset_from, BOARD_SIZE**2):
			n_y = n_offset // BOARD_SIZE
			n_x = n_offset % BOARD_SIZE
			if not_threaten(l_solution, n_y, n_x):
				l_tmp_solution = copy.deepcopy(l_solution)
				l_tmp_solution.append({"y": n_y, "x": n_x})
				add_queens(l_tmp_solution, n_queens-1, n_offset+(BOARD_SIZE-n_x))
	else:
		print("SOLUTION FOUND!")
		print_solution(l_solution)
		l_solutions.append(l_solution)

# Main function
def main():
	add_queens()
	print("SOLUTIONS: %d" % (len(l_solutions)))
# ---------------------------------------------------------

# PRINCIPAL
# ---------------------------------------------------------
l_solutions = []
main()
exit(EXIT_SUCCESS)
# ---------------------------------------------------------

