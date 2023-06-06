//
// Created by Jacek Bogdański
//

#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include "GraphList.h"

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

    adjacencyList[start].emplace_back(end, weight);
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

    // Kolejka priorytetowa najkrótszych krawędzi
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> shortestEdges;

    // Odwiedzone wierzchołki
    std::vector<bool> visited(verticlesNum, false);

    // wierzcholek startowy
    int startVertex = 0;
    visited[startVertex] = true;

    // Dodanie kolejki priorytetowej krawędzi wychodzących z wierzchołka startowego
    for (const auto &neighbor: adjacencyList[startVertex]) {
        int dest = neighbor.first;
        int w = neighbor.second;
        shortestEdges.emplace(w, dest);
    }

    int prevVertex = startVertex;

    while (!shortestEdges.empty()) {
        int weight = shortestEdges.top().first;
        int currentVertex = shortestEdges.top().second;
        shortestEdges.pop();

        // kontynuuj, jesli odwiedzony
        if (visited[currentVertex])
            continue;

        visited[currentVertex] = true;
        result.addEdge(prevVertex, currentVertex, weight);

        // Dodanie do kolejki priorytetowej krawędzi wychodzących z aktualnego wierzchołka
        for (const auto &neighbor: adjacencyList[currentVertex]) {
            int destination = neighbor.first;
            int edgeWeight = neighbor.second;
            shortestEdges.emplace(edgeWeight, destination);
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
ShortestPathReturn GraphList::dijkstraShortestPath(int start) {
    // Odległości od wierzkolka startowego do pozostałych wierzchołków
    std::vector<int> distances(verticlesNum, std::numeric_limits<int>::max());

    // ścieżki
    std::vector<std::string> paths(verticlesNum, "");

    distances[start] = 0;
    paths[start] = "0 ";

    std::priority_queue<std::tuple<int, int, std::string>, std::vector<std::tuple<int, int, std::string>>, std::greater<std::tuple<int, int, std::string>>> queue;
    queue.emplace(0, start, "0 ");
    printf("step 31\n");
    while (!queue.empty()) {
        int currentVertex = std::get<0>(queue.top());
        int currentDistance = std::get<1>(queue.top());
        std::string currentPath = std::get<2>(queue.top());
        queue.pop();

        // zakonczenie, jesli obecny dystans jest wiekszy od zapisanego
        if (currentDistance > distances[currentVertex])
            continue;
        printf("step 32\n");
        // przejście przez sąsiadów danego wierzchołka
        for (const auto &neighbor: adjacencyList[currentVertex]) {
            printf("step 33 - %d, %d\n",neighbor.first,neighbor.second);
            int neighborVertex = neighbor.first;
            int edgeWeight = neighbor.second;
            int distanceThroughCurrent = currentDistance + edgeWeight;
            printf("step 34\n");

            // jeśli znaleziono krótszą drogę, dodaj do kolejki i zapisz
            if (distanceThroughCurrent < distances[neighborVertex]) {
                printf("step 341\n");
                queue.emplace(distanceThroughCurrent, neighborVertex,
                              currentPath + std::to_string(neighborVertex) + " ");
                printf("step 342\n");
                distances[neighborVertex] = distanceThroughCurrent;
                printf("step 343\n");
                paths[neighborVertex] = currentPath + std::to_string(neighborVertex) + " ";
                printf("step 344\n");
            }
            printf("step 345\n");
        }
    }

    ShortestPathReturn out;
    out.distances = distances;
    out.paths = paths;

    return out;
}
