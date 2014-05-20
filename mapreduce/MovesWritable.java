/*
 * CS61C Spring 2014 Project2
 * Reminders:
 *
 * DO NOT SHARE CODE IN ANY WAY SHAPE OR FORM, NEITHER IN PUBLIC REPOS OR FOR DEBUGGING.
 *
 * DO NOT MODIFY THIS FILE. ANY CHANGES HERE WILL NOT BE RUN IN THE GRADING OF YOUR PROJECT.
 */
import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

import org.apache.hadoop.io.Writable;

/**
 * Used to store a game board state (win/loss/tie/undecided), number of moves till the board
 * resolves to that win state and all possible parent states. We recommend you use this class
 * to store all the necessary data about each game state in the tree and update the value field
 * as you walk up the three.
 */
public class MovesWritable implements Writable {
    /**
     * Stores the predicted win state and the number of moves required to reach
     * that win state from the associated game board. Note that these are two
     * distinct items that have been combined into a compact byte representation
     * minimize space usage.
     *
     * The top (most significant) 6 bits contain the unsigned representation of the
     * number of moves required to reach the specified win state.
     *
     * The bottom (least significant) 2 bites represent the predicted win state:
     *     - 0b00 = Undecided
     *     - 0b01 = O Wins
     *     - 0b10 = X Wins
     *     - 0b11 = Tie
     *
     * To correctly use these two fields, simply use the provided getter and
     * setter methods
     *
     */
    private byte value;

    /**
     * Contains the 32 bit integer hash representation of all parent game states, i.e.
     * all game states which can result in the associated game state after 1 turn.
     *
     * To correctly use this field, simply use the provided getter and setter methods.
     */
    private int[] moves;
    private int counter;

    /**
     * Creates a default MovesWritable with empty parents array, undeicded state and
     * zero moves to achieve that state.
     *
     */
    public MovesWritable() { }

    /**
     * Creates a MovesWritable with the fields set to the provided values.
     *
     * @param status the status of the move, where 0 means undecided, 1 means O wins,
     * 2 means X wins, and 3 means draw
     * @param movesToEnd the number of moves left before the game terminates from this
     * board position (assuming optimal play from both sides)
     * @param moves the array of parent moves for this particular board configuration
     */
    public MovesWritable(int status, int movesToEnd, int[] moves) {
        this.value = (byte) ((status & 3) | (movesToEnd << 2));
        if (moves == null) {
            this.counter = 0;
            this.moves = null;
        } else {
            this.counter = moves.length;
            this.moves = new int[counter];
            System.arraycopy(moves, 0, this.moves, 0, this.counter);
        }
    }

    /**
     * Another moves writable that takes in the inputs for value and an array of parent moves.
     *
     * @param value the combined byte representation of the number of moves to game termination
     * and the board state, where the last two bits are board state and the remaining six bits
     * are the moves to end of game
     * @param moves the array of parent moves for this particular board configuration
     */
    public MovesWritable(byte value, int[] moves) {
        this.value = value;
        if (moves == null) {
            this.counter = 0;
            this.moves = null;
        } else {
            this.counter = moves.length;
            this.moves = new int[counter];
            System.arraycopy(moves, 0, this.moves, 0, this.counter);
        }
    }

    /**
     * Returns the status of the given board position, where 0 means undecided, 1 means O wins,
     * 2 means X wins, and 3 means draw.
     *
     */
    public int getStatus() {
        return this.value & 3;
    }

    /**
     * Sets the status of the given board position, where 0 means undecided, 1 means O wins,
     * 2 means X wins, and 3 means draw.
     *
     * @param status the status of the board position
     */
    public void setStatus(int status) {
        this.value = (byte)(((this.value >> 2) << 2) | (status & 3));
    }

    /**
     * Returns the number of moves until the termination of the given board position.
     *
     */
    public int getMovesToEnd() {
        return this.value >> 2;
    }

    /**
     * Sets the number of moves until the termination of the given board position.
     *
     * @param movesToEnd the number of moves left until the game ends (assuming optimal play)
     */
    public void setMovesToEnd(int movesToEnd) {
        this.value = (byte)((this.value & 3) | (movesToEnd << 2));
    }

    /**
     * Sets the value of the given board position, where the last 2 bits are the
     * status of the state, and the remaining bits are the moves until termination.
     *
     * @param val the value of the current position where the last 2 bits are the status
     * and the remaining bits are moves to termination
     */
    public void setValue(byte val) {
        this.value = val;
    }

    /**
     * Returns the value of the given board position, where the last 2 bits are the status
     * of the state, and the remaining bits are the moves until termination.
     *
     */
    public byte getValue() {
        return this.value;
    }

    /**
     * Sets the array of parents of this board position.
     *
     * @param in_moves the array of parents to be written into this MovesWritable
     */
    public void setMoves(int [] in_moves) {
        this.counter = in_moves.length;
        this.moves = new int[counter];
        System.arraycopy(in_moves, 0, this.moves, 0, this.counter);
    }

    /**
     * Returns the array of parents of this board position.
     *
     */
    public int[] getMoves() {
        return this.moves;
    }

    /**
     * Write function required by the Writable, for processing the data.
     * (You don't need to worry about this)
     *
     * @param out the location the data outputs to
     */
    public void write(DataOutput out) throws IOException {
        out.writeByte(value);
        out.writeInt(counter);
        for (int i = 0; i < counter; i++)
            out.writeInt(moves[i]);
    }

    /**
     * Read function required by the Writable, for processing the data.
     * (You don't need to worry about this)
     *
     * @param in the location the data reads from
     */
    public void readFields(DataInput in) throws IOException {
        this.value = in.readByte();
        this.counter = in.readInt();
        this.moves = new int[counter];
        for (int i = 0; i < counter; i++)
            moves[i] = in.readInt();
    }
}
