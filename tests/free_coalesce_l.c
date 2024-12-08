#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "vmlib.h"
#include "vm.h"

int main()
{
    vminit(4096);

    void *ptr1 = vmalloc(64);
    void *ptr2 = vmalloc(64);
    vmalloc(64); // ptr3
vminfo();
    /* create a free block at ptr1*/
    vmfree(ptr1);
vminfo();
    /* this free should coalesce with the free block from ptr1*/
    vmfree(ptr2);
vminfo();
    /* this allocation should utilize the coalesced free block */
    void *ptr4 = vmalloc(144);
vminfo();
    assert(ptr1 == ptr4);

    vmdestroy();
    return 0;
}
