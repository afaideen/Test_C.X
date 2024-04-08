#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for a person
struct Person {
    int age;
    char sex;
    char *firstname;
    char *lastname;
};

// Define a structure for the list
struct List {
    struct Person **persons;
    size_t size;
};

// Function to create a new person
struct Person* createPerson(int age, char sex, const char *firstname, const char *lastname) {
    struct Person *person = malloc(sizeof(struct Person));
    if (person == NULL) {
        fprintf(stderr, "Failed to allocate memory for person\n");
        exit(EXIT_FAILURE);
    }
    person->age = age;
    person->sex = sex;
    person->firstname = strdup(firstname);
    person->lastname = strdup(lastname);
    if (person->firstname == NULL || person->lastname == NULL) {
        fprintf(stderr, "Failed to allocate memory for firstname or lastname\n");
        exit(EXIT_FAILURE);
    }
    return person;
}

// Function to create a new list
struct List* createList() {
    struct List *list = malloc(sizeof(struct List));
    if (list == NULL) {
        fprintf(stderr, "Failed to allocate memory for list\n");
        exit(EXIT_FAILURE);
    }
    list->persons = NULL;
    list->size = 0;
    return list;
}

// Function to add a person to the end of the list
void addToList(struct List *list, struct Person *person) {
    list->persons = realloc(list->persons, (list->size + 1) * sizeof(struct Person *));
    if (list->persons == NULL) {
        fprintf(stderr, "Failed to reallocate memory for persons array\n");
        exit(EXIT_FAILURE);
    }
    list->persons[list->size] = person;
    list->size++;
}

// Function to print the list
void printList(struct List *list) {
    for (size_t i = 0; i < list->size; i++) {
        printf("Person %zu: Age: %d, Sex: %c, Firstname: %s, Lastname: %s\n", i + 1,
               list->persons[i]->age, list->persons[i]->sex,
               list->persons[i]->firstname, list->persons[i]->lastname);
    }
}

// Function to free memory allocated for the list
void freeList(struct List *list) {
    for (size_t i = 0; i < list->size; i++) {
        free(list->persons[i]->firstname);
        free(list->persons[i]->lastname);
        free(list->persons[i]);
    }
    free(list->persons);
    free(list);
}

// Function to remove a person from the list at the specified index
void removeFromList(struct List *list, size_t index) {
    if (index >= list->size) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    free(list->persons[index]->firstname);
    free(list->persons[index]->lastname);
    free(list->persons[index]);
    for (size_t i = index; i < list->size - 1; i++) {
        list->persons[i] = list->persons[i + 1];
    }
    list->size--;
    list->persons = realloc(list->persons, list->size * sizeof(struct Person *));
    if (list->size > 0 && list->persons == NULL) {
        fprintf(stderr, "Failed to reallocate memory for persons array\n");
        exit(EXIT_FAILURE);
    }
}

int main() {
    // Create the list
    struct List *mylist = createList();

    // Create and add person objects to the list
    struct Person *person1 = createPerson(25, 'm', "John", "Doe");
    struct Person *person2 = createPerson(30, 'f', "Jane", "Smith");
    struct Person *person3 = createPerson(43, 'm', "Capt", "Han");
    addToList(mylist, person1);
    addToList(mylist, person2);
    addToList(mylist, person3);

    // Print the list
    printf("After adding:\n");
    printList(mylist);
    
    // Remove person1 from the list
    removeFromList(mylist, 0);

    printf("\nAfter removing:\n");
    printList(mylist);


    // Free memory allocated for the list
    freeList(mylist);

    return 0;
}
