#include <stdbool.h>

#define CHECK_ALLOC(p) if(p == NULL) { perror(__func__); exit(EXIT_FAILURE); }

typedef struct _l {
     char        *string;
     struct _l   *next;
} LIST;

extern	LIST	*new_list(void);

extern	LIST	*add_list(LIST *list, char *newstring);
extern	bool	find_list(LIST *list, char *wanted);

extern	void	print_list(LIST *list);
