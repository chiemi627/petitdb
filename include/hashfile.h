typedef struct hashfile_header{
  unsigned short nofbackets_init;
  unsigned short nofbackets_all;
} hashfile_header;

typedef struct HFILE{
  FILE *fp;
  PAGE *page;
  hashfile_header *header;
} HFILE;

HFILE *new_HFILE();
hashfile_header *db_hashfile_get_header(HFILE *hfile);
void db_hashfile_get_page(HFILE *hfile,short pno);
short db_hashfile_get_number_of_initial_backets(HFILE *hfile);
short db_hashfile_get_number_of_all_backets(HFILE *hfile);
void db_hashfile_prepare(HFILE *hfile,short nofbackets);
short db_hashfile_get_the_last_page(HFILE *hfile);
short db_hashfile_get_hash(char *value, short nofbackets);
int db_hashfile_insert(HFILE *hfile, record *rd);
int db_hashfile_close(HFILE *hfile);