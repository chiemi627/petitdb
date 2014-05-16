#define HEADERSIZE 8

int db_create_and_open(char *filename);
void get_page(int fd,short rno, PAGE *page);
int write_page(int fd,PAGE *page);
int db_open(char *filename);
void db_close(int fd);
int db_drop(char *filename);
int db_save(int fd, PAGE *page);
int db_delete(int fd, PAGE *page);
