//
// Created by Jacek Bogdański
//

#ifndef PWR_PROJEKT_SDIZO_2_GRAPHMATRIX_H
#define PWR_PROJEKT_SDIZO_2_GRAPHMATRIX_H

#include <iostream>
#include <vector>
#include <limits>
#include "ShortestPathReturn.h"

class GraphMatrix {
    private:
        /**
         * Liczba wierzchołków
         */
        int verticlesNum;

        /**
         * Macierz sąsiedztwa
         */
        std::vector<std::vector<int>> adjacencyMatrix;

    public:

        /**
         * Domyslny konstruktor
         */
        GraphMatrix();

        /**
         * Konstruktor
         * @param verticlesNum liczba wierzchołków
         */
        GraphMatrix(int verticlesNum);

        /**
         * Dodanie krawędzi grafu
         * @param start indeks wierzchołka początkowego
         * @param end indeks wierzchołka końcowego
         * @param weight waga krawędzi
         */
        void addEdge(int start, int end, int weight);

        /**
         * Wypisanie macierzy sąsiedztwa
         */
        void printMatrix();
        /**
         * Wypisanie listy MST
         */
        void printMST();

        /**
         * Obliczenie sumy wag krawędzi
         * @return suma wag krawedzi
         */
        int getEdgesSum();

        /**
         * Algorytm Prima do wyznaczania MST
         * @return GraphList graf MST
         */
        GraphMatrix primMST();

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
};


#endif //PWR_PROJEKT_SDIZO_2_GRAPHMATRIX_H
