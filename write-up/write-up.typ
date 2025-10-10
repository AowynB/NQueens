#import "@preview/diagraph:0.3.6": raw-render
#show link: underline


#align(center, block[#set text(17pt); *The N-Queens Problem as a Digraph*])
#linebreak()

The N-Queens problem is a rather simple one: if you have a $N times N$ chess board (for fixed $N in NN$), can you place $N$ queens such that none of them are checking each other, or in other words, so that none of them are able to take each other.
A more detailed description is given at #link("https://en.wikipedia.org/wiki/N_queens").
We can visualise this as an $N times N$ matrix, where $1$ represents a queen being in a specific position, and $0$ represents an empty tile.
For example, the matrix
#align(center, block[$M := mat(
0, 1, 0, 0;
0, 0, 0, 1;
1, 0, 0, 0;
0, 0, 1, 0;
)$])
is a valid solution, as none of the $1$s can "attack" each other on a diagonal, horizontal, or vertical line (the queen's valid moves).
Notice that this could form a digraph, where the top of the matrix is labelled $a_0, a_1, ..., a_N$, and the side $b_0, b_1, ..., b_N$, and an edge from $a_n$ to $b_m$ (with $n,m in [0, N] inter NN_0$) existing iff $M_(m,n) = 1$ (where $M$ is the matrix in question).
Thus, our matrix above would form the digraph
#align(center, block[#raw-render(```dot digraph {
a->c
b->a
c->d
d->b
}```)])
Notice the cycle in the digraph.
When we checked the first 8 solutions computationally, we found that _all_ of the corresponding digraphs had similar cycles (the code is available in the Github repository #link("https://github.com/AowynB/NQueens"), along with the source for this document, and a copy of the PDF).
Some of them -- for example, at $N = 7$ -- had multiple small cycles, the matrix and digraph for which is shown below.
#align(center, block[$mat(
1,0,0,0,0,0,0;
0,0,1,0,0,0,0;
0,0,0,0,1,0,0;
0,0,0,0,0,0,1;
0,1,0,0,0,0,0;
0,0,0,1,0,0,0;
0,0,0,0,0,1,0;
)
#raw-render(```dot digraph {
a->a
b->c
c->e
e->b
g->f
f->d
d->g
}```)$])
