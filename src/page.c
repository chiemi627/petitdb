#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "page.h"

/**
  作業バッファ上に読み込まれたページから1レコード文を読む
  ページ上にこれ以上読むレコードがなければNULLを返す
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
  新しいページを作って作業バッファ上に置く
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

  //次のページへのポインタ
  short next_page = -1;
  //memcpy(ptr,&next_page,sizeof(short));
  set_next_page_no(page,next_page);
  ptr += sizeof(short);
  //レコードの最終地点
  short fptr = sizeof(short)*3;
  memcpy(ptr,&fptr,sizeof(short));
  ptr += sizeof(short);
  //レコード情報の最終地点  
  short eptr = PAGESIZE-sizeof(short);   
  memcpy(ptr,&eptr,sizeof(short));
  //レコード情報の先頭にRECORDENDを置く
  short rinfo = RECORDEND;
  ptr = page->pagebuf + PAGESIZE - sizeof(short);
  memcpy(ptr,&rinfo,sizeof(short));
  
}

/**
  作業バッファ上に置かれたページに対して、1レコード追加
  する空白があるかを確認する。あれば１をなければー１を返す
 */  
int has_more_space(PAGE *page){
  short fptr;
  short eptr;
  char *ptr = page->pagebuf + sizeof(short);
  //ページの途中に間が空いていても気にしない
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
  作業バッファ上に置かれたページに対して、1レコードを追加
  する。追加できたら1を、できなければ-1を返す
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
  
  //fptrのところに新しいレコードを置く
  memcpy(fptr,rd,sizeof(record));
  //eptrのところに新しいレコード情報を置く
  memcpy(eptr,&rinfo,sizeof(short));
  eptr -= sizeof(short);
  memcpy(eptr,&fptr_no,sizeof(short));
  
  //eptrの最後にRECORDENDを置く
  eptr -= sizeof(short);  
  rinfo = RECORDEND;
  memcpy(eptr,&rinfo,sizeof(short));
  
  //レコードの最終地点を更新する
  fptr_no += sizeof(record);  
  memcpy(header_fptr,&fptr_no,sizeof(short));
  //レコード情報の最終地点を更新する
  eptr_no -= sizeof(short)*2;
  memcpy(header_eptr,&eptr_no,sizeof(short));
  
  //書き込んだフラグをPAGEにセットする
  page->write_flg=1;
  
  return 1;
}

/**
  作業バッファ上に置かれたページの次のページ番号を返す
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