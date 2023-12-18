#ifndef SERVER_H
#define SERVER_H

#define MAX_DICT_SIZE 200

#include "../../include/common/common.h"

typedef struct
{
    char word[50];
    char definition[800];
} DictionaryEntry;

typedef struct
{
    DictionaryEntry entries[MAX_DICT_SIZE];
    int count;
    bool initialized;
} Dictionary;

void initializeDictionary(Dictionary *dict);
int findWordIndex(Dictionary *dict, const char *word);
void addWord(Dictionary *dict, const char *word);
void addDefinition(Dictionary *dict, const char *word, const char *definition);
void deleteWord(Dictionary *dict, const int index);
void handleClient(int clientSocket, Dictionary *dict);

#endif /* SERVER_H */
