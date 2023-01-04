/**
 * @file   item_stack.c
 * @author Ed Holmes (edholmes2232(at)gmail.com)
 * @brief  Interacts with Stacks of "items". Items should be enums.
 *         All functions return -1 for error. 
 *         Limitations:
 *             - item.id range 0-255 (uint8_t)
 *             - item.data can only be a 32 bit number
 *             - multiple items with same .id, different .data not supported
 * @date   2022-09-23
 */
#include "item_stack.h"
#include "stdint.h"
#include "string.h"


/**
 * @brief  Check if item is in the stack.
 * 
 * @param  ItemStack
 * @param  item
 * @return int8_t 
 */
static int FindItem(ITEM_STACK_T *ItemStack, uint8_t itemId) {
	/* check not empty */
	if (ItemStack->top != -1) {
		/* Search down the stack for error */
		for (int i = ItemStack->top; i >= 0; i--) { 
			if (ItemStack->stackItems[i].id == itemId) {
				return i;
			}
		}
		return -1;
	} else {
		return -1;
	}
}

void STACK_Init(ITEM_STACK_T *itemStack, ITEM_STACK_ORDERING_T stackOrdering) {
	memset(itemStack, 0, sizeof(ITEM_STACK_T));
	itemStack->maxSize 		= MAX_STACKED_ITEMS;
	itemStack->top 			= -1;
	itemStack->stackOrdering 	= stackOrdering;
}


int STACK_AddItem(ITEM_STACK_T *ItemStack, uint8_t itemId, uint32_t itemData) {
	/* Check not full */
	if (ItemStack->top != ItemStack->maxSize-1) {
		int pos = FindItem(ItemStack, itemId);
		/* Only add if error not in stack */
		if (pos == -1) {

			switch (ItemStack->stackOrdering) {
				case STACK_LIFO:
					ItemStack->top++;
					ItemStack->stackItems[ItemStack->top].id 	= itemId;
					ItemStack->stackItems[ItemStack->top].data 	= itemData;
					/* Return item location */
					return ItemStack->top;
					break;
				
				case STACK_ORDERED:
					for (int i = ItemStack->top; i >= 0; i--) { 
						if (ItemStack->stackItems[i].id > itemId) {
							/* Shift up Items */
							for (int j = ItemStack->top; j > i ; j--) {
								ItemStack->stackItems[j+1].id = ItemStack->stackItems[j].id;
								ItemStack->stackItems[j+1].data = ItemStack->stackItems[j].data;
							} 
							/* Increase Top */
							ItemStack->top++;
							ItemStack->stackItems[i+1].id 	= itemId;
							ItemStack->stackItems[i+1].data	= itemData;
							/* Return item location */
							return i+1;
						} else if (i == 0) {
							/* If at bottom, stick at bottom */
							/* Shift up Items */
							for (int j = ItemStack->top; j >= 0 ; j--) {
								ItemStack->stackItems[j+1].id = ItemStack->stackItems[j].id;
								ItemStack->stackItems[j+1].data = ItemStack->stackItems[j].data;
							} 
							/* Increase Top */
							ItemStack->top++;
							ItemStack->stackItems[i].id 	= itemId;
							ItemStack->stackItems[i].data	= itemData;
							/* Return item location */
							return i;
						}

					}

					/* If empty, just add */
					ItemStack->top++;
					ItemStack->stackItems[ItemStack->top].id 	= itemId;
					ItemStack->stackItems[ItemStack->top].data	= itemData;

					/* Return item location */
					return ItemStack->top;
					break;

				default:
					return -1;
					break;
			}

		} else {
			/* Already in stack, return error */
			return -1;
		}

	} else {
		/* Buffer full, return error */
		return -1;
	}
}

void STACK_RemoveItem(ITEM_STACK_T *ItemStack, uint8_t itemId) {
	int pos = FindItem(ItemStack, itemId);

	/* Return if not found */
	if (pos == -1) return;

	/* Go back up the stack, shifting all items down */
	for (int i = pos; i <= ItemStack->top; i++) {
		ItemStack->stackItems[i].id		= ItemStack->stackItems[i+1].id;
		ItemStack->stackItems[i].data 	= ItemStack->stackItems[i+1].data;
	}
	__asm("nop");
	ItemStack->stackItems[ItemStack->top].id = 0;
	ItemStack->stackItems[ItemStack->top].data = 0;

	ItemStack->top--;
}

ITEM_T *STACK_GetTopItem(ITEM_STACK_T *ItemStack) {
	if (ItemStack->top == -1) {
		/* If stack empty */
		return NULL;//-1;
	} 
	return &ItemStack->stackItems[ItemStack->top];
}
