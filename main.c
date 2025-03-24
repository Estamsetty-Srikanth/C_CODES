#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure for Student Record
typedef struct StudentRecord {
    char Name[50];
    unsigned short int Age;
    float Weight;
    float Height;
    unsigned int UID;
    char Country[57];
    struct StudentRecord *next;
} StudentRecord;

// Function to check if input is a valid positive number
int check_num(const char *str) {  
    if (*str == '\0') return 1;  
    while (*str) {
        if (!isdigit(*str)) return 1;
        str++;
    }
    return 0;  
}

// Function to insert student record into the linked list
StudentRecord* insertRecord(StudentRecord *head) {
    StudentRecord *newNode = (StudentRecord*)malloc(sizeof(StudentRecord));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        return head;
    }

    // Keep asking for correct input
    while (1) {
        printf("Enter student details (Name Age Weight Height UID Country):\n");
        if (scanf("%49s %hu %f %f %u %56s", newNode->Name, &newNode->Age, &newNode->Weight, 
                  &newNode->Height, &newNode->UID, newNode->Country) == 6) {
            break; // Valid input, exit loop
        }
        printf("Invalid input format. Please enter again.\n");
        while (getchar() != '\n'); // Clear input buffer
    }

    // Insert at the beginning of the linked list
    newNode->next = head;
    return newNode;
}

// Function to copy a linked list
StudentRecord* copyList(StudentRecord *head) {
    if (!head) return NULL;

    StudentRecord *newHead = NULL, *temp = NULL;
    while (head) {
        StudentRecord *newNode = (StudentRecord*)malloc(sizeof(StudentRecord));
        if (!newNode) {
            printf("Memory allocation failed!\n");
            return newHead;
        }
        *newNode = *head;
        newNode->next = NULL;

        if (!newHead) newHead = temp = newNode;
        else {
            temp->next = newNode;
            temp = newNode;
        }
        head = head->next;
    }
    return newHead;
}

// Function to swap data between two nodes
void swapData(StudentRecord *a, StudentRecord *b) {
    StudentRecord temp = *a;
    
    strcpy(a->Name, b->Name);
    a->Age = b->Age;
    a->Weight = b->Weight;
    a->Height = b->Height;
    a->UID = b->UID;
    strcpy(a->Country, b->Country);

    strcpy(b->Name, temp.Name);
    b->Age = temp.Age;
    b->Weight = temp.Weight;
    b->Height = temp.Height;
    b->UID = temp.UID;
    strcpy(b->Country, temp.Country);
}

// Optimized Bubble Sort with flag
void sortList(StudentRecord *head, int (*compare)(StudentRecord *, StudentRecord *)) {
    if (!head) return;
    
    int swapped;
    StudentRecord *ptr1, *lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (compare(ptr1, ptr1->next) > 0) {
                swapData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Comparison functions for sorting
int compareByName(StudentRecord *a, StudentRecord *b) {
    return strcmp(a->Name, b->Name);
}
int compareByAge(StudentRecord *a, StudentRecord *b) {
    return a->Age - b->Age;
}
int compareByUID(StudentRecord *a, StudentRecord *b) {
    return a->UID - b->UID;
}
int compareByWeight(StudentRecord *a, StudentRecord *b) {
    return (a->Weight > b->Weight) - (a->Weight < b->Weight);
}
int compareByHeight(StudentRecord *a, StudentRecord *b) {
    return (a->Height > b->Height) - (a->Height < b->Height);
}

// Function to write sorted data to a file
void writeToFile(const char *filename, StudentRecord *head) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    StudentRecord *temp = head;
    while (temp) {
        fprintf(file, "%s %hu %.2f %.2f %u %s\n", temp->Name, temp->Age, temp->Weight, 
                temp->Height, temp->UID, temp->Country);
        temp = temp->next;
    }
    fclose(file);
}

// Function to free memory of linked list
void freeList(StudentRecord *head) {
    StudentRecord *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    char input[100];
    unsigned int num;

    // Get number of students
    while (1) {
        printf("Enter No of Students: ");
        scanf("%s", input);
        if (check_num(input)) {
            printf("Invalid input! Please enter a positive number.\n");
        } else {
            num = atoi(input);
            break;
        }
    }

    // Initialize linked list
    StudentRecord *head = NULL;

    // Insert student records
    for (unsigned int i = 0; i < num; i++) {
        head = insertRecord(head);
    }

    printf("\nStudent records inserted successfully.\n");

    // Save original records before sorting
    writeToFile("MasterRecord.txt", head);

    // Sort copies of the original linked list to keep master record unchanged
    StudentRecord *nameSorted = copyList(head);
    sortList(nameSorted, compareByName);
    writeToFile("NAME_MasterRecord.txt", nameSorted);

    StudentRecord *ageSorted = copyList(head);
    sortList(ageSorted, compareByAge);
    writeToFile("AGE_MasterRecord.txt", ageSorted);

    StudentRecord *uidSorted = copyList(head);
    sortList(uidSorted, compareByUID);
    writeToFile("UID_MasterRecord.txt", uidSorted);

    StudentRecord *weightSorted = copyList(head);
    sortList(weightSorted, compareByWeight);
    writeToFile("WEIGHT_MasterRecord.txt", weightSorted);

    StudentRecord *heightSorted = copyList(head);
    sortList(heightSorted, compareByHeight);
    writeToFile("HEIGHT_MasterRecord.txt", heightSorted);

    printf("\nRecords sorted and saved to files successfully.\n");

    // Free memory
    freeList(head);
    freeList(nameSorted);
    freeList(ageSorted);
    freeList(uidSorted);
    freeList(weightSorted);
    freeList(heightSorted);

    return 0;
}
