# Runs rules for given commands.
# Copyright (C) 2017  Elliott Sobek
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# After running autoscan, configuring, and makeing the Makefile.am you can then
# run this script

build-dist () {
	local cur_path=$(pwd)

	mv configure.ac ..
	mv Makefile.am ..
	cd ..
	mv makefile debugmakefile.bak

	aclocal
	autoconf
	autoheader
	automake --add-missing
	./configure
	make distcheck
	mv debugmakefile.bak makefile

	rm -rf .deps aclocal.m4 autom4te.cache config.h config.h.in config.log \
		config.guess config.status config.sub ./configure compile depcomp \
		install-sh Makefile Makefile.in missing stamp-h1

	mv configure.ac build/
	mv Makefile.am build/
	cd $cur_path
}

build-dist
