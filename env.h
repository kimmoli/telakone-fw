#ifndef TK_ENV_H
#define TK_ENV_H

#define ENV_PAGE_SIZE 1024

int envdump(char *buf, const char *delim, int *amount);
int envload(int *page);
int envstore(int *page);
int enverase(void);
int envLoader(void);

#endif
