#include "page.h"

typedef struct recordNode{
	record *record;
	struct recordNode *next;
} recordNode;

typedef struct recordList {
	struct recordNode *first;
	struct recordNode *last;
    struct recordNode *current;
} recordList;

recordList *newList();
int add_record(recordList *,record *);
void init_list(recordList *);
record *get_next(recordList *);
int has_next(recordList *);
