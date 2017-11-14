#!/bin/bash

test () {

	# Test .exe
	if [ ! -x ./pwd-list-gen ]; then
		echo "Fail file executable"
		return 1
	fi

	# Test file size calculation

	# Single letter
	./pwd-list-gen -q -l 1 -f t1.txt

	local byte_size=$(du -b t1.txt | cut -f1)
	local entries=$(wc -l t1.txt | cut --delimiter=" " -f1)

	if [ "$entries" != 10 -a "$byte_size" != 20 ]; then
		echo "Fail single character"
		return 1
	fi
	echo "Pass single character"

	# Multicharacter
	./pwd-list-gen -q -l 4 -f t2.txt

	byte_size=$(du -b t2.txt | cut -f1)
	entries=$(wc -l t2.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 10000 -a "$byte_size" -ne 50000 ]; then
		echo "Fail multicharacter"
		return 1
	fi
	echo "Pass multicharacter"

	# All multicharacter
	./pwd-list-gen -aq -l 4 -f t3.txt

	byte_size=$(du -b t3.txt | cut -f1)
	entries=$(wc -l t3.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 11110 -a "$byte_size" -ne 55550 ]; then
		echo "Fail all multicharacter"
		return 1
	fi
	echo "Pass all multicharacter"

	# Set min (len: 2-4 NUMS)
	./pwd-list-gen -q -L 2 -l 4 -f t4.txt

	byte_size=$(du -b t4.txt | cut -f1)
	entries=$(wc -l t4.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 11100 -a "$byte_size" -ne 54300 ]; then
		echo "Fail set min"
		return 1
	fi
	echo "Pass set min"

	# Lower
	./pwd-list-gen -q -l 4 -c l -f t6.txt

	byte_size=$(du -b t6.txt | cut -f1)
	entries=$(wc -l t6.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 456976 -a "$byte_size" -ne 2284880 ]; then
		echo "Fail lower"
		return 1
	fi
	echo "Pass lower"

	# Upper
	./pwd-list-gen -q -l 4 -c u -f t7.txt

	byte_size=$(du -b t7.txt | cut -f1)
	entries=$(wc -l t7.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 456976 -a "$byte_size" -ne 2284880 ]; then
		echo "Fail upper"
		return 1
	fi
	echo "Pass upper"

	# ALPHA
	./pwd-list-gen -q -l 4 -c p -f t8.txt

	byte_size=$(du -b t8.txt | cut -f1)
	entries=$(wc -l t8.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 7311616 -a "$byte_size" -ne 36558080 ]; then
		echo "Fail alpha"
		return 1
	fi
	echo "Pass alpha"

	# ALNUM
	./pwd-list-gen -q -l 4 -c a -f t9.txt

	byte_size=$(du -b t9.txt | cut -f1)
	entries=$(wc -l t9.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 14776336 -a "$byte_size" -ne 73881680 ]; then
		echo "Fail alnum"
		return 1
	fi
	echo "Pass alnum"

	# ALL
	./pwd-list-gen -q -l 4 -c s -f t10.txt

	byte_size=$(du -b t10.txt | cut -f1)
	entries=$(wc -l t10.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 81450625 -a "$byte_size" -ne 407253125 ]; then
		echo "Fail all choice set"
		return 1
	fi
	echo "Pass all choice set"

	# Redirection
	./pwd-list-gen -l 4 > t11.txt

	byte_size=$(du -b t11.txt | cut -f1)
	entries=$(wc -l t11.txt | cut --delimiter=" " -f1)

	if [ "$entries" -ne 10000 -a "$byte_size" -ne 50000 ]; then
		echo "Fail redirection"
		return 1
	fi
	echo "Pass redirection"

	make clean

	echo "All tests PASSED"

	return 0
}

test
