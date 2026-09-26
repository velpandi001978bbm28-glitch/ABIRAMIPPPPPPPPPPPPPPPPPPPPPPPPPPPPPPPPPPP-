#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 1024

typedef struct {
    char* left;
    int left_top;
    int left_cap;
    
    char* right;
    int right_top;
    int right_cap;
    
    char* res_buf; // Helper buffer to safely return strings up to 10 chars
} TextEditor;

// Helper to push to a dynamic stack
void pushStack(char** stack, int* top, int* cap, char c) {
    if (*top >= *cap) {
        *cap *= 2;
        *stack = (char*)realloc(*stack, *cap * sizeof(char));
    }
    (*stack)[(*top)++] = c;
}

TextEditor* textEditorCreate() {
    TextEditor* obj = (TextEditor*)malloc(sizeof(TextEditor));
    
    obj->left_cap = INITIAL_CAPACITY;
    obj->left_top = 0;
    obj->left = (char*)malloc(obj->left_cap * sizeof(char));
    
    obj->right_cap = INITIAL_CAPACITY;
    obj->right_top = 0;
    obj->right = (char*)malloc(obj->right_cap * sizeof(char));
    
    obj->res_buf = (char*)malloc(15 * sizeof(char)); // Fits max 10 chars + null terminator
    
    return obj;
}

void textEditorAddText(TextEditor* obj, char* text) {
    while (*text) {
        pushStack(&obj->left, &obj->left_top, &obj->left_cap, *text);
        text++;
    }
}

int textEditorDeleteText(TextEditor* obj, int k) {
    int deleted = 0;
    while (k > 0 && obj->left_top > 0) {
        obj->left_top--;
        deleted++;
        k--;
    }
    return deleted;
}

// Helper to fetch up to 10 characters to the left of the cursor
char* getLeftString(TextEditor* obj) {
    int count = obj->left_top < 10 ? obj->left_top : 10;
    int start = obj->left_top - count;
    for (int i = 0; i < count; i++) {
        obj->res_buf[i] = obj->left[start + i];
    }
    obj->res_buf[count] = '\0';
    return obj->res_buf;
}

char* textEditorCursorLeft(TextEditor* obj, int k) {
    while (k > 0 && obj->left_top > 0) {
        char c = obj->left[--obj->left_top];
        pushStack(&obj->right, &obj->right_top, &obj->right_cap, c);
        k--;
    }
    return getLeftString(obj);
}

char* textEditorCursorRight(TextEditor* obj, int k) {
    while (k > 0 && obj->right_top > 0) {
        char c = obj->right[--obj->right_top];
        pushStack(&obj->left, &obj->left_top, &obj->left_cap, c);
        k--;
    }
    return getLeftString(obj);
}

void textEditorFree(TextEditor* obj) {
    if (obj) {
        free(obj->left);
        free(obj->right);
        free(obj->res_buf);
        free(obj);
    }
}
