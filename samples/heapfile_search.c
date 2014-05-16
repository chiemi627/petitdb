#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "recordlist.h"
#include "dbfile.h"
#include "heapfile.h"

int main(){
  char *filename = "testfile.db";
  HEAPFILE *hfile = new_HEAPFILE();
    
  //データベースを開く
  db_heapfile_open(hfile,filename);
  
  recordList *result = db_heapfile_search_by_scan(hfile,"xxx");
  init_list(result);
  while(has_next(result)>0){
  	record *rd = get_next(result);
  	printf("name = %s, age=%d\n",rd->name,rd->age);
  }
  
  //閉じる
  db_heapfile_close(hfile);
}