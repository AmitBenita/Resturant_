#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef _Header_1
#define _Header_1
#include <string.h>
#include <stdio.h> 

 
typedef struct product {   //dish
	char* productName;
	float price;
	int quantity;
	int sales;
}product, * Pproduct;

typedef struct kitchen_list { //linked list of products in kitchen
	Pproduct dishes;
	struct kitchen_list* next;
	//int twosided;
} kitchen_list, * Pkitchen_list;

typedef struct table_order {   //two direction linked list of products
	Pproduct dish;
	struct table_order* next;
	struct table_order* prev;

}order, * Porder;

typedef struct table
{
	float Total_Price;
	Porder meal_list;
	
} table , *Ptable;




void ErrorMsg(char* msg, FILE* output,FILE* manot, FILE* instructions);
Pkitchen_list CreateProduct(FILE* manot, FILE* output, Pkitchen_list kitch);
int check_name(Pkitchen_list list_head, char* temp_name); //function checks if temp_name already exists in list
int AddItems(Pkitchen_list product_list, char* product_name, int Quantity, FILE* output);
int OrderItem(int table_number, char* prodect_name, int Quantity, Pkitchen_list kitchen,FILE *output, int max_tables, Ptable* tables, Ptable current_table);
int RemoveTable(int table_number, Ptable* current_table, Pkitchen_list kitchen , FILE *output, int max_tables);
void freeAll(Pkitchen_list kitchen, Ptable* tables,int num_of_tables);
int RemoveItem(char* product_name, int table_number, Pkitchen_list kitchen, Ptable* tables, int Quantity, FILE* output, int tables_max);
int CreateTableArr(Ptable* table_arr, int num_of_tables);


#endif