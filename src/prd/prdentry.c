/* 
 * prd/prdentry.c
 */

#include <stdlib.h>

#include "prdentry.h"

PRDEntry *
PRDEntry_New()
{
	PRDEntry *entry = malloc(sizeof(PRDEntry)); /* XXX check for error */
	entry->name = NULL;
	return entry;
}


void
PRDEntry_Free(PRDEntry *entry)
{
	if (entry->name) {
		free(entry->name);
	}
	free(entry);
}
