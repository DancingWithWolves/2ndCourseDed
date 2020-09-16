#include "sortFunctions.h"
#include "windowManager.h"
#include <stdio.h>

/**
 * Выводит на экран зависимость количества swap'ов и сравнений разных сортировок от количества элементов с использованием простейшей графической визуализации
 */

// Глобальные переменные -- количество сравнений и свапов
int compares_qty = 0, swaps_qty = 0;


const int default_margin = 5;

const int button_height = 55;

const int buttons_qty = 3;



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
        , "Bubble sort"
        , ShowBubbleSortStat);

    button = new (buttons + 1) Button 
        ( window_width / buttons_qty + default_margin
        , default_margin
        , window_width / buttons_qty - 2 * default_margin
        , button_height
        , "Insertion sort"
        , ShowInsertionSortStat);

    button = new (buttons + 2) Button 
        ( window_width * 2 / buttons_qty + default_margin
        , default_margin
        , window_width / buttons_qty - 2 * default_margin
        , button_height
        , "Quick sort"
        , ShowQuickSortStat);

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