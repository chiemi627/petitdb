#define RECORDINI 0
#define RECORDEND 1
#define RECORDEMPTY 2
#define RECORDEXIST 3
#define PAGESIZE 256
#define PAGEHEADERSIZE 6

typedef struct record{
  char name[16];
  short age;
} record;

typedef struct page{
  char *pagebuf;
  unsigned short pageno;
  char *rinfo_ptr;
  short write_flg;
} PAGE;

struct record *read_record(PAGE *page);
int generate_page(PAGE *page);
int has_more_space(PAGE *page);
int insert_record_into_page(PAGE *page, record *rd);
int get_next_page_no(PAGE *page);
unsigned short get_the_first_pointer(PAGE *page);
unsigned short get_the_last_pointer(PAGE *page);
int set_next_page_no(PAGE *page, short pageno);
