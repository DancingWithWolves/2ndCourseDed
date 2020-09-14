#include "sortFunctions.h"
#include "stdio.h"
#include <GL/glut.h>

int compare(const void* arg_1, const void* arg_2)
{
    return *(int*)arg_1 - *(int*)arg_2;
}
int comparator(int arg_1, int arg_2)
{
    return arg_1 - arg_2;
}

int main(int argc, char** argv)
{
    int arr[] = { 0, 5, 2, 3, 4, 1, 6, 7, 4, 8, 8, 6, 2 };
    insertionSort(arr, sizeof(int), compare, 0, 0);
    for (auto &i : arr) {
        printf("%d ", i);
        i = rand() % 50;
    }
    printf("\n");

    return 0;
}