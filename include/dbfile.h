#define HEADERSIZE 8

FILE *db_create_and_open(char *filename);
void get_page(FILE *fp,short rno, PAGE *page);
int write_page(FILE *fp,PAGE *page);
FILE *db_open(char *filename);
void db_close(FILE *fp);
int db_drop(char *filename);
int db_save(FILE *fp, PAGE *page);
int db_delete(FILE *fp, PAGE *page);
