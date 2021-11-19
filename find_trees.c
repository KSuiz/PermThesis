#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

static void iterateTrees(int nk, int n);
static int findDeg(int nk, int n, int *L, int *map, int *deg, int init);
static void confirmTree(int nk, int n, int *map, int *L, int *deg);

int main(int argv, char **argc) {
    if (argv != 3) {
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);
    }

    int n = atoi(argc[1]);
    int k = atoi(argc[2]);

    if (n < 2) {
        fprintf(stderr, "Invalid n value (>= 2).\n");
        exit(1);
    }
    if (k < 2) {
        fprintf(stderr, "Invalid k value (>= 2).\n");
        exit(1);
    }

    int nk = intPow(n, k - 1);

    iterateTrees(nk, n);
}

static void iterateTrees(int nk, int n) {
    int *L = malloc(nk * sizeof (int));
    int *prev = malloc(nk * sizeof (int));
    int *save = malloc(nk * sizeof (int));
    int *deg = malloc(nk * sizeof (int));
    int *map = malloc(nk * sizeof (int));
    int P = nk - 1;

    int nTree = 0;

    memset(save, 0, nk * sizeof (int));
    for (int i = 0; i < nk; i++) {
        L[i] = prev[i] = i;
    }

    map[0] = 0;
    confirmTree(nk, n, L, map, deg);
    if (deg[0] != -1)
        nTree++;

    while (P > 0) {
        L[P]--;
        if (P < nk - 1 && (L[P] != 1 || L[P - 1] != 1)) {
            int diff = P - prev[L[P]];
            while (P < nk - 1) {
                save[P] = prev[L[P]];
                prev[L[P]] = P;
                P++;
                L[P] = L[P - diff];
            }
        }
        while (L[P] == 1) {
            P--;
            prev[L[P]] = save[P];
        }

        confirmTree(nk, n, L, map, deg);
        if (deg[0] != -1)
            nTree++;
    }

    // printf("%d trees found\n", nTree);

    free(map);
    free(deg);
    free(L);
    free(prev);
    free(save);
}

static int findDeg(int nk, int n, int *L, int *map, int *deg, int init) {
    if (init >= nk - 1)
        return nk;
    int next = init + 1;
    while (next < nk && L[next] > L[init]) {
        deg[init]++;
        if (deg[init] > n)
            return nk + 1;
        map[next] = init;
        next = findDeg(nk, n, L, map, deg, next);
    }
    return next;
}

static void confirmTree(int nk, int n, int *L, int *map, int *deg) {
    memset(deg, 0, nk * sizeof (int));
    int res = findDeg(nk, n, L, map, deg, 0);
    if (res == nk + 1 || deg[0] >= n) {
        // printf("Tree failed degree check!\n");
        deg[0] = -1;
        return;
    }

    // printf("Tree: \n");
    // for (int i = 0; i < nk; i++) 
    //     printf("%d ", L[i] + 1);
    // putchar('\n');
    // for (int i = 0; i < nk; i++) 
    //     printf("%d ", deg[i]);
    // putchar('\n');
    for (int i = 0; i < nk; i++) 
        printf("%d ", map[i]);
    putchar('\n');
}