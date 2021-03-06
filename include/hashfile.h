typedef struct hashfile_header{
  unsigned short nofbackets_init;
  unsigned short nofbackets_all;
} hashfile_header;

typedef struct HFILE{
  int fd;
  PAGE *page;
  hashfile_header *header;
} HFILE;

HFILE *new_HFILE();
void db_hashfile_create_and_open(HFILE *hfile,char *filename);
void db_hashfile_prepare(HFILE *hfile,short nofbackets);
int db_hashfile_open(HFILE *hfile,char *filename);
int db_hashfile_close(HFILE *hfile);

void db_hashfile_get_header(HFILE *hfile);
short db_hashfile_get_number_of_initial_backets(HFILE *hfile);
short db_hashfile_get_number_of_all_backets(HFILE *hfile);

short db_hashfile_get_hash(char *value, short nofbackets);
void db_hashfile_get_page(HFILE *hfile,short pno);
record *db_hashfile_read(HFILE *hfile);

short db_hashfile_get_the_last_page(HFILE *hfile);
int db_hashfile_insert(HFILE *hfile, record *rd);
void db_hashfile_write_page(HFILE *hfile);

recordList *db_hashfile_search_by_scan(HFILE *hfile,char *keyword);
recordList *db_hashfile_search_by_hash(HFILE *hfile,char *keyword);
