#include <stdio.h>
#include <stdlib.h>

// Define structure for doubly linked list node
typedef struct Node {
    char data;
    struct Node* prev;
    struct Node* next;
} Node;

// Define structure for the stack to store undo operations
typedef struct StackNode {
    char operation;
    char character;
    int position;
    struct StackNode* next;
} StackNode;

Node* head = NULL;
Node* tail = NULL;
StackNode* undoStack = NULL;

// Function to create a new doubly linked list node
Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Function to push an operation to the undo stack
void pushUndoStack(char operation, char character, int position) {
    StackNode* newStackNode = (StackNode*)malloc(sizeof(StackNode));
    newStackNode->operation = operation;
    newStackNode->character = character;
    newStackNode->position = position;
    newStackNode->next = undoStack;
    undoStack = newStackNode;
}

// Function to pop an operation from the undo stack
StackNode* popUndoStack() {
    if (undoStack == NULL) {
        return NULL;
    }
    StackNode* temp = undoStack;
    undoStack = undoStack->next;
    return temp;
}

// Function to insert a character at a given position
void insertChar(char ch, int position) {
    Node* newNode = createNode(ch);
    if (head == NULL) {
        head = tail = newNode;
    } else {
        Node* temp = head;
        int i = 1;
        while (temp != NULL && i < position) {
            temp = temp->next;
            i++;
        }
        if (temp == NULL) {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        } else {
            newNode->next = temp;
            newNode->prev = temp->prev;
            if (temp->prev != NULL) {
                temp->prev->next = newNode;
            } else {
                head = newNode;
            }
            temp->prev = newNode;
        }
    }
    // Push the operation to the undo stack
    pushUndoStack('i', ch, position);
}

// Function to delete a character at a given position
void deleteChar(int position) {
    if (head == NULL) {
        printf("Text is empty!\n");
        return;
    }
    Node* temp = head;
    int i = 1;
    while (temp != NULL && i < position) {
        temp = temp->next;
        i++;
    }
    if (temp == NULL) {
        printf("Invalid position!\n");
        return;
    }
    if (temp->prev != NULL) {
        temp->prev->next = temp->next;
    } else {
        head = temp->next;
    }
    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    } else {
        tail = temp->prev;
    }
    // Push the operation to the undo stack
    pushUndoStack('d', temp->data, position);
    free(temp);
}

// Function to display the current text
void displayText() {
    Node* temp = head;
    while (temp != NULL) {
        printf("%c", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

// Function to undo the last operation
void undo() {
    StackNode* lastOperation = popUndoStack();
    if (lastOperation == NULL) {
        printf("Nothing to undo!\n");
        return;
    }
    if (lastOperation->operation == 'i') {
        // Undo insertion
        deleteChar(lastOperation->position);
    } else if (lastOperation->operation == 'd') {
        // Undo deletion
        insertChar(lastOperation->character, lastOperation->position);
    }
    free(lastOperation);
}

// Main function
int main() {
    int choice, position;
    char ch;

    while (1) {
        printf("\nText Editor Menu:\n");
        printf("1. Insert Character\n");
        printf("2. Delete Character\n");
        printf("3. Display Text\n");
        printf("4. Undo\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter character to insert: ");
                scanf(" %c", &ch);
                printf("Enter position: ");
                scanf("%d", &position);
                insertChar(ch, position);
                break;
            case 2:
                printf("Enter position to delete: ");
                scanf("%d", &position);
                deleteChar(position);
                break;
            case 3:
                printf("Current text: ");
                displayText();
                break;
            case 4:
                undo();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
