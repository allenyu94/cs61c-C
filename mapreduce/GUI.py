import sys
import time
import random

def aiMove(board, X_turn, width, height):
	time.sleep(0.5)
	best_board = ""
	value = 0
	for i in range(0,width):
		for j in range(0,height):
			check = board[i * height + j]
			if check == ' ':
				if X_turn:
					board[i * height + j] = 'X'
				else:
					board[i * height + j] = 'O'
				board_string = "".join(x for x in board)
				f = open('all_data/final/part-r-00000', 'r')
				for line in f:
					if board_string in line:
						cur_value = int(line.split()[-1])
						if cur_value == value:
							if (random.random() > 0.5):
								best_board = list(board)
						elif (X_turn and cur_value & 3 == 2) or ((not X_turn) and cur_value & 3 == 1):
							if value & 3 != cur_value & 3 or (value >> 2) > (cur_value >> 2):
								best_board = list(board)
								value = cur_value
						elif value == 0 or value & 3 < cur_value & 3 or (value & 3 == cur_value & 3 and (value >> 2) < (cur_value >> 2)):
							if (X_turn and value & 3 == 2) or ((not X_turn) and value & 3 == 1):
								break
							best_board = list(board)
							value = cur_value
						break
				board[i * height + j] = ' '
	return best_board

def main(argv):
	if len(argv) != 3:
		print "Make sure to put width, height, connection required to win in that order!"
		sys.exit(1)
	width = int(argv[0])
	height = int(argv[1])
	connect = int(argv[2])
	f = open('all_data/final/part-r-00000', 'r')
	game_going = 1
	board = []
	AI_ONLY = raw_input("Would you like to see the AI play itself? (y,n): ")
	go_first = 'n'
	if AI_ONLY == 'n':
		go_first = raw_input("Would you like to start? (y,n): ")
	X_turn = True
	AI_turn = True
	if go_first == 'y':
		AI_turn = False
	for i in range(0,width):
		for j in range(0,height):
			board.append(' ')
	for i in range(0,height):
		row = '|'
		for j in range(0,width):
			row += ' ' + board[j* height + (height - i - 1)] + ' |'
		print row
	while (game_going):
		if AI_turn:
			board = aiMove(board, X_turn, width, height)
			print "AI made his move"
		else:
			var = int(raw_input("Choose your next move: "))
			piece = 'O'
			if (X_turn):
				piece = 'X'
			for i in range(0,height):
				check = board[var * height + i]
				if check == ' ':
					board[var * height + i] = piece
					break
		for i in range(0,height):
			row = '|'
			for j in range(0,width):
				row += ' ' + board[j* height + (height - i - 1)] + ' |'
			print row
		board_string = "'" + "".join(x for x in board) + "'"
		f = open('all_data/final/part-r-00000', 'r')
		for line in f:
			if board_string in line:
				cur_value = int(line.split()[-1])
				if cur_value < 4:
					if cur_value == 1:
						print "O Wins!"
					elif cur_value == 2:
						print "X Wins!"
					elif cur_value == 3:
						print "Draw Game!"
					game_going = 0
		if AI_ONLY == 'n':
			AI_turn = not AI_turn
		X_turn = not X_turn

if __name__ == "__main__":
	main(sys.argv[1:])
