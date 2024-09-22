#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
using namespace chrono;

int getRandomNumber(int min, int max) {
    return rand() % (max - min) + min;
}

// Функция для сортировки простыми вставками
void insertionSort(vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        // Перемещение элементов, которые больше key, на одну позицию вперед
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Функция для вывода массива в файл
void printArrayToFile(const vector<int>& arr, const string& filename) {
    ofstream outFile(filename);
    for (int num : arr) {
        outFile << num << " ";
    }
    outFile << endl;
    outFile.close();
}

// Функция для вывода времени в нужном формате
void printDuration(microseconds duration) {
    milliseconds millisec = duration_cast<milliseconds>(duration); // Получаем время в миллисекундах
    int microsec = duration.count() % 1000; // Остаток времени в микросекундах

    cout << millisec.count() 
         << setfill('0') << setw(3) << microsec
         << " micorsec" << endl;
}

// Функция сравнения для qsort
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    srand(time(0)); // Инициализация генератора случайных чисел

    vector<int> sizes = {16, 100, 500, 1000, 5000}; // Размеры массивов для теста

    // Цикл по разным размерам массивов
    for (int arraySize : sizes) {
        vector<int> arr(arraySize); // Инициализация вектора с размером arraySize
        vector<int> arrCopy(arraySize); // Копия для qsort

        // Генерация случайных чисел в зависимости от размера массива
        if (arraySize < 500) {
            for (int i = 0; i < arraySize; i++) {
                arr[i] = getRandomNumber(100, 1000); // диапазон [100, 1000)
            }
        } else {
            for (int i = 0; i < arraySize; i++) {
                arr[i] = getRandomNumber(1000, 10000); // диапазон [1000, 10000)
            }
        }

        // Копируем массив для qsort
        arrCopy = arr;

        // Вывод несортированного массива в файл
        string originalFile = "d" + to_string(arraySize) + ".txt";
        printArrayToFile(arr, originalFile);

        // Замер времени сортировки простыми вставками
        high_resolution_clock::time_point startInsertion = high_resolution_clock::now();
        insertionSort(arr);
        high_resolution_clock::time_point stopInsertion = high_resolution_clock::now();

        // Подсчет времени выполнения сортировки вставками
        microseconds durationInsertion = duration_cast<microseconds>(stopInsertion - startInsertion);
        cout << "Insertion sort time for " << arraySize << " elements: ";
        printDuration(durationInsertion);

        // Вывод отсортированного массива в файл
        string sortedFile = "d" + to_string(arraySize) +"-s" + ".txt";
        printArrayToFile(arr, sortedFile);

        // Замер времени стандартной функции qsort
        high_resolution_clock::time_point startQSort = high_resolution_clock::now();
        qsort(arrCopy.data(), arrCopy.size(), sizeof(int), compare);
        high_resolution_clock::time_point stopQSort = high_resolution_clock::now();

        // Подсчет времени выполнения qsort
        microseconds durationQSort = duration_cast<microseconds>(stopQSort - startQSort);
        cout << "qsort time for " << arraySize << " elements: ";
        printDuration(durationQSort);

        cout << "---------------------------------------------------" << endl;
    }

    return 0;
}
