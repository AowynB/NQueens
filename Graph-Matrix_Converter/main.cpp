//
// Created by aowyn on 10/10/25.
//
#include <iostream>
#include <string>
#include <vector>

struct node{
    char name;
    std::vector<node*> neighbors;
};

const int a = 'a';

static std::string matrix_to_graph(std::vector<std::vector<bool>>& matrix);
static std::vector<std::vector<bool>> graph_to_matrix(std::vector<node*>& graph);

int main(int argc, const char * argv[]) {
    std::cout << a << std::endl;
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

        while(getline(std::cin, line)){
        }
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

