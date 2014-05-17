#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "page.h"

/**
  ��ƃo�b�t�@��ɓǂݍ��܂ꂽ�y�[�W����1���R�[�h����ǂ�
  �y�[�W��ɂ���ȏ�ǂރ��R�[�h���Ȃ����NULL��Ԃ�
 */
struct record *read_record(PAGE *page){

  char *ptr,*record_ptr;

  short *rinfo = (short *)(page->rinfo_ptr);
  while((*rinfo)!=RECORDEND){
    if((*rinfo)==RECORDEMPTY){
      rinfo -= 2; //go to next record info
    }
    else if((*rinfo)==RECORDEXIST){
      short *addr = rinfo-1;
      record *rd = (record *)(page->pagebuf + (*addr));
      page->rinfo_ptr =(char *)(rinfo-2);
      return rd;
    }
    else{
      fprintf(stderr,"read record error\n");
      return NULL;
    }
  }
  return NULL;
}

/**
  �V�����y�[�W������č�ƃo�b�t�@��ɒu��
 */
int generate_page(PAGE *page){
  
  if(page->pagebuf==NULL){
	page->pagebuf = (char *)malloc(PAGESIZE);
	page->pageno = 0;
	page->write_flg = 0;
  }
  else{
    if(page->write_flg==1){
       return -1;
    }
  }

  char *ptr = page->pagebuf;

  //���̃y�[�W�ւ̃|�C���^
  short next_page = -1;
  //memcpy(ptr,&next_page,sizeof(short));
  set_next_page_no(page,next_page);
  ptr += sizeof(short);
  //���R�[�h�̍ŏI�n�_
  short fptr = sizeof(short)*3;
  memcpy(ptr,&fptr,sizeof(short));
  ptr += sizeof(short);
  //���R�[�h���̍ŏI�n�_  
  short eptr = PAGESIZE-sizeof(short);   
  memcpy(ptr,&eptr,sizeof(short));
  //���R�[�h���̐擪��RECORDEND��u��
  short rinfo = RECORDEND;
  ptr = page->pagebuf + PAGESIZE - sizeof(short);
  memcpy(ptr,&rinfo,sizeof(short));
  
}

/**
  ��ƃo�b�t�@��ɒu���ꂽ�y�[�W�ɑ΂��āA1���R�[�h�ǉ�
  ����󔒂����邩���m�F����B����΂P���Ȃ���΁[�P��Ԃ�
 */  
int has_more_space(PAGE *page){
  short fptr;
  short eptr;
  char *ptr = page->pagebuf + sizeof(short);
  //�y�[�W�̓r���ɊԂ��󂢂Ă��Ă��C�ɂ��Ȃ�
  memcpy(&fptr,ptr,sizeof(short));
  ptr += sizeof(short);
  memcpy(&eptr,ptr,sizeof(short));
  ptr += sizeof(short);

  if( (fptr + sizeof(record)) >= (eptr - 4) ){
    return -1;
  }
  else{
    return 1;
  }
}

/**
  ��ƃo�b�t�@��ɒu���ꂽ�y�[�W�ɑ΂��āA1���R�[�h��ǉ�
  ����B�ǉ��ł�����1���A�ł��Ȃ����-1��Ԃ�
 */
int insert_record_into_page(PAGE *page, record *rd){

  if(has_more_space(page)==-1){
    return -1;
  }
  char *header_fptr  = page->pagebuf + sizeof(short);
  char *header_eptr = header_fptr + sizeof(short);
  short fptr_no;
  short eptr_no;
  char *fptr;
  char *eptr;
  short rinfo = RECORDEXIST;
  
  memcpy(&fptr_no,header_fptr,sizeof(short));
  memcpy(&eptr_no,header_eptr,sizeof(short));
  
  fptr = page->pagebuf + fptr_no;
  eptr = page->pagebuf + eptr_no;
  
  //fptr�̂Ƃ���ɐV�������R�[�h��u��
  memcpy(fptr,rd,sizeof(record));
  //eptr�̂Ƃ���ɐV�������R�[�h����u��
  memcpy(eptr,&rinfo,sizeof(short));
  eptr -= sizeof(short);
  memcpy(eptr,&fptr_no,sizeof(short));
  
  //eptr�̍Ō��RECORDEND��u��
  eptr -= sizeof(short);  
  rinfo = RECORDEND;
  memcpy(eptr,&rinfo,sizeof(short));
  
  //���R�[�h�̍ŏI�n�_���X�V����
  fptr_no += sizeof(record);  
  memcpy(header_fptr,&fptr_no,sizeof(short));
  //���R�[�h���̍ŏI�n�_���X�V����
  eptr_no -= sizeof(short)*2;
  memcpy(header_eptr,&eptr_no,sizeof(short));
  
  //�������񂾃t���O��PAGE�ɃZ�b�g����
  page->write_flg=1;
  
  return 1;
}

/**
  ��ƃo�b�t�@��ɒu���ꂽ�y�[�W�̎��̃y�[�W�ԍ���Ԃ�
 */  
int get_next_page_no(PAGE *page){
   short pageno;
   memcpy(&pageno,page->pagebuf,sizeof(short));
   return pageno;
}

unsigned short get_the_first_pointer(PAGE *page){
   short fptr;
   char *ptr = page->pagebuf+sizeof(short);
   memcpy(&fptr,ptr,sizeof(short));
   return fptr;
}

unsigned short get_the_last_pointer(PAGE *page){
   short lptr;
   char *ptr = page->pagebuf+sizeof(short)*2;
   memcpy(&lptr,ptr,sizeof(short));
   return lptr;
}

int set_next_page_no(PAGE *page, short pageno){
  memcpy(page->pagebuf,&pageno,sizeof(short));
}