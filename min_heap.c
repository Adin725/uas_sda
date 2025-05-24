// min_heap.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// Heapify untuk min-heap (frekuensi terkecil di root)
void min_heapify(WordFreq* arr, int n, int i) {
    int smallest = i; // Anggap root sebagai terkecil
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Cek anak kiri, pilih frekuensi lebih kecil atau kata alfabetis lebih besar
    if (left < n && (arr[left].freq < arr[smallest].freq || 
        (arr[left].freq == arr[smallest].freq && strcmp(arr[left].word, arr[smallest].word) > 0))) {
        smallest = left;
    }

    // Cek anak kanan, pilih frekuensi lebih kecil atau kata alfabetis lebih besar
    if (right < n && (arr[right].freq < arr[smallest].freq || 
        (arr[right].freq == arr[smallest].freq && strcmp(arr[right].word, arr[smallest].word) > 0))) {
        smallest = right;
    }

    // Jika ada yang lebih kecil, tukar dan lanjut heapify
    if (smallest != i) {
        swap(&arr[i], &arr[smallest]);
        min_heapify(arr, n, smallest);
    }
}

// Fungsi untuk mendapatkan k kata teratas
WordFreq* get_top_k(WordFreq* words, int W, int k) {
    // Alokasi memori untuk hasil
    WordFreq* result = (WordFreq*)malloc(k * sizeof(WordFreq));
    if (!result) {
        printf("Error: Gagal mengalokasikan memori untuk top-k!\n");
        return NULL;
    }

    // Salin k elemen pertama ke result
    for (int i = 0; i < k; i++) {
        result[i] = words[i];
    }

    // Bangun min-heap dari k elemen
    for (int i = k / 2 - 1; i >= 0; i--) {
        min_heapify(result, k, i);
    }

    // Proses sisa elemen
    for (int i = k; i < W; i++) {
        // Jika frekuensi lebih besar atau sama tetapi kata alfabetis lebih kecil
        if (words[i].freq > result[0].freq || 
            (words[i].freq == result[0].freq && strcmp(words[i].word, result[0].word) < 0)) {
            result[0] = words[i]; // Ganti root
            min_heapify(result, k, 0); // Perbaiki min-heap
        }
    }

    // Urutkan hasil secara descending
    for (int i = k - 1; i > 0; i--) {
        swap(&result[0], &result[i]);
        min_heapify(result, i, 0);
    }

    return result;
}