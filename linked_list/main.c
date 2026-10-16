#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct for a customer */
struct customer {
    char* name;
    char* state;
    struct customer* next;
};

/* Create a customer struct using dynamic memory allocation. */
struct customer *createCustomer(char* name, char* state) {
    struct customer *currCustomer = malloc(sizeof(struct customer));

    // allocate memory for each of the parameters
    currCustomer->name  = calloc(strlen(name)+1, sizeof(char));
    currCustomer->state = calloc(strlen(state)+1, sizeof(char));

    // copy the arguments into the parameters
    strcpy(currCustomer->name, name);
    strcpy(currCustomer->state, state);

    // set the next node to NULL in the newly created customer
    currCustomer->next = NULL;
    return currCustomer;

}

/* Print the linked list of customers. */
void printCustomerList(struct customer *list) {
    while (list != NULL) {
        printf("%s %s\n", list->name, list->state);
        list = list->next;
    }
}

/* Free a single customer. */
void freeCustomer(struct customer *customer) {
    free(customer->name);
    free(customer->state);
    free(customer);
}

/* Free a list of customers. */
void freeCustomerList(struct customer* list) {
    struct customer *customer;
    while (list != NULL) {
        customer = list;
        list = list->next;
        freeCustomer(customer);
    }
}

int main(void) {
    char name[16];
    char state[16];

    // The head of the linked list
    struct customer* head = NULL;
    // The tail of the linked list
    struct customer* tail = NULL;

    struct customer* newElem;

    printf("Create a linked list with info of 3 customers\n");
    for(int i = 0; i < 3; i++){
        printf("Enter the name of the customer (upto 15 chars): ");
        scanf("%s", name);
        printf("Enter the state they live in (upto 15 chars): ");
        scanf("%s", state);
        newElem = createCustomer(name, state);

        // Is this the first element in the linked list?
        if(head == NULL){
            // This is the first element in the linked link
            // Set the head and the tail to this element
            head = newElem;
            tail = newElem;
        } else{
            // This is not the first element.
            // Add this element to the list and advance the tail
            tail->next = newElem;
            tail = newElem;
        }
    }
    printCustomerList(head);
    // Free the memory dynamically allocated for the customers
    freeCustomerList(head);
    return 0;
}