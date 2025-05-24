// sorting.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

// Fungsi untuk menukar dua elemen
void swap(WordFreq* a, WordFreq* b) {
    WordFreq temp = *a;
    *a = *b;
    *b = temp;
}

// Insertion Sort (urut descending: frekuensi besar ke kecil, alfabetis jika sama)
void insertion_sort(WordFreq* arr, int n) {
    for (int i = 1; i < n; i++) {
        WordFreq key = arr[i];
        int j = i - 1;
        while (j >= 0 && (arr[j].freq < key.freq || (arr[j].freq == key.freq && strcmp(arr[j].word, key.word) > 0))) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Quick Sort
int partition(WordFreq* arr, int low, int high) {
    WordFreq pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].freq > pivot.freq || (arr[j].freq == pivot.freq && strcmp(arr[j].word, pivot.word) < 0)) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quick_sort(WordFreq* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}

// Heapify untuk max-heap (frekuensi terbesar di root)
void heapify(WordFreq* arr, int n, int i) {
    int largest = i; // Anggap root sebagai terbesar
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Cek anak kiri, pilih frekuensi lebih besar atau kata alfabetis lebih kecil
    if (left < n && (arr[left].freq > arr[largest].freq || 
        (arr[left].freq == arr[largest].freq && strcmp(arr[left].word, arr[largest].word) < 0))) {
        largest = left;
    }

    // Cek anak kanan, pilih frekuensi lebih besar atau kata alfabetis lebih kecil
    if (right < n && (arr[right].freq > arr[largest].freq || 
        (arr[right].freq == arr[largest].freq && strcmp(arr[right].word, arr[largest].word) < 0))) {
        largest = right;
    }

    // Jika ada yang lebih besar, tukar dan lanjut heapify
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

// Heap Sort menggunakan max-heap
void heap_sort(WordFreq* arr, int n) {
    // Bangun max-heap (frekuensi terbesar di root)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Ambil elemen terbesar satu per satu dan tempatkan di akhir array
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);     // Pindahkan root (terbesar) ke akhir array
        heapify(arr, i, 0);         // Bangun ulang max-heap untuk sisa elemen
    }

    // Setelah proses di atas, array menjadi ascending → kita balik agar jadi descending
    for (int i = 0; i < n / 2; i++) {
        swap(&arr[i], &arr[n - i - 1]);
    }
}
