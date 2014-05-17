INCDIR	=	../include
LIBS	=	petitdb
LIBDIR	= 	.

all:	heapfile_insertdata heapfile_search hashfile_insertdata hashfile_search	
	

heapfile_insertdata:
	gcc -o heapfile_insertdata -g -L$(LIBDIR) -l$(LIBS) -I$(INCDIR) heapfile_insertdata.c
	
hashfile_insertdata:
	gcc -o hashfile_insertdata -g -L$(LIBDIR) -l$(LIBS) -I$(INCDIR) hashfile_insertdata.c

heapfile_search:
	gcc -o heapfile_search -g -L$(LIBDIR) -l$(LIBS) -I$(INCDIR) heapfile_search.c

hashfile_search:
	gcc -o hashfile_search -g -L$(LIBDIR) -l$(LIBS) -I$(INCDIR) hashfile_search.c

clean:
	rm heapfile_search heapfile_insertdata hashfile_search hashfile_insertdata

