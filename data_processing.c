// data_processing.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// Fungsi untuk membaca file vocab (daftar kata)
int read_vocab_file(const char* filename, char** vocab, int W) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Tidak dapat membuka file %s!\n", filename);
        return 0;
    }

    for (int i = 0; i < W; i++) {
        vocab[i] = (char*)malloc(MAX_WORD_LENGTH * sizeof(char));
        if (!vocab[i]) {
            printf("Error: Gagal mengalokasikan memori untuk vocab!\n");
            for (int j = 0; j < i; j++) {
                free(vocab[j]);
            }
            fclose(file);
            return 0;
        }
        if (fscanf(file, "%s\n", vocab[i]) != 1) {
            printf("Error: Gagal membaca kata dari %s!\n", filename);
            for (int j = 0; j <= i; j++) {
                free(vocab[j]);
            }
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}

// Fungsi untuk membaca file docword (frekuensi kata)
int read_docword_file(const char* filename, int* freq, int W) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Tidak dapat membuka file %s!\n", filename);
        return 0;
    }

    int D, N;
    if (fscanf(file, "%d\n%d\n%d\n", &D, &W, &N) != 3) {
        printf("Error: Format file %s tidak valid!\n", filename);
        fclose(file);
        return 0;
    }

    int docID, wordID, count;
    for (int i = 0; i < N; i++) {
        if (fscanf(file, "%d %d %d\n", &docID, &wordID, &count) != 3) {
            printf("Error: Gagal membaca baris dari %s!\n", filename);
            fclose(file);
            return 0;
        }
        // Tambah frekuensi untuk wordID yang valid
        if (wordID >= 1 && wordID <= W) {
            freq[wordID - 1] += count;
        }
    }
    fclose(file);
    return 1;
}

// Fungsi untuk memuat dataset
int load_dataset(const char* dataset, char*** vocab, int** freq, int* W) {
    char docword_path[MAX_FILENAME_LENGTH];
    char vocab_path[MAX_FILENAME_LENGTH];
    snprintf(docword_path, MAX_FILENAME_LENGTH, "datasets/docword.%s.txt", dataset);
    snprintf(vocab_path, MAX_FILENAME_LENGTH, "datasets/vocab.%s.txt", dataset);

    FILE* file = fopen(docword_path, "r");
    if (!file) {
        printf("Error: Tidak dapat membuka file %s!\n", docword_path);
        return 0;
    }
    int D, N;
    if (fscanf(file, "%d\n%d\n%d\n", &D, W, &N) != 3) {
        printf("Error: Format file %s tidak valid!\n", docword_path);
        fclose(file);
        return 0;
    }
    fclose(file);

    // Alokasi memori untuk vocab dan freq
    *vocab = (char**)malloc(*W * sizeof(char*));
    *freq = (int*)calloc(*W, sizeof(int)); // Inisialisasi freq dengan 0
    if (!*vocab || !*freq) {
        printf("Error: Gagal mengalokasikan memori untuk vocab atau freq!\n");
        free(*vocab);
        free(*freq);
        return 0;
    }

    if (!read_vocab_file(vocab_path, *vocab, *W) || !read_docword_file(docword_path, *freq, *W)) {
        free(*vocab);
        free(*freq);
        return 0;
    }

    return 1;
}

// Fungsi untuk membuat array WordFreq
WordFreq* create_wordfreq_array(char** vocab, int* freq, int W) {
    WordFreq* words = (WordFreq*)malloc(W * sizeof(WordFreq));
    if (!words) {
        printf("Error: Gagal mengalokasikan memori untuk words!\n");
        return NULL;
    }

    // Isi array dengan semua kata dan frekuensinya
    for (int i = 0; i < W; i++) {
        words[i].word = vocab[i];
        words[i].freq = freq[i];
    }
    return words;
}

// Fungsi untuk menyalin array WordFreq
WordFreq* copy_wordfreq_array(WordFreq* words, int W) {
    // Alokasi memori untuk array baru
    WordFreq* copy = (WordFreq*)malloc(W * sizeof(WordFreq));
    if (!copy) {
        printf("Error: Gagal mengalokasikan memori untuk copy_wordfreq_array!\n");
        return NULL;
    }

    // Salin setiap elemen
    for (int i = 0; i < W; i++) {
        copy[i].word = words[i].word;
        copy[i].freq = words[i].freq;
    }
    return copy;
}

// Fungsi untuk membersihkan memori
void free_dataset(char** vocab, int* freq, WordFreq* words, int W) {
    if (vocab) {
        for (int i = 0; i < W; i++) {
            if (vocab[i]) free(vocab[i]);
        }
        free(vocab);
    }
    if (freq) free(freq);
    if (words) free(words);
}