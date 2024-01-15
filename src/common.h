#ifndef RISKV_COMMON
#define RISKV_COMMON

#define GROW_LIST(x) ( (x) == 0 ? 8 : (2 * (x)))

void importFile(const char* path, int* size, char*** s, char*** values);
int parseNumber(char* buffer, int* index);
char* readFile(const char* path);
void toLowercaseC(char* c);
void toLowercase(char** s);
void copyWord(char** from, char** to, int length);

#endif
