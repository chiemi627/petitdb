INCDIR	=	../include
LIBS	=	petitdb
LIBDIR	= 	.

all:	heapfile_test hashfile_test recordlist_test
	

heapfile_test: 
	gcc -o heapfile_test -g -L$(LIBDIR) -l$(LIBS) -I$(INCDIR) heapfile_test.c
	
hashfile_test:
	gcc -o hashfile_test -g -L$(LIBDIR) -l$(LIBS) -I$(INCDIR) hashfile_test.c

recordlist_test: 
	gcc -o recordlist_test -g -L$(LIBDIR) -l$(LIBS) -I$(INCDIR) recordlist_test.c

clean:
	rm *_test
