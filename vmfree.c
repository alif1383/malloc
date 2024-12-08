#include "vm.h"
#include "vmlib.h"

/**
 * The vmfree() function frees the memory space pointed to by ptr,
 * which must have been returned by a previous call to vmmalloc().
 * Otherwise, or if free(ptr) has already been called before,
 * undefined behavior occurs.
 * If ptr is NULL, no operation is performed.
 */
void vmfree(void *ptr)
{
    if (ptr == NULL){
    return;
    }
	struct block_header *currBlock = (struct block_header *)((char *)ptr-sizeof(struct block_header));
	int busy = currBlock->size_status & VM_BUSY ? 1 : 0;
	int prevBusy = currBlock->size_status & VM_PREVBUSY ? 1: 0;
	size_t blocksz = BLKSZ(currBlock);
	struct block_header *nextBlock = (struct block_header *)((char *)currBlock+blocksz);
	int nextBusy = nextBlock->size_status & VM_BUSY ? 1:0;
	if (!busy){
	return;
	}
	vmFooter(currBlock);
	//currBlock ->size_status &= ~VM_BUSY;
	if (nextBusy){
	printf("\nnext Busy \n");
	nextBlock->size_status &= ~VM_PREVBUSY;
	}
	else{
		printf("\n nextr Not Busy\n");
	size_t nextSize = BLKSZ(nextBlock);
	currBlock->size_status = nextSize + blocksz;
	}
	currBlock->size_status &= ~VM_BUSY;
	
	if (!prevBusy){
	printf("\n prevBusy \n");
		struct block_footer *prev_footer = (struct block_footer *)((char *)currBlock - /*sizeof(struct block_footer) -*/ sizeof(struct block_footer));
	size_t total_size = BLKSZ(currBlock);
	printf("prev size: %zu, total size: %zu", prev_footer->size,total_size);
	struct block_header *prev_header = (struct block_header *)((char *)prev_footer - prev_footer->size + sizeof(struct block_header));
	int prev_prev_busy = prev_header->size_status & VM_PREVBUSY ? 1:0;
	prev_header ->size_status = total_size + BLKSZ(prev_header);
	prev_header->size_status &= ~VM_BUSY;
	if (prev_prev_busy){
	prev_header->size_status |= VM_PREVBUSY;
	}else{
	prev_header->size_status &= ~VM_PREVBUSY;
	}
	
	}
	else{
		printf("\n prev not busy \n");
	currBlock->size_status |= VM_PREVBUSY;
	
	}
return;	
}
