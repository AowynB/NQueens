//
// Created by Aowyn on 10/10/25.
//
#include <iostream>
#include <cstring>
#include <regex>
#include <string>
#include <vector>

/* represents a single node in a one way graph, with pointers to connected noded */
struct node{
    int label; //node identifier
    std::vector<node*> neighbors; //connected nodes
};

constexpr int a = 'a'; //the ascii value of lowercase a used for displaying graph nodes as alphabetical characters

/* CONVERSION FUNCTIONS */
static std::vector<node*> matrix_to_graph(const std::vector<std::vector<bool>>& matrix);
static std::vector<std::vector<bool>> graph_to_matrix(const std::vector<node*>& graph);

/* VERIFICATION FUNCTIONS */
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
 * After this if the program is set to accept a matrix it accepts it as a series of n lines of size n consisting of
 * ones and zeroes and saves it to a 2d boolean vector in matrix(the local variable) and calls matrix_to_graph(matrix)
 * and stores the result in graph(the local variable).
 *
 * if the program is set to accept a graph, it initializes graph to hold n labels in alphabetical order then
 * accepts a series of strings in the form [label1]->[label2] and stores a pointer to graph[label 2] in the
 * neighbors vector of graph[label2]. it then calls graph_to_matrix(graph) and stores the resulting matrix in matrix
 *
 * OUTPUT:
 * the program then outputs all requested output information.
 *
 * matrices are output as a series of n strings of size n as ones and zeroes.
 * graphs are output as a series of [label1]->[label2] strings each detailing a pointer from graph[label1] to graph[label2]
 *
 * the validity of the graph is output by printing the return value of validate_Nqueens(matrix, graph)
 *
 * @param argc the number of arguments passed to the program
 * @param argv a list of arguments argv[0] is the program name
 * @return 1 if there are not enough arguments, 2 if too many input arguments are provided, 0 if all has gone correctly
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
        return 2;
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
                if(line[j] != '0' && line[j] != '1') {
                    std::cerr << "Invalid Input: " << std::to_string(line[j]) << "is not a boolean value" << std::endl;
                    return 3;
                }
                matrix[i][j] = line[j] - '0';
            }
        }

        graph = matrix_to_graph(matrix);
    }
    else if(InputFlags[1]){
        const auto LABELS_REGEX = std::regex("[0-9]+");

        graph = std::vector<node*>(n);
        for(int i = 0; i < n; i++){
            graph[i] = new node(i, std::vector<node*>());
        }

        while(true){
            std::cin >> line;
            if(line == "END"){ break; }
            if (line.find("->") == std::string::npos) {
                std::cerr << "Invalid Input: graph input must be of the form '[label1]->[label2]'" << std::endl;
                return 3;
            }

            int indexes[2];
            int count = 0;
            for (std::smatch match; std::regex_search(line, match, LABELS_REGEX);) {
                indexes[count] = std::stoi(match.str());
                count++;
                line = match.suffix();
            }

            graph[indexes[0]]->neighbors.push_back(graph[indexes[1]]);
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
                std::cout << graph[i]->label << "->" << room->label << std::endl;
            }
        }
    }
    if(OutputFlags[2]){
        std::cout << validate_Nqueens(matrix, graph) << std::endl;
    }

    for(const auto node : graph){ delete node; }
    return 0;
}

/**
 * matrix_to_graph(matrix) takes a matrix of true false values and converts it into a graph represented by a set of
 * nodes and one way connections. The graph is returned in the form of a vector of node pointers each containing a
 * char representing the label of a matrix row, the label of each node neighbors points to is the column on that row
 * where a value of true exists in the original matrix.
 * @param matrix a 2d vector of boolean values representing a matrix of boolean values
 * @return a vector of node pointers representing a graph
 */
static std::vector<node*> matrix_to_graph(const std::vector<std::vector<bool>>& matrix){
    //initialize graph with n values each representing and column
    std::vector<node*> graph(matrix.size());
    for(int i = 0; i < matrix.size(); i++){
        graph[i] = new node(i, std::vector<node*>());
    }

    //for each row and column in matrix
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[i].size(); j++){
            //if a queen is at the intersection
            if(matrix[i][j] == true){
                //add the location of the queen to graph
                graph[i]->neighbors.push_back(graph[j]);
            }
        }
    }

    return graph;
}


/**
 * graph_to_matrix(graph) takes a graph in the form of a vector of node pointers and represents it as a matrix of boolean
 * values where the true value represents a point where the node representing that row points to a node representing
 * that column.
 * @param graph a vector of node pointers representing a graph
 * @return a 2d vector of boolean values representing a matrix of boolean values
 */
static std::vector<std::vector<bool>> graph_to_matrix(const std::vector<node*>& graph){
    //initialize an empty n by n 2d boolean matrix storing the graph
    std::vector<std::vector<bool>> matrix(graph.size(), std::vector<bool>(graph.size()));

    //for each row/column
    for(const auto vertex : graph){
        //get its row/column index
        int x = vertex->label;
        //for each queen in its row
        for(const auto & neighbor : vertex->neighbors){
            //add it to the matrix in the correct position
            int y = neighbor->label;
            matrix[x][y] = true;
        }
    }
    return matrix;
}

/**
 * validate_Nqueens(matrix, graph) accepts a 2d matrix of boolean values, and it's corresponding graph and determines
 * weather or not they satisfy the constraints of the Nqueens problem: each true values is placed in such a way that it
 * does not share a diagonal, column, or row, with any other true values.
 * @param matrix a 2d vector of boolean values representing a matrix of boolean values
 * @param graph a vector of node pointers representing a graph
 * @return weather or not the given matrix and graph satisfy Nqueens
 */
static bool validate_Nqueens(const std::vector<std::vector<bool>>& matrix, const std::vector<node*>& graph){
    //declare an array to hold the number of pointers to each node
    int columnCounts[graph.size()];
    //for each node
    for(const auto node : graph){
        //if the row that node represents contains more than one queen
        if(node->neighbors.size() > 1) {
            //this graph is not a valid Nqueens solution
            return false;
        }
        //if the row has only one queen, add to the count for its column
        columnCounts[node->neighbors[0]->label] += 1;
    }
    //for each column
    for (const int count: columnCounts) {
        //if that coumn has more than one queen
        if (count > 1) {
            //this graph is not a valid Nqueens solution
            return false;
        }
    }

    //for every queen in row order
    for(int i = 0; i < matrix.size(); i++){
        //store the starting points for checking it's diagonals
        int x1= graph[i]->label + 1;
        int x2= graph[i]->label - 1;
        int y2;
        int y1 = y2 = graph[i]->neighbors[0]->label + 1;

        //for each value in the down-right diagonal
        while (x1 < matrix.size() && y1 < matrix.size()){
            //
            if(matrix[x1][y1] == true){
                return false;
            }
            x1++;
            y1++;
        }

        while (x2 >= 0 && y2 < matrix.size()){
            if(matrix[x2][y2] == true){
                return false;
            }
            x2--;
            y2++;
        }
    }
    return true;
}

