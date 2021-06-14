#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./include/list.h"

#include "./private/common.h"
#include "./private/sorting.h"

#define customSize 6
#define customAlgoSize 3

extern inline void showlist(struct list_head *);

void (*sorting[customAlgoSize])(struct list_head *) = {
    list_insertsort, list_qsort, list_mergesort};
int customSizeArr[customSize] = {10000, 20000, 30000, 40000, 50000, 60000};
char *sortingName[customAlgoSize] = {"insertion-sort", "quick-sort",
                                     "merge-sort"};
double executeTime[customAlgoSize] = {0.0};

int main(void)
{
    struct list_head testlist;
    struct listitem *item, *is = NULL;
    size_t i;
    FILE *fp = fopen("worst.txt", "w");
    char input[30];

    if (fp == NULL) {
        return 1;
    }

    for (int tsize = 0; tsize < customSize; tsize++) {
        int algo;
        for (algo = 0; algo < customAlgoSize; algo++) {
            uint16_t *values =
                (uint16_t *) malloc(sizeof(uint16_t) * customSizeArr[tsize]);

            // generate random numbers
            random_shuffle_array_worst(values, customSizeArr[tsize]);
            INIT_LIST_HEAD(&testlist);
            assert(list_empty(&testlist));
            for (i = 0; i < (uint16_t) customSizeArr[tsize]; i++) {
                item = (struct listitem *) malloc(sizeof(*item));
                assert(item);
                item->i = values[i];
                list_add_tail(&item->list, &testlist);
            }
            assert(!list_empty(&testlist));
            // sorting(answer)
            qsort(values, customSizeArr[tsize], sizeof(values[0]), cmpint);

            // sorting(for each sorting algorithm)
            clock_t begin = clock();
            (*sorting[algo])(&testlist);
            clock_t end = clock();
            executeTime[algo] = (double) (end - begin) / CLOCKS_PER_SEC;
            // verify
            i = 0;
            list_for_each_entry_safe (item, is, &testlist, list) {
                assert(item->i == values[i]);
                list_del(&item->list);
                free(item);
                i++;
            }
            assert(i == (size_t) customSizeArr[tsize]);
            assert(list_empty(&testlist));
            free(values);
            printf("%s %d\n", sortingName[algo], customSizeArr[tsize]);
        }

        sprintf(input, "%d %lf %lf %lf\n", customSizeArr[tsize], executeTime[0],
                executeTime[1], executeTime[2]);
        // write result to file
        fputs(input, fp);
    }
    fclose(fp);
    return 0;
}
