#include "list.h"

typedef	LIST *	HASHTABLE;

extern	HASHTABLE	*new_hashtable(void);

extern	void		add_hashtable(HASHTABLE *,  char *string);
extern	bool		find_hashtable(HASHTABLE *, char *string);
