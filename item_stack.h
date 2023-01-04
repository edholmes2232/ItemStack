#ifndef ITEM_STACK_H_
#define ITEM_STACK_H_

#include "stdio.h"
#include "stdint.h"

/** Avoids Dynamic Memory Allocation **/
#define MAX_STACKED_ITEMS	16


typedef enum {
	STACK_LIFO,			/*!< Last In, First Out stack */
	STACK_ORDERED,		/*!< Items are ordered by their ID value */
} ITEM_STACK_ORDERING_T;


typedef struct {
	uint8_t 	id;
	uint32_t 	data;
} ITEM_T;


typedef struct {
	uint8_t 				maxSize;
	ITEM_STACK_ORDERING_T	stackOrdering;
	int16_t 				top;
	ITEM_T 					stackItems[MAX_STACKED_ITEMS];
} ITEM_STACK_T;


void		STACK_Init(ITEM_STACK_T *itemStack, ITEM_STACK_ORDERING_T stackOrdering);
int 		STACK_AddItem(ITEM_STACK_T *ItemStack, uint8_t itemId, uint32_t itemData);
void 		STACK_RemoveItem(ITEM_STACK_T *ItemStack, uint8_t itemId);
ITEM_T	*STACK_GetTopItem(ITEM_STACK_T *ItemStack);


#endif
