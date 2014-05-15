#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "page.h"
#include "dbfile.h"
#include "hashfile.h"

/**
  ハッシュファイルの作業用変数領域を作る
 */
HFILE *new_HFILE(){
  HFILE *hfile = (HFILE *)malloc(sizeof(HFILE));
  hfile->page = (PAGE *)malloc(sizeof(PAGE));
  hfile->page->pagebuf = (char *)malloc(PAGESIZE);
  hfile->header = (hashfile_header *)malloc(sizeof(hashfile_header));  
  return hfile;
}

void db_hashfile_create_and_open(HFILE *hfile,char *filename){
  hfile->fp = db_create_and_open(filename);
}

/**
  データベースファイルを作って開く
 */
void db_hashfile_prepare(HFILE *hfile,short nofbackets){
   int i;
   FILE *fp = hfile->fp;
   PAGE *page = (PAGE *)malloc(PAGESIZE);
   for(i=0;i<nofbackets;i++){
     generate_page(page);
     page->pageno=i;
     write_page(fp,page);   
   }
   hfile->header->nofbackets_init = nofbackets;
   hfile->header->nofbackets_all = nofbackets;
   db_hashfile_write_header(hfile);
 }
/**
  ヘッダを保存する
 */
int db_hashfile_write_header(HFILE *hfile){
  if(hfile->header==NULL){
    return -1;
  }
  fseek(hfile->fp,0,SEEK_SET);
  fwrite(hfile->header,sizeof(hashfile_header),1,hfile->fp);
  fflush(hfile->fp);
}

/**
   ヘッダを取得する
 */
hashfile_header *db_hashfile_get_header(HFILE *hfile){
  fseek(hfile->fp,0,SEEK_SET);
  fread(hfile->header,sizeof(hashfile_header),1,hfile->fp);
  return hfile->header;
}

/**
   初期バケット数を出力する
 */
short db_hashfile_get_number_of_initial_backets(HFILE *hfile){
  if(hfile->header==NULL){
    return -1;
  }
  else{
    return hfile->header->nofbackets_init;
  }
}

/**
   全バケット数を出力する
 */
short db_hashfile_get_number_of_all_backets(HFILE *hfile){
  if(hfile->header==NULL){
    return -1;
  }
  else{
    return hfile->header->nofbackets_all;
  }
}

/**
  レコードの属性値でハッシュ値を求める
*/
short db_hashfile_get_hash(char *value, short nofbackets){
  int i=0;
  unsigned int hashvalue = 0;
  unsigned int mask = nofbackets-1;

  for(i=0;i<strlen(value);i++){
    hashvalue += (value[i]-'0');
  }
  return (short)(hashvalue & mask);
}

/**
  バケットチェーンを辿って最後の1ページを取り出す
 */
short db_hashfile_get_the_last_page(HFILE *hfile){
  short pageno = hfile->page->pageno;
  short nextpage = hfile->page->pageno;
  int overflow = -1;
  while(nextpage>=0){
    pageno = nextpage;
    //get_page(hfile->fp,pageno,hfile->page);
    db_hashfile_get_page(hfile,pageno);
    nextpage = get_next_page_no(hfile->page);
    overflow++;
  }
  return hfile->page->pageno;
}

/**
  バケットにレコードを挿入する（name属性を使う）
 */
int db_hashfile_insert(HFILE *hfile, record *rd){
  short overflow = 0;
  short backetno = db_hashfile_get_hash(rd->name,hfile->header->nofbackets_init);
  db_hashfile_get_page(hfile,backetno);
  overflow = db_hashfile_get_the_last_page(hfile);
  short nextpage;  
  //オーバーフロー
  if(insert_record_into_page(hfile->page,rd)<0){
     nextpage = hfile->header->nofbackets_all;
     //前のページに次のページの番号を書いて保存する
     memcpy((hfile->page->pagebuf),&nextpage,sizeof(short));
     write_page(hfile->fp,hfile->page);
     //次のページを作成する(pagebufは上書きする)
     generate_page(hfile->page);
     hfile->page->pageno = nextpage;
     if(insert_record_into_page(hfile->page,rd)<0){
       return -1;
     }
     hfile->header->nofbackets_all++;
     overflow++;
  }
  return overflow;
}

void db_hashfile_get_page(HFILE *hfile,short pno){
  get_page(hfile->fp,pno,hfile->page);
}


int db_hashfile_close(HFILE *hfile){
  if(hfile==NULL){
    return -1;
  }
  if(hfile->fp!=NULL){
    db_close(hfile->fp);
  }
  free(hfile->page->pagebuf);
}

