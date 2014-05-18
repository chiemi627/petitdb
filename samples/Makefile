INCDIR	=	../include
LIBS	=	petitdb
LIBDIR	= 	../lib

all:	heapfile_insertdata heapfile_search hashfile_insertdata hashfile_search	
	

heapfile_insertdata:
	gcc -o heapfile_insertdata -g heapfile_insertdata.c -L$(LIBDIR) -l$(LIBS) -I$(INCDIR)
	
hashfile_insertdata:
	gcc -o hashfile_insertdata -g hashfile_insertdata.c -L$(LIBDIR) -l$(LIBS) -I$(INCDIR)

heapfile_search:
	gcc -o heapfile_search -g heapfile_search.c -L$(LIBDIR) -l$(LIBS) -I$(INCDIR)

hashfile_search:
	gcc -o hashfile_search -g hashfile_search.c -L$(LIBDIR) -l$(LIBS) -I$(INCDIR)

clean:
	rm heapfile_search heapfile_insertdata hashfile_search hashfile_insertdata *.db

