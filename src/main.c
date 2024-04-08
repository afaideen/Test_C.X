#include <stdio.h>

// Define a function to greet a person
void greet(const char *message) {
    printf("%s\n", message);
}

// Define a structure for a person
struct Person {
    int age;
    char sex;
    char *firstname;
    char *lastname;
    void (*greet)(const char *message); // Function pointer
} han = {43, 'm', "Aizul Faideen", "Hamim", greet}; // Initialize function pointer

int main() {
    // Call the greet function using the function pointer
    han.greet("Hello welcome. Thank you");
    return -1;
}