// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "header.h"

int main() {
    char dataset[MAX_FILENAME_LENGTH];
    char** vocab = NULL;
    int* freq = NULL;
    WordFreq* words = NULL;
    int W = 0;
    int loaded = 0;

    while (1) {
        // Minta input nama dataset
        printf("Masukkan nama dataset (kos, nips, enron, nytimes, pubmed, atau 'exit' untuk keluar): ");
        if (scanf("%s", dataset) != 1) {
            while (getchar() != '\n');
            printf("Error: Input tidak valid! Coba lagi.\n");
            continue;
        }

        if (strcmp(dataset, "exit") == 0) {
            break;
        }

        if (!validate_dataset(dataset)) {
            printf("Error: Dataset tidak valid! Pilih: kos, nips, enron, nytimes, pubmed.\n");
            continue;
        }

        // Bersihkan data sebelumnya jika ada
        if (loaded) {
            free_dataset(vocab, freq, words, W);
            vocab = NULL;
            freq = NULL;
            words = NULL;
            W = 0;
        }

        // Muat dataset
        if (!load_dataset(dataset, &vocab, &freq, &W)) {
            continue;
        }
        words = create_wordfreq_array(vocab, freq, W);
        if (!words) {
            free_dataset(vocab, freq, NULL, W);
            continue;
        }
        loaded = 1;

        while (1) {
            // Tampilkan menu
            printf("\nPilihan:\n");
            printf("1) Urutkan dengan insertion sort\n");
            printf("2) Urutkan dengan quicksort\n");
            printf("3) Urutkan dengan heapsort\n");
            printf("4) Tampilkan k kata dengan frekuensi terbesar\n");
            printf("5) Ganti dataset\n");
            printf("6) Selesai\n");
            printf("Pilihan anda: ");

            int option;
            if (scanf("%d", &option) != 1) {
                while (getchar() != '\n');
                printf("Error: Input opsi harus angka! Coba lagi.\n");
                continue;
            }
            while (getchar() != '\n');

            if (!validate_option(option)) {
                printf("Error: Opsi tidak valid! Pilih 1-6.\n");
                continue;
            }

            if (option == 5) {
                break;
            }

            if (option == 6) {
                free_dataset(vocab, freq, words, W);
                return 0;
            }

            char output_file[MAX_FILENAME_LENGTH];
            WordFreq* sorted_words = NULL;
            clock_t start, end;
            double time_ms;

            if (option == 4) {
                // Minta input k untuk top-k
                printf("Masukkan jumlah kata (10 < k < 150): ");
                int k;
                if (scanf("%d", &k) != 1) {
                    while (getchar() != '\n');
                    printf("Error: Input k harus angka! Coba lagi.\n");
                    continue;
                }
                while (getchar() != '\n');

                if (!validate_k(k, W)) {
                    printf("Error: Nilai k harus antara 11 dan 149, dan tidak lebih dari %d! Coba lagi.\n", W);
                    continue;
                }

                start = clock();
                sorted_words = get_top_k(words, W, k);
                end = clock();
                time_ms = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;

                if (!sorted_words) {
                    printf("Error: Gagal mendapatkan k kata teratas!\n");
                    continue;
                }

                display_output(sorted_words, k, time_ms);
                snprintf(output_file, MAX_FILENAME_LENGTH, "output/top_%d_words_%s.txt", k, dataset);
                write_output(output_file, sorted_words, k, time_ms);
                free(sorted_words);
            } else {
                // Salin array untuk sorting
                sorted_words = copy_wordfreq_array(words, W);
                if (!sorted_words) {
                    continue;
                }

                int k = W; // Urutkan semua kata

                start = clock();
                if (option == 1) {
                    insertion_sort(sorted_words, W);
                    snprintf(output_file, MAX_FILENAME_LENGTH, "output/sorting_by_insertion_%s.txt", dataset);
                } else if (option == 2) {
                    quick_sort(sorted_words, 0, W - 1);
                    snprintf(output_file, MAX_FILENAME_LENGTH, "output/sorting_by_quicksort_%s.txt", dataset);
                } else if (option == 3) {
                    heap_sort(sorted_words, W);
                    snprintf(output_file, MAX_FILENAME_LENGTH, "output/sorting_by_heapsort_%s.txt", dataset);
                }
                end = clock();
                time_ms = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;

                write_output(output_file, sorted_words, k, time_ms);
                free(sorted_words);
            }
        }
    }

    if (loaded) {
        free_dataset(vocab, freq, words, W);
    }

    return 0;
}