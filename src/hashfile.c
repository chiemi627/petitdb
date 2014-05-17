#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "page.h"
#include "recordlist.h"
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
  hfile->fd = db_create_and_open(filename);
}

int db_hashfile_open(HFILE *hfile,char *filename){
  hfile->fd = db_open(filename);
  return hfile->fd;
}

/**
  データベースファイルを作って開く
 */
void db_hashfile_prepare(HFILE *hfile,short nofbackets){
   int i;
   int fd = hfile->fd;
   for(i=0;i<nofbackets;i++){
     generate_page(hfile->page);
     hfile->page->pageno=i;
     db_hashfile_write_page(hfile);   
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
  db_write_header(hfile->fd,(char *)(hfile->header),sizeof(hashfile_header));
}

/**
   ヘッダを取得する
 */
hashfile_header *db_hashfile_get_header(HFILE *hfile){
   db_get_header(hfile->fd,(char *)hfile->header,sizeof(hashfile_header));
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
     set_next_page_no(hfile->page, nextpage);
     db_hashfile_write_page(hfile);   
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

/**
  ヒープファイルから1レコードを読み込む
 */
record *db_hashfile_read(HFILE *hfile){

  record *rd=NULL;
  
  if(hfile->page->pagebuf == NULL){
    db_hashfile_get_page(hfile,0);
  }
  
  while(rd==NULL){
  	rd = read_record(hfile->page);
    if(rd==NULL){
      short nextpage = get_next_page_no(hfile->page);
      if(nextpage==-1){
      	return NULL;
      }
      else{
      	//新しいページに移動する
        db_hashfile_get_page(hfile,nextpage);
      }
    }
  }
  return rd; 
}


void db_hashfile_get_page(HFILE *hfile,short pno){
  get_page(hfile->fd,pno,hfile->page);
}


int db_hashfile_close(HFILE *hfile){
  if(hfile==NULL){
    return -1;
  }
  db_close(hfile->fd);
  free(hfile->page->pagebuf);
}

recordList *db_hashfile_search_by_scan(HFILE *hfile,char *keyword){
	int i;
	record *rd;
	recordList *result = newList();
	for(i=0;i<db_hashfile_get_number_of_initial_backets(hfile);i++){
		db_hashfile_get_page(hfile,i);
		while((rd=db_hashfile_read(hfile))!=NULL){
			if(strcmp(rd->name,keyword)==0){
				record *rd2=(record *)malloc(sizeof(record));
				strcpy(rd2->name,rd->name);
				rd2->age = rd->age;	
				if(add_record(result,rd2)<0){
					return NULL;
				}
			}
		}
  	}
  	return result;
}

recordList *db_hashfile_search_by_hash(HFILE *hfile,char *keyword){
	short h;
	record *rd;
	recordList *result = newList();
	h = db_hashfile_get_hash(keyword, hfile->header->nofbackets_init);
	db_hashfile_get_page(hfile,h);
	while((rd=db_hashfile_read(hfile))!=NULL){
		if(strcmp(rd->name,keyword)==0){
			record *rd2=(record *)malloc(sizeof(record));
			strcpy(rd2->name,rd->name);
			rd2->age = rd->age;
			if(add_record(result,rd2)<0){
				return NULL;
			}
		}	
  	}
  	return result;
}

void db_hashfile_write_page(HFILE *hfile){
     write_page(hfile->fd,hfile->page);
}

