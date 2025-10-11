//
// Created by aowyn on 10/10/25.
//
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

/* represents a single node in a one way graph, with pointers to connected noded */
struct node{
    char name; //node identifier
    std::vector<node*> neighbors; //connected nodes
};

constexpr int a = 'a'; //the ascii value of lowercase a used for displaying graph nodes as alphabetical characters

/* CONVERSION FUNCTIONS */
/**
 * matrix_to_graph(matrix) takes a matrix of true false values and converts it into a graph represented by a set of
 * nodes and one way connections. The graph is returned in the form of a vector of node pointers each containing a
 * char representing the label of a matrix row, the label of each node neighbors points to is the columb on that row
 * where a value of true exists in the original matrix.
 * @param matrix a 2d vector of boolean values representing a matrix of boolean values
 * @return a vector of node pointers representing a graph
 */
static std::vector<node*> matrix_to_graph(const std::vector<std::vector<bool>>& matrix);

/**
 * graph_to_matrix(graph) takes a graph in the form of a vector of node pointers and represents it as a matrix of boolean
 * values where the true value represents a point where the node representing that row points to a node representing
 * that column.
 * @param graph a vector of node pointers representing a graph
 * @return a 2d vector of boolean values representing a matrix of boolean values
 */
static std::vector<std::vector<bool>> graph_to_matrix(const std::vector<node*>& graph);

/* VERIFICATION FUNCTIONS */
/**
 * validate_Nqueens(matrix, graph) accepts a 2d matrix of boolean values, and it's corresponding graph and determines
 * weather or not they satisfy the constraints of the Nqueens problem: each true values is placed in such a way that it
 * does not share a diagonal, column, or row, with any other true values.
 * @param matrix a 2d vector of boolean values representing a matrix of boolean values
 * @param graph a vector of node pointers representing a graph
 * @return weather or not the given matrix and graph satisfy Nqueens
 */
static bool validate_Nqueens(const std::vector<std::vector<bool>>& matrix, const std::vector<node*>& graph);

/* MAIN FUNCTION */
/**
 * The main function accepts a number of arguments that determine how the program accepts input and what output it gives
 * Input is accepted in the form of two strings of lowercase letters and at least one input and one output argument must
 * be provided.
 * The first string determines what form of input is accepted(only one option is allowed):
 *      - m indicates that the program should accept a matrix.
 *      - g indicated that the program should accept a graph.
 *
 * The second string determines what output the program should give(multiple options may be selected):
 *      - m indicates that the program should output the matrix form of the input.
 *      - g indicated that the program should output the graph form of the input.
 *      - v indicates that the program should output weather or not the input is valid for the Nqueens problem
 *
 * The program uses two arrays of boolean flags to store input choices for ease of access.
 *
 * INPUT:
 * The program first accepts an integer value n which is the size of the matrix and the number of available nodes for
 * the graph.
 *
 * The function accepts
 *
 * @param argc
 * @param argv
 * @return
 */
int main(const int argc, const char * argv[]) {
    if(argc < 3){
        std::cerr << "ERROR: Not enough Arguments Provided\n";
        return 1;
    }

    bool InputFlags[2] = {false, false};
    bool OutputFlags[3] = {false, false, false};
    int n;
    std::string line;
    std::vector<std::vector<bool>> matrix;
    std::vector<node*> graph;

    for(int i = 0; i < strlen(argv[1]); i++){
        if(argv[1][i] == 'm'){ InputFlags[0] = true; }
        if(argv[1][i] == 'g'){ InputFlags[1] = true; }
    }
    if(InputFlags[0] && InputFlags[1]){
        std::cerr << "ERROR: Too many input arguments provided\n";
        return 1;
    }

    for(int i = 0; i < strlen(argv[2]); i++){
        if(argv[2][i] == 'm'){ OutputFlags[0] = true; }
        if(argv[2][i] == 'g'){ OutputFlags[1] = true; }
        if(argv[2][i] == 'v'){ OutputFlags[2] = true; }
    }

    std::cin >> n;
    if(InputFlags[0]){
        matrix = std::vector<std::vector<bool>>(n, std::vector<bool>(n));
        for(int i = 0; i < n; i++){
            std::cin >> line;
            for(int j = 0; j < n; j++){
                matrix[i][j] = line[j] - '0';
            }
        }

        graph = matrix_to_graph(matrix);
    }
    else if(InputFlags[1]){
        graph = std::vector<node*>(n);
        for(int i = 0; i < n; i++){
            graph[i] = new node(char(a + i), std::vector<node*>());
        }

        while(true){
            std::cin >> line;
            if(line == "END"){ break; }

            graph[line[0] - a]->neighbors.push_back(graph[line[3] - a]);
        }

        matrix = graph_to_matrix(graph);
    }

    if(OutputFlags[0]){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                std::cout << matrix[i][j];
            }
            std::cout << std::endl;
        }
    }
    if(OutputFlags[1]){
        for(int i = 0; i < n; i++){
            for(auto room : graph[i]->neighbors){
                std::cout << graph[i]->name << "->" << room->name << std::endl;
            }
        }
    }
    if(OutputFlags[2]){
        std::cout << validate_Nqueens(matrix, graph) << std::endl;
    }

    for(const auto node : graph){ delete node; }
    return 0;
}

static std::vector<node*> matrix_to_graph(const std::vector<std::vector<bool>>& matrix){
    std::vector<node*> graph(matrix.size());
    for(int i = 0; i < matrix.size(); i++){
        graph[i] = new node(char(a + i), std::vector<node*>());
    }

    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[i].size(); j++){
            if(matrix[i][j] == true){
                graph[i]->neighbors.push_back(graph[j]);
            }
        }
    }

    return graph;
}

static std::vector<std::vector<bool>> graph_to_matrix(const std::vector<node*>& graph){
    int n = graph.size();
    std::vector<std::vector<bool>> matrix(n, std::vector<bool>(n));

    for(int i = 0; i < n; i++){
        int x = graph[i]->name - a;
        for(const auto & neighbor : graph[i]->neighbors){
            int y = neighbor->name - a;
            matrix[x][y] = true;
        }
    }
    return matrix;
}

static bool validate_Nqueens(const std::vector<std::vector<bool>>& matrix, const std::vector<node*>& graph){
    for(const auto node : graph){
        if(node->neighbors.size() > 1){
            return false;
        }
    }
    for(int i = 0; i < matrix.size(); i++){
        int x1= graph[i]->name - a + 1;
        int x2= graph[i]->name - a - 1;
        int y2;
        int y1 = y2 = graph[i]->neighbors[0]->name - a + 1;

        while (x1 < matrix.size() && y1 < matrix.size()){
            if(matrix[x1][y1] == true){
                return false;
            }
            x1++;
            y1++;
        }

        while (x2 >= 0 && y2 < matrix.size()){
            if(matrix[x2][y2] == true){
                std::cout << graph[i]->name << std::endl;
                std::cout << x2 << " " << y2 << std::endl;
                return false;
            }
            x2--;
            y2++;
        }
    }
    return true;
}

