#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#pragma execution_character_set("utf-8")

#define N 10000
#define K 3274
#define DATA_FILE "Number_10000.txt"

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int  high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

int quickSelect(int arr[], int low, int high, int k) {
    if (low == high) {
        return arr[low];
    }

    int pivotIndex = low + rand() % (high - low + 1);
    swap(&arr[pivotIndex], &arr[high]);

    pivotIndex = partition(arr, low, high);

    if (pivotIndex == k) {
        return arr[pivotIndex];
    }
    else if (k < pivotIndex) {
        return quickSelect(arr, low, pivotIndex - 1, k);
    }
    else {
        return quickSelect(arr, pivotIndex + 1, high, k);
    }
}

// 현재 폴더에서 먼저 찾고, 없으면 실행 파일이 있는 폴더에서 찾는다
FILE* openDataFile(const char* name) {
    FILE* fp = fopen(name, "r");
    if (fp != NULL) {
        return fp;
    }

    char path[MAX_PATH];
    DWORD len = GetModuleFileNameA(NULL, path, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) {
        return NULL;
    }

    char* slash = strrchr(path, '\\');
    if (slash == NULL || (size_t)(slash - path) + 1 + strlen(name) >= MAX_PATH) {
        return NULL;
    }

    strcpy(slash + 1, name);
    return fopen(path, "r");
}

int main(void) {
    SetConsoleOutputCP(CP_UTF8);

    HANDLE hOut = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD mode;
        if (GetConsoleMode(hOut, &mode)) {
            SetConsoleMode(hOut, mode | ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
        }
        CloseHandle(hOut);
    }

    int arr[N];
    FILE *fp = openDataFile(DATA_FILE);

    if (fp == NULL) {
        printf("%s 파일을 열 수 없습니다.\n", DATA_FILE);
        return 1;
    }

    for (int i = 0; i < N; i++) {
        if (fscanf(fp, "%d", &arr[i]) != 1) {
            printf("파일의 데이터가 부족하거나 잘못되었습니다.\n");
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    srand((unsigned int)time(NULL));

    int result = quickSelect(arr, 0, N - 1, K - 1);

    printf("전체 데이터 개수: %d\n", N);
    printf("%d번째로 작은 값: %d\n", K, result);

    return 0;
}