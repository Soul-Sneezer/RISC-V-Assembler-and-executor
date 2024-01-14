#ifndef RISKV_COMMON
#define RISKV_COMMON

#define GROW_LIST(x) ( (x) == 0 ? 8 : (2 * (x)))

char** importFile(const char* path, int* size, char** s, char** v);
int parseNumber(char* buffer, int* index);
char* readFile(const char* path);
void toLowercaseC(char* c);
void toLowercase(char** s);

#endif
