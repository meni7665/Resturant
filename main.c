#include "Header.h"

int main()
{
	int table = 0, Quantity, index = 0, flag = 0, OrderQuant = 0, i, Rindex = 0, Rquant = 0, RTindex = 0, Repindex = 0;
	char z, tav, buffer[256], *ProductName, *Mealname, Rep[2];
	FILE *openManot;																		//declartion for file* to open manot text
	ManotList List;
	Table *Mangger;
	printf("Please enter the number of tables in the Restaurant : ");						//recive the number of tables
	scanf("%d", &table);
	Mangger = (Table*)malloc(sizeof(Table)*table);											//malloc for all the tables
	for (i = 0; i < table; i++)
	{
		Mangger[i].Head = Mangger[i].tail = NULL;											//input head and tail of each table to null
		Mangger[i].bill = Mangger[i].counter = Mangger[i].index = 0;						//input 0 to the fields of each table
	}
	z = printmain();																		//first input return from function z=0; 

	FILE *open = fopen("Instructions.txt", "rt");											//open file
	if (open == NULL)
		printf("Problem with open the file.");
	while (fscanf(open, "%s", &tav) == 1)													//loop until end of file
	{
		flag = 0;
		switch (tav)																		//switch case for the number of instruction fron instructions file
		{
		case '1':
			openManot = fopen("Manot.txt", "rt");
			if (openManot == NULL)
			{
				printf("Problem with open the file.");
				exit(1);
			}
			List.Head = List.tail = NULL;													//first head and tail point to NULL before reciving the meals from file manot
			List.count = 0;
			CreateProducts(openManot, &List);												//call func with FILE* of manot ,and address of the MANOTLIST that created in the main. 
			fclose(openManot);
			z = printmain();																//recive the next instruction.
			break;
		case '2':
			fscanf(open, "%s%d", buffer, &Quantity);										//recuve from file 2 information : name of meal and quantity to add in the list
			ProductName = (char*)malloc(sizeof(char)*strlen(buffer) + 1);					//minimize the name to exact memory needed
			if (ProductName == NULL)														//if the malloc for name failed , out of program
			{
				printf("no memory.");
			}
			strcpy(ProductName, buffer);													//copy the name to the struct
			if (Quantity < 1)																//if quantity to add 0 or less , not make sense jump to break
			{
				printf("The Quantity to add for %s is lower than 1.\nPlease check file.\n\n", ProductName);
			}
			else
			{
				AddItem(ProductName, Quantity, &List);										//if everything is o.k in details from file go to function
				z = printmain();															//recive new instruction
			}
			break;
		case '3':
			fscanf(open, "%d%s%d", &index, buffer, &OrderQuant);							//recive information from file
			if (index > table)																//if table number is bigger than what exsist , print apropriate massage ang raise flag
			{
				printf("Table number doesnt exsist.\n");
				flag = 2;
			}
			Mealname = (char*)malloc(sizeof(char)*strlen(buffer) + 1);						//minimize the name to exact memory needed				
			if (Mealname == NULL)
			{
				printf("no memory.");
				flag = 1;																	//raise special flag if malloc failed by that cancel the enter to function
			}
			strcpy(Mealname, buffer);														//copy shrink name to place in struct
			if (OrderQuant < 1)																//if quantity to add 0 or less , not make sense jump to break
			{
				printf("The Quantity to add for %s is lower than 1.\nPlease check file.\n\n", Mealname);	//the quantity to add to the table is 0 or less ,not make sense raise flag and jump to break
				flag = 2;
			}
			if (flag == 0)																	//if flag = 0 pass all checks enter func
			{
				OrderItem(index, Mealname, OrderQuant, Mangger, &List);						//call func with relevant info from file
			}
			else
				if (flag == 2)																//if flag = 2 not faild in malloc of name so need free , else faild in malloc so dont need free
					free(Mealname);
			z = printmain();
			break;
		case '4':
			flag = 0;
			fscanf(open, "%d%s%d", &Rindex, buffer, &Rquant);								//recive information from the file
			if (Rindex > table)																//if index bigger than table not make sense raise flag 
			{
				flag = 1;
				printf("There is no table with this number (%d).\n", Rindex);
			}
			if (Rquant < 1)																	//if the number of products to delete is 0 or less not make sense raise flag and print appropriate message
			{
				flag = 1;
				printf("The number of Items to delete is neggative number.\n");
			}
			if (flag == 0)																	//if flag = 0 pass all checks enter func
				RemoveItem(Rindex, buffer, Rquant, Mangger, &List);							//enter function
			z = printmain();																//wait for new instruction
			break;
		case '5':
			fscanf(open, "%d", &RTindex);													//recive number of table from file
			if (RTindex < table)															//if number of table bigger than exsist out from function
			{
				if (Mangger[RTindex - 1].counter == 0)											//if the counter of meals = 0 ,no meals on the table ,the table is free. no need enter the function
					printf("The table is empty.\n");
				else
				{
					printf("Thank YOU FOR EATING IN  \n--- HABOKRIM ---  \n\nYour bill is : \n\n");		//defult message before print the bill
					RemoveTable(RTindex, Mangger, &List);												//call function
				}
			}
			else
			{
				printf("The number of %d Table is bigger than exsist.\n",RTindex);
			}
			z = printmain();																		//wait for new instruction
			break;
		case '6':
			fscanf(open, "%s%d", Rep, &Repindex);														//open informatoin from file
			Report(Rep, Repindex, Mangger, &List, open);												//call function with the list and array for reciving information needed
			z = printmain();																		//wait for new instruction
			break;

		default:
			printf("One of the instructions didnt recognized well\nPlease check your file.\n");		//if the input from file is wrong but not the end of file ,exit.																				
			break;
		}
	}																		
	fclose(open);
	free(Mangger);																				//free all the tables
	printf("\n\n\n\nTHANK YOU FOR USING OUR RESTURAUNT SYSTEM.\n\nHOPE TO SEE YOU AGAIN SOON.\n");		//finish print on the screen
	exit(1);
}

