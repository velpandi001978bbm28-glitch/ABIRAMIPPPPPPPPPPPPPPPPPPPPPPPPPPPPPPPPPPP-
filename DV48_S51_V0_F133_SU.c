#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 2053

// Hash table node structure
typedef struct HashNode {
    char* key;
    char* value;
    struct HashNode* next;
} HashNode;

// Simple DJB2 hash function
unsigned int getHash(const char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_SIZE;
}

// Insert key-value pairs into the hash table
void insert(HashNode** hashTable, char* key, char* value) {
    unsigned int index = getHash(key);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = key;
    newNode->value = value;
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

// Search for a key in the hash table
char* search(HashNode** hashTable, const char* key) {
    unsigned int index = getHash(key);
    HashNode* curr = hashTable[index];
    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }
    return "?"; // Return "?" if the key is not found
}

char* evaluate(char* s, char*** knowledge, int knowledgeSize, int* knowledgeColSize) {
    // 1. Initialize the Hash Table
    HashNode* hashTable[HASH_SIZE] = { NULL };
    for (int i = 0; i < knowledgeSize; i++) {
        insert(hashTable, knowledge[i][0], knowledge[i][1]);
    }

    // 2. Pre-allocate buffer for the dynamic result string
    // Maximum possible result size would occur if keys are very short and values are long,
    // but a safe starting allocation prevents frequent reallocations.
    int capacity = strlen(s) * 2 + 100; 
    char* result = (char*)malloc(capacity * sizeof(char));
    int resIdx = 0;

    char keyBuffer[11]; // Problem constraints specify key length <= 10

    // 3. Parse the input string
    for (int i = 0; s[i] != '\0'; i++) {
        // Resize buffer if it gets close to full capacity
        if (resIdx >= capacity - 15) {
            capacity *= 2;
            result = (char*)realloc(result, capacity * sizeof(char));
        }

        if (s[i] == '(') {
            int keyLen = 0;
            i++; // Skip '('
            
            // Extract the key inside the brackets
            while (s[i] != ')') {
                keyBuffer[keyLen++] = s[i];
                i++;
            }
            keyBuffer[keyLen] = '\0'; // Null-terminate the key

            // Look up the value and append it to the result
            char* val = search(hashTable, keyBuffer);
            int valLen = strlen(val);
            
            // Ensure there's enough space for the value
            if (resIdx + valLen >= capacity) {
                capacity += valLen + 100;
                result = (char*)realloc(result, capacity * sizeof(char));
            }
            
            strcpy(&result[resIdx], val);
            resIdx += valLen;
        } else {
            // Append plain text characters directly
            result[resIdx++] = s[i];
        }
    }
    
    result[resIdx] = '\0'; // Null-terminate the final string

    // 4. Free the hash table allocations
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* curr = hashTable[i];
        while (curr != NULL) {
            HashNode* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }

    return result;
}
