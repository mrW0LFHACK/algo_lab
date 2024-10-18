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

// Функция для получения случайного числа в заданном диапазоне
int getRandomNumber(int min, int max) {
    return rand() % (max - min) + min;
}

#include <iostream>
#include <vector>

using namespace std;

// H4 [Продвинуться вниз]:
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1; 
    int right = 2 * i + 2; 

    // Н5: Найти большего потомка
    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    // Н6: Больше, чем K?
    if (largest != i) {
        swap(arr[i], arr[largest]); // Н7: Поднять его вверх
        heapify(arr, n, largest);   // Рекурсивный вызов для поддерева
    }
}

// Функция для пирамидальной сортировки
void heapSort(vector<int>& arr) {
    int n = arr.size();

    // Н1: Начальная установка (Построение пирамиды)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Н2: Уменьшение размера пирамиды и извлечение элементов
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]); // Н8: Занести Ri
        heapify(arr, i, 0);   // Н3: Приготовиться к протаскиванию (восстановление пирамиды)
    }
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
        vector<int> arrInsertion(arraySize); // Копия для сортировки вставками

        // Считывание данных из файла
        string filename = "d" + to_string(arraySize) + ".txt";
        ifstream inputFile(filename);
        if (inputFile.is_open()) {
            int num;
            int i = 0;
            while (inputFile >> num) {
                arr[i] = num;
                arrCopy[i] = num;
                arrInsertion[i] = num;
                i++;
            }
            inputFile.close();
        } else {
            cout << "Невозможно открыть файл: " << filename << endl;
            return 1; // Выход с кодом ошибки
        }

        // Замер времени сортировки пирамидой
        high_resolution_clock::time_point startHeap = high_resolution_clock::now();
        heapSort(arr);
        high_resolution_clock::time_point stopHeap = high_resolution_clock::now();

        // Подсчет времени выполнения сортировки пирамидой
        microseconds durationHeap = duration_cast<microseconds>(stopHeap - startHeap);
        cout << "Heap sort time for " << arraySize << " elements: ";
        printDuration(durationHeap);

        // Вывод отсортированного массива в файл
        string sortedFile = "d" + to_string(arraySize) + "-s" + ".txt";
        printArrayToFile(arr, sortedFile);

        // Замер времени сортировки простыми вставками
        high_resolution_clock::time_point startInsertion = high_resolution_clock::now();
        insertionSort(arrInsertion);
        high_resolution_clock::time_point stopInsertion = high_resolution_clock::now();

        // Подсчет времени выполнения сортировки вставками
        microseconds durationInsertion = duration_cast<microseconds>(stopInsertion - startInsertion);
        cout << "Insertion sort time for " << arraySize << " elements: ";
        printDuration(durationInsertion);

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