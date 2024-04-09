#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define Person structure
typedef struct Person {
    char firstname[50];
    char lastname[50];
    int age;
    char sex[10];
    int married;
    struct Person* children[10]; // Assuming a maximum of 10 children
} Person;

// Equality comparison function for Person objects
int personEquals(const Person* lhs, const Person* rhs) {
    return strcmp(lhs->firstname, rhs->firstname) == 0 &&
           strcmp(lhs->lastname, rhs->lastname) == 0 &&
           lhs->age == rhs->age &&
           strcmp(lhs->sex, rhs->sex) == 0 &&
           lhs->married == rhs->married;
}

// Comparator function for sorting by age (descending)
int compareByAge(const void* a, const void* b) {
    const Person* person1 = *(const Person**)a;
    const Person* person2 = *(const Person**)b;
    return person2->age - person1->age;
}

// Function to remove a specified person from the list and update children lists
void removeItemAndUpdateChildren(Person** personList, Person* personToRemove, int* size) {
    for (int i = 0; i < *size; ++i) {
        if (personList[i] == personToRemove) {
            // Remove personToRemove from the personList
            free(personList[i]);
            for (int j = i; j < *size - 1; ++j) {
                personList[j] = personList[j + 1];
            }
            (*size)--;

            // Update children lists of other persons
            for (int j = 0; j < *size; ++j) {
                Person* person = personList[j];
                for (int k = 0; person->children[k] != NULL; ++k) {
                    if (person->children[k] == personToRemove) {
                        // Shift remaining children to remove personToRemove
                        for (int l = k; person->children[l] != NULL; ++l) {
                            person->children[l] = person->children[l + 1];
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
}

// Function to remove a specified person from the list without affecting children lists
int removePersonInList(Person** personList, Person* personToRemove) {
    int size = countElements(personList);
    printf("Removing %s...\n\n", personToRemove->firstname);
    for (int i = 0; i < size; ++i) {
        if (personList[i] == personToRemove) {
            // Remove personToRemove from the personList
//            free(personList[i]);
            for (int j = i; j < size - 1; ++j) {
                personList[j] = personList[j + 1];
            }
            personList[size - 1] = NULL; // Set the last element to NULL
            return size - 1; // Return the new size of the list
        }
    }
    return size; // Return the original size if personToRemove is not found
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

// Function to print list of persons
void printPersonList(const Person** personList) {
    int size = countElements(personList);
    for (int i = 0; i < size; ++i) {
        printPerson(personList[i]);
    }
}

int countElements(Person** personList) {
    int count = 0;
    while (personList[count] != NULL) {
        count++;
    }
    return count;
}

Person* createPerson(const char* firstname, const char* lastname, int age, const char* sex, int married) {
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
#define MAX_PERSONS 10
unsigned int addToList(Person** personList, Person* newPerson) {
    int size = countElements(personList);
    if (size >= MAX_PERSONS) {
        fprintf(stderr, "Person list is full\n");
        return;
    }
    personList[size] = newPerson; // Add the new person at the next available index
    printf("Added new...%s\n\n", newPerson->firstname);
    return size + 1;
}

int main() {
    // Create Person objects using the createPerson function
    Person* person1 = createPerson("Alice", "Smith", 30, "female", 1);
    Person* person2 = createPerson("Bob", "Johnson", 25, "male", 1);
    Person* person3 = createPerson("Charlie", "Brown", 22, "male", 0);

    // Create an array of pointers to Persons
    Person* personList[MAX_PERSONS];
    addToList(personList, person1);
    addToList(personList, person2);
    addToList(personList, person3);
    int size = countElements(personList);
    printf("Number of elements: %d\n", size);

    // Set children for Charlie Brown
    person3->children[0] = person1;

    person3->children[1] = person2;

    printf("Initial person list...\n\n");
    printPersonList((const Person**)personList);

    // Remove the person named Bob from the list and update children lists
//    removeItemAndUpdateChildren(personList, person2, &size);
    size = removePersonInList(personList, person2);    
    printPersonList((const Person**)personList);

    // Create and add Han
    Person* person4 = createPerson("Han", "Faideen", 43, "male", 0);
    size = addToList(personList, person4);
    printf("Added Han...\n\n");

    // Update ages
    person1->age = 3;
    person2->age = 5;

    // Sort the list by age (descending)
    qsort(personList, size, sizeof(Person*), compareByAge);

    // Print information for each person in the updated list
    printPersonList((const Person**)personList);

//    // Free memory for remaining persons
//    for (int i = 0; i < size; ++i) {
//        free(personList[i]);
//    }

    return 0;
}

/*OUTPUT
 Number of elements: 3
Initial person list...

First Name: Alice
Last Name: Smith
Age: 30
Sex: female
Marriage status: Married

First Name: Bob
Last Name: Johnson
Age: 25
Sex: male
Marriage status: Married

First Name: Charlie
Last Name: Brown
Age: 22
Sex: male
Marriage status: Single
Children:
   - Alice Smith, 30
   - Bob Johnson, 25

Removing Bob...

First Name: Alice
Last Name: Smith
Age: 30
Sex: female
Marriage status: Married

First Name: Charlie
Last Name: Brown
Age: 22
Sex: male
Marriage status: Single
Children:
   - Alice Smith, 30
   - Bob Johnson, 25

Added Han...

First Name: Han
Last Name: Isaac
Age: 43
Sex: male
Marriage status: Single

First Name: Charlie
Last Name: Brown
Age: 22
Sex: male
Marriage status: Single
Children:
   - Alice Smith, 3
   - Bob Johnson, 5

First Name: Alice
Last Name: Smith
Age: 3
Sex: female
Marriage status: Married


 */
