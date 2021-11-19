CC = gcc
CFLAGS = -g -O3

.PHONY: all
all: find_trees check_partial_order
find_trees: find_trees.o util.o
check_partial_order: check_partial_order.o util.o
find_trees.o: find_trees.c util.h
check_partial_order.o: check_partial_order.c util.h
util.o: util.c util.h