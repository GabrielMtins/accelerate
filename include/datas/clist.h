#ifndef CLIST_H
#define CLIST_H

/* A Generic Type Linked List Structure */

/* macro for doing for in lists
 * list: pointer to a CList
 * item: the name of the iterator */
#define CList_ForEach(item, list) for(CItem *item = CList_GetFront(list); item != NULL; CItem_Next(list, &item))

typedef struct CItem_S{
	void *key;

	struct CItem_S *prev;
	struct CItem_S *next;
} CItem;

typedef struct{
	CItem *front;
	CItem *back;
} CList;

CItem * CItem_Create(void *key);

void * CItem_GetKey(CItem *item);

void CItem_SetKey(CItem *item, void *key);

void CItem_Previous(CList *list, CItem **item);

void CItem_Next(CList *list, CItem **item);

void CItem_Destroy(CItem *item);

CList * CList_Create(void);

CItem * CList_GetFront(CList *list);

CItem * CList_GetBack(CList *list);

void CList_PushFront(CList *list, void *key);

void CList_PushBack(CList *list, void *key);

void * CList_PopFront(CList *list);

void * CList_PopBack(CList *list);

/* Removes the item from the list and returns its key.
 * It will also change the reference to the pointer to the
 * item before the one removed on the list */
void * CList_Remove(CList *list, CItem **item);

void CList_Destroy(CList *list);

#endif
