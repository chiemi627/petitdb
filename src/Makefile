INCDIR	=	../include

TARGET	=	libpetitdb.so
OBJS	=	page.o dbfile.o heapfile.o hashfile.o recordlist.o
$(TARGET):	$(OBJS)
	gcc -shared -Wl,-soname,$(TARGET) -o $(TARGET) $(OBJS)
	cp $(TARGET) ../test/.
	mv $(TARGET) ../lib/.
	rm *.o
.c.o:
	gcc -c $< -g -I $(INCDIR)
