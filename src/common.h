#ifndef RISKV_COMMON
#define RISKV_COMMON

#define GROW_LIST(x) ( (x) == 0 ? 8 : (2 * (x)))

void importFile(const char* path, int32_t* size, char*** s, char*** values);
int32_t parseNumber(char* buffer, int32_t* index);
char* readFile(const char* path);
void toLowercaseC(char* c);
void toLowercase(char** s);
void copyWord(char** from, char** to, int32_t length);

#endif
