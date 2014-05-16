#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "page.h"
#include "recordlist.h"
#include "dbfile.h"
#include "hashfile.h"

/**
  �n�b�V���t�@�C���̍�Ɨp�ϐ��̈�����
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
  �f�[�^�x�[�X�t�@�C��������ĊJ��
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
  �w�b�_��ۑ�����
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
   �w�b�_���擾����
 */
hashfile_header *db_hashfile_get_header(HFILE *hfile){
  fseek(hfile->fp,0,SEEK_SET);
  fread(hfile->header,sizeof(hashfile_header),1,hfile->fp);
  return hfile->header;
}

/**
   �����o�P�b�g�����o�͂���
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
   �S�o�P�b�g�����o�͂���
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
  ���R�[�h�̑����l�Ńn�b�V���l�����߂�
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
  �o�P�b�g�`�F�[����H���čŌ��1�y�[�W�����o��
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
  �o�P�b�g�Ƀ��R�[�h��}������iname�������g���j
 */
int db_hashfile_insert(HFILE *hfile, record *rd){
  short overflow = 0;
  short backetno = db_hashfile_get_hash(rd->name,hfile->header->nofbackets_init);
  db_hashfile_get_page(hfile,backetno);
  overflow = db_hashfile_get_the_last_page(hfile);
  short nextpage;  
  //�I�[�o�[�t���[
  if(insert_record_into_page(hfile->page,rd)<0){
     nextpage = hfile->header->nofbackets_all;
     //�O�̃y�[�W�Ɏ��̃y�[�W�̔ԍ��������ĕۑ�����
     memcpy((hfile->page->pagebuf),&nextpage,sizeof(short));
     write_page(hfile->fp,hfile->page);
     //���̃y�[�W���쐬����(pagebuf�͏㏑������)
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
  �q�[�v�t�@�C������1���R�[�h��ǂݍ���
 */
record *db_hashfile_read(HFILE *hfile){

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
      	//�V�����y�[�W�Ɉړ�����
      	get_page(hfile->fp,nextpage,hfile->page);
      }
    }
  }
  return rd; 
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
