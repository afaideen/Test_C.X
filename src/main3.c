#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct Person Person;
// Function pointer type for setChildren method

// Define Person structure
struct Person {
    char firstname[50];
    char lastname[50];
    int age;
    char sex[10];
    int married;
    Person* children[100]; // Assuming a maximum of 10 children
    void (*setChildren)(Person* self, Person** children_list, size_t num_children);
//    void (*setChildren)(Person** children_list, size_t num_children);
};

// Define a structure for the list
struct List {
    Person **persons;
    size_t size;
};

// Function to set children for a person
static void setChildrenWrapper(Person* self, Person** children_list, size_t num_children) {
    // Ensure num_children does not exceed the capacity of the children array
    size_t num_to_copy = num_children < 100 ? num_children : 100;

    // Copy children pointers to the person's children array
    for (size_t i = 0; i < num_to_copy; ++i) {
        self->children[i] = children_list[i];
    }

    // Set the remaining elements in the children array to NULL
    for (size_t i = num_to_copy; i < 100; ++i) {
        self->children[i] = NULL;
    }
}


// Comparator function for sorting by age (descending)
int compareByAge(const void* a, const void* b) {
    const Person* person1 = *(const Person**)a;
    const Person* person2 = *(const Person**)b;
    return person2->age - person1->age;
}

// Function to remove a person from the list at the specified index
void removeFromList(struct List *list, size_t index) {
    if (index >= list->size) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    printf("Removing %s from list...\n", list->persons[index]->firstname);

//    free(list->persons[index]);
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

void removeFromListByPerson(struct List *list, Person *person) {
    for (size_t i = 0; i < list->size; ++i) {
        if (list->persons[i] == person) {
            removeFromList(list, i);
            return; // Exit the function once the person is removed
        }
    }
    printf("Person not found in the list.\n");
}


// Function to print person information
void printPerson(const Person* person) {
    printf("First Name: %s\n", person->firstname);
    printf("Last Name: %s\n", person->lastname);
    printf("Age: %d\n", person->age);
    printf("Sex: %s\n", person->sex);
    printf("Marriage status: %s\n", (person->married ? "Married" : "Single"));
    // Print children if there are any
    if (person->children[0] != NULL) {
        printf("Children:\n");
        for (int i = 0; person->children[i] != NULL; ++i) {
            printf("   - %s %s, %d\n", person->children[i]->firstname, person->children[i]->lastname, person->children[i]->age);
        }
    }

    printf("\n");
}


// Function to print the list
void printPersonList(const struct List *list) {
    for (size_t i = 0; i < list->size; i++) {
        printPerson(list->persons[i]);
    }
}

Person* createPerson(const char* firstname, const char* lastname, int age, const char* sex, int married) 
{
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (newPerson == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newPerson->firstname, firstname);
    strcpy(newPerson->lastname, lastname);
    newPerson->age = age;
    strcpy(newPerson->sex, sex);
    newPerson->married = married;
    return newPerson;
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
unsigned int addToList(struct List *list, Person *person) {
    list->persons = realloc(list->persons, (list->size + 1) * sizeof(struct Person *));
    if (list->persons == NULL) {
        fprintf(stderr, "Failed to reallocate memory for persons array\n");
        exit(EXIT_FAILURE);
    }
    list->persons[list->size] = person;
    list->size++;
    return list->size;
}

int main() {

    // Create Person objects using the createPerson function
    Person* person1 = createPerson("Alice", "Smith", 30, "woman", 1);
    Person* person2 = createPerson("Bob", "Johnson", 25, "man", 1);
    Person* person3 = createPerson("Charlie", "Brown", 22, "man", 0);
    // Set the setChildren function pointer of person3 to the setChildren function
    person3->setChildren = setChildrenWrapper;  

    // Create the list
    struct List *personList = createList();
    addToList(personList, person1);
    addToList(personList, person2);
    addToList(personList, person3);

    // Set children for Charlie Brown
    Person* children_list[] = {person1, person2};
    size_t num_children = sizeof(children_list) / sizeof(children_list[0]);
    person3->setChildren( person3, children_list, num_children);
//    person3->children[0] = person1;
//    person3->children[1] = person2;
    

    printf("Initial person list...\n\n");
    printPersonList(personList);

    // Remove the person named Bob from the list and update children lists
//    removeFromList(personList, 1);
    removeFromListByPerson(personList, person2);
    printPersonList(personList);

    // Create and add Han
    Person* person4 = createPerson("Han", "Faideen", 43, "man", 0);
    addToList(personList, person4);
    printf("Added Han...\n\n");

    // Update ages
    person1->age = 3;
    person2->age = 5;

    // Sort the list by age (descending)
    qsort(personList->persons, personList->size, sizeof(struct Person *), compareByAge);

    // Print information for each person in the updated list
    printPersonList(personList);

    // Free memory for remaining persons
    for (int i = 0; i < personList->size; ++i) {
        free(personList->persons[i]);
    }
    free(personList->persons);
    free(personList);

    return 0;
}

/*OUTPUT
Initial person list...

First Name: Alice
Last Name: Smith
Age: 30
Sex: woman
Marriage status: Married

First Name: Bob
Last Name: Johnson
Age: 25
Sex: man
Marriage status: Married

First Name: Charlie
Last Name: Brown
Age: 22
Sex: man
Marriage status: Single
Children:
   - Alice Smith, 30
   - Bob Johnson, 25

Removing Bob from list...
First Name: Alice
Last Name: Smith
Age: 30
Sex: woman
Marriage status: Married

First Name: Charlie
Last Name: Brown
Age: 22
Sex: man
Marriage status: Single
Children:
   - Alice Smith, 30
   - Bob Johnson, 25

Added Han...

First Name: Han
Last Name: Faideen
Age: 43
Sex: man
Marriage status: Single

First Name: Charlie
Last Name: Brown
Age: 22
Sex: man
Marriage status: Single
Children:
   - Alice Smith, 3
   - Bob Johnson, 5

First Name: Alice
Last Name: Smith
Age: 3
Sex: woman
Marriage status: Married



 */
