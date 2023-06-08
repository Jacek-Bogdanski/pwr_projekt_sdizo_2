//
// Created by Jacek Bogdański
//

#ifndef PWR_PROJEKT_SDIZO_2_GRAPHLIST_H
#define PWR_PROJEKT_SDIZO_2_GRAPHLIST_H

#include <iostream>
#include <vector>
#include <limits>
#include "ShortestPathReturn.h"

class GraphList {
    private:
        /**
         * Liczba wierzchołków
         */
        int verticlesNum;

        /**
         * Lista sąsiedztwa
         */
        std::vector<std::vector<std::pair<int, int>>> adjacencyList;

    public:
        /**
        * Domyslny konstruktor
        */
        GraphList();
        /**
        * Konstruktor
        * @param verticlesNum liczba wierzchołków
        */
        GraphList(int verticlesNum);

        /**
         * Dodanie krawędzi grafu
         * @param start indeks wierzchołka początkowego
         * @param end indeks wierzchołka końcowego
         * @param weight waga krawędzi
         */
        void addEdge(int start, int end, int weight);

        /**
         * Wypisanie listy sąsiadów
         */
        void printList();

        /**
         * Obliczenie sumy wag krawędzi
         * @return suma wag krawedzi
         */
        int getEdgesSum();
        /**
         * Algorytm Prima do wyznaczania MST
         * @return GraphList graf MST
         */
        GraphList primMST();

        /**
         * Algorytm Dijkstry do wyznaczania najkrótszej drogi w grafie
         * @param start wierzchołek początkowy
         * @return ShortestPathReturn - lista wag dróg i lista ścieżek
         */
        ShortestPathReturn dijkstraShortestPath(int start);

        /**
         * Metoda czyszczaca dane grafu i ustawiajaca nowy rozmiar
         * @param verticlesNum
         */
        void resize(int verticlesNum);

        /**
        * Usuwa krawedzie wychodzace z danego wierzcholka
        * @param start
        */
        void removeEdges(int start);
};


#endif //PWR_PROJEKT_SDIZO_2_GRAPHLIST_H
