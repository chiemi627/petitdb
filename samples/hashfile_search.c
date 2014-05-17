#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "recordlist.h"
#include "dbfile.h"
#include "hashfile.h"

int main(){
  char *filename = "testfile.hash";
  HFILE *hfile = new_HFILE();
    
  //データベースを開く
  db_hashfile_open(hfile,filename);
  
  recordList *result = db_hashfile_search_by_hash(hfile,"xxx");
  init_list(result);
  while(has_next(result)>0){
  	record *rd = get_next(result);
  	printf("name = %s, age=%d\n",rd->name,rd->age);
  }
  
  //閉じる
  db_hashfile_close(hfile);
}