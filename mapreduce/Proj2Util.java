/*
 * CS61C Spring 2014 Project2
 * Reminders:
 *
 * DO NOT SHARE CODE IN ANY WAY SHAPE OR FORM, NEITHER IN PUBLIC REPOS OR FOR DEBUGGING.
 *
 * DO NOT MODIFY THIS FILE. ANY CHANGES HERE WILL NOT BE RUN IN THE GRADING OF YOUR PROJECT.
 */
import java.util.*;

/**
 * This class provides some Connect N game utility methods to aid in the CS61C Spring 2014 Project 2.
 *
 * The methods provided include:
 *     - A hasher, and an unHasher for game state representation compression
 *     - A win checker, that tells you if a game state representation includes a win
 *
 */
public class Proj2Util {
	/**
	 * Returns a reversible 32 bit integer hash for a given a string representation
	 * of the game board. The hashed value can be reverted to the original string
	 * representation by calling the {@link #gameUnhasher(int, int, int) gameUnhasher()} method.
	 *
	 * @param position the string representing the board state
	 * @param width the width of the board
	 * @param height the height of the board
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

	/**
	 *  Returns a string representation for the game board represented by the provided 32 bit
	 *  integer hash.
	 *
	 *  @param hash the hash representing board state
	 *  @param width the width of the board
	 *  @param height the height of the board
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

	/**
	 * Returns a boolean value to signify if the provided game board has a winner. It does NOT check
	 * who the winner is, just that there is a winner.
	 *
	 * @param key the string repressing the board state
	 * @param boardWidth the width of the board
	 * @param boardHeight the height of the board
	 * @param connectWin the number of pieces that must be connected to win
	 */
	public static boolean gameFinished(String key, int boardWidth, int boardHeight, int connectWin) {
		char[] keyArray = key.toCharArray();
		for (int i = 0; i < boardWidth; i++) {
			for (int j = boardHeight - 1; j >= 0 ; j--) {
				char current = keyArray[i*boardHeight + j];
				if (current == ' ') {
					continue;
				}
				boolean iPass = true;
				boolean jPass = true;
				boolean ijPass = true;
				boolean ij2Pass = true;
				for (int k = 1; k < connectWin; k++) {
					int ik = i + k;
					int jk = j - k;
					int jk2 = j + k;
					if (iPass && (!(ik < boardWidth) || (current != keyArray[ik * boardHeight + j]))) {
						iPass = false;
					}
					if (jPass && (!(jk >= 0) || (current != keyArray[i * boardHeight + jk]))) {
						jPass = false;
					}
					if (ijPass && (!(jk >= 0) || !(ik < boardWidth) || (current != keyArray[ik * boardHeight + jk]))) {
						ijPass = false;
					}
					if (ij2Pass && (!(jk2 < boardHeight) || !(ik < boardWidth) || (current != keyArray[ik * boardHeight + jk2]))) {
						ij2Pass = false;
					}
				}
				if (iPass || jPass || ijPass || ij2Pass) {
					return true;
				}
			}
		}
		return false;
	}
}
