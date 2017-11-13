#!/bin/bash

test () {

	# Test .exe
	if [ ! -x ./pwd-list-gen ]; then
		return 1
	fi

	# Test file size calculation

	# Single letter
	./pwd-list-gen -q -l 1 -f t1.txt

	local byte_size=$(du -b t1.txt | cut -f1)
	local entries=$(wc -l t1.txt | cut --delimiter=" " -f1)

	if [ "$entries" != 10 -a "$byte_size" != 20 ]; then
		return 1
	fi

	# Multicharacter (DEFAULTS)
	./pwd-list-gen -q -f t2.txt

	byte_size=$(du -b t2.txt | cut -f1)
	entries=$(wc -l t2.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 100000000 -a "$byte_size" -ne 900000000 ]; then
		return 1
	fi

	# All multicharacter (DEFAULTS)
	./pwd-list-gen -aq -f t3.txt

	byte_size=$(du -b t3.txt | cut -f1)
	entries=$(wc -l t3.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 111111110 -a "$byte_size" -ne 999999990 ]; then
		return 1
	fi

	# Set min (len: 6-8 NUMS)
	./pwd-list-gen -q -L 6 -f t4.txt

	byte_size=$(du -b t4.txt | cut -f1)
	entries=$(wc -l t4.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 111000000 -a "$byte_size" -ne 999000000 ]; then
		return 1
	fi

	# Set min and mabyte_size (len: 6-8 NUMS)
	./pwd-list-gen -q -L 6 -l 8 -f t5.txt

	byte_size=$(du -b t5.txt | cut -f1)
	entries=$(wc -l t5.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 111000000 -a "$byte_size" -ne 999000000 ]; then
		return 1
	fi

	# Lower
	./pwd-list-gen -q -l 4 -f t6.txt

	byte_size=$(du -b t6.txt | cut -f1)
	entries=$(wc -l t6.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 456976 -a "$byte_size" -ne 2284880 ]; then
		return 1
	fi

	# Upper
	./pwd-list-gen -q -l 4 -f t7.txt

	byte_size=$(du -b t7.txt | cut -f1)
	entries=$(wc -l t7.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 456976 -a "$byte_size" -ne 2284880 ]; then
		return 1
	fi

	# ALPHA
	./pwd-list-gen -q -l 4 -f t8.txt

	byte_size=$(du -b t8.txt | cut -f1)
	entries=$(wc -l t8.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 7311616 -a "$byte_size" -ne 36558080 ]; then
		return 1
	fi

	# ALNUM
	./pwd-list-gen -q -l 4 -f t9.txt

	byte_size=$(du -b t9.txt | cut -f1)
	entries=$(wc -l t9.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 14776336 -a "$byte_size" -ne 73881680 ]; then
		return 1
	fi

	# ALL
	./pwd-list-gen -q -l 4 -f t10.txt

	byte_size=$(du -b t10.txt | cut -f1)
	entries=$(wc -l t10.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 81450625 -a "$byte_size" -ne 407253125 ]; then
		return 1
	fi

	# Redirection
	./pwd-list-gen -l 4 > t11.txt

	byte_size=$(du -b t11.txt | cut -f1)
	entries=$(wc -l t11.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 10000 -a "$byte_size" -ne 50000 ]; then
		return 1
	fi

	make clean

	echo "All tests PASSED"

	return 0
}

test
