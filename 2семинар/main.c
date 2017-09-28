/*В аргументе командной строки передаётся имена текстовых файлов в которых записаны двумерные матрицы вещественных чисел. Необходимо напечатать на стандартный выходной поток результат Max-нормы матрицы, находящейся во входном файле:
max по i ∑ по j | a[i][j] |
Формат хранения матриц в файлах – разреженный – в первой строке файла записано количество строк матрицы; во второй – количество столбцов матрицы; в третьей – количество ненулевых элементов матрицы; в последующих строках файла записаны индексы и значения этих ненулевых элементов матрицы (информация о каждом элементе матрицы записана на отдельной строке) в следующей последовательности: номер_строки, номер_столбца, значение_элемента. Считать, что пары индексов элементов матрицы в файле упорядочены по возрастанию. В качестве внутреннего представления матрицы в памяти программы использовать двумерный массив вещественных чисел. Целевой алгоритм не должен зависеть от представления матрицы в файле и в памяти, т.е. для работы с матрицей должны быть реализованы и использованы в алгоритме функции: double get_elem(void *matr, int row, int col), которая возвращает значение элемента матрицы по его индексам, и void set_elem(void *matr, int row, int col, double elem), которая устанавливает значение элемента матрицы по его индексам.*/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <float.h>
#include <math.h>

double **create_matrix(int m_column, int m_line){ //выделяет память или возвращает NULL в случае ошибки
    //создаём массив указателей на double
    double **result = (double **)calloc((size_t)m_column, sizeof(double *));
    if (result == NULL){
        fprintf(stderr, "No ability allocate memory\n");
        return NULL;
    }
    for(int i = 0; i < m_column; i++){
        result[i] = (double *)calloc((size_t)m_line, sizeof(double));
        if (result == NULL){
            fprintf(stderr, "No ability allocate memory\n");
            return NULL;
        }
    }
    return result;
}

double get_elem(void *matr, int row, int col){
    //возвращает значение элемента матрицы по его индексам;
    double **matrix = matr;
    return matrix[row][col]; //"слишком просто, что бы быть правдой!" - но, как оказалось, это так и работает.
}

void set_elem(void *matr, int row, int col, double elem){
    //устанавливает значение элемента матрицы по его индексам
    double **matrix = matr;
    matrix[row][col] = elem;
}

double matrix_max_norm(void * matr, int row, int col){
    //max по i ∑ по j | a[i][j] |
    double max_norm = -DBL_MAX;
    double **matrix = matr;
    for(int i = 0; i < row; i++){
        double sum = 0;
        for(int j = 0; j < col; j++){
            sum += fabs(get_elem(matrix, i, j));
        }
        max_norm = max_norm > sum? max_norm: sum;
    }
    return max_norm;
}

int main(int argc, char * argv[]) {
    //открываем файл
    if (argc != 2){
        fprintf(stderr, "Неверное количество аргументов, должно быть только имя файла\n");
        return EINVAL; //22 - Invalid argument
    }
    FILE *input = fopen(argv[1], "r");
    if(input == NULL){
        if(errno = ENOENT){
            fprintf(stderr, "No such file or directory\n");
        } else if(errno == EACCES) {
            fprintf(stderr, "Permission denied\n");
        }
        return errno;
    }
    int m_column = 0, m_line = 0, m_non_zero_elements = 0;
    int number_of_reads = fscanf(input, "number_of_columns = %d\n", &m_column);
    if(number_of_reads != 1){
        fprintf(stderr, "ошибка при чтении парамера number_of_columns\n");
        return ENODATA; // 61 - No data available
    }
    number_of_reads = fscanf(input, "number_of_lines = %d\n", &m_line);
    if(number_of_reads != 1){
        fprintf(stderr, "ошибка при чтении парамера number_of_lines\n");
        return ENODATA; // 61 - No data available
    }
    number_of_reads = fscanf(input, "non_zero_elements = %d\n", &m_non_zero_elements);
    if(number_of_reads != 1){
        fprintf(stderr, "ошибка при чтении парамера non_zero_elements\n");
        return ENODATA; // 61 - No data available
    }

    //тут мы знаем размер и создаём матрицу
    double **matrix = create_matrix(m_column, m_line);
    if(matrix == NULL){
        fprintf(stderr, "Not able to allocate memory.\n");
        return ENOMEM; 	//12 - Out of memory
    }

    //теперь считываем в матрицу числа
    for(int i = 0; i < m_non_zero_elements; i++){
        int column = 0, line = 0;
        double elem = 0;
        number_of_reads = fscanf(input, "matrix[%d][%d] = %lf\n", &line, &column, &elem);
        if(number_of_reads != 3){
            fprintf(stderr, "ошибка при чтении matrix[][] на позиции %d\n", i);
            return ENODATA; // 61 - No data available
        }
        if(line >= m_line || line < 0 || column >= m_column || column < 0){
            fprintf(stderr, "индекс за пределами матрицы на позиции %d\n", i);
            return ENODATA; // 61 - No data available
        }
        set_elem(matrix, line, column, elem);
    }
    fclose(input);

    //test output
    for(int i = 0; i < m_line; i++){
        for(int j = 0; j < m_column; j++){
            printf("%5.1f ", get_elem(matrix, i, j));
        }
        printf("\n");
    }

    printf("Макс-норма матрицы = %.2lf", matrix_max_norm(matrix, m_line, m_column));

    return 0;
}