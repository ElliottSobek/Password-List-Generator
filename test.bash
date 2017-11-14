#!/bin/bash

test () {

	make test

	# Test .exe
	if [ ! -x ./test.exe ]; then
		echo "Fail file executable"
		return 1
	fi
	echo "Pass file exe"

	# Test file size calculation

	# Single letter
	./test.exe -q -l 1 -f t1.txt

	local byte_size=$(du -b t1.txt | cut -f1)
	local entries=$(wc -l t1.txt | cut --delimiter=" " -f1)

	if [ "$entries" != 10 -a "$byte_size" != 20 ]; then
		echo "Fail single character"
		return 1
	fi
	echo "Pass single character"

	# Multicharacter
	./test.exe -q -l 4 -f t2.txt

	byte_size=$(du -b t2.txt | cut -f1)
	entries=$(wc -l t2.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 10000 -a "$byte_size" -ne 50000 ]; then
		echo "Fail multicharacter"
		return 1
	fi
	echo "Pass multicharacter"

	# All multicharacter
	./test.exe -aq -l 4 -f t3.txt

	byte_size=$(du -b t3.txt | cut -f1)
	entries=$(wc -l t3.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 11110 -a "$byte_size" -ne 55550 ]; then
		echo "Fail all multicharacter"
		return 1
	fi
	echo "Pass all multicharacter"

	# Set min (len: 2-4 NUMS)
	./test.exe -q -L 2 -l 4 -f t4.txt

	byte_size=$(du -b t4.txt | cut -f1)
	entries=$(wc -l t4.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 11100 -a "$byte_size" -ne 54300 ]; then
		echo "Fail set min"
		return 1
	fi
	echo "Pass set min"

	# Lower
	./test.exe -q -l 4 -c l -f t6.txt

	byte_size=$(du -b t6.txt | cut -f1)
	entries=$(wc -l t6.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 456976 -a "$byte_size" -ne 2284880 ]; then
		echo "Fail lower"
		return 1
	fi
	echo "Pass lower"

	# Upper
	./test.exe -q -l 4 -c u -f t7.txt

	byte_size=$(du -b t7.txt | cut -f1)
	entries=$(wc -l t7.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 456976 -a "$byte_size" -ne 2284880 ]; then
		echo "Fail upper"
		return 1
	fi
	echo "Pass upper"

	# ALPHA
	./test.exe -q -l 3 -c p -f t8.txt

	byte_size=$(du -b t8.txt | cut -f1)
	entries=$(wc -l t8.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 140608 -a "$byte_size" -ne 562432 ]; then
		echo "Fail alpha"
		return 1
	fi
	echo "Pass alpha"

	# ALNUM
	./test.exe -q -l 3 -c a -f t9.txt

	byte_size=$(du -b t9.txt | cut -f1)
	entries=$(wc -l t9.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 238328 -a "$byte_size" -ne 953312 ]; then
		echo "Fail alnum"
		return 1
	fi
	echo "Pass alnum"

	# ALL
	./test.exe -q -l 3 -c s -f t10.txt

	byte_size=$(du -b t10.txt | cut -f1)
	entries=$(wc -l t10.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 857375 -a "$byte_size" -ne 3429500 ]; then
		echo "Fail all choice set"
		return 1
	fi
	echo "Pass all choice set"

	# Redirection
	./test.exe -q -l 4 > t11.txt

	byte_size=$(du -b t11.txt | cut -f1)
	entries=$(wc -l t11.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 10000 -a "$byte_size" -ne 50000 ]; then
		echo "Fail redirection"
		return 1
	fi
	echo "Pass redirection"
	echo "All tests PASSED"

	make clean
	rm ./test.exe

	return 0
}

test
