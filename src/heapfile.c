#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "page.h"
#include "recordlist.h"
#include "dbfile.h"
#include "heapfile.h"
/**
  ヒープファイルの作業用変数領域を作る
 */
HEAPFILE *new_HEAPFILE(){
  HEAPFILE *hfile = (HEAPFILE *)malloc(sizeof(HEAPFILE));
  hfile->page = (PAGE *)malloc(sizeof(PAGE));
  hfile->page->pagebuf = (char *)malloc(PAGESIZE);
  return hfile;
}

void db_heapfile_create_and_open(HEAPFILE *hfile,char *filename){
  hfile->fp = db_create_and_open(filename);
}

/**
  ディスクから最後の1ページを取り出す
  （スキャンする）
 */
short db_heapfile_get_the_last_page(HEAPFILE *hfile){
  short pageno = 0;
  short nextpage = 0;
  while(nextpage>=0){
    pageno = nextpage;
    get_page(hfile->fp,pageno,hfile->page);
    nextpage = get_next_page_no(hfile->page);
  }
  return hfile->page->pageno;
}


/**
  ヒープファイルから1レコードを読み込む
 */
record *db_heapfile_read(HEAPFILE *hfile){

  record *rd=NULL;
  
  if(hfile->page->pagebuf == NULL){
    get_page(hfile->fp,0,hfile->page);
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
      	get_page(hfile->fp,nextpage,hfile->page);
      }
    }
  }
  return rd; 
}

/**
  ヒープファイルへ1レコードを挿入する
  作業バッファのページがいっぱいになったところで
  初めてディスクへ書き込む
  それ以外のタイミングは db_saveを呼び出したとき
*/
int db_heapfile_insert(HEAPFILE *hfile, record *rd){

  short pageno,nextpage;

  if(hfile->page->pagebuf == NULL){
    pageno = db_heapfile_get_the_last_page(hfile);
  }
  if(insert_record_into_page(hfile->page,rd)<0){
     nextpage = hfile->page->pageno + 1;
     //前のページに次のページの番号を書いて保存する
     memcpy((hfile->page->pagebuf),&nextpage,sizeof(short));
     write_page(hfile->fp,hfile->page);
     //次のページを作成する(pagebufは上書きする)
     generate_page(hfile->page);
     hfile->page->pageno = nextpage;
     if(insert_record_into_page(hfile->page,rd)<0){
       return -1;
     }
  }
}

void db_heapfile_get_page(HEAPFILE *hfile,short pno){
  get_page(hfile->fp,pno,hfile->page);
}

int db_heapfile_close(HEAPFILE *hfile){
  if(hfile==NULL){
    return -1;
  }
  if(hfile->fp!=NULL){
    db_close(hfile->fp);
  }
  free(hfile->page->pagebuf);
}

recordList *db_heapfile_search_by_scan(HEAPFILE *hfile,char *keyword){
	int i;
	record *rd;
	recordList *result = newList();
	db_heapfile_get_page(hfile,0);	
	while((rd=db_heapfile_read(hfile))!=NULL){
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