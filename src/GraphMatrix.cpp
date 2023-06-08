//
// Created by Jacek Bogdański
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <queue>
#include "GraphMatrix.h"

GraphMatrix::GraphMatrix(){
    verticlesNum = 0;
    // Inicjalizacja macierzy sąsiedztwa z wartościami infinity
    adjacencyMatrix.resize(verticlesNum, std::vector<int>(verticlesNum, std::numeric_limits<int>::max()));
}

/**
 * Konstruktor
 * @param verticlesNum liczba wierzchołków
 */
GraphMatrix::GraphMatrix(int verticlesNum) : verticlesNum(verticlesNum) {
    // Inicjalizacja macierzy sąsiedztwa z wartościami infinity
    adjacencyMatrix.resize(verticlesNum, std::vector<int>(verticlesNum, std::numeric_limits<int>::max()));
}

/**
 * Dodanie krawędzi grafu
 * @param start indeks wierzchołka początkowego
 * @param end indeks wierzchołka końcowego
 * @param weight waga krawędzi
 */
void GraphMatrix::addEdge(int start, int end, int weight) {
    // Sprawdź poprawność wierzchołków
    if (start >= 0 && start < verticlesNum && end >= 0 && end < verticlesNum) {
        // Ustaw wagę krawędzi w macierzy sąsiedztwa
        adjacencyMatrix[start][end] = weight;
    } else {
        std::cout << "Błędne indeksy wierzchołków!" << std::endl;
    }
}

/**
 * Wypisanie macierzy sąsiedztwa
 */
void GraphMatrix::printMatrix() {
    for (int i = 0; i < verticlesNum; i++) {
        for (int j = 0; j < verticlesNum; j++) {
            if (adjacencyMatrix[i][j] == std::numeric_limits<int>::max()) {
                std::cout << "   ∞ ";
            } else {
                std::cout << std::setw(4)<<adjacencyMatrix[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }
}

/**
 * Wypisanie listy MST
 */
void GraphMatrix::printMST() {
    std::cout << "Edge    Weight" << std::endl;
    for (int i = 0; i < verticlesNum; i++) {
        for (int j = 0; j < verticlesNum; j++) {
            if (adjacencyMatrix[i][j] != std::numeric_limits<int>::max()) {
                std::cout << "(" << i << ", " << j << ")  " << adjacencyMatrix[i][j] << std::endl;
            }
        }
    }
}

/**
 * Obliczenie sumy wag krawędzi
 * @return suma wag krawedzi
 */
int GraphMatrix::getEdgesSum() {
    int sum = 0;
    for (int i = 0; i < verticlesNum; i++) {
        for (int j = 0; j < verticlesNum; j++) {
            if (adjacencyMatrix[i][j] != std::numeric_limits<int>::max()) {
                sum += adjacencyMatrix[i][j];
            }
        }
    }

    return sum;
}

/**
 * Algorytm Prima do wyznaczania MST
 * @return GraphList graf MST
 */
GraphMatrix GraphMatrix::primMST() {
    GraphMatrix result(this->verticlesNum);
    if(verticlesNum < 1) return result;

    // Kolejka priorytetowa najkrótszych krawędzi
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> shortestEdges;

    // Odwiedzone wierzchołki
    std::vector<bool> visited(verticlesNum, false);

    // wierzcholek startowy
    int startVertex = 0;
    visited[startVertex] = true;

    // Dodanie do kolejki priorytetowej krawędzi wychodzących z aktualnego wierzchołka
    for (int i = 0; i < verticlesNum; i++) {
        if(startVertex == i) continue;
        if (adjacencyMatrix[startVertex][i] != std::numeric_limits<int>::max()) {
            shortestEdges.push(std::make_pair(adjacencyMatrix[startVertex][i], i));
        }
    }

    int prevVertex = startVertex;

    while (!shortestEdges.empty()) {
        int weight = shortestEdges.top().first;
        int currentVertex = shortestEdges.top().second;
        shortestEdges.pop();

        // kontynuuj, jesli odwiedzony
        if (visited[currentVertex])
            continue;

        result.addEdge(prevVertex, currentVertex, weight);
        visited[currentVertex] = true;

        // Dodaj do kolejki wszystkie krawędzie wychodzące z aktualnego wierzchołka
        for (int i = 0; i < verticlesNum; i++) {
            if(currentVertex == i) continue;
            if (!visited[i] && adjacencyMatrix[currentVertex][i] != std::numeric_limits<int>::max()) {
                shortestEdges.push(std::make_pair(adjacencyMatrix[currentVertex][i], i));
            }
            if (adjacencyMatrix[i][currentVertex] != std::numeric_limits<int>::max()) {
                shortestEdges.push(std::make_pair(adjacencyMatrix[i][currentVertex], i));
            }
        }

        prevVertex = currentVertex;
    }

    return result;
}

/**
 * Algorytm Dijkstry do wyznaczania najkrótszej drogi w grafie
 * @param start wierzchołek początkowy
 * @return ShortestPathReturn - lista wag dróg i lista ścieżek
 */
ShortestPathReturn GraphMatrix::dijkstraShortestPath(int start) {
    bool smaller = false;
    if(verticlesNum < 1){
        smaller = true;
        verticlesNum = 1;
    }
    // Odległości od wierzkolka startowego do pozostałych wierzchołków
    std::vector<int> distances(verticlesNum, std::numeric_limits<int>::max());
    // ścieżki
    std::vector<std::string> paths(verticlesNum, "");

    if(smaller){
        ShortestPathReturn out;
        out.distances = distances;
        out.paths = paths;

        return out;
    }

    distances[start] = 0;
    paths[start] = "0 ";

    std::priority_queue<std::tuple<int, int, std::string>, std::vector<std::tuple<int, int, std::string>>, std::greater<std::tuple<int, int, std::string>>> queue;
    queue.emplace(0, start, "0 ");

    while (!queue.empty()) {
        int currentVertex = std::get<1>(queue.top());
        int currentDistance = std::get<0>(queue.top());
        std::string currentPath = std::get<2>(queue.top());
        queue.pop();

        // zakonczenie, jesli obecny dystans jest wiekszy od zapisanego
        if (currentDistance > distances[currentVertex])
            continue;

        // przejście przez sąsiadów danego wierzchołka
        for (int neighbor = 0; neighbor < verticlesNum; ++neighbor) {
            int edgeWeight = adjacencyMatrix[currentVertex][neighbor];
            if (edgeWeight != std::numeric_limits<int>::max()) {
                int distanceThroughCurrent = currentDistance + edgeWeight;

                // jeśli znaleziono krótszą drogę, dodaj do kolejki i zapisz
                if (distanceThroughCurrent < distances[neighbor]) {
                    queue.emplace(distanceThroughCurrent, neighbor, currentPath + std::to_string(neighbor) + " ");
                    distances[neighbor] = distanceThroughCurrent;
                    paths[neighbor] = currentPath + std::to_string(neighbor) + " ";
                }
            }
        }
    }

    ShortestPathReturn out;
    out.distances = distances;
    out.paths = paths;

    for(int i=0;i<distances.size();i++){
        if(distances[i]<0){
            out.hasNegative = true;
        }
    }

    return out;
}

void GraphMatrix::resize(int n) {
    verticlesNum = n;
    adjacencyMatrix.clear();
    adjacencyMatrix.resize(verticlesNum, std::vector<int>(verticlesNum, std::numeric_limits<int>::max()));
}