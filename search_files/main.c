#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "collection.h"
#include "menu.h"
#include "process_file.h"
#include "search_menu.h"

int main(int argc, char **argv)
{
    //if (argc < 2){printf("You must provide a csv file.\n");}
    //collection_t *collection = loadCollection(argv[1]);
    //if (collection == NULL) {return 1;}

    // REPL
    while (1)
    {
        srand(time(NULL)); // set the random number seed
        int choice = 0;
        printf("1. Select file to process\n2. Exit the program\n\nEnter a choice from 1 or 2: \n");
        // TODO: handle a invalid choice < 1 or > 2
        scanf("%d",&choice);
        if (choice == 1) {
            int sub_choice = 0;
            printf("Which file do you want to process?\n");
            printf("Enter 1 to pick the largest file\nEnter 2 to pick the smallest file\n"
                   "Enter 3 to specify the name of a file\n\n");
            printf("Enter a choice from 1 to 3: \n");
            scanf("%d", &sub_choice);
            switch(sub_choice) {
                case 1:
                {
                    file_t largest = {0};
                    int processed_files = searchChoiceOne(".", &largest);
                    if (processed_files == 0) {
                        printf("There are no movies_*.csv files in this directory.\n");
                        continue;
                    }else {
                        printf("Now processing the chosen file named %s.\n", largest.name);
                        collection_t *collection = loadCollection(largest.name);
                        set_t *set_years = makeYearsSet(collection);
                        int file_processed = processFile(collection, set_years);
                        if (file_processed == -1) {
                            printf("Failed to process the file.\n");
                            perror(largest.name);
                            freeYearsSet(set_years);
                            freeCollection(collection);
                            return 1;
                        }

                        //printCollection(collection);
                        freeYearsSet(set_years);
                        freeCollection(collection);

                    }
                    break;
                }
                case 2:
                {
                    file_t smallest = {0};
                    int processed_files = searchChoiceTwo(".", &smallest);
                    if (processed_files == 0) {
                        printf("There are no movies_*.csv files in this directory.\n");
                        continue;
                    } else {
                        printf("Now processing the chosen file named %s.\n", smallest.name);
                        collection_t *collection = loadCollection(smallest.name);
                        set_t *set_years = makeYearsSet(collection);
                        int file_processed = processFile(collection, set_years);
                        if (file_processed == -1) {
                            printf("Failed to process the file.\n");
                            perror(smallest.name);
                            freeYearsSet(set_years);
                            freeCollection(collection);
                            return 1;
                        }
                        freeYearsSet(set_years);
                        freeCollection(collection);
                    }
                    break;
                }


                case 3: {
                    printf("Enter the complete file name: \n");
                    file_t entered = {0};
                    char file_entered[MAX_FILE_LEN + 1];
                    scanf("%s", file_entered);
                    size_t length = strlen(file_entered);
                    int processed_files = searchChoiceThree(file_entered, length, ".", &entered);
                    if (processed_files == -1) {
                        printf("The file %s was not found. Try again.\n", file_entered);
                        continue;
                    }else {
                        printf("Now processing the chosen file named %s.\n", entered.name);
                        collection_t *collection = loadCollection(entered.name);
                        set_t *set_years = makeYearsSet(collection);
                        int file_processed = processFile(collection, set_years);
                        if (file_processed == -1) {
                            printf("Failed to process the file.\n");
                            perror(entered.name);
                            freeYearsSet(set_years);
                            freeCollection(collection);
                            return 1;
                        }
                        freeYearsSet(set_years);
                        freeCollection(collection);
                    }
                    break;
                }

                default:
                    break;

            }

        }else if (choice == 2) {break;}
        if (choice < 1 || choice > 2)
        {
            printf("You entered an incorrect choice. Try again.\n\n");
        }
    }

    //printCollection(collection);
    //freeCollection(collection);
    return 0;
}
