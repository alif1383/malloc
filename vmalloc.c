#include "vm.h"
#include "vmlib.h"
#include <stdio.h>


void *vmalloc(size_t size)
{

if (size == 0){
return NULL;
}	
//printf("\n malloc \n");
    size_t rounded_size = ROUND_UP(size + 8, BLKALIGN);
    struct block_header *block = heapstart;
      int busy, prev_busy;
    size_t blocksz;
    size_t bestFitSize = 0;
  //  printf("%d",block->size_status !=VM_ENDMARK);
    while (block->size_status != VM_ENDMARK) {
	   // printf("goingBloicks");
        blocksz = BLKSZ(block);
        busy = block->size_status & VM_BUSY ? 1 : 0;
        prev_busy = block->size_status & VM_PREVBUSY ? 1 : 0;
	//printf("\n prevBusy, busy, blocksz");
        if (busy){
            block = (struct block_header *)((char *)block + blocksz);
             continue;
        }
        else if (blocksz >= rounded_size && (bestFitSize == 0 || blocksz < bestFitSize)){
            bestFitSize = blocksz;
	   }
            block = (struct block_header *)((char *)block + blocksz);
    //    printf("\n prevBysy:%d, blocksz: %zu, busy: %d, betFitSize: %zu  \n",prev_busy,blocksz,busy,bestFitSize);
    
   }
    if (bestFitSize < rounded_size){
    return NULL;
    }
    block = heapstart;
    while (block->size_status != VM_ENDMARK) {
        blocksz = BLKSZ(block);
        busy = block->size_status & VM_BUSY ? 1 : 0;
        prev_busy = block->size_status & VM_PREVBUSY ? 1 : 0;
//	printf("\n secondLoop %zu blocksz: %zu busy: %d \n",bestFitSize,blocksz, busy);

        if (busy){
            block = (struct block_header *)((char *)block + blocksz);
             continue;
        }
        else if (blocksz == bestFitSize && !busy){
		int prevBusy = block->size_status & VM_PREVBUSY ? 1: 0;

		if (blocksz > rounded_size){
		
		struct block_header *new_block = (struct block_header *)((char *)block+rounded_size);
		new_block->size_status |= VM_PREVBUSY;
		block->size_status = rounded_size;
		block->size_status |= VM_BUSY;
		size_t newSize = blocksz-rounded_size;

	        new_block->size_status = newSize;
		if (new_block->size_status != VM_ENDMARK){
		new_block -> size_status |= VM_PREVBUSY;
		vmFooter(new_block);
		}
		else{
		
		}
		new_block->size_status &= ~VM_BUSY;		
		//block->size_status = (block->size_status & VM_BLKSZMASK) | VM_BUSY | (block->size_status & VM_PREVBUSY);
		/*struct block_header *next_block = (struct block_header *)((char *) new_block + (rounded_size));
		if (next_block->size_status !=VM_ENDMARK){
		//new_block->size_status = new_block->size_status |  VM_PREVBUSY;
		}else{
		//new_block->size_status &= ~VM_PREVBUSY;
		}*/

		}
	else{
		
            block->size_status |= VM_BUSY;
            struct block_header *nextBlock = (struct block_header *)((char *)block+blocksz);
	    if (nextBlock->size_status != VM_ENDMARK){
            nextBlock->size_status |= VM_PREVBUSY;
	    }
	}
//	printf("\n inside   \n");

	if (prev_busy){
	block->size_status |= VM_PREVBUSY;
	}
	else{
	block->size_status &= ~VM_PREVBUSY;
	}

      return (void *)((char *)block + sizeof(struct block_header));
	break;

        }
  // printf("\n bestBlock size: %zu \n",bestFitSize); 
    block = (struct block_header *)((char *)block+blocksz);
    
    }
    // TODO

    return NULL;
}
