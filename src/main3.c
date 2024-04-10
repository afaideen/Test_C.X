
#include "helpers.h"

void (*systemFunctions[])(List*, Person*) = 
{ 
    addToList, 
    removeFromListByPerson 
};

// Assign the function pointers outside of main
System mysystem; 

// Function to initialize mysystem
void initializeSystem() {
    mysystem.addToList = systemFunctions[0];
    mysystem.removeFromListByPerson = systemFunctions[1];
}

int main() {

    // Create a System object and initialize its addToList function pointer
//    System system;
//    system.addToList = addToList;
//    system.removeFromListByPerson = removeFromListByPerson;
    initializeSystem();

    // Create Person objects using the createPerson function
    Person* person1 = createPerson("Alice", "Smith", 30, "woman", 1);
    Person* person2 = createPerson("Bob", "Johnson", 25, "man", 1);
    Person* person3 = createPerson("Charlie", "Brown", 22, "man", 0);
    // Set the setChildren function pointer of person3 to the setChildren function
    person3->setChildren = setChildrenWrapper;  

    // Create the list
    List *personList = createList();

    mysystem.addToList(personList, person1);
    mysystem.addToList(personList, person2);
    mysystem.addToList(personList, person3);

    // Set children for Charlie Brown
    Person* children_list[] = {person1, person2};
    size_t num_children = sizeof(children_list) / sizeof(children_list[0]);
    person3->setChildren( person3, children_list, num_children);
    

    printf("Initial person list...\n\n");
    printPersonList(personList);

    // Remove the person named Bob from the list and update children lists
//    removeFromList(personList, 1);
    mysystem.removeFromListByPerson(personList, person2);
    printPersonList(personList);

    // Create and add Han
    Person* person4 = createPerson("Han", "Faideen", 43, "man", 0);
    mysystem.addToList(personList, person4);
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
