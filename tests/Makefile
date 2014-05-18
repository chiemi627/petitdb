INCDIR	=	../include
LIBS	=	petitdb
LIBDIR	= 	../lib

all:	heapfile_test hashfile_test recordlist_test

heapfile_test: 
	gcc -o heapfile_test -g heapfile_test.c -L$(LIBDIR) -l$(LIBS) -I$(INCDIR)
	
hashfile_test:
	gcc -o hashfile_test -g hashfile_test.c -L$(LIBDIR) -l$(LIBS) -I$(INCDIR)

recordlist_test: 
	gcc -o recordlist_test -g recordlist_test.c -L$(LIBDIR) -l$(LIBS) -I$(INCDIR)

clean:
	rm *_test

	
test:	heapfile_test hashfile_test recordlist_test
	./heapfile_test;./hashfile_test;./recordlist_test
