#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "recordlist.h"
#include "dbfile.h"
#include "heapfile.h"

int main(int argc,char *argv[]){

  int nofrecords = 1000;
  
  if(argc==2){
  	nofrecords = atoi(argv[1]);
  }

  char *filename = "heapfile.db";
  HEAPFILE *hfile = new_HEAPFILE();
  record *rd = (record *)malloc(sizeof(record));
    
  db_heapfile_create_and_open(hfile,filename);
  db_heapfile_get_page(hfile,0);

  int i;  
  for(i=0;i<nofrecords;i++){
  	  if(i%128==0){
	      strcpy(rd->name,"xxx");        	  	
      }
      else{
	      strcpy(rd->name,"abc");      
      }
      rd->age = i;
	  db_heapfile_insert(hfile,rd);
  }
  //•Â‚¶‚é
  db_heapfile_close(hfile);
}
