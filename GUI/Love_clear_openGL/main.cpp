#include "sortFunctions.h"
#include "layoutConstants.h"
#include "clearWindowManager.cpp"
#include <stdio.h>

/**
 * Выводит на экран зависимость количества swap'ов и сравнений разных сортировок от количества элементов с использованием простейшей графической визуализации
 */

// Глобальные переменные -- количество сравнений и свапов
int compares_qty = 0, swaps_qty = 0;


extern const int default_margin;

extern const int button_height;

extern const int graph_height;

extern const int buttons_qty;

extern const int graphs_qty;


const int start_qty = 100;
const int step = 100;
const int max_qty = 5000;

const int elems_qtys_qty = ((max_qty - start_qty) / step) + 2;


int elems_qtys[elems_qtys_qty];

/*!
 * Сравнивает 
 * @param elem_1 указатель на первый элемент
 * @param elem_2 указатель на второй элемент
 * @param elem_size размер элемента
 */
int Compare(const void* arg_1, const void* arg_2)
{
    compares_qty++;
    return *(int*)arg_1 - *(int*)arg_2;
}

/*!
 * Меняет местами содержимое памяти elem_1 и elem_2 размером elem_size
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
    
    ++swaps_qty;
}

void GenerateRandomArray(int n, int *arr)
{
    assert(arr);
    assert(n > 0);
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % n;
    }

}

/*!
*  Обрабатывает функцию BubbleSort()
*/
void ShowBubbleSortStat()
{
    int compares[elems_qtys_qty], swaps[elems_qtys_qty];
    compares[0] = 0;
    swaps[0] = 0;
    int i = 0;
    for (int array_size = start_qty; array_size <= max_qty; array_size += step) {

        int array[array_size];
        GenerateRandomArray(array_size, array);

        BubbleSort(array, sizeof(int), Compare, Swap, 0, array_size-1);

        compares[i+1] = static_cast<int>(compares_qty);
        swaps[i+1] = static_cast<int>(swaps_qty);

        swaps_qty = 0;
        compares_qty = 0;
        ++i;
    }

    graph_managers[0].AddGraph(elems_qtys_qty, elems_qtys, compares);
    graph_managers[1].AddGraph(elems_qtys_qty, elems_qtys, swaps);

}

/*!
*  Обрабатывает функцию InsertionSort()
*/
void ShowInsertionSortStat()
{
    int compares[elems_qtys_qty], swaps[elems_qtys_qty];
    compares[0] = 0;
    swaps[0] = 0;
    int i = 0;
    for (int array_size = start_qty; array_size <= max_qty; array_size += step) {

        int array[array_size];
        GenerateRandomArray(array_size, array);

        InsertionSort(array, sizeof(int), Compare, Swap, 0, array_size-1);

        compares[i+1] = static_cast<int>(compares_qty);
        swaps[i+1] = static_cast<int>(swaps_qty);

        swaps_qty = 0;
        compares_qty = 0;
        ++i;
    }
    

    graph_managers[0].AddGraph(elems_qtys_qty, elems_qtys, compares);
    graph_managers[1].AddGraph(elems_qtys_qty, elems_qtys, swaps);

}

/*!
*  Обрабатывает функцию QuickSort()
*/
void ShowQuickSortStat()
{
    int compares[elems_qtys_qty], swaps[elems_qtys_qty];
    compares[0] = 0;
    swaps[0] = 0;
    int i = 0;
    for (int array_size = start_qty; array_size <= max_qty; array_size += step) {

        int array[array_size];
        GenerateRandomArray(array_size, array);

        QuickSort(array, sizeof(int), Compare, Swap, 0, array_size-1);

        compares[i+1] = static_cast<int>(compares_qty);
        swaps[i+1] = static_cast<int>(swaps_qty);

        swaps_qty = 0;
        compares_qty = 0;
        ++i;
    }

    graph_managers[0].AddGraph(elems_qtys_qty, elems_qtys, compares);
    graph_managers[1].AddGraph(elems_qtys_qty, elems_qtys, swaps);
}

/*!
*  Подчищает память после glutMainLoop()
*/
void Clear()
{
    for (int i = 0; i < buttons_qty; ++i) {
        buttons[i].~Button();
    }
    ::operator delete(buttons);

    for (int i = 0; i < graphs_qty; ++i) {
        graph_managers[i].~GraphManager();
    }
    ::operator delete(graph_managers);
}

int main(int argc, char** argv)
{
    elems_qtys[0] = 0;
    for (int i = 1; i < elems_qtys_qty; ++i) {
        elems_qtys[i] = elems_qtys[i-1]+step;
    }

    buttons = reinterpret_cast<Button*>(::operator new(sizeof(Button) * buttons_qty));

    Button* button = nullptr;
    button = new (buttons) Button 
        ( default_margin
        , default_margin
        , window_width / buttons_qty - 2 * default_margin
        , button_height
        , ShowBubbleSortStat
        , "Bubble sort");

    button = new (buttons + 1) Button 
        ( window_width / buttons_qty + default_margin
        , default_margin
        , window_width / buttons_qty - 2 * default_margin
        , button_height
        , ShowInsertionSortStat
        , "Insertion sort");

    button = new (buttons + 2) Button 
        ( window_width * 2 / buttons_qty + default_margin
        , default_margin
        , window_width / buttons_qty - 2 * default_margin
        , button_height
        , ShowQuickSortStat
        , "Quick sort");

    for (int i = 0; i < buttons_qty; ++i) {
        drawable_list.push_front(&buttons[i]);
    }



    graph_managers = reinterpret_cast<GraphManager*>(::operator new(sizeof(GraphManager) * graphs_qty));

    GraphManager* graphManager = nullptr;
    graphManager = new (graph_managers) GraphManager
        ( default_margin
        , button_height + default_margin * 4 
        , window_width / 2 - 2 * default_margin
        , graph_height
        , 8*default_margin
        , 8*default_margin
        , "Compares"
        , "Elems qty"
        , "Compares qty"
        , 0
        , 0
        , 0
        , 0
        );

    graphManager = new (graph_managers + 1) GraphManager
        ( default_margin + graph_managers[0].width + 2 * default_margin
        , graph_managers[0].y
        , graph_managers[0].width
        , graph_height
        , 8*default_margin
        , 8*default_margin
        , "Swaps"
        , "Elems qty"
        , "Compares qty"
        , 0
        , 0
        , 0
        , 0
        );

    for (int i = 0; i < graphs_qty; ++i) {
        drawable_list.push_front(&graph_managers[i]);
    }


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("Sorts");

    glutDisplayFunc(Draw);
    glutReshapeFunc(Resize);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(MousePassiveMotion);

    atexit(Clear);
    glutMainLoop();

    return 0;
}