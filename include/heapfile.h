typedef struct HEAPFILE{
  FILE *fp;
  PAGE *page;
} HEAPFILE;

HEAPFILE *new_HEAPFILE();
void db_heapfile_create_and_open(HEAPFILE *hfile,char *filename);
short db_heapfile_get_the_last_page(HEAPFILE *hfile);
record *db_heapfile_read(HEAPFILE *hfile);
int db_heapfile_insert(HEAPFILE *hfile, record *rd);
void db_heapfile_get_page(HEAPFILE *hfile,short pno);
int db_heapfile_close(HEAPFILE *hfile);
