#include <stdlib.h>
#include <stdio.h>
#include "util.h"

typedef char index;

typedef struct pair {
    index a;
    index b;
} pair;

typedef pair (*map)(pair, pair);

static pair **initialise(index nk);
static void runDfs(pair **visited, map fMap, int n, index nk, char choice);
static map getMaps(int n, index nk, char choice);
static void dfs(pair **visited, map fMap, int n, index nk, pair curr, pair root, char choice);
static int pairCmp(pair x, pair y);
static void freeMem(int n, index nk);

static index **BMAP = NULL;
static pair ****DMAP = NULL;

// Need to grab the size of inputs, and functions
// Grab n, k, b/d
// Size for b is n^{k - 1}
// Size for d is n^{k - 1} (n^{k - 1} - 1) + 1

int main(int argv, char **argc) {
    if (argv != 4) {
        fprintf(stderr, "Invalid number of arguments\n");
        exit(1);
    }

    int n = atoi(argc[1]);
    int k = atoi(argc[2]);
    char choice = argc[3][0];

    if (n < 2) {
        fprintf(stderr, "Invalid n value (>= 2).\n");
        exit(1);
    }
    if (k < 2) {
        fprintf(stderr, "Invalid k value (>= 2).\n");
        exit(1);
    }
    if (choice != 'b' && choice != 'd') {
        fprintf(stderr, "Invalid function type.\n");
        exit(1);
    }

    // Calculate number of elements in the graphs
    int nk = intPow(n, k - 1);
    index nkInd = (index) nk;

    // Grab the functions
    // Format: For b n sets of nk values, ith value is f_set(i)
    //         For d n^2 sets of nk(nk - 1) pairs, ijth pair is f_set(ij)
    map fMap = getMaps(n, nkInd, choice);

    pair **visited = initialise(nkInd);
    runDfs(visited, fMap, n, nkInd, choice);
    freeMem(n, nk);
}

static int pairCmp(pair x, pair y) {
    return x.a == y.a && x.b == y.b;
}

static pair **initialise(index nk) {
    pair **arr = malloc(nk * sizeof (pair *));
    for (index i = 0; i < nk; i++) {
        arr[i] = malloc(nk * sizeof (pair));
        for (index j = 0; j < nk; j++)
            arr[i][j].a = arr[i][j].b = -1;
    }
    return arr;
}

static void runDfs(pair **visited, map fMap, int n, index nk, char choice) {
    // Search through every off-diagonal element to find a cycle.
    for (index i = 0; i < nk; i++) {
        for (index j = 0; j < nk; j++) {
            pair ij = {i, j};
            dfs(visited, fMap, n, nk, ij, ij, choice);
        }
    }
}

static pair bEval(pair i, pair x) {
    pair res = {.a = BMAP[i.a][x.a], .b = BMAP[i.a][x.b]};
    return res;
}

static pair dEval(pair i, pair x) {
    return DMAP[i.a][i.b][x.a][x.b];
}

static map getMaps(int n, index nk, char choice) {
    if (choice == 'b') {
        BMAP = malloc(n * sizeof(*BMAP));
        for (int i = 0; i < n; i++) {
            BMAP[i] = malloc(nk * sizeof (**BMAP));
            for (index j = 0; j < nk; j++) {
                int val;
                if (scanf("%d", &val) != 1) {
                    fprintf(stderr, "Invalid map definition\n");
                    exit(1);
                }
                BMAP[i][j] = (index) val;
            }
        }
        return &bEval;
    }
    DMAP = malloc(n * sizeof(*DMAP));
    for (int i = 0; i < n; i++) {
        DMAP[i] = malloc(n * sizeof (**DMAP));
        for (int j = 0; j < n; j++) {
            DMAP[i][j] = malloc(nk * sizeof (***DMAP));
            for (index k = 0; k < nk; k++) {
                DMAP[i][j][k] = malloc(nk * sizeof (****DMAP));
                for (index l = 0; l < nk; l++) {
                    int val1, val2;
                    if (scanf("%d %d", &val1, &val2) != 2) {
                        fprintf(stderr, "Invalid map definition\n");
                        exit(1);
                    }
                    DMAP[i][j][k][l] = (pair) {.a = (index) val1, .b = (index) val2};
                }
            }
        }
    }
    return &dEval;
}

static void dfs(pair **visited, map fMap, int n, index nk, pair curr, pair root, char choice) {
    pair visit = visited[curr.a][curr.b];
    if (pairCmp(visit, root)) {
        fprintf(stderr, "Loop found at root %d %d\n", root.a, root.b);
        exit(1);
    }
    visited[curr.a][curr.b] = root;
    for (int i = 0; i < n; i++) {
        char lim = (choice == 'b' ? 1 : n);
        for (int j = 0; j < lim; j++) {
            pair ij = {i, j};
            pair next = fMap(ij, curr);
            if (next.a == next.b)
                continue;
            if (pairCmp(curr, next)) {
                fprintf(stderr, "Self-loop in function %d %d for value %d %d\n", i, j, curr.a, curr.b);
                exit(1);
            }
            dfs(visited, fMap, n, nk, next, root, choice);
        }
    }
}

static void freeMem(int n, index nk) {
    if (BMAP) {
        for (int i = 0; i < n; i++)
            free(BMAP[i]);
        free(BMAP);
    }
    if (DMAP) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (index k = 0; k < nk; k++)
                    free(DMAP[i][j][k]);
                free(DMAP[i][j]);
            }
            free(DMAP[i]);
        }
        free(DMAP);
    }
}