#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct StudentRecordsSheet {
    char Name[50];
    unsigned short int Age;
    float Weight;
    float Height;
    unsigned int UID;
    char Country[57];
    struct StudentRecordsSheet *next;
} Student_Record;

// Function to convert name to uppercase
void toUpperCase(char *str) {
    while (*str) {
        *str = toupper(*str);
        str++;
    }
}

// Function to insert a new student into the linked list
Student_Record* insertRecord(Student_Record *head, char *name, unsigned short age, float weight, float height, unsigned int uid, char *country) {
    Student_Record *newNode = (Student_Record *)malloc(sizeof(Student_Record));
    if (!newNode) {
        printf("Memory allocation failed\n");
        return head;
    }
    strcpy(newNode->Name, name);
    toUpperCase(newNode->Name);
    newNode->Age = age;
    newNode->Weight = weight;
    newNode->Height = height;
    newNode->UID = uid;
    strcpy(newNode->Country, country);
    newNode->next = head;  // Insert at head
    return newNode;
}

// Function to write records to a file
void writeToFile(const char *filename, Student_Record *head) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return;
    }
    Student_Record *temp = head;
    while (temp) {
        fprintf(file, "%s,%hu,%.2f,%.2f,%u,%s\n", temp->Name, temp->Age, 
                temp->Weight, temp->Height, temp->UID, temp->Country);
        temp = temp->next;
    }
    fclose(file);
}

// Sorting functions (Bubble Sort for linked list)
void sortByField(Student_Record **head, int (*cmp)(Student_Record*, Student_Record*)) {
    if (!*head) return;
    int swapped;
    Student_Record *ptr1, *lptr = NULL;
    do {
        swapped = 0;
        ptr1 = *head;
        while (ptr1->next != lptr) {
            if (cmp(ptr1, ptr1->next) > 0) {
                // Swap data
                Student_Record temp = *ptr1;
                *ptr1 = *(ptr1->next);
                *(ptr1->next) = temp;
                
                // Fix next pointers
                temp.next = ptr1->next->next;
                ptr1->next->next = ptr1;
                ptr1->next = temp.next;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Comparison functions
int compareByName(Student_Record *a, Student_Record *b) { return strcmp(a->Name, b->Name); }
int compareByAge(Student_Record *a, Student_Record *b) { return a->Age - b->Age; }
int compareByUID(Student_Record *a, Student_Record *b) { return a->UID - b->UID; }
int compareByWeight(Student_Record *a, Student_Record *b) { return (a->Weight > b->Weight) - (a->Weight < b->Weight); }
int compareByHeight(Student_Record *a, Student_Record *b) { return (a->Height > b->Height) - (a->Height < b->Height); }

// Function to duplicate a linked list
Student_Record* duplicateList(Student_Record *head) {
    Student_Record *newHead = NULL, *tail = NULL;
    while (head) {
        Student_Record *newNode = (Student_Record *)malloc(sizeof(Student_Record));
        if (!newNode) return newHead;
        *newNode = *head;
        newNode->next = NULL;
        if (!newHead) newHead = tail = newNode;
        else { tail->next = newNode; tail = newNode; }
        head = head->next;
    }
    return newHead;
}

// Free linked list memory
void freeList(Student_Record *head) {
    while (head) {
        Student_Record *temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    unsigned int num;
    printf("Enter number of students: ");
    if (scanf("%u", &num) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    while (getchar() != '\n'); // Clear input buffer

    Student_Record *head = NULL;
    FILE *file = fopen("MasterRecord.txt", "w");
    if (!file) {
        printf("Error opening file\n");
        return 1;
    }

    printf("\nEnter student details (Name Age Weight Height UID Country):\n");
    for (unsigned int i = 0; i < num; i++) {
        char name[50], country[57];
        unsigned short age;
        float weight, height;
        unsigned int uid;
        
        if (scanf("%49s %hu %f %f %u %56s", name, &age, &weight, &height, &uid, country) != 6) {
            printf("Invalid input format.\n");
            freeList(head);
            return 1;
        }

        head = insertRecord(head, name, age, weight, height, uid, country);
        fprintf(file, "%s,%hu,%.2f,%.2f,%u,%s\n", head->Name, head->Age, head->Weight, head->Height, head->UID, head->Country);
    }
    fclose(file);

    // Sorting and Writing to individual files
    Student_Record *tempList;

    tempList = duplicateList(head);
    sortByField(&tempList, compareByName);
    writeToFile("NAME_MasterRecord.txt", tempList);
    freeList(tempList);

    tempList = duplicateList(head);
    sortByField(&tempList, compareByAge);
    writeToFile("AGE_MasterRecord.txt", tempList);
    freeList(tempList);

    tempList = duplicateList(head);
    sortByField(&tempList, compareByUID);
    writeToFile("UID_MasterRecord.txt", tempList);
    freeList(tempList);

    tempList = duplicateList(head);
    sortByField(&tempList, compareByWeight);
    writeToFile("WEIGHT_MasterRecord.txt", tempList);
    freeList(tempList);

    tempList = duplicateList(head);
    sortByField(&tempList, compareByHeight);
    writeToFile("HEIGHT_MasterRecord.txt", tempList);
    freeList(tempList);

    freeList(head);
    printf("Records saved successfully.\n");

    return 0;
}
