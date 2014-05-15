#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "page.h"
#include "dbfile.h"

/**
  �f�[�^�x�[�X�t�@�C��������ĊJ��
 */
FILE *db_create_and_open(char *filename){
    FILE *fp;
    PAGE *page = (PAGE *)malloc(PAGESIZE);
    page->pagebuf = NULL;
    
	if((fp = fopen(filename, "wb+")) == NULL ) {
		printf("file open error\n");
		return NULL;
	}
	
   if(generate_page(page)>0){
	   write_page(fp,page);
   }else{
       fprintf(stderr,"ERROR: write_flg = 1\n");
   }
   return fp;
}

/**
  �f�B�X�N����1�y�[�W���o��
 */
void get_page(FILE *fp,short pageno, PAGE *page){
  if(page->pagebuf==NULL){
    page->pagebuf = (char *)malloc(PAGESIZE);
  }
  else{
    if(page->write_flg==1){
      write_page(fp,page);
    }
  }
  fseek(fp,HEADERSIZE+pageno*PAGESIZE,SEEK_SET);
  fread(page->pagebuf,PAGESIZE,1,fp);
  page->pageno = pageno;
  page->rinfo_ptr = (page->pagebuf) + PAGESIZE - sizeof(short);
  page->write_flg = 0;
}

/**
  ��ƃo�b�t�@��̃y�[�W���f�B�X�N�ɕۑ�����
 */
int write_page(FILE *fp,PAGE *page){
  fseek(fp,HEADERSIZE+(page->pageno)*PAGESIZE,SEEK_SET);
  int hsize = HEADERSIZE;
  if(fwrite(page->pagebuf,PAGESIZE,1,fp)!=1){
    return -1;
  }
  fflush(fp);
  page->write_flg=0;
  return 1;
}

/**
  �f�[�^�x�[�X�t�@�C�����J��
 */
FILE *db_open(char *filename){   
    FILE *fp;
    if((fp = fopen(filename, "r")) == NULL ) {
		return NULL;      
    }
    fclose(fp);
	if((fp = fopen(filename, "rb+")) == NULL ) {
		printf("file open error\n");
		return NULL;
	}
	return fp;
}

/**
  �f�[�^�x�[�X�t�@�C�������
 */
void db_close(FILE *fp){
  if(fp!=NULL){
    fclose(fp);
  }
}

/**
  �f�[�^�x�[�X�t�@�C�����폜����
 */
int db_drop(char *filename){
  if(remove(filename)==0){
    return 1;
  }
  else{
    fprintf(stderr,"fail to drop the database\n");
    return -1;
  }
}

int db_save(FILE *fp, PAGE *page){
	return write_page(fp,page);
}

int db_delete(FILE *fp, PAGE *page){
    if(page==NULL){
      return -1;
    }
    short flg = RECORDEMPTY;
    memcpy(page->rinfo_ptr,&flg,sizeof(short));
    return 1;
}