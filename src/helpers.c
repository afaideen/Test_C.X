

#include "helpers.h"

// Function to set children for a person
void setChildrenWrapper(Person* self, Person** children_list, size_t num_children) {
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
void removeFromList(List *list, size_t index) {
    if (index >= list->size) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    printf("Removing %s from list...\n", list->persons[index]->firstname);
    
    // Get the person to be removed
    Person *personToRemove = list->persons[index];
    // Update the next and prev pointers of adjacent persons
    if (personToRemove->prev != NULL) {
        personToRemove->prev->next = personToRemove->next;
    }
    if (personToRemove->next != NULL) {
        personToRemove->next->prev = personToRemove->prev;
    }
    // Update the top and bottom pointers if needed
    if (personToRemove == list->top) {
        list->top = personToRemove->next;
    }
    if (personToRemove == list->bottom) {
        list->bottom = personToRemove->prev;
    }

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

void removeFromListByPerson(List *list, Person *person) {
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
void printPersonList(const List *list) {
    for (size_t i = 0; i < list->size; i++) {
        printPerson(list->persons[i]);
    }
}

Person* createPerson(const char* firstname, const char* lastname, int age, const char* sex, int married) 
{
    Person* person = (Person*)malloc(sizeof(Person));
    if (person == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(person->firstname, firstname);
    strcpy(person->lastname, lastname);
    person->age = age;
    strcpy(person->sex, sex);
    person->married = married;
    return person;
}

// Function to create a new list
List* createList() {
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
void addToList(List *list, Person *person) {
    list->persons = realloc(list->persons, (list->size + 1) * sizeof(struct Person *));
    if (list->persons == NULL) {
        fprintf(stderr, "Failed to reallocate memory for persons array\n");
        exit(EXIT_FAILURE);
    }
    list->persons[list->size] = person;
    list->size++;
    // Update the next and prev pointers for the newly added person and the last person in the list
    if (list->size > 1) {
        // Update next and prev pointers for the last person in the list
        list->persons[list->size - 2]->next = person;
        person->prev = list->persons[list->size - 2];
    }
    person->next = NULL; // The new person is always added at the end, so its next pointer should be NULL
    
    // Update top and bottom pointers
    if (list->size == 1) {
        // If the list was empty before, both top and bottom point to the new person
        list->top = person;
    }
    else
    {
        list->top = list->persons[0];
    }
    list->bottom = person; // Update the bottom pointer to point to the new person
}

Person* findListByPerson(List *list, Person *personToFind) 
{
    for (size_t i = 0; i < list->size; ++i) {
        Person *currentPerson = list->persons[i];
//        if (strcmp(currentPerson->firstname, personToFind->firstname) == 0 &&
//            strcmp(currentPerson->lastname, personToFind->lastname) == 0) 
        if(currentPerson == personToFind)
        {
            printf("findListByPerson found %s %s\r\n", currentPerson->firstname, currentPerson->lastname);
            return currentPerson; // Person found
        }
    }
    return NULL; // Person not found
}

void convertFullName2Lower(unsigned char *fullname, Person *currentPerson)
{
    // Concatenate firstname, space, and lastname into fullname
    snprintf(fullname, 50, "%s %s", currentPerson->firstname, currentPerson->lastname);
    
    // Convert fullname to lowercase
    for (size_t i = 0; i < strlen(fullname); ++i) {
        fullname[i] = tolower(fullname[i]);
    }
}

Person* findListByName(List *list, unsigned char *name) 
{
    unsigned char fullname[100];
    for (size_t i = 0; i < list->size; ++i) {
        Person *currentPerson = list->persons[i];
        convertFullName2Lower(fullname, currentPerson);
        
        // Compare the name of the current person with the given name (case insensitive)
        if (strcmp(fullname, name) == 0) {
            printf("findListByName found %s %s\r\n", currentPerson->firstname, currentPerson->lastname);
            return currentPerson; // Person found
        }
    }
    return NULL; // Person not found
}

int strcasecmp_(const char *s1, const char *s2) 
{
    while (*s1 && *s2) {
        int c1 = toupper(*s1);
        int c2 = toupper(*s2);
        if (c1 != c2) {
            return c1 - c2;
        }
        s1++;
        s2++;
    }
    // Return the difference in string lengths if one string is shorter than the other
    return *s1 - *s2;
}

void toLower(char *str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        str[i] = tolower(str[i]);
    }
}

