//
// Created by aowyn on 10/10/25.
//
#include <iostream>
#include <string>
#include <vector>

/* represents a single node in a one way graph, with pointers to connected noded */
struct node{
    char name; //node identifier
    std::vector<node*> neighbors; //connected nodes
};

const int a = 'a'; //the ascii value of lowercase a, used for displaying graph nodes as alphabetical characters

/* Declare available functions */
static std::string matrix_to_graph(std::vector<std::vector<bool>>& matrix);
static std::vector<std::vector<bool>> graph_to_matrix(std::vector<node*>& graph);

int main(int argc, const char * argv[]) {
    if(argc < 2){
        std::cerr << "ERROR: No Arguments Provided\n";
        return 1;
    }

    int n;
    std::string line;

    if(argv[1][0] == 'm'){
        std::cin >> n;
        std::vector<std::vector<bool>> matrix(n, std::vector<bool>(n));
        for(int i = 0; i < n; i++){
            std::cin >> line;
            for(int j = 0; j < n; j++){
                matrix[i][j] = line[j] - '0';
            }
        }
        std::cout << matrix_to_graph(matrix);
        return 0;
    }
    else if(argv[1][0] == 'g'){
        std::cin >> n;
        std::vector<node*> graph(n);
        for(int i = 0; i < n; i++){
            graph[i] = new node(char(a + i), std::vector<node*>());
        }

        while(1){
            std::cin >> line;
            if(line == "END"){
                break;
            }

            graph[line[0] - a]->neighbors.push_back(graph[line[3] - a]);
        }

        std::vector<std::vector<bool>> matrix = graph_to_matrix(graph);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                std::cout << matrix[i][j];
            }
            std::cout << std::endl;
        }

        for(auto node : graph){ delete node; }
        return 0;
    }
}

static std::string matrix_to_graph(std::vector<std::vector<bool>>& matrix){
    std::string graph;
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[i].size(); j++){
            if(matrix[i][j] == true){
                graph += char(a + i);
                graph += "->";
                graph += char(a + j);
                graph += "\n";
            }
        }
    }
    return graph;
}

static std::vector<std::vector<bool>> graph_to_matrix(std::vector<node*>& graph){
    int n = graph.size();
    std::vector<std::vector<bool>> matrix(n, std::vector<bool>(n));

    for(int i = 0; i < n; i++){
        int x = graph[i]->name - a;
        for(int j = 0; j < graph[i]->neighbors.size(); j++){
            int y = graph[i]->neighbors[j]->name - a;
            matrix[x][y] = true;
        }
    }
    return matrix;
}

