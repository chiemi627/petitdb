#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "string.h"
#include "stdlib.h"
#include "page.h"
#include "dbfile.h"

/**
  �f�[�^�x�[�X�t�@�C��������ĊJ��
 */
int db_create_and_open(char *filename){
    int fd;
//    FILE *fp;
    PAGE *page = (PAGE *)malloc(PAGESIZE);
    page->pagebuf = NULL;
    
    fd = open(filename,O_RDWR|O_CREAT|O_TRUNC);
    if(fd<0){
		printf("file open error\n");
		return -1;
	}
	
   if(generate_page(page)>0){
	   write_page(fd,page);
   }else{
       fprintf(stderr,"ERROR: write_flg = 1\n");
   }
   return fd;
}

/**
  �f�B�X�N����1�y�[�W���o��
 */
void get_page(int fd,short pageno, PAGE *page){
  if(page->pagebuf==NULL){
    page->pagebuf = (char *)malloc(PAGESIZE);
  }
  else{
    if(page->write_flg==1){
      write_page(fd,page);
    }
  }
  lseek(fd,HEADERSIZE+PAGESIZE*pageno,SEEK_SET);
  if(read(fd,page->pagebuf,PAGESIZE)!=PAGESIZE){
    return;
  }
  
  page->pageno = pageno;
  page->rinfo_ptr = (page->pagebuf) + PAGESIZE - sizeof(short);
  page->write_flg = 0;
}

/**
  ��ƃo�b�t�@��̃y�[�W���f�B�X�N�ɕۑ�����
 */
int write_page(int fd,PAGE *page){
  lseek(fd,HEADERSIZE+(page->pageno)*PAGESIZE,SEEK_SET);
  int hsize = HEADERSIZE;
  if(write(fd,page->pagebuf,PAGESIZE)!=PAGESIZE){
    return -1;
  }
  //fflush(fp);
  fsync(fd);
  page->write_flg=0;
  return 1;
}

/**
  �f�[�^�x�[�X�t�@�C�����J��
 */
int db_open(char *filename){   
    int fd;
    
    fd = open(filename,O_RDWR);
	if(fd<0){
		printf("file open error\n");
		return -1;
	}
	return fd;
}

/**
  �f�[�^�x�[�X�t�@�C�������
 */
void db_close(int fd){
  if(fd>0){
    close(fd);
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

int db_save(int fd, PAGE *page){
	return write_page(fd,page);
}

int db_delete(int fd, PAGE *page){
    if(page==NULL){
      return -1;
    }
    short flg = RECORDEMPTY;
    memcpy(page->rinfo_ptr,&flg,sizeof(short));
    return 1;
}

/**
  �w�b�_��ۑ�����
 */
int db_write_header(int fd,char *header,int size){
  if(header==NULL){
    return -1;
  }
  lseek(fd,0,SEEK_SET);
  write(fd,header,size);
  fsync(fd);
}

/**
   �w�b�_���擾����
 */
void db_get_header(int fd,char *header,int size){
  lseek(fd,0,SEEK_SET);
  read(header,size,fd);
  return;
}