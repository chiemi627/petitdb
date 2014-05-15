INCDIR	=	../include

TARGET	=	libpetitdb.so
OBJS	=	page.o dbfile.o heapfile.o hashfile.o
$(TARGET):	$(OBJS)
	gcc -shared -Wl,-soname,$(TARGET) -o $(TARGET) $(OBJS)
	mv $(TARGET) ../lib/.
	rm *.o
.c.o:
	gcc -c $< -g -I $(INCDIR)
