#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <assert.h>

#define REGISTRY_NAME ints
#define REGISTRY_VALUE_TYPE int
#include "reg.h"

REGISTRY_DECLARE(ints);

void what(int i)
{
    printf("%d\n", i);
}

int main(int ac, char **av) 
{
    void** hdls = calloc(ac - 1, sizeof(void *));
    size_t hdls_size = 0;

    assert(hdls != NULL);
    for (unsigned int i = 1; i < ac; ++i)
    {
        void* hdl = dlopen(av[i], RTLD_NOW);
        if (hdl == NULL)
            continue;
        hdls[hdls_size++] = hdl;
    }

    registry_of_ints_foreach(what);

    for (unsigned int i = 0; i < hdls_size; ++i)
    {
        dlclose(hdls[i]);
    }
    return 0;
}
