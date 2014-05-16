INCDIR	=	../include
LIBS	=	petitdb
LIBDIR	= 	.

all:	heapfile_insertdata heapfile_search	
	

heapfile_insertdata:
	gcc -o heapfile_insertdata -g -L$(LIBDIR) -l$(LIBS) -I$(INCDIR) heapfile_insertdata.c
	
heapfile_search:
	gcc -o heapfile_search -g -L$(LIBDIR) -l$(LIBS) -I$(INCDIR) heapfile_search.c
clean:
	rm heapfile_search heapfile_insertdata
