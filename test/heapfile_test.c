#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "page.h"
#include "recordlist.h"
#include "dbfile.h"
#include "heapfile.h"

void db_open_returns_NULL_if_file_is_not_found(){

  fprintf(stderr,"db_open_returns_NULL_if_file_is_not_found ...");
  fflush(stderr);

  char *dammyfile = "xxxxx";
  int fd = db_open(dammyfile);
  assert(fd==-1);
  
  fprintf(stderr,"ok\n");
  fflush(stderr);
}

void db_create_and_open_has_the_first_page(){

  fprintf(stderr,"db_create_and_open_has_the_first_page ...");
  fflush(stderr);


  HEAPFILE *hfile = new_HEAPFILE();

  char *filename = "testfile.db";  
  db_heapfile_create_and_open(hfile,filename);  
  db_heapfile_get_page(hfile,0);
  unsigned short fptr = get_the_first_pointer(hfile->page);
  printf("fptr=%d\n",fptr);
  assert(fptr==6);
  unsigned short lptr = get_the_last_pointer(hfile->page);
  assert(lptr==254);
  
  short pageno = get_next_page_no(hfile->page);  
  assert(pageno==-1);
  
  db_heapfile_close(hfile);
  db_drop(filename);
  
  fprintf(stderr,"ok\n");
  fflush(stderr);
    
}

void db_insert_store_the_sample_record_in_pagebuffer(){

  fprintf(stderr,"db_insert_store_the_sample_record_in_pagebuffer ...");
  fflush(stderr);
  
  HEAPFILE *hfile = new_HEAPFILE();
  
  char *filename = "testfile.db";

  record *rd = (record *)malloc(sizeof(record));
  strcpy(rd->name,"abc");
  rd->age = 10;
  
  //�f�[�^�x�[�X���J���i���͋�j
  db_heapfile_create_and_open(hfile,filename);
  db_heapfile_get_page(hfile,0);
  
  //�T���v�����R�[�h����}������
  db_heapfile_insert(hfile,rd);
  
  //Test1: ���R�[�h�̋󂫃|�C���^�� 6+sizeof(record)�ɂȂ��Ă���
  unsigned short fptr = get_the_first_pointer(hfile->page);
  assert(fptr == 6+sizeof(record));
  //Test2: ���R�[�h���̋󂫃|�C���^�� BUFSIZE-sizeof(short)*(2+1)�ɂȂ��Ă���
  short lptr = get_the_last_pointer(hfile->page);
  assert(lptr == PAGESIZE-sizeof(short)*3);
  //Test3: �ŏ��̃��R�[�h��񂪍ŏ��̃��R�[�h�̈ʒu���w���Ă���  
  char *ptr = (hfile->page->pagebuf)+PAGESIZE-sizeof(short)*2;
  short *record_pos = (short *)ptr;
  assert((*record_pos)==sizeof(short)*3);
  //Test4:Test3�Ŏh���ꂽ�Ƃ���Ƀ��R�[�h���i�[����Ă���
  rd = (record *)((hfile->page->pagebuf) + (*record_pos));
  assert(strcmp(rd->name,"abc")==0);
  assert(rd->age==10);
  //��Еt��
  db_heapfile_close(hfile);
  db_drop(filename);  

  fprintf(stderr,"ok\n");
  fflush(stderr);

}

void db_insert_store_the_sample_record_in_dbfile(){

  fprintf(stderr,"db_insert_store_the_sample_record_in_dbfile ...");
  fflush(stderr);

  char *filename = "testfile.db";
  HEAPFILE *hfile = new_HEAPFILE();
  
  record *rd = (record *)malloc(sizeof(record));
  strcpy(rd->name,"abc");
  rd->age = 10;
  
  //�f�[�^�x�[�X���J���i���͋�j
  db_heapfile_create_and_open(hfile,filename);
  db_heapfile_get_page(hfile,0);
  //�T���v�����R�[�h����}������
  db_heapfile_insert(hfile,rd);
  
  //Test1: ���R�[�h�̋󂫃|�C���^�� 6+sizeof(record)�ɂȂ��Ă���
  short fptr2 = get_the_first_pointer(hfile->page);
  assert(fptr2 == 6+sizeof(record));
  //db_save(hfile->fd,hfile->page);
    
  //������x�J���Ȃ���
  //��ԍŏ��̃y�[�W���擾����
  db_heapfile_get_page(hfile,0);
  //Test1: ���R�[�h�̋󂫃|�C���^�� 6+sizeof(record)�ɂȂ��Ă���
  short fptr = get_the_first_pointer(hfile->page);
  assert(fptr == 6+sizeof(record));
  //Test2: ���R�[�h���̋󂫃|�C���^�� PAGESIZE-sizeof(short)*(1+2)�ɂȂ��Ă���
  short lptr = get_the_last_pointer(hfile->page);
  assert(lptr == PAGESIZE-sizeof(short)*3);
  //Test3: �ŏ��̃��R�[�h��񂪍ŏ��̃��R�[�h�̈ʒu���w���Ă���  
  char *ptr = (hfile->page->pagebuf)+PAGESIZE-sizeof(short)*2;
  short *record_pos = (short *)ptr;
  assert((*record_pos)==sizeof(short)*3);
  //Test4:Test3�Ŏh���ꂽ�Ƃ���Ƀ��R�[�h���i�[����Ă���
  rd = (record *)((hfile->page->pagebuf) + (*record_pos));
  assert(strcmp(rd->name,"abc")==0);
  assert(rd->age==10);
  //��Еt��
  db_heapfile_close(hfile);
  db_drop(filename);

  fprintf(stderr,"ok\n");
  fflush(stderr);

}

void generate_page_has_the_header_for_the_first_page(){

  fprintf(stderr,"generate_page_has_the_header_for_the_first_page ...");
  fflush(stderr);
  
  PAGE *page = (PAGE *)malloc(PAGESIZE);
  page->pagebuf = NULL;
  generate_page(page);
  short pageno = get_next_page_no(page);
  assert(pageno==-1);
  short fptr = get_the_first_pointer(page);
  assert(fptr == 6);
  short lptr = get_the_last_pointer(page);
  assert(lptr == PAGESIZE-sizeof(short));
  free(page->pagebuf);
  free(page);

  fprintf(stderr,"ok\n");
  fflush(stderr);

}

void db_insert_11_records_in_one_page(){

  fprintf(stderr,"db_insert_11_records_in_one_page ...");
  fflush(stderr);

  HEAPFILE *hfile = new_HEAPFILE();
  char *filename = "testfile.db";

  record *rd = (record *)malloc(sizeof(record));
    
  //�f�[�^�x�[�X���J���i���͋�j
  db_heapfile_create_and_open(hfile,filename);
  db_heapfile_get_page(hfile,0);
  
  //�T���v�����R�[�h����}������
  int i;
  for(i=0;i<11;i++){
      strcpy(rd->name,"abc");
      rd->age = i;
	  db_heapfile_insert(hfile,rd);
  }
  //db_save(hfile->fp,hfile->page);
  
  //��ԍŏ��̃y�[�W���擾����
  db_heapfile_get_page(hfile,0);
  //Test1: ���R�[�h�̋󂫃|�C���^�� 6+sizeof(record)�ɂȂ��Ă���
  short fptr = get_the_first_pointer(hfile->page);
  assert(fptr == 204);
  //Test2: ���R�[�h���̋󂫃|�C���^�� PAGESIZE-sizeof(short)*2�ɂȂ��Ă���
  short lptr = get_the_last_pointer(hfile->page);
  assert(lptr == 210);
    
  //��Еt��
  db_heapfile_close(hfile);
  db_drop(filename);

  fprintf(stderr,"ok\n");
  fflush(stderr);

}

void db_insert_12_records_in_two_pages(){

  fprintf(stderr,"db_insert_12_records_in_two_pages ...");
  fflush(stderr);

  char *filename = "testfile.db";
  HEAPFILE *hfile = new_HEAPFILE();

  record *rd = (record *)malloc(sizeof(record));
    
  //�f�[�^�x�[�X���J��
  db_heapfile_create_and_open(hfile,filename);
  db_heapfile_get_page(hfile,0);
  //�T���v�����R�[�h����}������
  int i;
  for(i=0;i<12;i++){
      strcpy(rd->name,"abc");
      rd->age = i;
	  db_heapfile_insert(hfile,rd);
  }
  //db_save(hfile->fp,hfile->page);  
  //�Ō�̃y�[�W
  assert(hfile->page->pageno==1);
  short pno = db_heapfile_get_the_last_page(hfile);
  assert(pno==1);

  db_heapfile_get_page(hfile,0);
  //Test1: ���R�[�h�̋󂫃|�C���^��204�ɂȂ��Ă���
  short fptr = get_the_first_pointer(hfile->page);
  assert(fptr == 204);
  //Test2: ���R�[�h���̋󂫃|�C���^��212�ɂȂ��Ă���
  short lptr = get_the_last_pointer(hfile->page);
  assert(lptr == 210);

  db_heapfile_get_page(hfile,1);
  //Test1: ���R�[�h�̋󂫃|�C���^��24�ɂȂ��Ă���
  fptr = get_the_first_pointer(hfile->page);
  assert(fptr == 24);
  //Test2: ���R�[�h���̋󂫃|�C���^��250�ɂȂ��Ă���
  lptr = get_the_last_pointer(hfile->page);
  assert(lptr == 250);


  //��Еt��
  db_heapfile_close(hfile);
  db_drop(filename);

  fprintf(stderr,"ok\n");
  fflush(stderr);

}


void db_insert_100_records_generate_9_pages(){

  fprintf(stderr,"db_insert_100_records_generate_9_pages ...");
  fflush(stderr);

  char *filename = "testfile.db";
  HEAPFILE *hfile = new_HEAPFILE();
  record *rd = (record *)malloc(sizeof(record));
    
  //�f�[�^�x�[�X���J��
  db_heapfile_create_and_open(hfile,filename);
  db_heapfile_get_page(hfile,0);
  //�T���v�����R�[�h����}������
  int i;
  for(i=0;i<100;i++){
      strcpy(rd->name,"abc");
      rd->age = i;
	  db_heapfile_insert(hfile,rd);
  }
  //db_save(hfile->fp,hfile->page);
  
  //�Ō�̃y�[�W
  assert(hfile->page->pageno==9);
  short pno = db_heapfile_get_the_last_page(hfile);
  assert(pno==9);
   
  //��Еt��
  db_heapfile_close(hfile);
  db_drop(filename);

  fprintf(stderr,"ok\n");
  fflush(stderr);
}

void db_insert_100_records_and_read(){

  fprintf(stderr,"db_insert_100_records_and_read ...");
  fflush(stderr);


  char *filename = "testfile.db";
  HEAPFILE *hfile = new_HEAPFILE();
  record *rd = (record *)malloc(sizeof(record));
    
  //�f�[�^�x�[�X���J���i���͋�j
  db_heapfile_create_and_open(hfile,filename);
  db_heapfile_get_page(hfile,0);
  //�T���v�����R�[�h����}������
  int i;
  for(i=0;i<100;i++){
      strcpy(rd->name,"abc");
      rd->age = i;
	  db_heapfile_insert(hfile,rd);
  }
  db_save(hfile->fd,hfile->page);
  
  hfile->page->pagebuf = NULL;
  int cnt=0;
  while((rd = db_heapfile_read(hfile))!=NULL){
    assert(rd->age==cnt);
    cnt++;
  }
  
   
  //��Еt��
  db_heapfile_close(hfile);
  db_drop(filename);

  fprintf(stderr,"ok\n");
  fflush(stderr);

}

int main(){
	db_open_returns_NULL_if_file_is_not_found();
	generate_page_has_the_header_for_the_first_page();
	db_create_and_open_has_the_first_page();
	db_insert_store_the_sample_record_in_pagebuffer();
	db_insert_store_the_sample_record_in_dbfile();
	db_insert_11_records_in_one_page();
    db_insert_12_records_in_two_pages();
    db_insert_100_records_generate_9_pages();
    db_insert_100_records_and_read();
	printf("All test are passed\n");
}