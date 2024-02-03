#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <iostream>

using namespace std;

struct Data {
    int line_count;
    int column_count;
    long long value;
};

void Zfunc(long long* array, int* zfunc, int size) {
    int left = 0;
    int right = 0;
    for (int i = 0; i < size; ++i) {
        zfunc[i] = 0;
    }
    for (int index = 1; index < size; ++index) {
        zfunc[index] = max(0, min(right - index, zfunc[index - left]));
        while (zfunc[index] + index < size && array[zfunc[index]] == array[index + zfunc[index]]) {
            zfunc[index]++;
        }
        if (index + zfunc[index] > right) {
            left = index;
            right = zfunc[index] + index;
        }
    }
}

void Print(Data* array, int size) {
    for (int i = 0; i < size; ++i) {
        cout << array[i].value << " " << array[i].line_count << " " << array[i].column_count << "\n";
    }
}

void StringFind(Data* pattern, Data* array, int pattern_size, int array_size) {
    long long merged_array[array_size + pattern_size + 1];
    int zfunc_array[pattern_size + array_size + 1];
    for (int index = 0; index < pattern_size; ++index) {
        merged_array[index] = pattern[index].value;
    }
    merged_array[pattern_size] = -1;
    for (int index = 0; index < array_size; ++index) {
        merged_array[index + pattern_size + 1] = array[index].value;
    }
    Zfunc(merged_array, zfunc_array, pattern_size + array_size + 1);
    for (int i = pattern_size + 1; i < array_size + pattern_size + 1; ++i) {
        if (zfunc_array[i] == pattern_size) {
            cout << array[i - pattern_size - 1].line_count << ", "
                 << array[i - pattern_size - 1].column_count << '\n';
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    Data* array = (Data*)malloc(sizeof(Data));
    Data* pattern;
    char buffer = '\0';
    long long number = 0;
    int is_number = 0;
    int size = 0;
    int pattern_size;
    int max_size = 1;
    int is_pattern = 1;
    int line_number = 1;
    int column_number = 1;
    while (buffer != EOF) {
        buffer = getchar();
        if (size == max_size) {
            max_size *= 2;
            array = (Data*)realloc(array, max_size * sizeof(Data));
        }
        if (buffer >= '0' && buffer <= '9') {
            is_number = 1;
            number = number * 10 + (int)(buffer - '0');

        } else if (buffer == '\n') {
            if (is_number) {
                array[size].value = number;
                array[size].column_count = column_number;
                array[size].line_count = line_number;
                number = 0;
                is_number = 0;
                size++;
            }
            ++line_number;
            column_number = 1;
            if (is_pattern) {
                pattern = array;
                pattern_size = size;
                is_pattern = 0;
                size = is_number = number = 0;
                max_size = 1;
                array = (Data*)malloc(sizeof(Data));
                column_number = 1;
                line_number = 1;
            }

        } else if (buffer != EOF) {
            if (is_number) {
                array[size].value = number;
                array[size].column_count = column_number;
                array[size].line_count = line_number;
                column_number++;
                size++;
            }
            number = 0;
            is_number = 0;
        }
    }

    if (is_number) {
        array[size].value = number;
        array[size].column_count = column_number;
        array[size].line_count = line_number;
        number = 0;
        is_number = 0;
        size++;
    }

    unsigned int start_time = clock();
    unsigned int end_time;
    StringFind(pattern, array, pattern_size, size);
    end_time = clock();
    cout << "Time: " << end_time - start_time << "\n";
    free(array);
    free(pattern);
    return 0;
}