#include <stdio.h>
#include "recordlist.h"
#include "assert.h"

void make_recordList_test(){

  fprintf(stderr,"TEST:%s ... ", __func__);
  fflush(stderr);

  record rdlist[3] = {{"aaa",1},{"bbb",2},{"ccc",3}};
  recordList *list = newList();
  int i;
  for(i=0;i<1;i++){
	  add_record(list,&(rdlist[i]));
  }
  init_list(list);
  
  i=0;
  while( has_next(list) >0 ){
    record *rd = get_next(list);
	assert(strcmp(rd->name,rdlist[i].name)==0);
	assert(rd->age==rdlist[i].age);
	i++;
  }
  
  printf("ok\n");  
}

int main(){
 make_recordList_test();
}