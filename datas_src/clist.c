#include "datas/clist.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

CItem * CItem_Create(void *key){
	CItem *item;

	item = (CItem *) malloc(sizeof(CItem));

	item->key = key;
	item->prev = item->next = NULL;

	return item;
}

void * CItem_GetKey(CItem *item){
	if(item == NULL) return NULL;

	return item->key;
}

void CItem_SetKey(CItem *item, void *key){
	item->key = key;
}

void CItem_Previous(CList *list, CItem **item){
	if(*item != NULL)
		*item = (*item)->prev;
	else
		*item = list->back;
}

void CItem_Next(CList *list, CItem **item){
	if(*item != NULL)
		*item = (*item)->next;
	else
		*item = list->front;
}

void CItem_Destroy(CItem *item){
	free(item);
}

CList * CList_Create(void){
	CList *list;

	list = (CList *) malloc(sizeof(CList));

	list->front = list->back = NULL;

	return list;
}

CItem * CList_GetFront(CList *list){
	if(list == NULL) return NULL;
	return list->front;
}

CItem * CList_GetBack(CList *list){
	if(list == NULL) return NULL;
	return list->back;
}

void CList_PushFront(CList *list, void *key){
	CItem *tmp;

	if(list->front == NULL){ /* if front is NULL, the list is empty */
		list->front = CItem_Create(key);
	}
	else if(list->back == NULL){ /* if back is NULL, the list has only one item */
		list->back = list->front;
		list->front = CItem_Create(key);
		list->front->next = list->back;
		list->back->prev = list->front;
	}
	else{ /* if the list has two or more items */
		tmp = list->front;
		list->front = CItem_Create(key);
		list->front->next = tmp;
		tmp->prev = list->front;
	}
}

void CList_PushBack(CList *list, void *key){
	CItem *tmp;

	if(list == NULL) return;

	if(list->front == NULL){ /* if front is NULL, the list is empty */
		list->front = CItem_Create(key);
	}
	else if(list->back == NULL){ /* if back is NULL, the list has only one item */
		list->back = CItem_Create(key);
		list->front->next = list->back;
		list->back->prev = list->front;
	}
	else{ /* if the list has two or more items */
		tmp = list->back;
		list->back = CItem_Create(key);
		list->back->prev = tmp;
		tmp->next = list->back;
	}
}

void * CList_PopFront(CList *list){
	CItem *tmp;
	void *key;

	if(list == NULL) return NULL;

	if(list->front == NULL) key = NULL; /* list is empty */
	else if(list->back == NULL){ /* only one item */
		key = list->front->key;
		CItem_Destroy(list->front);
		list->front = list->back;
		list->back = NULL;
	}
	else{
		key = list->front->key;
		if(list->front->next == list->back){ /* if the list has two items */
			CItem_Destroy(list->front);

			list->back->prev = NULL;
			list->front = list->back;
			list->back = NULL;
		}
		else{ /* otherwise */
			tmp = list->front->next;

			CItem_Destroy(list->front);

			list->front = tmp;
			list->front->prev = NULL;
		}
	}

	return key;
}

void * CList_PopBack(CList *list){
	CItem *tmp;
	void *key;

	if(list == NULL) return NULL;

	if(list->front == NULL) key = NULL; /* list is empty */
	else if(list->back == NULL){ /* only one item */
		key = list->front->key;
		CItem_Destroy(list->front);
		list->front = NULL;
	}
	else{
		key = list->back->key;
		if(list->front->next == list->back){ /* if the list has two items */
			CItem_Destroy(list->back);
			list->back = NULL;
			list->front->next = NULL;
		}
		else{ /* otherwise */
			tmp = list->back->prev;

			CItem_Destroy(list->back);

			list->back = tmp;
			list->back->next = NULL;
		}
	}

	return key;
}

void * CList_Remove(CList *list, CItem **item){
	CItem *prev_item, *next_item;
	void *key;

	if(list == NULL) return NULL;

	if(list->front == *item){ /* the item is on the front, so just use CList_PopFront */
		key = CList_PopFront(list);
		*item = NULL;
	}
	else if(list->back == *item){ /* the item is on the back, so just use CList_PopBack */
		key = CList_PopBack(list);
		*item = list->back;
	}
	else{ /* if it isn't on the start or the end of the list */
		key = (*item)->key;

		prev_item = (*item)->prev;
		next_item = (*item)->next;

		prev_item->next = next_item;
		next_item->prev = prev_item;

		CItem_Destroy(*item);

		*item = prev_item;
	}

	return key;
}

void CList_Destroy(CList *list){
	CItem *item, *tmp;

	item = list->front;

	while(item != NULL){
		tmp = item->next; /* saves the reference to the next element */
		CItem_Destroy(item); /* destroys current item */
		item = tmp; /* iterates to the next item */
	}

	free(list);
}
