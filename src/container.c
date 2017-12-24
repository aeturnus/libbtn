#include <stdlib.h>

#include <btn/container.h>

void btn_free_shim(void * arg)
{
    void * ptr = *(void **)arg;
    free(ptr);
}
