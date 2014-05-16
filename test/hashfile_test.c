#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "recordlist.h"
#include "dbfile.h"
#include "hashfile.h"

void db_create_hashfile_and_open_makes_backet(){
  fprintf(stderr,"TEST:%s ... ", __func__);
  fflush(stderr);
  
  short nofbacket = 4;
  char *filename = "testfile.db";
  
  HFILE *hfile = new_HFILE();
  db_hashfile_create_and_open(hfile,filename);
  db_hashfile_prepare(hfile,nofbacket);
  
  //バケット数がページヘッダについている
  hashfile_header *header = db_hashfile_get_header(hfile);
  assert(db_hashfile_get_number_of_initial_backets(hfile)==4);
  //1~4page目にヘッダがちゃんとある
  short i;  
  for(i=0;i<4;i++){    
    db_hashfile_get_page(hfile,i);
    assert(get_next_page_no(hfile->page)==-1);
    assert(get_the_first_pointer(hfile->page)==sizeof(short)*3);
    assert(get_the_last_pointer(hfile->page)==PAGESIZE-sizeof(short));
  }
  db_close(hfile->fd);
  db_drop(filename);
  printf("ok\n");
}

void db_hashfile_get_hash_returns_correct_value(){
  fprintf(stderr,"TEST:%s ... ", __func__);
  fflush(stderr);
  int i;
  for(i=0;i<26;i++){
    char *str=(char *)malloc(2);
    str[0]=i+'A';
    str[1]='\0';
    short hvalue = db_hashfile_get_hash(str,4);
    assert(hvalue == (65+i)%4);
  }
  printf("ok\n");
}

void db_hashfile_inserting_5_records_store_in_file(){
  fprintf(stderr,"TEST:%s ... ", __func__);
  fflush(stderr);
  
  short nofbacket = 4;
  char *filename = "testfile2.db";
  record *rd = (record *)malloc(sizeof(record));
  
  // == PREPARATION ==
  HFILE *hfile = new_HFILE();
  db_hashfile_create_and_open(hfile,filename);
  db_hashfile_prepare(hfile,nofbacket);
  
  int i;
  for(i=0;i<5;i++){
      char *str=(char *)malloc(2);
      str[0]=i+'A';
      str[1]='\0';
      strcpy(rd->name,str);
      rd->age = i;
	  db_hashfile_insert(hfile,rd);
  }
  db_save(hfile->fd,hfile->page);
  
  // == TEST ==
  short fptr_answers[4] = {24,42,24,24};
  short lptr_answers[4] = {250,246,250,250};
  for(i=0;i<4;i++){
    db_hashfile_get_page(hfile,i);
    short fptr = get_the_first_pointer(hfile->page);
    short lptr = get_the_last_pointer(hfile->page);
	assert(fptr==fptr_answers[i]);
	assert(lptr==lptr_answers[i]);
  }
  
  db_close(hfile->fd);
  db_drop(filename);  
  printf("ok\n");
}

void db_hashfile_inserting_12_records_makes_overflow_backet(){
  fprintf(stderr,"TEST:%s ... ", __func__);
  fflush(stderr);
  
  short nofbacket = 4;
  char *filename = "testfile.db";
  record *rd = (record *)malloc(sizeof(record));
  
  // == PREPARATION ==
  HFILE *hfile = new_HFILE();
  db_hashfile_create_and_open(hfile,filename);
  db_hashfile_prepare(hfile,nofbacket);
  int i;
  for(i=0;i<12;i++){
      char *str="A";
      strcpy(rd->name,str);
      rd->age = i;
	  db_hashfile_insert(hfile,rd);
  }
  db_save(hfile->fd,hfile->page);
  // == TEST ==
  short fptr_answers[5] = {6,204,6,6,24};
  short lptr_answers[5] = {254,210,254,254,250};
  for(i=0;i<4;i++){
    db_hashfile_get_page(hfile,i);
    short fptr = get_the_first_pointer(hfile->page);
    short lptr = get_the_last_pointer(hfile->page);
	assert(fptr==fptr_answers[i]);
	assert(lptr==lptr_answers[i]);
  }
  
  free(rd);
  
  db_hashfile_close(hfile);
  db_drop(filename);
  printf("ok\n");
}

void db_hashfile_query_by_scan_success(){
  fprintf(stderr,"TEST:%s ... ", __func__);
  fflush(stderr);
  
  short nofbacket = 4;
  char *filename = "testfile.db";
  record *rd = (record *)malloc(sizeof(record));
  
  // == PREPARATION ==
  HFILE *hfile = new_HFILE();
  db_hashfile_create_and_open(hfile,filename);
  db_hashfile_prepare(hfile,nofbacket);
  int i;
  for(i=0;i<26;i++){
      char *str=(char *)malloc(2);
      str[0]=i+'A';
      str[1]='\0';
      strcpy(rd->name,str);
      rd->age = i;
	  db_hashfile_insert(hfile,rd);
  }
  db_save(hfile->fd,hfile->page);
  // == TEST ==
  recordList *result = db_hashfile_search_by_scan(hfile,"E");
  init_list(result);
  while(has_next(result)>0){
  	rd = get_next(result);
  	assert(strcmp(rd->name,"E")==0);
  	assert(rd->age==4);
  }
  
  db_hashfile_close(hfile);
  db_drop(filename);
  printf("ok\n");
}

void db_hashfile_query_by_hash_success(){
  fprintf(stderr,"TEST:%s ... ", __func__);
  fflush(stderr);
  
  short nofbacket = 4;
  char *filename = "testfile.db";
  record *rd = (record *)malloc(sizeof(record));
  
  // == PREPARATION ==
  HFILE *hfile = new_HFILE();
  db_hashfile_create_and_open(hfile,filename);
  db_hashfile_prepare(hfile,nofbacket);
  int i;
  for(i=0;i<26;i++){
      char *str=(char *)malloc(2);
      str[0]=i+'A';
      str[1]='\0';
      strcpy(rd->name,str);
      rd->age = i;
	  db_hashfile_insert(hfile,rd);
  }
  db_save(hfile->fd,hfile->page);
  // == TEST ==
  recordList *result = db_hashfile_search_by_hash(hfile,"E");
  init_list(result);
  while(has_next(result)>0){
  	rd = get_next(result);
  	assert(strcmp(rd->name,"E")==0);
  	assert(rd->age==4);
  }
  
  db_hashfile_close(hfile);
  db_drop(filename);
  printf("ok\n");
}

int main(){
 db_create_hashfile_and_open_makes_backet();
 db_hashfile_get_hash_returns_correct_value();
 db_hashfile_inserting_12_records_makes_overflow_backet();
 db_hashfile_inserting_5_records_store_in_file();
 db_hashfile_query_by_scan_success();
 db_hashfile_query_by_hash_success();
 printf("All test are passed\n");
}