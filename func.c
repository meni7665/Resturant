#include "Header.h"

char printmain()
{
	char z = '1'; //make sure z!=0 for enter the loop every call
	printf("--------- Welcome To Habokrim Restaurant ---------\n");		//opening func:
	printf("Please choose option acording to the list :\n\n");
	printf("     1) Create Products.\n");
	printf("     2) Add Items.\n");
	printf("     3) Order Item.\n");
	printf("     4) Remove Item.\n");
	printf("     5) Remove table.\n");
	printf("     6) Report.\n\n");
	while (z != '0')													//if input !=0 ask for anouther input until 0
	{
		printf("\nPlease enter 0 to get new instruction : \n");
		z = getch();
		flushall();														//we gett 0 in and print him to the screen
		putch(z);
		printf("\n\n");
	}
	return z;
}
void CreateProducts(FILE *openManot, ManotList *List)					//Absorption the dishes to kitchen
{
	int i = 0, flag = 0;							//flag turn on if one of the input is wrong , if flag=1 block all the next fscanf . by that he cancel only the "problematic line in manot".
	float priceTemp = 0;							//float for check if price is hole number
	char name[256];
	List->count = 0;
	Manot *temp, *p;								//pointers to Manot
	temp = (Manot*)malloc(sizeof(Manot));			//allocation of one struct
	if (temp == NULL)
	{
		printf("No memory to add first meal to the kitchen list.");	// if there is no memory to add the struct we cant continue the function
		exit(1);
	}
	while (fscanf(openManot, "%s", name) == 1)		//while the file not empty
	{
		temp->name = (char*)malloc(sizeof(char)*(strlen(name) + 1));	//we made temp->name in the size of the letters of the name of the meal
		if (temp->name == NULL)											// cheack if the malloc Succeeded if not we free temp and go out
		{
			printf("no memory to add %s meal to the list.", name);
			free(temp);
			exit(1);
		}
		strcpy(temp->name, name);										//copy the name to his place in the struct
		p = List->Head;
		while (p && flag != 1)											//run wit *p untill that he get to the end of the List
		{
			if (strcmp(temp->name, p->name) == 0)						//if the names are similar
			{
				printf("One of the meals showed too times .\n");
				flag = 1;												//make sure no more scanf for this line
				free(temp->name);										//free name

			}
			p = p->next;												//promte p
		}
		fscanf(openManot, "%f", &temp->price);							//gett the price from the file
		if (temp->price < 1)
		{
			flag = 1;													//checf if the number is legal
		}
		fscanf(openManot, "%d", &temp->qunt);							//gett the Quantity from file to the struct
		if (temp->qunt < 1)
		{																//check if the Quantity is legal
			flag = 1;
		}
		fscanf(openManot, "%s", temp->pre);								//gett the char from file to struct to know if its a Premium or  not
		List->count++;													//count the Quantity for each meal in the kitchen
		if (flag == 1)													//if one of checks is ilegal free this temp and Missing from the count
		{
			free(temp);
			List->count--;
		}
		if (List->count == 1 && flag == 0)								//if we are in the first do the link here
		{
			List->Head = List->tail = temp;
			temp->next = temp->prev = NULL;
		}
		else if (flag == 0)												//enter to here from the second list
		{
			temp->next = NULL;											//link to the next list
			temp->prev = List->tail;
			List->tail->next = temp;
			List->tail = temp;
		}
		flag = 0;
		temp = (Manot*)malloc(sizeof(Manot));							//allocation of one struct
		if (temp == NULL)												//if the malloc not successful gett out
		{
			printf("no memory for new Meal.");
			exit(1);
		}
	}
	free(temp);
}

void AddItem(char *str, int num, ManotList *List)						//add stock to the kitchen
{
	int i, flag = 0;
	Manot *p;															//pointer to Manot
	p = List->Head;														//p start to the first link 
	while (p || flag != 1)												//while p not gett to end 
	{
		if (strcmp(str, p->name) == 0)									//check if the names are similar 
		{
			flag = 1;
			p->qunt += num;												//add to the struct the number of maels that we add 								
		}
		p = p->next;													//promte p
	}
	if (flag == 0)
		printf("This product doesnt exsist in the kitchen.\n");
}

void OrderItem(int index, char *name, int quant, Table *Mangger, ManotList *List) //order to table
{
	Manot *p;															//pointer to Manot
	Meal *Mana, *temp;													//pointers to Meal
	char buffer[256];
	p = List->Head;														//we Initializing the pionter to the head of the list
	Mangger[index - 1].index = index;									//to print the number of the table
	int flag = 0, sum = 0, checkflag = 0, Generalflag = 0;
	temp = Mangger[index - 1].Head;										//we Initializing temp to the head of the list
	while (p)															//loop untill end of meals
	{
		if (strcmp(name, p->name) == 0)									//check if the names are similar
			break;
		p = p->next;													//promte p
	}
	if (p == NULL)														//if end loop without find a match
	{
		printf("There is no such meal");
		Generalflag = -1;
	}
	if (quant > p->qunt && Generalflag != -1)								//if the Quantity is ilegal							
	{
		printf("The order of %s is bigger than the stock.\n", name);
		Generalflag = -1;
	}
	while (temp && Generalflag != -1)									//loop untill the end of temp and that the Quantity and name are legal
	{
		if (strcmp(temp->name, name) == 0)						//if the names are similar add to the same struct the data
		{
			temp->total += quant;
			sum = p->price*quant;
			Mangger[index - 1].bill += sum;
			p->qunt -= quant;
			flag = 1;
		}
		temp = temp->next;											//promte temp
	}

	if (flag != 1 && Generalflag != -1)									//if we get a new order creat a new node and enter to him all the struct
	{
		Mana = (Meal*)malloc(sizeof(Meal));								//bulid a new "Mana" struct if we faild exit .
		if (Mana == NULL)
		{
			exit(1);
		}
		Mana->total = 0;
		Mana->next = Mangger[index - 1].Head;
		Mangger[index - 1].Head = Mana;
		Mangger[index - 1].Head->name = (char*)malloc(sizeof(strlen(name) + 1));
		if (Mangger[index - 1].Head->name == NULL)
		{
			printf("no memory to add %s meal.\n", name);				//put the name of the meal in his right place and in the correct size (malloc for name).
			exit(1);
		}
		strcpy(Mangger[index - 1].Head->name, name);					//copy the name to his place in the struct
		sum = p->price*quant;											//enter the data to the struct
		Mangger[index - 1].bill += sum;
		Mangger[index - 1].counter++;
		Mangger[index - 1].Head->total += quant;
		p->qunt -= quant;
		if (Mangger[index - 1].counter == 1)
		{
			Mangger[index - 1].Head = Mangger[index - 1].tail = Mana;			//in the first node we enter here and bulid the node
		}
		else
		{
			Mangger[index - 1].Head = Mana;										//if not the first linking the node
		}
	}
}
void RemoveItem(int index, char *name, int quant, Table *Mangger, ManotList *List)//Cancel order items
{
	Meal *check;																//pointer to meal
	Manot *temp;																//pointer to Manot
	int sum = 0, flag = 0;
	temp = List->Head;															//Initializing temp to the head of the list	
	check = Mangger[index - 1].Head;											//Initializing check to the head of the Mangger
	while (check)																//while check 
	{
		if (strcmp(name, check->name) == 0 && check->total >= quant)			//if the names are similar and the Quantity is legal
		{
			flag = 1;
			check->total -= quant;												//Initializing the total in the struct
			if (check->total == 0)
			{
				Mangger[index - 1].counter--;									//if specific meal = 0 table->counter --
			}
			while (temp)														//loop for Manot list
			{
				if (strcmp(name, temp->name) == 0)								//if the names are similar
				{
					sum = temp->price * quant;									//calc the price of the meal*Quantity
					Mangger[index - 1].bill -= sum;								//calc the new bill
				}
				temp = temp->next;												//promte temp
			}
		}
		check = check->next;													//promte check
	}
	if (check == NULL || flag == 0)												//if check arrive to the end and we didnrt find a match between the names print appropriate message
	{
		printf("There is no such Meal in this table .\n");

	}

}
void RemoveTable(int index, Table *Mangger, ManotList *List)					//close table
{
	Manot *temp = List->Head;													//pointer to manot and Initializing him to list head
	Meal *tempNext;																//pointer to Meal
	float bill = 0;
	char buffer[256];
	int flagPremium = 0;
	Meal *tableTemp = Mangger[index - 1].Head;									//Initializing tabletemp to first meal
	while (tableTemp)															//loop for Manot
	{
		if (tableTemp->total != 0)												//if no meals in the table
		{
			strcpy(buffer, tableTemp->name);										//copy the shrink name to his place in the struct
			while (temp)														//loop for Manot
			{
				if (strcmp(temp->name, buffer) == 0)							//check if the names are similar
				{
					if (temp->pre == 'Y')										//if its a premium 
						flagPremium = 1;

				}
				temp = temp->next;												//prmote temp
			}
		}
		printf("     %s  X  %d   \n\n", tableTemp->name, tableTemp->total);		//print the meal name and his Quantity
		tableTemp = tableTemp->next;											//promte tabletemp
		temp = List->Head;														//promte temp
	}
	if (flagPremium == 1)
	{
		printf("This is a PREMIUM TABLE (+15%) \n\n");							//if it is a premium meal, calc the new bill and print it:
		bill = (Mangger[index - 1].bill)*0.15;
		bill += (Mangger[index - 1].bill);
		printf("The new bill of TABLE no. %d is : %.3f\n", index, bill);
		printf("Not include service :)\n\n");
	}
	else
	{
		printf("This is NOT a PREMIUM TABLE \n\n");								//	//if it is not a premium meal,  print the bill:		
		printf("The bill of TABLE no. %d is : %d\n", index, Mangger[index - 1].bill);
		printf("Not include service :)\n\n");
	}
	while (tableTemp)
	{
		tempNext = tableTemp->next;												//link to the new node
		free(tableTemp);
		tableTemp = tempNext;
	}

}
void Report(char Rep[2], int Repindex, Table *Mangger, ManotList *List, FILE *open)	//report
{
	Manot *temp = List->Head;												////Initializing temp to the head of manot
	Meal *temp2;															//Initializing temp2
	char buffer[256];
	if (Rep[0] == 'A')														//current sitution of kitchen
	{
		while (temp)														//loop for manot
		{
			printf("Name of product :%s\nQuantity of product : %d\nPrice of product : %.3f\n", temp->name, temp->qunt, temp->price); //print the information
			if (temp->pre[0] == 'Y')
				printf("Premium product \n\n");
			else
				printf("NOT a Premium product \n\n");
			temp = temp->next;											//promte temp
		}
		fscanf(open, "%s", buffer);										//get the Null from file.
	}
	else if(Rep[0] == 'B')
	{
		temp2 = Mangger[Repindex - 1].Head;								//print current situation of table
		if (Mangger[Repindex - 1].counter == 0)
			printf("This table is empety.");							//if empty print an annoucment
		else
		{
			while (temp2)
			{
				printf("The current order of Table number %d is :\n\n", Repindex);
				printf("%s ---> %d\n", temp2->name, temp2->total);
				temp2 = temp2->next;									//promte temp
			}

		}
	}
	else
	{
		printf("The char in function REPORT is ilegal.\n");
	}
}
