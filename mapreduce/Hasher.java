import java.util.*;
/*
    This is the hasher for CS61C Project 2, Spring 2014.

    Reminder: DO NOT SHARE CODE IN ANY WAY SHAPE OR FORM, NEITHER IN PUBLIC REPOS OR FOR DEBUGGING.

    DO NOT MODIFY THIS FILE. ANY CHANGES HERE WILL NOT BE RUN IN THE GRADING OF YOUR PROJECT.
 */
public class Hasher {
	/*
		This is the game hasher that takes the string of the board and turns it into a 32 bit hash.
		@position: the string of board state.
		@width: the width of the board.
		@height: the height of the board.
	*/
	public static int gameHasher(String position, int width, int height) {
		int bitLength = height + 1;
		int totalHash = 0;
		for (int i = 0; i < width; i++) {
			int rowValue = 0;
			for (int j = 0; j < height; j++) {
				char currentChar = position.charAt(i*height + j);
				if (currentChar == ' ')
					break;
				else if (currentChar == 'O')
					rowValue += 1 << j;
				else if (currentChar == 'X')
					rowValue += 2 << j;
			}
			totalHash = (totalHash << bitLength) + rowValue;
		}
		return totalHash;
	}

	/*
		This is the game unhasher that takes the hash of the board and turns it into a string of board state.
		@position: the hash of board state.
		@width: the width of the board.
		@height: the height of the board.
	*/
	public static String gameUnhasher(int hash, int width, int height) {
		char[] positionArray = new char[width * height];
		Arrays.fill(positionArray, ' ');
		int bitLength = height + 1;
		int bitMask = (1 << (height + 1)) - 1;
		for (int i = 0; i < width; i++) {
			int current = (hash >>> (bitLength * i)) & bitMask;
			for (int j = 0; j < height; j++) {
				if (current == 0) {
					break;
				} else if (current %2 == 0) {
					current = (current - 2) >>> 1;
					positionArray[(width - i - 1)*height + j] = 'X';
				} else {
					current = current >>> 1;
					positionArray[(width - i - 1)*height + j] = 'O';
				}
			}
		}
		return new String(positionArray);
	}
}
