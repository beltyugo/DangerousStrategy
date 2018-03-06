/*
Алгоритм:
1. Сделайте начальную клетку текущей и отметьте ее как посещенную.
2. Пока есть непосещенные клетки
  1. Если текущая клетка имеет непосещенных «соседей»
    1. Протолкните текущую клетку в стек
    2. Выберите случайную клетку из соседних
    3. Уберите стенку между текущей клеткой и выбранной
    4. Сделайте выбранную клетку текущей и отметьте ее как посещенную.
  2. Иначе если стек не пуст
    1. Выдерните клетку из стека
    2. Сделайте ее текущей
  3. Иначе (не реализованно)
    1. Выберите случайную непосещенную клетку,
          сделайте ее текущей и отметьте как посещенную.
*/

/* Суть: вся карта состоит из блоков, каждый из которых является матрицей 3х3 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* Высота, ширина лабиринта */
#define WIDTH 29
#define HEIGHT 13
/* Размер стека */
#define MAX_SIZE_STACK (WIDTH * HEIGHT)
/* Структура клетки 3х3 */
typedef struct big_cell {
  char cells[3][3];                                 //массив клеток, из которых состоит клетка 3х3
  int empty;                                        //пустота(нет стены):  1 - да, 0 - нет
  int visited;                                      //посещенность: 1 - да, 0 - нет
} big_cell_t;
/* Структура стека */
typedef struct stack {                              //главный маршрут - путь из начальной клетки до конечной, без учета тупиков
  big_cell_t *data[MAX_SIZE_STACK];                 //массив ссылок на клетки главного маршрута
  int size;                                         //количество клеток на главном маршруте
} stack_t;
/* Структура "соседей" */
typedef struct neighbors {
  int left;
  int right;
  int top;
  int bottom;
} neighbor_t;



/* Положить адрес клетки в стек */
void push(stack_t *stack, big_cell_t *cell);
/* Выдернуть адрес клетки из стека */
big_cell_t *pop(stack_t *stack);
/* Проверка на непосещенных соседей клетки */
int checkNeighbor(big_cell_t *current_cell, big_cell_t *big_maze, neighbor_t *neighbors);
/* Отобразить лабиринт */
void print_maze(big_cell_t *big_maze);
/* Первичное заполнение структуры лабиринта */
void init_maze(big_cell_t *big_maze, int *count_un);
/* Очистить клетку 3х3 */
void destroy_cell(big_cell_t *current_cell, int distance, char symbol);

int main() {
    big_cell_t *current_cell = NULL;                //указатель на текущую клетку
    int count_un = 0;                               //количество непроверенных клеток
    big_cell_t big_maze[HEIGHT * WIDTH];            //массив всех клеток лабиринта

    int done;                                       //метка выхода из цикла
    srand(time(NULL));                              //использование текущего времени
                                                    //для генерации случайных чисел

    stack_t stack;                                  //создаем структуру стека
    stack.size = 0;                                 //количество элементов стека = 0

    neighbor_t neighbors;                           //создаем структуру "соседей"
    /* Первичное заполнение лабиринта */
    init_maze(big_maze, &count_un);
    current_cell = &(*(big_maze + 1 * WIDTH + 1));  //текущая клетка - (1,1)
    (*current_cell).visited = 1;                    //текущая клетка посещена
    count_un--;                                     //уменьшаем количество непроверенных клеток
    /* Работа алгоритма */
    while(count_un) {
        /* Если есть непосещенные соседи */
        if (checkNeighbor(current_cell, big_maze, &neighbors) == 1) {
            /* Положить текущую клетки в стек */
            push(&stack, current_cell);
            done = 1;                               //флаг для работы выбора соседа
            /* Выбор соседа */
            while(done) {
                switch(rand() % 4 + 1) {            //случайное число от 1 до 4
                    case 1:
                        /* Если слева непосещенный сосед */
                        if(neighbors.left == 1) {
                            /* Убрать стену */
                            destroy_cell(current_cell, -1, ' ');
                            /* Сделать соседа текущей клеткой */
                            current_cell = &(*(current_cell - 2));
                            /* Выходим из цикла */
                            done = 0;
                        }
                        break;
                    case 2:
                        /* Если справа непосещенный сосед */
                        if(neighbors.right == 1) {
                            destroy_cell(current_cell, 1, ' ');
                            current_cell = &(*(current_cell + 2));
                            done = 0;
                        }
                        break;
                    case 3:
                        /* Если снизу непосещенный сосед */
                        if(neighbors.bottom == 1) {
                            destroy_cell(current_cell, WIDTH, ' ');
                            current_cell = &(*(current_cell + 2 * WIDTH));
                            done = 0;
                        }
                        break;
                    case 4:
                        /* Если сверху непосещенный сосед */
                        if(neighbors.top == 1) {
                            destroy_cell(current_cell, -WIDTH, ' ');
                            current_cell = &(*(current_cell - 2 * WIDTH));
                            done = 0;
                        }
                        break;
                }
            }
            (*(current_cell)).visited = 1;          //текущая клетка посещена
            count_un--;                             //уменьшаем количество непроверенных клеток
          /* Если непосещенных соседей нет и стек не пуст */
          /* Нужно для выхода из тупиков */
        } else if(stack.size != 0) {
            /* Вытаскиваем клетку из стека и делаем ее текущей */
            current_cell = pop(&stack);
          /* Иначе завершаем работу алгоритма */
        } else {
            break;
        }
    }
    /* Отобразить лабиринт */
    print_maze(big_maze);
    return 0;
}

void push(stack_t *stack, big_cell_t *cell) {
    stack->data[stack->size] = cell;                //кладем адрес клетки в стек
    stack->size++;                                  //увеличиваем количество элементов стека
}

big_cell_t *pop(stack_t *stack) {
  stack->size--;                                    //уменьшаем количество элементов стека
  return stack->data[stack->size];                  //вытаскиваем вверхнюю клетку
}

void destroy_cell(big_cell_t *current_cell, int distance, char symbol) {
    /* Меняем символы матрицы 3х3 */
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            (*(current_cell + distance)).cells[i][j] = symbol;
        }
    }
}

int checkNeighbor(big_cell_t *current_cell, big_cell_t *big_maze, neighbor_t *neighbors) {
    /* Если слева есть непосещенный сосед */
    if((*(current_cell - 2)).empty == 1 && (*(current_cell - 2)).visited == 0) {
        neighbors->left = 1;
    } else { neighbors->left = 0; }
    /* Если справа есть непосещенный сосед */
    if((*(current_cell + 2)).empty == 1 && (*(current_cell + 2)).visited == 0) {
        neighbors->right = 1;
    } else { neighbors->right = 0; }
    /* Если снизу есть непосещенный сосед */
    if((current_cell - big_maze) / WIDTH < HEIGHT - 2
        && (*(current_cell + 2 * WIDTH)).empty == 1
        && (*(current_cell + 2 * WIDTH)).visited == 0) {
        neighbors->bottom = 1;
    } else { neighbors->bottom = 0; }
    /* Если сверху есть непосещенный сосед */
    if((current_cell - big_maze) / WIDTH > 1
        && (*(current_cell - 2 * WIDTH)).empty == 1
        && (*(current_cell - 2 * WIDTH)).visited == 0) {
        neighbors->top = 1;
    } else { neighbors->top = 0; }

    return neighbors->left == 1 || neighbors->right == 1 || neighbors->bottom == 1
        || neighbors->top == 1 ? 1 : 0;
}

void init_maze(big_cell_t *big_maze, int *count_un) {
    /* Первичное заполнение лабиринта */
    for(int i = 0; i < HEIGHT * WIDTH; i+=WIDTH) {
        for (int j = 0; j < WIDTH; j++) {
            if ((i > 0 && j > 0)
                && (i < HEIGHT * WIDTH - 1 && j < WIDTH - 1)
                && (i % 2 != 0 && j % 2 != 0)) {
                /* Матрица 3х3 */
                for(int k = 0; k < 3; k++) {
                  for (int h = 0; h < 3; h++) {
                      (*(big_maze + i + j)).cells[k][h] = ' ';
                  }
                }
                (*(big_maze + i + j)).empty = 1;    //клетка пуста
                (*(big_maze + i + j)).visited = 0;  //клетка непосещена
                (*count_un)++;                      //считаем непосещенную клетку
            } else {
                /* Матрица 3х3 */
                for(int k = 0; k < 3; k++) {
                  for (int h = 0; h < 3; h++) {
                      (*(big_maze + i + j)).cells[k][h] = '#';
                  }
                }
                (*(big_maze + i + j)).empty = 0;    //клетка - стена
            }
        }
    }
}

/* Нужно доработать */
void print_maze(big_cell_t *big_maze) {
    int h;
    for(int i = 0; i < HEIGHT * WIDTH; i += WIDTH) {
        h = 0;
        for(int j = 0; j < WIDTH; j++) {
            for(int k = 0; k < 3; k++) {
                    printf("%c", (*(big_maze + i + j)).cells[h][k]);
            }
            if(h == 2 && j == WIDTH - 1) {
                printf("\n");
                break;
            }
            if(j == WIDTH - 1) {
                printf("\n");
                j = -1;
                h++;
            }
        }
    }
}
