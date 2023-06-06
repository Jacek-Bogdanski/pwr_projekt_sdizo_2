#include <iostream>
#include <vector>
#include <limits>

#include "src/ShortestPathReturn.cpp"
#include "src/GraphMatrix.h"
#include "src/GraphList.h"

#include "src/MenuHandler.cpp"


int main() {
//    SetConsoleOutputCP(CP_UTF8);

    // Autor: Jacek Bogdański, 263895

    MenuHandler menu;
    menu.mainLoop();
return 0;

    std::cout << "WERSJA MACIERZOWA" << std::endl;
    {
        // Wersja macierzowa
        // Tworzenie grafu o 5 wierzchołkach
        GraphMatrix graph(5);

        // Dodawanie krawędzi
        graph.addEdge(0, 1, 4);
        graph.addEdge(0, 2, 2);
        graph.addEdge(1, 3, 2);
        graph.addEdge(1, 4, 3);
        graph.addEdge(2, 1, 1);
        graph.addEdge(2, 3, 4);
        graph.addEdge(3, 4, 2);

        // Wyświetlanie macierzy sąsiedztwa
        graph.printMatrix();

        std::cout << std::endl;

        GraphMatrix primMST = graph.primMST();
        std::cout << "Minimalne drzewo rozpinające (algorytm Prima):" << std::endl;
        primMST.printMST();
        std::cout << "MST = " <<primMST.getEdgesSum()<<std::endl;

        std::cout << std::endl;

        int startVertex = 0;
        std::cout << "Najkrótsze drogi (algorytm Dijkstry) z wierzchołka " << startVertex << ":" << std::endl;
        ShortestPathReturn dijkstraReturn = graph.dijkstraShortestPath(startVertex);
        std::cout << "Start = "<<startVertex<<std::endl;
        std::cout << "End \tDist  \tPath"<<std::endl;
        for (int i = 0; i < dijkstraReturn.distances.size(); ++i) {
            std::cout << "| "<<i << " \t| " << dijkstraReturn.distances[i] <<" \t| "<<dijkstraReturn.paths[i]<<std::endl;
        }

        std::cout << std::endl;

    }

    std::cout << "WERSJA LISTOWA" << std::endl;

    {
        // Wersja listowa
        GraphList graph(5);

        // dodawanie krawędzi do grafu
        graph.addEdge(0, 1, 4);
        graph.addEdge(0, 2, 2);
        graph.addEdge(1, 3, 2);
        graph.addEdge(1, 4, 3);
        graph.addEdge(2, 1, 1);
        graph.addEdge(2, 3, 4);
        graph.addEdge(3, 4, 2);

        // Wyświetlanie listy sąsiedztwa
        graph.printList();

        std::cout << std::endl;

        std::cout << "Minimalne drzewo rozpinające (algorytm Prima):" << std::endl;
        GraphList primMST = graph.primMST();
        primMST.printList();
        std::cout << "MST = " <<primMST.getEdgesSum()<<std::endl;
        std::cout << std::endl;

        int startVertex = 0;

        std::cout << "Najkrótsze drogi z wierzchołka " << startVertex << " (algorytm Dijkstry):" << std::endl;
        ShortestPathReturn dijkstraReturn = graph.dijkstraShortestPath(startVertex);
        std::cout << "Start = "<<startVertex<<std::endl;
        std::cout << "End \tDist  \tPath"<<std::endl;
        for (int i = 0; i < dijkstraReturn.distances.size(); ++i) {
            std::cout << "| "<<i << " \t| " << dijkstraReturn.distances[i] <<" \t| "<<dijkstraReturn.paths[i]<<std::endl;
        }

        std::cout << std::endl;
    }


    return 0;
}