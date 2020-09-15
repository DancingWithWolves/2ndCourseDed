
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef int (*compare_t)(const void*, const void*);

/*!
 * Меняет местами содержимое памяти elem_1 и elem_2
 * @param elem_1 указатель на первый элемент
 * @param elem_2 указатель на второй элемент
 * @param elem_size размер элемента
 */
void Swap(void* elem_1, void* elem_2, int elem_size)
{
    assert(elem_1);
    assert(elem_2);
    char cur = *(char*)elem_1;
    for (int i = 0; i < elem_size; i++) {
        cur = *(char*)elem_1;
        *(char*)elem_1 = *(char*)elem_2;
        *(char*)elem_2 = cur;
        elem_1 = (char*)elem_1 + 1;
        elem_2 = (char*)elem_2 + 1;
    }
}
/*!
 * "раскидывает" все элементы массива относительно стержня, равному среднему элементу массива
 * @param arr сортируемый массив
 * @param elem_size размер элемента
 * @param compare компаратор
 * @param left индекс левого элемента сортируемого подмассива
 * @param right индекс второго элемента сортируемого подмассива
 * @return возвращает новое значение стержня. Справа и слева от него -- два неотсортированных подмассива
 */
int Partition(void* arr, int elem_size, compare_t compare, int left, int right)
{
    assert(arr);
    int mid = (right + left) / 2;
    void* leftElem = (char*)arr + (left * elem_size);
    void* pivot = (char*)arr + (mid * elem_size);
    Swap(leftElem, pivot, elem_size);
    int last = left;
    void *tmp, *tmp2, *root = (char*)arr + (left * elem_size);
    for (int i = left + 1; i <= right; i++) {

        tmp = (char*)arr + (i * elem_size);
        if (compare(root, tmp) > 0) {
            last++;
            tmp2 = (char*)arr + (last * elem_size);
            Swap(tmp2, tmp, elem_size);
        }
    }
    tmp = (char*)arr + (last * elem_size);
    Swap(tmp, root, elem_size);
    return last;
}

/*!
 * Сортирует массив arr в соответствии с компаратором compare
 * @param arr Массив, элементы которого будут отсортированы
 * @param elem_size Размер одного элемента массива
 * @param compare Указатель на функцию, которая сравнивает два элемента и возвращает целое значение >0, если первый элемент больше второго, <0, если первый меньше, =0, если равны
 * @param left индекс самого левого элемента (в общем случае 0)
 * @param right индекс самого правого элемента в общем случае ( n - 1 ), где n -- количество элементов в массиве
 */
void QuickSort(void* arr, int elem_size, compare_t compare, int left, int right)
{
    assert(arr);
    if (left < right) {
        int newIndex = Partition(arr, elem_size, compare, left, right);
        QuickSort(arr, elem_size, compare, left, newIndex - 1);
        QuickSort(arr, elem_size, compare, newIndex + 1, right);
    }
}

/*!
 * Сортирует массив arr в соответствии с компаратором compare
 * @param arr Массив, элементы которого будут отсортированы
 * @param elem_size Размер одного элемента массива
 * @param compare Указатель на функцию, которая сравнивает два элемента и возвращает целое значение >0, если первый элемент больше второго, <0, если первый меньше, =0, если равны
 * @param left индекс самого левого элемента (в общем случае 0)
 * @param right индекс самого правого элемента в общем случае ( n - 1 ), где n -- количество элементов в массиве
 */
void BubbleSort(void* arr, int elem_size, compare_t compare, int left, int right)
{
    assert(arr);
    for (int i = 0; i < right; ++i) {

        for (int j = 0; j < right - i; ++j) {

            char* ptr_2 = (char*)arr + j * elem_size;

            if (compare(ptr_2, ptr_2 + elem_size) > 0)
                Swap(ptr_2, ptr_2 + elem_size, elem_size);
        }
    }
}

/*!
 * Сортирует массив arr в соответствии с компаратором compare
 * @param arr Массив, элементы которого будут отсортированы
 * @param elem_size Размер одного элемента массива
 * @param compare Указатель на функцию, которая сравнивает два элемента и возвращает целое значение >0, если первый элемент больше второго, <0, если первый меньше, =0, если равны
 * @param left индекс самого левого элемента (в общем случае 0)
 * @param right индекс самого правого элемента в общем случае ( n - 1 ), где n -- количество элементов в массиве
 */
void InsertionSort(void* arr, int elem_size, compare_t compare, int left, int right)
{
    assert(arr);
    char* char_arr = (char*)arr;
    int j = 0;
    for (int i = 0; i <= right; ++i) {
        char* key_ptr = char_arr + i * elem_size;
        j = i - 1;
        while (j >= 0 && compare(key_ptr - elem_size, key_ptr) > 0) {
            Swap(key_ptr, key_ptr - elem_size, elem_size);
            key_ptr -= elem_size;
            --j;
        }
    }
}

