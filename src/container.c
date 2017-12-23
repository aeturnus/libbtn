#include <stdlib.h>

#include <btn/container.h>

void container_free_shim(void * arg)
{
    void * ptr = *(void **)arg;
    free(ptr);
}
