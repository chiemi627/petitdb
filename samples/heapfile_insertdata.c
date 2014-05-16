#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "recordlist.h"
#include "dbfile.h"
#include "heapfile.h"

int main(){
  char *filename = "testfile.db";
  HEAPFILE *hfile = new_HEAPFILE();
  record *rd = (record *)malloc(sizeof(record));
    
  //データベースを開く
  db_heapfile_create_and_open(hfile,filename);
  db_heapfile_get_page(hfile,0);
  //レコードを100個挿入する
  int i;
  for(i=0;i<100;i++){
  	  if(i%23==0){
	      strcpy(rd->name,"xxx");        	  	
      }
      else{
	      strcpy(rd->name,"abc");      
      }
      rd->age = i;
	  db_heapfile_insert(hfile,rd);
  }
  //閉じる
  db_heapfile_close(hfile);
}