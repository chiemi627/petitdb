#include <stdio.h>
#include <stdlib.h>
#include "page.h"
#include "recordlist.h"

recordList *newList(){
 recordList *rlist = (recordList *)malloc(sizeof(recordList));
 rlist->first = NULL;
 rlist->last = NULL;
 rlist->current = NULL;
}

int add_record(recordList *list, record *rd){

 recordNode *node = (recordNode *)malloc(sizeof(recordNode));
 node->record = rd;
 node->next = NULL;
 
 if(list->first == NULL){
   list->first = node;
   list->last = node;
   list->current = node;
 }
 else{
	if(list->last==NULL){return -1;}
	list->last->next = node;
	list->last = node;
 }
}

void init_list(recordList *list){
	list->current = (recordNode *)malloc(sizeof(recordNode));
	list->current->record = NULL;
	list->current->next = list->first;
}

int has_next(recordList *list){
  if(list->current==NULL) return -1;
  if(list->current->next==NULL){
    return -1;
  }
  else{
    return 1;
  }
}

record *get_next(recordList *list){
  if(list->current==NULL) return NULL;
  if(list->current->next==NULL){
    return NULL;
  }
  else{
    list->current = list->current->next;
    record *rd = list->current->record;
    return rd;
  }  
}
