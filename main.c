#define _CRT_SECURE_NO_WARNINGS
#include "Header_1.h"
#define MAX_NAME 50
//check table creation
//check &
int main() {
    
    FILE* manot =NULL, *output=NULL, *instructions=NULL;
    Pkitchen_list kitchen = NULL;
    char product_name[MAX_NAME];
    int i,table_number,quantity, number_of_tables, activate_function, value, num_to_add;
    Ptable* table_arr = NULL; //dymacillay allocated array of table pointers
    if (!(output = fopen("output.txt", "w"))) {
        puts("could not create output file\n");
        exit(-1);
    }
    if (!(manot = fopen("Manot.txt", "rt"))) {
        ErrorMsg("could not open manot file\n", output, NULL, NULL);
    }
    if (!(instructions = fopen("instructions.txt", "rt"))) {
        fclose(manot);
        ErrorMsg("could not open instructions.txt\n", output, manot, NULL);
    }
    fscanf(instructions, "%d", &number_of_tables);
    table_arr = (Ptable*)malloc(number_of_tables * sizeof(Ptable)); //table pointer array
    if (!table_arr) {
        fclose(manot);
        fclose(instructions);
        ErrorMsg("ERROR table array alloation failed\n", output, manot, instructions);
    }

    for (i = 0;i < number_of_tables;i++) {
        table_arr[i] = NULL;
        
    }
    fscanf(instructions, "%d", &activate_function);
   // activate_function = fgetc(instructions);
    while(activate_function != 1) {
        fputs("function 1 must be called first!\n",output);
        fscanf(instructions, "%d", &activate_function);
    }
    //call func 1
    kitchen = CreateProduct(manot, output, NULL);
    if (!kitchen) { 
        freeAll(kitchen, table_arr, number_of_tables);
        ErrorMsg("function 1 failed\n", output, manot, instructions);
    }
    else fputs("The kitchen was created\n", output);

    while (fscanf(instructions, "%d", &activate_function) != EOF) {
        
        switch (activate_function)
        {
        case 1: //make sure it's head
        case 2: 
            fscanf(instructions, "%s %d", product_name, &num_to_add);
          value =  AddItems(kitchen, product_name, num_to_add, output);
         // if (value) fprintf(output, "%d %s were added to the kitchen\n", num_to_add, product_name);
           if (value == -1) {
              freeAll(kitchen, table_arr, number_of_tables);
              ErrorMsg("function 2 failed\n", output, manot, instructions);
          }
          break;
        case 3:
            fscanf(instructions, "%d %s %d", &table_number, product_name, &quantity);
            value = OrderItem(table_number-1, product_name, quantity,kitchen, output, number_of_tables,table_arr, table_arr[table_number-1] );
            if (value == -1) {
                freeAll(kitchen, table_arr, number_of_tables);
                ErrorMsg("function 3 failed\n", output, manot, instructions);
            }
            else if (value) fprintf(output, "%d %s %s added to table number %d\n",
                quantity, product_name, quantity == 1 ? "was" : "were", table_number);
            break;
        case 4:
            fscanf(instructions, "%d %s %d", &table_number, product_name, &quantity);
            value = RemoveItem(product_name, table_number-1, kitchen, table_arr, quantity, output, number_of_tables);
            if(value == -1){
                freeAll(kitchen, table_arr, number_of_tables);
                ErrorMsg("function 4 failed\n", output, manot, instructions);
            }
            if (value) fprintf(output, "%d %s has returned to kitchen from table number %d\n", quantity, product_name, table_number);
            break;
        case 5:
            fscanf(instructions, "%d", &table_number);
            value = RemoveTable(table_number-1, table_arr, kitchen, output, number_of_tables);
            if (value == -1){
                freeAll(kitchen, table_arr, number_of_tables);
                ErrorMsg("function 5 failed\n", output, manot, instructions);
            }
            break;
        default:
            fputs("invalid function number\n", output);
            break;
        }
    }

    freeAll(kitchen, table_arr, number_of_tables); //incase func 5 wasn't called
    fclose(instructions);
    fclose(manot);
    fclose(output);
    

    return 0;
}
