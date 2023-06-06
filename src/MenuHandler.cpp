//
// Created by Jacek Bogdański
//

#include <cstdio>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <vector>
#include "GraphList.h"
#include "GraphMatrix.h"

//#include <conio.h>
//#include <Windows.h>

using namespace std;

class MenuHandler {

public:
    MenuHandler(){
        graph_matrix = new GraphMatrix (0);
        graph_list = new GraphList(0);
    };

    ~MenuHandler(){
        delete graph_matrix;
        delete graph_list;
    };

private:
    string file_name = "input.txt";

    int edgesNum = 0;
    int verticlesNum = 0;
    int startVertex = 0;
    int endVertex = 0;


    GraphMatrix * graph_matrix;
    GraphList * graph_list;

    vector<tuple<int, int, int>> test_data;

    long long int frequency, start, elapsed;



    /**
     * Sprawdzenie, czy plik istnieje
     */
    bool fileExists(std::string filename) {
        std::ifstream file(filename.c_str());
        if (file) {
            return true;
        }
        return false;
    }

    /**
     * Ładowanie danych z pliku
     */
    void readTestData() {
        std::ifstream file(file_name.c_str());
        edgesNum = 0;
        verticlesNum = 0;
        startVertex = 0;
        endVertex = 0;
        if (file.is_open()) {
            file >> edgesNum;
            if (file.fail()) {
                cout << "file error" << endl;
            } else {
                file >> verticlesNum;
                file >> startVertex;
                file >> endVertex;

                test_data.clear();
                test_data.resize(verticlesNum);

                int a, b, c;
                for (int i = 0; i < edgesNum; i++) {
                    file >> a;
                    file >> b;
                    file >> c;
                    if (file.fail()) {
                        cout << "file read error" << endl;
                        break;
                    } else {
                        test_data.emplace_back(a, b, c);
                    }
                }
            }
            file.close();

            delete graph_matrix;
            delete graph_list;
            graph_matrix = new GraphMatrix(verticlesNum);
            graph_list = new GraphList(verticlesNum);
            for (auto row: test_data) {
                graph_matrix->addEdge(get<0>(row), get<1>(row), get<2>(row));
                graph_list->addEdge(get<0>(row), get<1>(row), get<2>(row));
            }
        } else {
            cout << "file open error" << endl;
        }
    }



    /**
     * Pobranie czasu systemowego, źródło: antoni.sterna.staff.iiar.pwr.edu.pl
     */
    /*
   //-------------------------------------------------------------------------
       long long int read_QPC() {
           LARGE_INTEGER count;
           QueryPerformanceCounter(&count);
           return ((long long int) count.QuadPart);
       }
   //-------------------------------------------------------------------------
*/

    /**
     *
     */
    void handleData() {
        char menu_data[] = "# OPERACJE:  \n#   1. Wybierz plik     \n#   2. Wygeneruj plik \n#   3. Wczytaj dane z pliku     \n#   4. Wyczyść dane   \n#   0. Powrot        \n\nWybierz z listy: ";

        while (true) {
            system("cls");
            printf("%s", "\n# Dane wejściowe\n\n");
            printf("Plik: %s\n", file_name.c_str());
            printf("Rozmiar wczytanych danych: %d\n\n", edgesNum);

            // pokazanie menu danych wejsciowych
            printf("%s", menu_data);
            int input;
            scanf("%d", &input);
            fflush(stdin);

            char tmp[] = "";
            int n;

            switch (input) {
                case 0:
                    return;
                case 1:
                    // ładowanie pliku o innej nazwie
                    printf("%s", "\nPodaj nazwę pliku: ");
                    scanf("%s", tmp);
                    fflush(stdin);

                    if (!fileExists(tmp)) {
                        printf("%s", "\nPlik nie istnieje!\n\n");
                    } else {
                        file_name = tmp;
                        readTestData();

                        printf("%s", "\nPlik został załadowany. \n\n");
                    }
                    return;

                case 3:
                    // ładowanie danych z ustawionego pliku
                    readTestData();
                    printf("%s", "\nPlik został załadowany. \n\n");
                    return;

                case 4:
                    // czyszczenie zapamietanych danych
                    test_data.clear();
                    edgesNum = 0;
                    verticlesNum = 0;
                    startVertex = 0;
                    endVertex = 0;
                    printf("%s", "\nDane zostały wyczyszczone. \n\n");
                    return;

                case 2:
                    // generowanie danych
                    printf("%s", "\nPodaj ilość wierzcholkow: ");
                    scanf("%d", &verticlesNum);

                    printf("%s", "\nPodaj ilość krawedzi: ");
                    scanf("%d", &edgesNum);
                    fflush(stdin);

                    this->generateData(verticlesNum, edgesNum);
                    if (fileExists(file_name)) {
                        printf("%s", "\nPlik został utworzony i załadowany. \n\n");
                    } else {
                        printf("%s", "\nWystapil blad generowania danych. \n\n");
                    }

                    return;
            }
        }
    }

    void generateData(int verticlesNum, int edgesNum){
        this->verticlesNum = verticlesNum;
        this->edgesNum = edgesNum;

        if(edgesNum < verticlesNum){
            printf("%s", "\nLiczba krawedzi nie moze byc mniejsza niz liczba wierzchlkow. \n\n");
            return;
        }

        startVertex = 0;
        endVertex = edgesNum - 1;
        bool isConnected;
        do{
            isConnected = true;

            ofstream file;
            file.open(file_name);
            file << edgesNum << " " << verticlesNum << " " << startVertex << " " << endVertex << "\n";

            int max = INT_MAX;
            if (edgesNum * 2 < max) {
                max = edgesNum * 2;
            }

            vector<pair<int,int>> added(0);

            int a,b;
            bool is;
            for (int i = 0; i < edgesNum; i++) {
                a = (rand()) % verticlesNum;
                b = (rand()) % verticlesNum;
                if(a == b){
                    i--;
                    continue;
                }
                is = false;
                for(auto item:added){
                    if(item.first == a && item.second == b) is = true;
                    if(item.first == b && item.second == a) is = true;
                }
                if(is){
                    i--;
                    continue;
                }
                added.emplace_back(a,b);
                if(a<b){
                    file << a << " ";
                    file << b << " ";
                }
                else{
                    file << b << " ";
                    file << a << " ";
                }

                file << (rand()) % max << "\n";
            }

            file.close();

            if (fileExists(file_name)) {
                readTestData();
            }

            delete graph_matrix;
            delete graph_list;
            graph_matrix = new GraphMatrix(verticlesNum);
            graph_list = new GraphList(verticlesNum);
            for (auto row: test_data) {
                graph_matrix->addEdge(get<0>(row), get<1>(row), get<2>(row));
                graph_list->addEdge(get<0>(row), get<1>(row), get<2>(row));
            }
            ShortestPathReturn dijkstraReturn = graph_list->dijkstraShortestPath(startVertex);
            for (int i = 0; i < dijkstraReturn.distances.size(); ++i) {
                if (dijkstraReturn.distances[i] == std::numeric_limits<int>::max()) {
                    isConnected = false;
                }
            }
        } while(!isConnected);
    }

    void handleMST() {

        int input;
        int density = 0;

        // pętla menu
        while (true) {
            system("cls");
            printf("%s", "\n# MST\n\n");
            // pokazanie menu tablicy
            printf("   1. Wczytaj z pliku\n   2. Wyswietl\n   3. Algorytm Prima\n   4. Pomiar czasu\n   0. Powrot\n");

            scanf("%d", &input);
            fflush(stdin);

            switch (input) {
                case 0:
                    return;

                case 1:
                    readTestData();

                    printf("%s", "\nPlik został załadowany. \n\n");
                    break;

                case 2:
                    cout << "Graf w zapisie macierzowym" << endl;

                    // Wyświetlanie macierzy sąsiedztwa
                    graph_matrix->printMatrix();

                    cout << endl;

                    cout << "Graf w zapisie listowym" << endl;
                    // Wyświetlanie listy sąsiedztwa
                    graph_list->printList();

                    cout << endl;
                    break;

                case 3:
                    std::cout << std::endl;
                    {
                        GraphMatrix primMST = graph_matrix->primMST();
                        std::cout << "Minimalne drzewo rozpinające (macierz, algorytm Prima):" << std::endl;
                        primMST.printMST();
                        std::cout << "MST = " << primMST.getEdgesSum() << std::endl;
                    }
                    std::cout << std::endl;
                    {
                        std::cout << "Minimalne drzewo rozpinające (lista, algorytm Prima):" << std::endl;
                        GraphList primMST = graph_list->primMST();
                        primMST.printList();
                        std::cout << "MST = " << primMST.getEdgesSum() << std::endl;
                    }

                    std::cout << std::endl;
                    break;

                case 4:
                    // pomiary czasu

                    int n = 100;

                    printf("%s", "\nPodaj ilość wierzcholkow: ");
                    scanf("%d", &verticlesNum);

                    printf("%s", "\nPodaj gestosc grafu: ");
                    scanf("%d", &density);
                    fflush(stdin);

                    double dens = ((double)density)/100;
                    double result = (dens * verticlesNum*verticlesNum - dens * verticlesNum) / 2;

                    // d = 2*verticlesNum / edgesNum / (edgesNum-1)

                    edgesNum = floor(result);
                    if(edgesNum == 0){
                        edgesNum = 1;
                    }

                    for(int i = 0;i<n;i++){
                        this->generateData(verticlesNum,edgesNum);


                        printf("%d.   %f krawedzi ", i, result);
                    }

                    break;
            }

            system("pause");
        }
    };

    void handleShortestPath() {

        int input;

        // pętla menu
        while (true) {
            system("cls");
            printf("%s", "\n# MST\n\n");
            // pokazanie menu tablicy
            printf("   1. Wczytaj z pliku\n   2. Wyswietl\n   3. Algorytm Dijkstry\n   0. Powrot\n");

            scanf("%d", &input);
            fflush(stdin);

            switch (input) {
                case 0:
                    return;

                case 1:
                    readTestData();

                    printf("%s", "\nPlik został załadowany. \n\n");
                    break;

                case 2:
                    cout << "Graf w zapisie macierzowym" << endl;

                    // Wyświetlanie macierzy sąsiedztwa
                    graph_matrix->printMatrix();

                    cout << endl;

                    cout << "Graf w zapisie listowym" << endl;
                    // Wyświetlanie listy sąsiedztwa
                    graph_list->printList();

                    cout << endl;
                    break;

                case 3:
                    std::cout << std::endl;
                    {
                        std::cout << "Najkrótsze drogi z wierzchołka " << startVertex
                                  << " (macierz, algorytm Dijkstry):" << std::endl;
                        ShortestPathReturn dijkstraReturn = graph_matrix->dijkstraShortestPath(startVertex);
                        std::cout << "Start = " << startVertex << std::endl;
                        std::cout << "End \tDist  \tPath" << std::endl;
                        for (int i = 0; i < dijkstraReturn.distances.size(); ++i) {
                            std::cout << "| " << i << " \t| " << dijkstraReturn.distances[i] << " \t| "
                                      << dijkstraReturn.paths[i] << std::endl;
                        }
                    }
                    std::cout << std::endl;
                    {
                        std::cout << "Najkrótsze drogi z wierzchołka " << startVertex << " (lista, algorytm Dijkstry):"
                                  << std::endl;
                        ShortestPathReturn dijkstraReturn = graph_list->dijkstraShortestPath(startVertex);
                        std::cout << "Start = " << startVertex << std::endl;
                        std::cout << "End \tDist  \tPath" << std::endl;
                        for (int i = 0; i < dijkstraReturn.distances.size(); ++i) {
                            if (dijkstraReturn.distances[i] == std::numeric_limits<int>::max()) {
                                std::cout << "| " << i << " \t| ∞   \t| "
                                          << dijkstraReturn.paths[i] << std::endl;
                            } else {
                                std::cout << "| " << i << " \t| " << dijkstraReturn.distances[i] << " \t| "
                                          << dijkstraReturn.paths[i] << std::endl;
                            }

                        }
                    }

                    std::cout << std::endl;
                    break;
            }

            system("pause");
        }
    };

public:
/**
 * Główna funkcja
 */
    void mainLoop() {
        char menu_header[] = "\n### Projekt SDiZO, PWR 2023 ###    \n### Autor:  Jacek Bogdański ###\n\n";
        char menu_1[] = "# MENU PROGRAMU:\n#   1. MST          \n#   2. Najkrotsza sciezka w grafie          \n#   3. Dane wejściowe\n#   0. Koniec                 \n\nWybierz z listy: ";

        int input;

        // pętla menu głównego
        while (true) {
            system("cls");
            printf("%s", menu_header);

            printf("Plik: %s\n", file_name.c_str());
            printf("Liczba wierzcholkow: %d\n", verticlesNum);
            printf("Liczba krawedzi: %d\n\n", edgesNum);

            // pokazanie menu 1
            printf("%s", menu_1);

            // pobranie 1
            scanf("%d", &input);
            fflush(stdin);

            switch (input) {
                case 0:
                    printf("%s", "\nProgram zakonczony.\n");
                    return;
                case 1:
                    // MST
                    handleMST();
                    break;
                case 2:
                    // shortest path
                    handleShortestPath();
                    break;
                case 3:
                    // wczytywanie, generowanie danych
                    handleData();
                    break;
            }
        }
    }
};



