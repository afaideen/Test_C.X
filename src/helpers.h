#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h> // Include for size_t

typedef struct List List;
typedef struct System System;
typedef struct Person Person;

typedef void (*SetChildrenFunc)(Person* self, Person** children_list, size_t num_children);

struct Person {
    char firstname[50];
    char lastname[50];
    int age;
    char sex[10];
    int married;
    Person* children[100];
    SetChildrenFunc setChildren;
};

struct List {
    Person **persons;
    size_t size;
};


struct System {
    void (*addToList)(List* list, Person* person);
    void (*removeFromListByPerson)(List* list, Person* person);
};

//// Declare the global instance of System
extern System mysystem;

void setChildrenWrapper(Person* self, Person** children_list, size_t num_children);
int compareByAge(const void* a, const void* b);
void removeFromList(List *list, size_t index);
void removeFromListByPerson(List *list, Person *person);
void printPerson(const Person* person);
void printPersonList(const List *list);
Person* createPerson(const char* firstname, const char* lastname, int age, const char* sex, int married);
List* createList();
void addToList(List *list, Person *person);
// Function to initialize mysystem
void initializeSystem();


#endif /* HELPERS_H */
