#include <stdio.h>
#include <sys/queue.h>
#include <string.h>
#include <stdlib.h>

#ifndef REGISTRY_NAME
#error "You must define a REGISTRY NAME"
#endif

#define _CAT(X, Y) X ## Y
#define CAT(X, Y) _CAT(X, Y)

#define _CAT3(X, Y, Z) X ##  Y ## Z
#define CAT3(X, Y, Z) _CAT3(X, Y, Z)

#define _CAT4(W, X, Y, Z) W ## X ##  Y ## Z
#define CAT4(W, X, Y, Z) _CAT4(W, X, Y, Z)

#ifndef registry_sname

#define registry_sname(N) CAT3(registry_, N, _s)
#define registry_type(N) CAT3(registry_, N, _t)

#define entry_sname(N) CAT3(registry_entry_, N, _s)
#define entry_type(N) CAT3(registry_entry_, N, _t)

#define fun_name(N, M) CAT4(registry_of_, N, _, M)
#define var_name(N) CAT(registry_of_, N)
#define REGISTRY_DECLARE(N) registry_type(N) var_name(N)
#endif

typedef struct entry_sname(REGISTRY_NAME)
{
    char loc[512];
    REGISTRY_VALUE_TYPE value;
    TAILQ_ENTRY(entry_sname(REGISTRY_NAME)) entries;
} entry_type(REGISTRY_NAME);

typedef struct registry_sname(REGISTRY_NAME)
{
    TAILQ_HEAD(, entry_sname(REGISTRY_NAME)) head;
} registry_type(REGISTRY_NAME);

extern registry_type(REGISTRY_NAME) var_name(REGISTRY_NAME);

static inline void fun_name(REGISTRY_NAME, foreach)(void (*f)(REGISTRY_VALUE_TYPE))
{
    for (struct entry_sname(REGISTRY_NAME) *np = var_name(REGISTRY_NAME).head.tqh_first; 
        np != NULL;
        np = np->entries.tqe_next)
    {
        f(np->value);
    }
}

#ifndef REGISTER

#define CTOR __attribute__((constructor))
#define DTOR __attribute__((destructor))

#define REGISTER(R, M)                                                         \
    static CTOR void CAT(__register_, __COUNTER__)()                           \
    {                                                                          \
        entry_type(R)* entry = (entry_type(R)*)calloc(1, sizeof(*entry));      \
        entry->value = (M);                                                    \
        TAILQ_INSERT_HEAD(&var_name(R).head, entry, entries);                  \
    }                                                                          \
    static DTOR void CAT(__unregister_, __COUNTER__)()                         \
    {                                                                          \
        char loc[512];                                                         \
        snprintf(loc, sizeof(loc), "%s:%d", __FILE__, __LINE__);               \
        for (entry_type(R)* np = var_name(R).head.tqh_first; np != NULL;       \
             np = np->entries.tqe_next)                                        \
        {                                                                      \
            if (strcmp(np->loc, loc) == 0)                                     \
            {                                                                  \
                TAILQ_REMOVE(&var_name(R).head, np, entries);                  \
            }                                                                  \
        }                                                                      \
    }

#endif
