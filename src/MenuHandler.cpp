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
#include <Windows.h>
#include <profileapi.h>
#include <unistd.h>
#include "GraphList.h"
#include "GraphMatrix.h"

//#include <conio.h>
//#include <Windows.h>

using namespace std;

class MenuHandler {

public:
    MenuHandler(){
        QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
    };

    ~MenuHandler(){
    };

private:
    string file_name = "input.txt";

    int edgesNum = 0;
    int verticlesNum = 0;
    int startVertex = 0;
    int endVertex = 0;


    GraphMatrix graph_matrix;
    GraphList graph_list;

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

            graph_matrix.resize(verticlesNum);
            graph_list.resize(verticlesNum);
            for (auto row: test_data) {
                graph_matrix.addEdge(get<0>(row), get<1>(row), get<2>(row));
                graph_list.addEdge(get<0>(row), get<1>(row), get<2>(row));
            }
        } else {
            cout << "file open error" << endl;
        }
    }



    /**
     * Pobranie czasu systemowego, źródło: antoni.sterna.staff.iiar.pwr.edu.pl
     */

   //-------------------------------------------------------------------------
       long long int read_QPC() {
           LARGE_INTEGER count;
           QueryPerformanceCounter(&count);
           return ((long long int) count.QuadPart);
       }
   //-------------------------------------------------------------------------


    /**
     *
     */
    void handleData() {
        char menu_data[] = "# OPERACJE:  \n#   1. Wybierz plik     \n#   2. Wygeneruj plik \n#   3. Wczytaj dane z pliku     \n#   4. Wyczysc dane   \n#   0. Powrot        \n\nWybierz z listy: ";

        while (true) {
            printf("\n\n\n\n\n\n\n\n\n");
            printf("%s", "\n# Dane wejsciowe\n\n");
            printf("Plik: %s\n", file_name.c_str());
            printf("Rozmiar wczytanych danych: %d\n\n", edgesNum);

            // pokazanie menu danych wejsciowych
            printf("%s", menu_data);
            int input;
            scanf("%d", &input);
            fflush(stdin);

            char tmp[100] = "";
            int n;

            switch (input) {
                case 0:
                    return;
                case 1:
                    // ładowanie pliku o innej nazwie
                    printf("%s", "\nPodaj nazwe pliku: ");
                    scanf("%[^\n]", tmp);
                    fflush(stdin);

                    if (!fileExists(tmp)) {
                        printf("\nPlik %s nie istnieje!\n\n", tmp);
                    } else {
                        file_name = tmp;
                        readTestData();

                        printf("%s", "\nPlik zostal zaladowany. \n\n");
                    }
                    return;

                case 3:
                    // ładowanie danych z ustawionego pliku
                    readTestData();
                    printf("%s", "\nPlik zostal zaladowany. \n\n");
                    return;

                case 4:
                    // czyszczenie zapamietanych danych
                    test_data.clear();
                    edgesNum = 0;
                    verticlesNum = 0;
                    startVertex = 0;
                    endVertex = 0;
                    printf("%s", "\nDane zostaly wyczyszczone. \n\n");
                    return;

                case 2:
                    // generowanie danych
                    printf("%s", "\nPodaj ilosc wierzcholkow: ");
                    scanf("%d", &verticlesNum);

                    printf("%s", "\nPodaj ilosc krawedzi: ");
                    scanf("%d", &edgesNum);
                    fflush(stdin);

                    this->generateData(verticlesNum, edgesNum);
                    if (fileExists(file_name)) {
                        printf("%s", "\nPlik zostal utworzony i zaladowany. \n\n");
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
            printf("%s", "\nLiczba krawedzi nie moze byc mniejsza niz liczba wierzcholkow. \n\n");
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
                ShortestPathReturn dijkstraReturn = graph_list.dijkstraShortestPath(startVertex);
                for (int i = 0; i < dijkstraReturn.distances.size(); ++i) {
                    if (dijkstraReturn.distances[i] == std::numeric_limits<int>::max()) {
                        isConnected = false;
                    }
                }
            }
            else{
                isConnected = false;
            }


        } while(!isConnected);
    }

    void handleMST() {

        int input;
        int density = 0;

        // pętla menu
        while (true) {
            printf("\n\n\n\n\n\n\n\n\n");
            printf("%s", "\n# MST\n\n");
            // pokazanie menu tablicy
            printf("   1. Wczytaj z pliku\n   2. Wyswietl\n   3. Algorytm Prima\n   4. Pomiar czasu\n   0. Powrot\n");

            scanf("%d", &input);
            fflush(stdin);

            switch (input) {
                case 0:
                    return;

                case 1:
                    handleData();
                    break;

                case 2:
                    cout << "Graf w zapisie macierzowym" << endl;

                    // Wyświetlanie macierzy sąsiedztwa
                    graph_matrix.printMatrix();

                    cout << endl;

                    cout << "Graf w zapisie listowym" << endl;
                    // Wyświetlanie listy sąsiedztwa
                    graph_list.printList();

                    cout << endl;
                    break;

                case 3:
                    std::cout << std::endl;
                    {
                        GraphMatrix primMST = graph_matrix.primMST();
                        std::cout << "Minimalne drzewo rozpinające (macierz, algorytm Prima):" << std::endl;
                        primMST.printMST();
                        std::cout << "MST = " << primMST.getEdgesSum() << std::endl;
                    }
                    std::cout << std::endl;
                    {
                        std::cout << "Minimalne drzewo rozpinajace (lista, algorytm Prima):" << std::endl;
                        GraphList primMST = graph_list.primMST();
                        primMST.printList();
                        std::cout << "MST = " << primMST.getEdgesSum() << std::endl;
                    }

                    std::cout << std::endl;
                    break;

                case 4:
                    // pomiary czasu

                    printf("%s", "\nPodaj ilosc wierzcholkow: ");
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

                    long long int sumElapsedMatrix = 0;
                    long long int sumElapsedList = 0;
                    int n = 100;

                    for(int i = 1;i<=n;i++){
                        this->generateData(verticlesNum,edgesNum);

                        start = read_QPC();
                        GraphList primMST1 = graph_list.primMST();
                        sumElapsedList += read_QPC() - start;

                        start = read_QPC();
                        GraphMatrix primMST2 = graph_matrix.primMST();
                        sumElapsedMatrix = read_QPC() - start;

                        cout << "M-- Time [ms] = " << fixed << showpoint<< setprecision(3) << (1000.0 * sumElapsedMatrix) / frequency / i<< endl;
                        cout << "M-- Time [us] = " << fixed << showpoint<< setprecision(3) << (1000000.0 * sumElapsedMatrix) / frequency / i<< endl << endl;

                        cout << "L-- Time [ms] = " << fixed << showpoint<< setprecision(3) << (1000.0 * sumElapsedList) / frequency / i<< endl;
                        cout << "L-- Time [us] = " << fixed << showpoint<< setprecision(3) << (1000000.0 * sumElapsedList) / frequency / i<< endl << endl;
                    }

                    cout << "M Time [ms] = " << fixed << showpoint<< setprecision(3) << (1000.0 * sumElapsedMatrix) / frequency / n<< endl;
                    cout << "M Time [us] = " << fixed << showpoint<< setprecision(3) << (1000000.0 * sumElapsedMatrix) / frequency / n<< endl << endl;

                    cout << "L Time [ms] = " << fixed << showpoint<< setprecision(3) << (1000.0 * sumElapsedList) / frequency / n<< endl;
                    cout << "L Time [us] = " << fixed << showpoint<< setprecision(3) << (1000000.0 * sumElapsedList) / frequency / n<< endl << endl;
                    break;
            }

            system("pause");
        }
    };

    void handleShortestPath() {

        int input, density;

        // pętla menu
        while (true) {
            printf("\n\n\n\n\n\n\n\n\n");
            printf("%s", "\n# Shortest Path\n\n");
            // pokazanie menu tablicy
            printf("   1. Wczytaj z pliku\n   2. Wyswietl\n   3. Algorytm Dijkstry\n   4. Pomiar czasu \n   0. Powrot\n");

            scanf("%d", &input);
            fflush(stdin);

            switch (input) {
                case 0:
                    return;

                case 1:
                    handleData();
                    break;

                case 2:
                    cout << "Graf w zapisie macierzowym" << endl;

                    // Wyświetlanie macierzy sąsiedztwa
                    graph_matrix.printMatrix();

                    cout << endl;

                    cout << "Graf w zapisie listowym" << endl;
                    // Wyświetlanie listy sąsiedztwa
                    graph_list.printList();

                    cout << endl;
                    break;

                case 3:
                    std::cout << std::endl;
                    {
                        std::cout << "Najkrotsze drogi z wierzcholka " << startVertex
                                  << " (macierz, algorytm Dijkstry):" << std::endl;
                        ShortestPathReturn dijkstraReturn = graph_matrix.dijkstraShortestPath(startVertex);

                        if(dijkstraReturn.hasNegative){
                            std::cout << "Graf zawiera ujemne krawedzie!" << std::endl;
                        }
                        else
                        {
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
                    }
                    std::cout << std::endl;
                    {
                        std::cout << "Najkrotsze drogi z wierzcholka " << startVertex << " (lista, algorytm Dijkstry):"
                                  << std::endl;
                        ShortestPathReturn dijkstraReturn = graph_list.dijkstraShortestPath(startVertex);
                        if(dijkstraReturn.hasNegative){
                            std::cout << "Graf zawiera ujemne krawedzie!" << std::endl;
                        }
                        else
                        {
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
                    }

                    std::cout << std::endl;
                    break;

                case 4:
                    // pomiary czasu

                    printf("%s", "\nPodaj ilosc wierzcholkow: ");
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

                    long long int sumElapsedMatrix = 0;
                    long long int sumElapsedList = 0;
                    int n = 100;

                    for(int i = 1;i<=n;i++){
                        this->generateData(verticlesNum,edgesNum);

                        start = read_QPC();
                        ShortestPathReturn dijkstraReturn_1 = graph_list.dijkstraShortestPath(startVertex);
                        sumElapsedList += read_QPC() - start;

                        start = read_QPC();
                        ShortestPathReturn dijkstraReturn_2 = graph_matrix.dijkstraShortestPath(startVertex);
                        sumElapsedMatrix = read_QPC() - start;

                        cout << "M-- Time [ms] = " << fixed << showpoint<< setprecision(3) << (1000.0 * sumElapsedMatrix) / frequency / i<< endl;
                        cout << "M-- Time [us] = " << fixed << showpoint<< setprecision(3) << (1000000.0 * sumElapsedMatrix) / frequency / i<< endl << endl;

                        cout << "L-- Time [ms] = " << fixed << showpoint<< setprecision(3) << (1000.0 * sumElapsedList) / frequency / i<< endl;
                        cout << "L-- Time [us] = " << fixed << showpoint<< setprecision(3) << (1000000.0 * sumElapsedList) / frequency / i<< endl << endl;
                    }

                    cout << "M Time [ms] = " << fixed << showpoint<< setprecision(3) << (1000.0 * sumElapsedMatrix) / frequency / n<< endl;
                    cout << "M Time [us] = " << fixed << showpoint<< setprecision(3) << (1000000.0 * sumElapsedMatrix) / frequency / n<< endl << endl;

                    cout << "L Time [ms] = " << fixed << showpoint<< setprecision(3) << (1000.0 * sumElapsedList) / frequency / n<< endl;
                    cout << "L Time [us] = " << fixed << showpoint<< setprecision(3) << (1000000.0 * sumElapsedList) / frequency / n<< endl << endl;

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
        char menu_header[] = "\n### Projekt SDiZO, PWR 2023 ###    \n### Autor:  Jacek Bogdanski ###\n\n";
        char menu_1[] = "# MENU PROGRAMU:\n#   1. MST          \n#   2. Najkrotsza sciezka w grafie          \n#   3. Dane wejsciowe\n#   0. Koniec                 \n\nWybierz z listy: ";

        int input;

        // pętla menu głównego
        while (true) {
            printf("\n\n\n\n\n\n\n\n\n");
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



