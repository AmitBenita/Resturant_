#define _CRT_SECURE_NO_WARNINGS
#include "Header_1.h"
#include <math.h>
#include <malloc.h>
#define MAX_NAME 50


Pkitchen_list CreateProduct(FILE* manot, FILE* output, Pkitchen_list existing_kitchen) {
	Pkitchen_list kitchen = existing_kitchen, new_dish=NULL;
	char temp_name[MAX_NAME];
	float temp_price;
	int temp_quantity;

	if (manot == NULL) {
		fputs("manot.txt is NULL", output);	
		return NULL;
	}
	if (output == NULL) {
		puts("output is null\n");
		return NULL;
	}
	
	while (kitchen != NULL && kitchen->next != NULL) kitchen = kitchen->next;

	while (fscanf(manot,"%s%d%f", temp_name, &temp_quantity, &temp_price) == 3) {
		if (existing_kitchen && check_name(existing_kitchen, temp_name) == 0) {  //if name already exists
			fputs("product already exists\n", output);
			continue;
		}
		if (temp_price <= 0 || temp_quantity <= 0) {
			fputs("invalid input\n", output);
			continue;
		}		
		new_dish = (Pkitchen_list)malloc(sizeof(kitchen_list));  //new node

		if (!new_dish) {
			fputs("list node allocation failed\n",output);
			return NULL;
		}
		new_dish->dishes = (Pproduct)malloc(sizeof(product));       //new product in node
		if (!new_dish->dishes) {
			fputs("product allocation failed\n",output);
			return NULL;
		}
		new_dish->dishes->productName = (char*)malloc(strlen(temp_name) + 1);    //product name
		if (!new_dish->dishes->productName) {
			fputs("name allocation failed\n", output);
			return NULL;
		}

		strcpy(new_dish->dishes->productName, temp_name); //put data
		new_dish->dishes->price = temp_price;
		new_dish->dishes->quantity = temp_quantity;
		new_dish->dishes->sales = 0;
		new_dish->next = NULL;

		if (!kitchen) {
			kitchen = new_dish;
			existing_kitchen = kitchen;
		}
		else {
			kitchen->next = new_dish;
			kitchen = kitchen->next;
		}	
	}
	return existing_kitchen;
}



int check_name(Pkitchen_list list_head, char* temp_name) { //returns 1 if name doesn't exist, 0 otherwise
	Pkitchen_list current = list_head;
	if (!temp_name) return -1;
	
	while (current != NULL) {
		if (strcmp(current->dishes->productName, temp_name) == 0)
			return 0;
		current = current->next;
	}
	return 1;
}




int AddItems(Pkitchen_list product_list, char* product_name, int Quantity, FILE* output) {
	Pkitchen_list temp =product_list ;
	if (!product_name || !product_list) {
		fputs("list pointer is null\n", output);
		return (-1);

	}
	if (Quantity <= 0) {
		fputs("invalid Quantity",output);
		return 0;
	}
	while (temp!=NULL) {
		if (strcmp(temp->dishes->productName, product_name) == 0) {
			temp->dishes->quantity +=Quantity;
			fprintf(output, "%d %s %s added to the kitchen\n", Quantity, product_name, Quantity > 1 ? "were" : "was");
			return 1;
		}
		else
		temp = temp->next;

	}
	
	fprintf(output,"%s does not exist in the kitchen\n",product_name);
	return 0;
}


//returns 0 if product not avelible, 1 if order succeded
int OrderItem(int table_number, char* product_name, int Quantity, Pkitchen_list kitchen, FILE* output, int max_tables, Ptable* tables, Ptable current_table) {

	Pkitchen_list current_product = kitchen;
	Porder current_order =NULL ;    //current order in current table
	Porder new_order = NULL;
	if (!output) {
		puts("output file pointer is null\n");
		return -1;
	}
	if (kitchen == NULL || product_name == NULL ) {
		fputs("Error! a pointer is null\n", output);
		return -1;
	}
	if (table_number >= max_tables || table_number < 0) {
		fputs("invalid table number\n", output);
		return 0;
	}
	if (Quantity <= 0) {
		fputs("invalid quantity\n",output);
	}
	while (current_product != NULL) {       //look for order in kitchen
		if (strcmp(product_name, current_product->dishes->productName) != 0)
			current_product = current_product->next;
		else break;

	}
	if (current_product == NULL) { //no such product
		fprintf(output,"We don't have %s, sorry!\n",product_name);
		return 0;
	}

	if (Quantity > current_product->dishes->quantity) {   

	//not in stock or order is more than stock

		fprintf(output,"we don't have %s, sorry!\n", product_name);
		return 0;
	}

	if (tables[table_number] == NULL) {    //open table
		current_table = (Ptable)malloc(sizeof(table));
		if (!current_table) {
			fputs("table allocation failed\n", output);
			return -1;
		}
		current_table->meal_list = NULL;
		current_table->Total_Price = 0;
		tables[table_number] = current_table;
	}
	else {
		current_table = tables[table_number];
	}
	
	//start order

	current_order = current_table->meal_list; //current node in dish list
	while (current_order != NULL && current_order->next != NULL) {
		current_order = current_order->next;
	}

	
	new_order = (Porder)malloc(sizeof(order)); //new node in linked list of table
	if (!new_order) {
		fputs("order allocation failed\n", output);
		return -1;
	}


	new_order->prev = NULL;
	new_order->next = NULL;
	new_order->dish = NULL;
	new_order->dish = (Pproduct)malloc(sizeof(product)); //new dish ordered
	if (!(new_order->dish)) {
		fputs("error in dish order allocation\n", output);
		return -1;
	}
	new_order->dish->productName = (char*)malloc(strlen(current_product->dishes->productName) + 1);
	if (!new_order->dish->productName) {
		fputs("meal list allocation failed\n", output);
		return -1;
	}
	strcpy(new_order->dish->productName , product_name); //put meal data
	new_order->dish->quantity = Quantity;
	new_order->dish->sales = Quantity;
	new_order->dish->price = Quantity * (current_product->dishes->price);

	current_table->Total_Price += new_order->dish->price; //update price

	current_product->dishes->quantity -= Quantity; //update quantity in kitchen
	current_product->dishes->sales += Quantity;

	if (!current_order) current_table->meal_list = new_order;     //link tolist
	else {
		new_order->prev = current_order;
		current_order->next = new_order;
	}
	return 1;
}





int RemoveTable(int table_number, Ptable* tables, Pkitchen_list kitchen, FILE* output, int max_tables) {

	int i = 0, counter = 0;
	Porder current_order;
	Pkitchen_list current_product = kitchen;
	Pproduct max_dish = NULL;

	if (!output) {
		puts("null file pointer\n");
		return -1;
	}
	if (!tables || !kitchen) {
		fputs("a pointer is null\n", output);
		return -1;
	}
	if (table_number < 0 || table_number >= max_tables) {
		fputs("Error! invalid table number\n", output);
		return 0;
	}


	if (tables[table_number] == NULL ) {
		fprintf(output, "table %d has not ordered yet\n", table_number+1);
		return 0;
	}
	if (tables[table_number]->meal_list == NULL) {
		fprintf(output, "table %d has not ordered yet\n", table_number+1);
		return 0;
	}

	current_order = tables[table_number]->meal_list;
	max_dish = kitchen->dishes;
	/*
	while (current_order->prev != NULL) { //go to head
		current_order = current_order->prev;
	}
	*/
	while (current_order != NULL) {			//check out
		if (current_order->dish->quantity > 0) {
			fprintf(output, "%d %s. ", current_order->dish->quantity, current_order->dish->productName);
		}
		fprintf(output, "%.2f nis please!\n", tables[table_number]->Total_Price);
		current_order = current_order->next;
	}

	// 
	
	
	while (tables[table_number]->meal_list) {
		current_order = tables[table_number]->meal_list;

		free(current_order->dish->productName);
		free(current_order->dish);
		tables[table_number]->meal_list = tables[table_number]->meal_list->next;
		free(current_order);
	}
	free(tables[table_number]);
	tables[table_number] = NULL;
		

		for (i = 0;i < max_tables;i++) {
			if (tables[table_number] == NULL)
				counter++;
			if (counter == max_tables) {   //all tables are closed
				while (current_product != NULL) {    //look for most popular dish
					if (current_product->dishes->sales > max_dish->sales) {
						max_dish = current_product->dishes;
					}
					current_product = current_product->next;
				}
				fprintf(output, "The most popular dish today was %s!(was ordered %d times)\n", max_dish->productName, max_dish->sales);
			}
		}
	
	
	return 1;
}




void freeAll(Pkitchen_list kitchen, Ptable* tables, int num_of_tables) {
	int i;
	Pkitchen_list temp = NULL;
	Porder temp_order = NULL;
	if (tables != NULL) {
		for (i = 0;i < num_of_tables;i++) {
			while (tables[i]!=NULL &&
				tables[i]->meal_list != NULL) {
			
				free(tables[i]->meal_list->dish->productName);
				free(tables[i]->meal_list->dish);
				temp = tables[i]->meal_list;
				tables[i]->meal_list = temp->next;
				free(temp);
				

			}
			free(tables[i]);
		}
		free(tables);
	}
		
		
	while (kitchen!= NULL) {
		temp = kitchen;
		kitchen = kitchen->next;
		free(temp->dishes->productName);
		free(temp->dishes);
		//free(temp);
	}
		
}
		

int RemoveItem(char* product_name, int table_number, Pkitchen_list kitchen, Ptable* tables, int Quantity,FILE* output,int tables_max) {
	float cancel_sum =0;
	Pkitchen_list current_dish = kitchen;
	Porder temp = NULL;

	if (!product_name || !kitchen || !tables) {
		fputs("Error! null pointer\n",output);
		return -1;
	}
	if (table_number < 0 || table_number >= tables_max) {
		fputs("invalid table number!\n",output);
		return 0;
	}
	if (tables[table_number] == NULL || tables[table_number]->meal_list == NULL){
		fputs("This table has not ordered yet\n", output);
		return 0;
		}
	if (Quantity > tables[table_number]->meal_list->dish->quantity ||
		Quantity <= 0) {
		fputs("inavalid quantity\n", output);
		return 0;
	}
	
	while (current_dish != NULL && (strcmp(current_dish->dishes->productName, product_name)!=0))
		current_dish = current_dish->next;
	if (current_dish == NULL) {
		fputs("The table has not ordered this dish\n", output);
		return 0;
	}
	cancel_sum = Quantity* (current_dish->dishes->price);
	tables[table_number]->meal_list->dish->quantity -= Quantity;
	tables[table_number]->meal_list->dish->price -= Quantity * current_dish->dishes->price;

	if (tables[table_number]->meal_list->dish->quantity == 0) {
		free(tables[table_number]->meal_list->dish->productName);
		free(tables[table_number]->meal_list->dish);

		temp = tables[table_number]->meal_list;
		if (temp->prev != NULL) { //if this is not first node
			tables[table_number]->meal_list = temp->prev;
			tables[table_number]->meal_list->next = temp->next;
		}
		else {   //if first
			tables[table_number]->meal_list = temp->next;
		}

		if (temp->next != NULL) {    //not last
			temp->next->prev = tables[table_number]->meal_list;
		}
		else {   //last
			if (temp->prev != NULL) temp->prev->next = NULL;
		}
		free(temp);
	}
			   //update table data
	tables[table_number]->Total_Price -= cancel_sum;
	
	return 1;
}

void ErrorMsg(char* msg, FILE* output,FILE* manot,FILE* instructions) {
	if (manot!= NULL) fclose(manot);
	if(instructions!=NULL)fclose(instructions);
	if (output != NULL) {
		fprintf(output, "%s", msg);
		fclose(output);
	}
	exit(-1);
}


