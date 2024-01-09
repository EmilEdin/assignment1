CC =gcc
FLAGS =-Wall -g
GCOVFLAGS = -fprofile-arcs -ftest-coverage
MEM = valgrind --leak-check=full
MEM_cache = valgrind --tool=cachegrind --cache-sim=yes
MASSIF = valgrind --tool=massif

%.o: %.c %.h
	$(CC) $(FLAGS) $< -c

make all: hash_table.o linked_list.o freq-count.o

build_ll_test: linked_list.o  linked_list_tests.c
	$(CC) $(FLAGS) $^ -lcunit -o $@

test_ll: build_ll_test
	./build_ll_test	

test_ll_mem: linked_list.o build_ll_test
	$(MEM) ./build_ll_test



build_ht_test: hash_table.o linked_list.o hash_table_tests.c
	$(CC) $(FLAGS) hash_table.c linked_list.c hash_table_tests.c -lcunit -o $@ 

test_ht: build_ht_test
	./build_ht_test

test_ht_mem: build_ht_test
	$(MEM) ./build_ht_test


memtest:
	make test_ll_mem 
	make test_ht_mem
	

gcov_ht: hash_table.o hash_table_tests.c linked_list.o
	$(CC) $(GCOVFLAGS) $(FLAGS) -o hash_table hash_table.c linked_list.c hash_table_tests.c -lcunit
	./hash_table
	gcov hash_table.c

gcov_ll: linked_list.o 
	$(CC) $(GCOVFLAGS) -o linked_list linked_list.c linked_list_tests.c -lcunit
	./linked_list
	gcov linked_list.c


freq_count: hash_table.o linked_list.o freq-count.o
	$(CC) $(FLAGS) $^ -o freq_count


freq_small: freq_count small.txt
	$(MEM) ./freq_count small.txt

gprof_small: freq-count.c
	$(CC) -pg freq-count.c -o freq_count
	./freq_count small.txt
	gprof freq_count gmon.out > prof_output



freq_1k: freq_count 1k-long-words.txt
	$(MEM) ./freq_count 1k-long-words.txt

gprof_1k: freq-count.c
	$(CC) -pg freq-count.c -o freq_count
	./freq_count 1k-long-words.txt
	gprof freq_count gmon.out > prof_output



freq_10k: freq_count 10k-words.txt
	$(MEM) ./freq_count 10k-words.txt

gprof_10k: freq-count.c
	$(CC) -pg freq-count.c -o freq_count
	./freq_count 10k-words.txt
	gprof freq_count gmon.out > prof_output



freq_16k: freq_count 16k-words.txt
	$(MEM) ./freq_count 16k-words.txt

gprof_16k: freq-count.c
	$(CC) -pg freq-count.c -o freq_count
	./freq_count 16k-words.txt
	gprof freq_count gmon.out > prof_output


cachegrind: freq_count small.txt
	$(MEM_cache) ./freq_count small.txt


massif: freq_count small.txt
	$(MASSIF) ./freq_count small.txt

test:
	make freq_small
	make freq_1k
	make freq_10k
	make freq_16k

clean:
	rm -f *.o
	rm -f *.gcov
	rm -f *.gcda
	rm -f *.gcno
	rm -f freq_count build_ht_test build_ll_test freq-count gmon.out hash_table hash_test prof_output test

.PHONY: test clean freq_small freq_1k freq_10k freq_16k gprof_small gprof_1k gprof_10k gprof_16k gcov_ht gcov_ll test_ht_mem test_ll_mem