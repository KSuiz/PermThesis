# Permutation Automorphisms of Cuntz Algebras

This repository contains an implementation for several parts of algorithm described in the accompanying thesis. In particular:
- `find_trees.c` contains an implementation of the algorithm of [BH80], with extra checks confirming the in-degree conditions of Corollary 4.2.2.
- `check_partial_order.c` implements a cycle checking algorithm via DFS to confirm whether a given family of functions admits a partial order satisfying either Lemma 3.4.3 or Lemma 3.5.2, depending on if the functions represent b or d submatrix functions.
- `perm_to_b_d.py` implements the computations required to convert a permutation into its b and d submatrix functions.

The `Makefile` is provided to compile the C programs.
