//
// Created by Jacek Bogdański
//

#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include "GraphList.h"

GraphList::GraphList(){
    verticlesNum = 0;
    adjacencyList.resize(0);
}

/**
 * Konstruktor
 * @param verticlesNum liczba wierzchołków
 */
GraphList::GraphList(int verticlesNum) : verticlesNum(verticlesNum) {
    adjacencyList.resize(verticlesNum);
}

/**
 * Dodanie krawędzi grafu
 * @param start indeks wierzchołka początkowego
 * @param end indeks wierzchołka końcowego
 * @param weight waga krawędzi
 */
void GraphList::addEdge(int start, int end, int weight) {
    // sprawdzenie poprawnosci wierzchołków
    if (start < 0 || start >= verticlesNum || end < 0 || end >= verticlesNum) {
        std::cout << "niepoprawny indeks wierzcholka" << std::endl;
        return;
    }

    if(start == 0 && end == 0 && weight == 0){
        return;
    }

    adjacencyList[start].emplace_back(end, weight);
//    adjacencyList[end].emplace_back(start, weight);
}


/**
 * Wypisanie listy sąsiadów
 */
void GraphList::printList() {
    std::cout << "Edge    Weight" << std::endl;
    for (int i = 0; i < verticlesNum; i++) {
        for (const auto &neighbor: adjacencyList[i]) {
            std::cout << "(" << i << ", " << neighbor.first << ")  " << neighbor.second << std::endl;
        }
    }
}

/**
 * Obliczenie sumy wag krawędzi
 * @return suma wag krawedzi
 */
int GraphList::getEdgesSum() {
    int sum = 0;
    for (int i = 0; i < verticlesNum; i++) {
        for (const auto &neighbor: adjacencyList[i]) {
            sum += neighbor.second;
        }
    }
    return sum;
}

/**
 * Algorytm Prima do wyznaczania MST
 * @return GraphList graf MST
 */
GraphList GraphList::primMST() {
    GraphList result(this->verticlesNum);
    if(verticlesNum < 1) return result;

    // Kolejka priorytetowa najkrótszych krawędzi
    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<std::tuple<int, int, int>>> shortestEdges;

    // Odwiedzone wierzchołki
    std::vector<bool> visited(verticlesNum, false);

    // wierzcholek startowy
    int startVertex = 0;
    visited[startVertex] = true;

// Dodanie kolejki priorytetowej krawędzi wychodzących z wierzchołka startowego
    for(int i=0;i<adjacencyList[startVertex].size();i++){
        int w = adjacencyList[startVertex][i].second;
        int dest = adjacencyList[startVertex][i].first;
        shortestEdges.push(std::make_tuple(w, startVertex, dest));
    }

    while (!shortestEdges.empty()) {
        int weight = std::get<0>(shortestEdges.top());
        int prevVertex = std::get<1>(shortestEdges.top());
        int currentVertex = std::get<2>(shortestEdges.top());

        shortestEdges.pop();

        // kontynuuj, jesli odwiedzony
        if (visited[currentVertex])
            continue;

        visited[currentVertex] = true;
        result.addEdge(prevVertex, currentVertex, weight);

        // Dodanie do kolejki priorytetowej krawędzi wychodzących z aktualnego wierzchołka
        for(int i=0;i<adjacencyList[currentVertex].size();i++){
            int w = adjacencyList[currentVertex][i].second;
            int dest = adjacencyList[currentVertex][i].first;
            shortestEdges.push(std::make_tuple(w, currentVertex, dest));
        }
        for(int k=0;k<verticlesNum;k++){
            if(k==currentVertex) continue;
            if(k==prevVertex) continue;

            for(int i=0;i<adjacencyList[k].size();i++){
                if(adjacencyList[k][i].first != currentVertex) continue;
                int w = adjacencyList[k][i].second;
                int dest = adjacencyList[k][i].first;
                shortestEdges.push(std::make_tuple(w, dest, k));
            };
        }
    }

    return result;
}

/**
 * Algorytm Dijkstry do wyznaczania najkrótszej drogi w grafie
 * @param start wierzchołek początkowy
 * @return ShortestPathReturn - lista wag dróg i lista ścieżek
 */
ShortestPathReturn GraphList::dijkstraShortestPath(int start) {
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

        for(int i=0;i<adjacencyList[currentVertex].size();i++) {
            int neighborVertex = adjacencyList[currentVertex][i].first;
            int edgeWeight = adjacencyList[currentVertex][i].second;
            int distanceThroughCurrent = currentDistance + edgeWeight;

            // jeśli znaleziono krótszą drogę, dodaj do kolejki i zapisz
            if (distanceThroughCurrent < distances[neighborVertex]) {
                queue.emplace(distanceThroughCurrent, neighborVertex,
                              currentPath + std::to_string(neighborVertex) + " ");
                distances[neighborVertex] = distanceThroughCurrent;
                paths[neighborVertex] = currentPath + std::to_string(neighborVertex) + " ";
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

void GraphList::resize(int n){
    verticlesNum = n;
    adjacencyList.clear();
    adjacencyList.resize(n);
}