all	:
	cd src;make
	cd tests;make clean;make 

test	:
	cd tests;make test

clean	:
	cd lib;rm libpetitdb.a
	cd tests;make clean
	cd samples;make clean
