#ifndef Header
#define Header

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <string.h>

typedef struct Manot		//definition struct of 1 dish :
{
	char *name;				//string pointer for dish name
	float price;			//int for price
	int qunt;				//int for quantity
	char pre[2];			//char for Y/N ,premium or not
	struct Manot *next;		//pointer for next struct
	struct Manot *prev;		//pointer for prev struct
}Manot;

typedef struct ManotList
{
	Manot *Head;
	Manot *tail;
	int count;				//counter for quantity of manot

}ManotList;

typedef struct Meal
{
	char *name;
	int total;				//counter of the same meal
	struct Meal *next;
}Meal;

typedef struct Table
{
	int bill, index, counter; //Index= table number , bill = save the bill of this table , counter = the numbers of the mealls that order to this table
	Meal *Head, *tail;
}Table;

char printmain();			//function for printing the manu
void CreateProducts(FILE *open, ManotList *List);											//function 1
void AddItem(char *str, int num, ManotList *List);											//function 2
void OrderItem(int index, char *name, int quant, Table *mangger, ManotList *List);			//function 3
void RemoveItem(int index, char *name, int quant, Table *Mangger, ManotList *List);			//function 4
void RemoveTable(int index, Table *Mangger, ManotList *List);								//function 5
void Report(char Report[2], int Repindex, Table *Mangger, ManotList *List, FILE *open);			//function 6

#endif // !Header

