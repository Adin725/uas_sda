// header.h
#ifndef HEADER_H
#define HEADER_H

#define MAX_FILENAME_LENGTH 100
#define MAX_WORD_LENGTH 50

// Struktur untuk menyimpan kata dan frekuensinya
typedef struct {
    char* word;
    int freq;
} WordFreq;

// Fungsi validasi
int validate_dataset(const char* dataset);
int validate_k(int k, int W);
int validate_option(int option);
void write_output(const char* filename, WordFreq* words, int k, double time_ms);
void display_output(WordFreq* words, int k, double time_ms);

// Fungsi pemrosesan dataset
int read_vocab_file(const char* filename, char** vocab, int W);
int read_docword_file(const char* filename, int* freq, int W);
int load_dataset(const char* dataset, char*** vocab, int** freq, int* W);
WordFreq* create_wordfreq_array(char** vocab, int* freq, int W);
void free_dataset(char** vocab, int* freq, WordFreq* words, int W);
WordFreq* copy_wordfreq_array(WordFreq* words, int W);

// Fungsi pengurutan
void swap(WordFreq* a, WordFreq* b); // Deklarasi swap untuk digunakan di sorting dan min_heap
void insertion_sort(WordFreq* arr, int n);
int partition(WordFreq* arr, int low, int high);
void quick_sort(WordFreq* arr, int low, int high);
void heapify(WordFreq* arr, int n, int i);
void heap_sort(WordFreq* arr, int n);

// Fungsi untuk top-k
WordFreq* get_top_k(WordFreq* words, int W, int k);

#endif