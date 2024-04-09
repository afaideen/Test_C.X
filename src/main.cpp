

#include <cstdio>
#include <string>
#include <list>
#include <algorithm>
#include <iostream>

class Person {
public:
    // Constructor to initialize attributes
    Person(const std::string& firstname, const std::string& lastname, int age, const std::string& sex, bool married)
        : firstname(firstname), lastname(lastname), age(age), sex(sex), married(married) {}

    // Attributes
    std::string firstname;
    std::string lastname;
    int age;
    std::string sex;
    bool married;
    std::list<Person*> children; // List of children
};

// Equality comparison operator for Person objects
bool operator==(const Person& lhs, const Person& rhs) {
    return lhs.firstname == rhs.firstname &&
           lhs.lastname == rhs.lastname &&
           lhs.age == rhs.age &&
           lhs.sex == rhs.sex &&
           lhs.married == rhs.married;
};

// Comparator function for sorting by age (descending)
struct CompareByAge {
    bool operator()(const Person* person1, const Person* person2) const {
        return person1->age > person2->age;
    }
};

// Predicate function for removing the person named Bob
struct RemoveBob {
    bool operator()(const Person* person) const {
        return person->firstname == "Bob";
    }
};

void removePerson(std::list<Person*>& personList, Person* personToRemove) {
//    // Remove the person from the children lists of other persons
//    for (auto& personPtr : personList) {
//        personPtr->children.remove(personToRemove);
//    }

    // Remove the person from the personList
    personList.remove(personToRemove);
//    delete personToRemove;    //will cause bug!
}


void printPersonList(const std::list<Person*>& personList) {
    for (const auto& personPtr : personList) {
        const Person& person = *personPtr;
        std::cout << "First Name: " << person.firstname << std::endl;
        std::cout << "Last Name: " << person.lastname << std::endl;
        std::cout << "Age: " << person.age << std::endl;
        std::cout << "Sex: " << person.sex << std::endl;
        std::cout << "Marriage status: " << (person.married ? "Married" : "Single") << std::endl;
        // Print children
        if (!personPtr->children.empty()) {
            std::cout << "Children: " << std::endl;
            for (const auto& child : personPtr->children) {
                std::cout << "   - " << child->firstname << " " << child->lastname << ", " << child->age << std::endl;
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    // Create three Person instances
    Person* person1 = new Person("Alice", "Smith", 30, "female", true);
    Person* person2 = new Person("Bob", "Johnson", 25, "male", true);
    Person* person3 = new Person("Charlie", "Brown", 22, "male", false);
    Person* person4 = new Person("Han", "Isaac", 43, "male", false);

    // Create a list of pointers to Persons
    std::list<Person*> personList;
    personList.push_back(person1);
    personList.push_back(person2);
    personList.push_back(person3);
    
    person3->children.push_back(person1);
    person3->children.push_back(person2);
    
    printf("Initial person list...\r\n\r\n");
    printPersonList(personList);

    // Remove the person named Bob from the list
//    personList.remove_if(RemoveBob());
//    personList.remove(person2);
    removePerson(personList, person2);
    printf("Removing Bob...\r\n\r\n");
    printPersonList(personList);
    
    personList.push_back(person4);
    printf("Added Han...\r\n\r\n");
    
    person1->age = 3;
    person2->age = 5;

    // Sort the list by age (descending)
    personList.sort(CompareByAge());

    // Print information for each person in the updated list
    printPersonList(personList);
    
    // Free memory for remaining persons
    for (const auto& personPtr : personList) {
        delete personPtr;
    }
    personList.clear(); // Clear the list

    return 0;
}

/*
 //output
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