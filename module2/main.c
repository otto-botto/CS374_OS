#include <stdio.h>

int factorial(int variable) {
    int result = 1;

    for (int i = 1; i <= variable; i++) {
        result *= i;
    }
    return result;
}

int main(void) {
    int denominator;
    int boundary;
    int counter = 0;
    int max = 5;

    printf("Enter a positive integer whose multiples you want printed: \n");
    scanf("%d", &denominator);
    printf("Enter another positive integer up to which you want to see multiples printed: \n");
    scanf("%d", &boundary);
    printf("Here are the first %d multiples between  %d and %d.\n", max, denominator, boundary);

    for(int i = 0; i  <= boundary; i++) {
        if ( i % denominator == 0 && counter < max) {
            printf("%d ", i);
            counter++;
        }
        if (counter == max) {
            break;
        }
    }
    printf("\n");
    printf("Number for factorial: \n");
    int result;
    scanf("%d", &result);

    printf("factorial: %d\n", factorial(result));
    return 0;
}