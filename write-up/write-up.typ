#import "@preview/diagraph:0.3.6": raw-render
#show link: underline
#set heading(numbering: "1.")

#align(center)[
    #set text(17pt); *TODO: Figure Out a Good Title*
]

#grid(
    columns: (1fr, 1fr),
    align(center)[Aowyn Brook],
    align(center)[Cam Ayres]
)

#linebreak()
#align(center)[
    #set par(justify: false)
    *Abstract*
    #lorem(80)
]


#pagebreak()
#outline()
#pagebreak()


= Introduction
To be clear, we will be defining $NN = {1, 2, 3, ...}$ in this write up, as apposed to $NN = {0, 1, 2, ...}$.

The N-Queens problem is a rather simple one: if you have a $N times N$ chess board (for fixed $N in NN$), can you place $N$ queens such that none of them are checking each other, or in other words, so that none of them are able to take each other.
A more detailed description is given at #link("https://en.wikipedia.org/wiki/N_queens").
We can visualise this as an $N times N$ matrix, where $1$ represents a queen being in a specific position, and $0$ represents an empty tile.
For example, the matrix
#align(center)[
    $M := mat(
    0,1,0,0;
    0,0,0,1;
    1,0,0,0;
    0,0,1,0;
)$]
is a valid solution, as none of the $1$s can "attack" each other on a diagonal, horizontal, or vertical line (the queen's valid moves).
Notice that this could form a digraph, where the top of the matrix is labelled $a_0, a_1, ..., a_N$, and the side $b_0, b_1, ..., b_N$, and an edge from $a_n$ to $b_m$ (with $n,m in {0, 1, ..., N}$) existing iff $M_(m,n) = 1$ (where $M$ is the matrix in question).
Thus, our matrix above would form the digraph
#figure(caption: [One of the two solutions for N=4],
align(center)[#raw-render(```dot digraph {
a_0->a_2
a_1->a_0
a_2->a_3
a_3->a_1
}```)]) <N4Sol1>
Notice the cycle in the digraph.
It is also important to note that there may exist multiple solutions to a given $N$.
When we checked the first 8 solutions computationally, we found that _all_ of the corresponding digraphs had similar cycles (the code is available in the Github repository #link("https://github.com/AowynB/NQueens"), along with the source for this document, and a copy of the PDF).
Some of them -- for example, at $N = 7$ -- had multiple small cycles, the matrix and digraph for which is shown below.
#align(center)[
    $mat(
    1,0,0,0,0,0,0;
    0,0,1,0,0,0,0;
    0,0,0,0,1,0,0;
    0,0,0,0,0,0,1;
    0,1,0,0,0,0,0;
    0,0,0,1,0,0,0;
    0,0,0,0,0,1,0;
    )
#raw-render(```dot digraph {
a_0->a_0
a_1->a_2
a_2->a_4
a_4->a_1
a_6->a_5
a_5->a_3
a_3->a_6
}```
)$]

We seek to show that this pattern of cyclic digraphs holds in general iff the matrix exists.
Note there are cases where no solution is possible, namely $N = 2 "and" N = 3$.
First, we need to understand what the digraph is representing about the board.
[Still needs to be filled in.]

= The Algorithm
== A Method For Storing Solutions
[Prove that the solutions always form the cyclic pattern, shouldn't be too hard]
If we were to store the full matrix, we'd be using $Theta(N^2)$ bits, which is very bad.
Thus, we cunningly use a better method, bringing this down to $Theta(N log_2(N))$ bits: adjacency lists.
Since each vertex has exactly $1$ edge going from, and exactly $1$ going to, we notice that each vertex in the adjacency list has exactly one vertex it points to.
Using the graph in @N4Sol1, we would write (with newlines and colons added for human readability, the machine shall not be given such luxuries) $ 00000010\ 00:01\ 01:11\ 10:00\ 11:10 $.
The first eight bits represent the number of nodes that we have -- for $N = 4$, we have four vertices, which we can represent in two bits.
Then, on each line, we have "vertex:linked-vertex".

We now prove that it takes $log_2(N) + 2N log_2(N) in Theta(N log_2(N))$ space (it's $8 + N log_2(N)$ in our actual program, since that's easier for the computer).
Notice that the number of bits required to represent a number $N in NN$ is $log_2(N)$.
Furthermore, we have $2N$ of these in our adjacency list.
We multiply, and hence have $log_2(N) + 2N log_2(N) in Theta(N log_2(N))$ bits required.
As said previously, the first $log_2(N)$ will likely be set to $8$, though for large enough $N$ (I.E., $N > 256$, which we are unlikely to successfully compute), this will no longer work, and another constant is needed.

It's worth noting that this'll add more overhead when getting solutions out of cold storage.
There are other methods for storage that we found and discussed, which #emph[may] be used in other parts of the program, say, while computing solutions.

== A Simplified Model
Much of the guts our algorithm was inspired by a wonderful paper by Richards @AlgorithmIdea.
Please note that this is a rough outline of the algorithm; later, we'll describe a more efficient approach.
When computing valid solutions to N-Queens, we start with an empty board, and then place down all possible Queen positions on the first row across $N$ different matrices.
This gives us $N$ matrices that we're currently working on.
Now, we repeat the process, starting on the next row, for each of these matrices, creating $N$ copies of it, and then placing a Queen in each spot on the row.
We now have $N^2 + N$ matrices.
However, we can cut this down substantially by pruning all solutions that form an invalid solution, that is, the Queens can take each other.
Due to this pruning, we'll have strictly less than $N^2 + N$ matrices.
We repeat this process until we have explored all possible routes through this tree.
Noting that the $N$-th level of our tree has $N^N$ vertices, it follows that to reach the $N$-th level, it'll take $Sigma_(i=0)^N N^i$ space.
Thus, we have $Omicron (Sigma_(i=0)^N N^i)$ as an upper bound to our space complexity (we will improve on this estimate later, once the algorithm is shown in all its glory).

Now, assume that each new level of the tree to take $Theta(1)$ time to compute (I.E., we have unlimited cores, and we are using $N$ of them to compute the $N$-th level).
Then, to reach the $N$-th level, it will take us $Sigma_(i=0)^N 1$ iterations, which gives us an asymptotic time complexity of $Theta(N)$.
We can trivially generalise this to a machine that can compute $c in NN$ computations simultaneously, to $Theta(ceil.l c/N ceil.r)$.
Notice that for $N <= c$, we still have $Theta(N)$.

In the real world, the time complexity will be worse, as we do not have this idealised computer, and thus must put an upper bound the number of simultaneous processes.
However, the space complexity will be better, since we can apply a set of "clever tricks" to cut down on the number of possible things we need to check, though it will still be bounded by the space usage.

== The Full Beast
One of the main tricks applied by our algorithm requires a small explanation.

#pagebreak()
#bibliography("sources.bib")
