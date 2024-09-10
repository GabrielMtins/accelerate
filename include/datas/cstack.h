#ifndef CSTACK_H
#define CSTACK_H

#include "clist.h"

/* Just a series of macros that use clist.h as a base
 * to create a stack-like structure implementation */

typedef CList CStack;

#define CStack_Create() CList_Create()
#define CStack_Push(stack, key) CList_PushFront(stack, key)
#define CStack_Pop(stack) CList_PopFront(stack)
#define CStack_Destroy(stack) CList_Destroy(stack)
#define CStack_Top(stack) CItem_GetKey(CList_GetFront(stack))
#define CStack_IsEmpty(stack) (CList_GetFront(stack) == NULL)

#endif
