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

}

/*!
*  Обрабатывает функцию InsertionSort()
*/
void ShowInsertionSortStat()
{
}

/*!
*  Обрабатывает функцию QuickSort()
*/
void ShowQuickSortStat()
{
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
        graphs[i].~GraphManager();
    }
    ::operator delete(graphs);
}

int main(int argc, char** argv)
{

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



    graphs = reinterpret_cast<GraphManager*>(::operator new(sizeof(GraphManager) * graphs_qty));

    GraphManager* graphManager = nullptr;
    graphManager = new (graphs) GraphManager
        ( default_margin
        , button_height + default_margin * 4 
        , window_width / 2 - 2 * default_margin
        , graph_height
        , default_margin
        , default_margin
        , "arf"
        , "arfff"
        , "arffff"
        , -5
        , 5
        , -5
        , 5
        );

    graphManager = new (graphs + 1) GraphManager
        ( default_margin + graphs[0].width + 2 * default_margin
        , graphs[0].y
        , graphs[0].width
        , graph_height
        , default_margin
        , default_margin
        , "123"
        , "12345"
        , "123456"
        , 0
        , 0
        , 0
        , 0
        );

    for (int i = 0; i < graphs_qty; ++i) {
        drawable_list.push_front(&graphs[i]);
    }
    const float x_val[] = {1, 2, 3, 4, 5};
    const float y_val[] = {2, 3, 2, 1, 4};


    const float x_val1[] = {2, 3, 4, 1, 5};
    const float y_val1[] = {15, 11, 12, 14, 14};

    graphs[0].AddGraph(5, x_val1, y_val1, false);
    graphs[0].AddGraph(5, x_val, y_val);


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