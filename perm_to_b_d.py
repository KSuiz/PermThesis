import itertools

n = int(input("n: "))
k = int(input("k: "))
def gen_basis(n, k):
    if k == 1:
        return [[i] for i in range(1, n + 1)]
    sub_basis = gen_basis(n, k - 1)
    return [item for sub in [[[i] + l for l in sub_basis] for i in range(1, n + 1)] for item in sub]

def to_index(n, arr):
    index = arr[0] - 1
    for v in arr[1:]:
        index = index * n + (v - 1)
    return index

basis = gen_basis(n, k)
size = n ** k
map_matrix = [0] * size
taken = [False] * size
for i in range(size):
    map_val = []
    index = 0
    while not len(map_val) == k or taken[index]:
        map_val = [int(c) for c in input(f"Maps {basis[i]} to: ")]
        index = to_index(n, map_val)
    map_matrix[i] = index
    taken[index] = True

def map_a(basis, n, alpha, beta, i, j, map_arr):
    def collapse(i, sam, sbm, j):
        if not i == sam[0]:
            return 0
        if not j == sbm[0]:
            return 0
        return [sam[1:], sbm[1:]]
    final = []
    for m in range(1, n + 1):
        ami = to_index(n, alpha + [m])
        bmi = to_index(n, beta + [m])
        sam = basis[map_arr[ami]]
        sbm = basis[map_arr[bmi]]
        val = collapse(i, sam, sbm, j)
        #print(f"for m = {m} we have {ami} {bmi} {sam} {sbm} to {val}")
        if not val == 0:
            final += [val]
    return final

def print_final(i, j, alpha, beta):
    final = map_a(basis, n, alpha, beta, i, j, map_matrix)
    if not alpha == beta:
        final = list(filter(lambda x: not x[0] == x[1], final))
    if not final:
        return
    final = ' + '.join([str(v) for v in final])
    desig = "b" if alpha == beta else "d"
    print(f">{desig} [{alpha}, {beta}] maps to: {final}")

def print_fn(mini_basis, i, j):
    print(f"For (i, j) = ({i}, {j}):")
    if i == j:
        for alpha in mini_basis:
            print_final(i, j, alpha, alpha)
    for alpha in mini_basis:
        for beta in mini_basis:
            if not alpha is beta:
                print_final(i, j, alpha, beta)

mini_basis = gen_basis(n, k - 1)
for i in range(1, n + 1):
    print_fn(mini_basis, i, i)
for i in range(1, n + 1):
    for j in range(1, n + 1):
        if not i == j:
            print_fn(mini_basis, i, j)

for i in range(1, n + 1):
    for j in range(1, n + 1):
        for alpha in mini_basis:
            for beta in mini_basis:
                if alpha is not beta:
                    final = map_a(basis, n, alpha, beta, i, j, map_matrix)
                    final = list(filter(lambda x: not x[0] == x[1], final))
                    if not final:
                        continue
                    for dom in final:
                        print(f"f_{{{i}{j}}}({to_index(n, dom[0])}, {to_index(n, dom[1])}) = ({to_index(n, alpha)}, {to_index(n, beta)})")

exit()
