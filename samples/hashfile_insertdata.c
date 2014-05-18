#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "recordlist.h"
#include "dbfile.h"
#include "hashfile.h"

int main(int argc,char *argv[]){

  int nofrecords = 1000;
  
  if(argc==2){
  	nofrecords = atoi(argv[1]);
  }

  char *filename = "hashfile.db";
  HFILE *hfile = new_HFILE();
  record *rd = (record *)malloc(sizeof(record));
    
  db_hashfile_create_and_open(hfile,filename);
  db_hashfile_prepare(hfile,4);

  int i;
  for(i=0;i<nofrecords;i++){
  	  if(i%128==0){
	      strcpy(rd->name,"xxx");        	  	
      }
      else{
	      strcpy(rd->name,"abc");      
      }
      rd->age = i;
	  db_hashfile_insert(hfile,rd);
  }
  //•Â‚¶‚é
  db_hashfile_close(hfile);
}
