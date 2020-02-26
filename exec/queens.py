#!/usr/bin/python3

BOARD_SIZE = 8
FIELD_EMPTY = '-'
FIELD_QUEEN = 'Q'

def print_board():
	for y in range(BOARD_SIZE):
		for x in range(BOARD_SIZE): print(board[y][x], end= " ")
		print()
	print()

def not_threaten(queen_y, queen_x):
	for y in range(BOARD_SIZE):
		for x in range(BOARD_SIZE):
			if (y == queen_y or x == queen_x) and board[y][x] == FIELD_QUEEN: return False
			if ((y - queen_y) == (x - queen_x)) and board[y][x] == FIELD_QUEEN: return False
			if ((y - queen_y) == -(x - queen_x)) and board[y][x] == FIELD_QUEEN: return False
	return True

def queen_try(queen_y_start, queen_x_start):
	board[queen_y_start][queen_x_start] = FIELD_QUEEN
	n_queens += 1
	print("start position y[%d] x[%d]" % (queen_y_start, queen_x_start))
	for queen_y in range(BOARD_SIZE):
		for queen_x in range(BOARD_SIZE):
			if not_threaten(queen_y, queen_x):
				board[queen_y][queen_x] = FIELD_QUEEN
				n_queens += 1
	print("queens: %d" % (n_queens))

#for offset in range(BOARD_SIZE ** 2):
#	board = [[FIELD_EMPTY for i in range(BOARD_SIZE)] for i in range(BOARD_SIZE)]
#	queen_y_start = offset // BOARD_SIZE
#	queen_x_start = offset % BOARD_SIZE
#	board[queen_y_start][queen_x_start] = FIELD_QUEEN
#	n_queens = 1
#	print("start position y[%d] x[%d]" % (queen_y_start, queen_x_start))
#	for queen_y in range(BOARD_SIZE):
#		for queen_x in range(BOARD_SIZE):
#			if not_threaten(queen_y, queen_x):
#				board[queen_y][queen_x] = FIELD_QUEEN
#				n_queens += 1
#	print("queens: %d" % (n_queens))
#	print_board()

