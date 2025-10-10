#align(center, block[#set text(17pt); *The N-Queens Problem as a Digraph*])
#linebreak()

The N-Queens problem is a rather simple one: if you have a $N times N$ chess board (for fixed $N in NN$), can you place $N$ queens such that none of them are checking each other, or in other words, so that none of them are able to take each other.
We can visualise this as an $N times N$ matrix, where $1$ represents a queen being in a specific position, and $0$ represents an empty tile.
For example, the matrix
#align(center, block[$mat(
0, 1, 0, 0;
0, 0, 0, 1;
1, 0, 0, 0;
0, 0, 1, 0;
)$])
is a valid solution, as none of the $1$s can "attack" each other on a diagonal, horizontal, or vertical line (the queen's valid moves).
