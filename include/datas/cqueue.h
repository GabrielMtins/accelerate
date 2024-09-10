#ifndef CQUEUE_H
#define CQUEUE_H

#include "clist.h"

/* Just a series of macros that use clist.h as a base
 * to create a queue-like structure implementation */

typedef CList CQueue;

#define CQueue_Create() CList_Create()
#define CQueue_Enqueue(stack, key) CList_PushBack(stack, key)
#define CQueue_Dequeue(stack) CList_PopFront(stack)
#define CQueue_Destroy(stack) CList_Destroy(stack)
#define CQueue_Front(stack) CItem_GetKey(CList_GetFront(stack))
#define CQueue_IsEmpty(stack) (CList_GetFront(stack) == NULL)

#endif
