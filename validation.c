// validation.c
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <errno.h>
#include "header.h"

// Cek apakah dataset valid
int validate_dataset(const char* dataset) {
    return (strcmp(dataset, "kos") == 0 ||
            strcmp(dataset, "nips") == 0 ||
            strcmp(dataset, "enron") == 0 ||
            strcmp(dataset, "nytimes") == 0 ||
            strcmp(dataset, "pubmed") == 0);
}

// Cek apakah k valid (11-149 dan <= W)
int validate_k(int k, int W) {
    return (k > 10 && k < 150 && k <= W);
}

// Cek apakah opsi valid (1-6)
int validate_option(int option) {
    return (option >= 1 && option <= 6);
}

// Tulis hasil ke file
void write_output(const char* filename, WordFreq* words, int k, double time_ms) {
    // Buat folder output jika belum ada
    if (_mkdir("output") != 0 && errno != EEXIST) {
        printf("Error: Tidak dapat membuat folder output! (errno: %d)\n", errno);
        return;
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Tidak dapat membuka file %s untuk menulis! (errno: %d)\n", filename, errno);
        return;
    }

    // Tulis kata dan frekuensi
    for (int i = 0; i < k; i++) {
        fprintf(file, "%s (%d)\n", words[i].word, words[i].freq);
    }
    // Tulis waktu dalam format presisi tinggi
    fprintf(file, "Waktu untuk mengurutkan:\n");
    fprintf(file, "  %.6f milidetik\n", time_ms);
    fprintf(file, "  %.6f detik\n", time_ms / 1000.0);
    if (time_ms >= 60000.0) {
        fprintf(file, "  %.6f menit\n", time_ms / (1000.0 * 60.0));
    }

    fclose(file);
    printf("File %s berhasil dibuat!\n", filename);
}

// Tampilkan hasil ke layar
void display_output(WordFreq* words, int k, double time_ms) {
    for (int i = 0; i < k; i++) {
        printf("%s (%d)\n", words[i].word, words[i].freq);
    }
    // Tampilkan waktu dalam format presisi tinggi
    printf("Waktu untuk mengurutkan:\n");
    printf("  %.6f milidetik\n", time_ms);
    printf("  %.6f detik\n", time_ms / 1000.0);
    if (time_ms >= 60000.0) {
        printf("  %.6f menit\n", time_ms / (1000.0 * 60.0));
    }
}