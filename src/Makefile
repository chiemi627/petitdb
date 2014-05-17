INCDIR	=	../include

TARGET	=	libpetitdb.a
OBJS	=	page.o dbfile.o heapfile.o hashfile.o recordlist.o
$(TARGET):	$(OBJS)
	ar rcs $(TARGET) $(OBJS)
	cp $(TARGET) ../test/.
	cp $(TARGET) ../samples/.
	mv $(TARGET) ../lib/.
	rm *.o
.c.o:
	gcc -c $< -g -I $(INCDIR)
