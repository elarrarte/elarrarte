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
def print_board(board):
	for n_y in range(BOARD_SIZE):
		for n_x in range(BOARD_SIZE):
			print(board[n_y][n_x], end=" ")
		print()
	print()

# Tell if a given coordinate is threaten by other queens
def not_threaten(board, n_queen_y, n_queen_x):
	for n_y in range(BOARD_SIZE):
		for n_x in range(BOARD_SIZE):
			if (n_y == n_queen_y or n_x == n_queen_x) and board[n_y][n_x] == FIELD_QUEEN: return False
			if ((n_y - n_queen_y) == (n_x - n_queen_x)) and board[n_y][n_x] == FIELD_QUEEN: return False
			if ((n_y - n_queen_y) == -(n_x - n_queen_x)) and board[n_y][n_x] == FIELD_QUEEN: return False
	return True

# Return a new board
def new_board():
	return [[FIELD_EMPTY for i in range(BOARD_SIZE)] for i in range(BOARD_SIZE)]

# Try to add a not threatening queen to the board
def add_queens(board=new_board(), n_queens=BOARD_SIZE, n_offset_from=0):
	global n_solutions

	if n_queens:
		for n_offset in range(n_offset_from, BOARD_SIZE**2):
			n_y = n_offset // BOARD_SIZE
			n_x = n_offset % BOARD_SIZE
			if not_threaten(board, n_y, n_x):
				tmp_board = copy.deepcopy(board)
				tmp_board[n_y][n_x] = FIELD_QUEEN
				add_queens(tmp_board, n_queens-1, n_offset+(BOARD_SIZE-n_x))
	else:
		print("SOLUTION FOUND!")
		print_board(board)
		n_solutions += 1

# Main function
def main():
	add_queens()
	print("SOLUTIONS: %d" % (n_solutions))
# ---------------------------------------------------------

# PRINCIPAL
# ---------------------------------------------------------
n_solutions = 0
main()
exit(EXIT_SUCCESS)
# ---------------------------------------------------------

