/*  
    Notification-Junction is an interface between multiple applications and multiple Notification Providers.
    Copyright (C) 2015      Navroop Kaur<watwanichitra@gmail.com>, 
                            Shivani Marathe<maratheshivani94@gmail.com>, 
                            Kaveri Sangale<sangale.kaveri9@gmail.com>
	All Rights Reserved.
	
    This program is //free software; you can redistribute it and/or modify it under the terms of the 
    GNU General Public License as published by the //free Software Foundation; either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the GNU General Public License for more details.
       
    You should have received a copy of the GNU General Public License along with this program; if not, write to the 
    //free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

/* 
*	The file contains the code for the dcll implementation with trailing singly linked lists for
*	Initializing app_list, 
*	Adding a node to app_list, 
*	Printing app_list, 
*	Searching app_node in app_list, 
*	Deleting a node from app_list, 
*	Adding a np to app_node, 
*	Deleting a np from app_node
*
*
*	At the end of this file is the commented code to test the linked list
*/

#include "app_dcll.h"

/* INITIALISE APPLICATION LIST */

void initApp(app_dcll * l)
{
	l->head = NULL;
	l->count = 0;
}

/* APPEND NEW APPLICATION-NODE TO THE LIST */

int addAppNode(app_dcll * l, char *val)
{
	
	app_node *new, *tmp;
	
	tmp = searchApp(l, val);	/* Search a app_node in app_list if it already exists */
	if (tmp != NULL)
		return ALREXST;	/* If node already exists, it returns ALREXST error */
	new = (app_node *) malloc(sizeof(app_node));

	if (new == NULL) {
		perror("> app_dcll.c addAppNode() : ERROR IN MALLOC");
		exit(1);
	}

	new->data =(char *)malloc((strlen(val) + 1) * sizeof(char));

	if (new->data == NULL) {
		perror("> app_dcll.c addAppNode() : ERROR IN MALLOC");
		exit(1);
	}

	new->np_count = 0;

	if (l->count == 0) {
		l->head = new;
		new->prev = new;
		new->next = new;
		strcpy(new->data, val);
		new->np_list_head = NULL;
		l->count++;
		return 0;
	}

	else if (l->count == 1) {
		l->head->prev = new;
		l->head->next = new;
		new->prev = l->head;
		new->next = l->head;
		strcpy(new->data, val);
		new->np_list_head = NULL;
		l->count++;
		return 0;
	}

	else {
		l->head->prev->next = new;
		new->prev = l->head->prev;
		new->next = l->head;
		l->head->prev = new;
		strcpy(new->data, val);
		new->np_list_head = NULL;
		l->count++;
		return 0;
	}
}

/* PRINT LIST */

void printApp(app_dcll * l)
{

	int i = l->count;
	app_node *ptr;
	np_node *np;
	
	if (l->count == 0) {
		printf("> app_dcll.c printApp() : NO APPS TO PRINT\n");
		return;
	}
	
	ptr = l->head;
	
	printf("\n> %s %d printApp() : Total number of applications : %d\n",__FILE__, __LINE__, l->count);
	printf("\n> %s %d printApp() :App\t\t\tNp_Count", __FILE__, __LINE__);
	printf("\n===================================\n");
	
	while (i) {		/* Printing list in forward direction */

		printf("%s\t\t\t", ptr->data);
		printf("%d\n", ptr->np_count);
		printNpKeyVal(ptr);
		printf("\n\n\n");
		ptr = ptr->next;
		i--;
	}

	// Printing registrations

	i = l->count;
	ptr = l->head;
	if (i != 0) {
		printf("\n> %s %d printApp() :App\t\t\tRegistered with", __FILE__, __LINE__);
		printf("\n===================================\n");
	}

	while (i) {
		np = ptr->np_list_head;
		printf("\n%s\t\t\t", ptr->data);
		
		while (np != NULL) {
			printf("%s\n", np->name);
			np = np->next;
			printf("\t\t\t");
		}

		ptr = ptr->next;
		i--;
	}

}

void printNpKeyVal(app_node * temp)
{

	np_node *head = temp->np_list_head;
	extr_key_val *vptr;
	char **kptr;
	

	if (!head) {
		printf("\n> %s %d printNpKeyVal() : No head in temp list\n\n", __FILE__, __LINE__);
		return;
	}

	printf("> %s %d printNpKeyVal() : HEAD DATA = %s\n", __FILE__, __LINE__, head->name);
	
	while (head) {
		vptr = head->key_val_ptr;
		
		if (vptr == NULL) {
			printf("> %s %d printNpKeyVal() : VPTR IS NULL\n", __FILE__, __LINE__);
			head = head->next;
			continue;
		}
		
		while (vptr) {
			kptr = vptr->key_val_arr;
			
			if (kptr == NULL) {
				printf("> %s %d printNpKeyVal() : The key_val_arr is NULL\n\n", __FILE__, __LINE__);
				return;
			}
			
			printf("> %s %d printNpKeyVal() : *KPTR is %s\n",__FILE__, __LINE__, *kptr);

			while (*kptr) {
				printf("\t\t\t\t%s\n", *kptr);
				kptr++;
			}
			
			vptr = vptr->next;
		}
		head = head->next;
	}
	return;
}

/* SEARCH APP, RETURN POINTER TO NODE */

app_node *searchApp(app_dcll * l, char *val)
{

	int i = l->count;
	int found = 0;
	app_node *ptr;

	if (l->count == 0)
		return NULL;

	ptr = l->head;

	while (i) {
		if (strcmp(ptr->data, val) == 0) {
			found = 1;
			break;
		}
		i--;
		ptr = ptr->next;
	}

	if (found == 0) {
		return NULL;
	}

	else
		return ptr;
}

int searchReg(app_dcll * l, char *appname, char *npname)
{

	app_node *ptr;
	np_node *nptr;
	
	ptr = searchApp(l, appname);

	printf("\n> %s %d searchReg() : Searching Registration\n", __FILE__, __LINE__);

	if (ptr == NULL)
		return 0;

	if (ptr != NULL) {

		printf("> %s %d searchReg() : App %s has been found. Checking np %s in app's list\n",__FILE__, __LINE__, appname, npname);
		nptr = ptr->np_list_head;

		while (nptr != NULL) {          /* Code for checking NP existance */
			if (!strcmp(npname, nptr->name)) {
				printf("> %s %d searchReg() : Duplicate Registration Detected", __FILE__, __LINE__);
				return -1;
			}
			nptr = nptr->next;
		}

	}

	return 0;
}

np_node *getReg(app_dcll * l, char *appname, char *npname)
{

	app_node *ptr;
	np_node *nptr;
	
	ptr = searchApp(l, appname);

	if (ptr == NULL)
		return 0;

	if (ptr != NULL) {

		printf("> %s %d getReg() : App %s has been found. Checking np %s in app's list\n",__FILE__, __LINE__, appname, npname);
		nptr = ptr->np_list_head;

		/* CODE FOR CHECKING NP EXISTANCE */
		while (nptr != NULL) {
			if (!strcmp(npname, nptr->name)) {
				return nptr;
			}
			nptr = nptr->next;
		}

	}

	return NULL;

}

/* DELETE APP */

int delApp(app_dcll * l, char *val)
{
	char **np_key_val_arr;
	int i = 0;
	np_node *np_tail, *np_temp;
	app_node *p, *temp, *q;
	struct extr_key_val *extr_kv, *extr_kv_temp;

	temp = searchApp(l, val);
	
	if (temp == NULL) {
		return NOTFND;
	}

	else if (temp == l->head && l->count == 1) {

		l->head->prev = NULL;
		l->head->next = NULL;
		l->head = NULL;
	}

	else if (temp == l->head && l->count > 1) {

		l->head = temp->next;
		p = temp->prev;
		(temp->next)->prev = p;
		p->next = temp->next;
	} 
	else {
		p = temp->prev;
		q = temp->next;
		p->next = q;
		q->prev = p;
	}
	
	np_tail = temp->np_list_head;
	
	while(np_tail != NULL) {
		extr_kv = np_tail->key_val_ptr;
		while(extr_kv != NULL) {
			np_key_val_arr = (extr_kv)->key_val_arr;
		 	i = 0;
			while(np_key_val_arr[i++] != NULL) {
				free(np_key_val_arr[i]);
			}
			extr_kv_temp = extr_kv;
			extr_kv = extr_kv->next;
			free(extr_kv_temp);
		}
		np_temp = np_tail;
		np_tail = np_tail->next;
		free(np_temp->name);
		free(np_temp);
	}
	
	free(temp->data);
	free(temp);
	l->count--;
	return 0;	
}


int addNpToApp(app_dcll * l, char *aval, char *nval)
{

	app_node *temp;
	np_node *n, *m, *b;

    temp = searchApp(l, aval);

	if (temp == NULL) {
		return NOTFND;
	}

	n = (np_node *)malloc(sizeof(np_node));

	if (n == NULL) {
		perror("APP_DCLL :  ERROR IN MALLOC");
		exit(1);
	}

	n->name = (char *)malloc((strlen(nval) + 1) * sizeof(char));
	n->key_val_ptr = NULL;

	if (n->name == NULL) {
		perror("APP_DCLL :  ERROR IN MALLOC");
		exit(1);
	}
	n->next = NULL;
	strcpy(n->name, nval);
	if (temp->np_list_head == NULL) {
		temp->np_list_head = n;
		temp->np_count++;
		return 0;
	} else {
		m = temp->np_list_head;
		b = m;
		if (m->name == nval) {
			return ALREXST;
		}
		b = m;
		m = m->next;
		while (m != NULL) {
			if (m->name == nval) {
				return ALREXST;
			}
			b = m;
			m = m->next;
		}
		b->next = n;
		temp->np_count++;
	}
	return 0;
}

/* DELETE NP FROM APPLICATION */

int delNpFromApp(app_dcll * l, char *aval, char *nval)
{
	char **np_key_val_arr;
	int i = 0;
	np_node *np_tail;
	struct extr_key_val *extr_kv, *extr_kv_temp;
	
	int flag = 0;
	app_node *temp = searchApp(l, aval);
	np_node *n, *m, *b;

	if (l->count == 0)
		return NOTFND;
		
	if (temp == NULL) {
		return NOTFND;
	}
	
	if (temp->np_list_head == NULL) {
		return 0;
	}
	
	if (temp->np_list_head->next == NULL) {
		m = temp->np_list_head;
		temp->np_list_head = m->next;
		temp->np_count--;
		printf("\n> %s %d delNpFromApp() : temp->np_count = %d\n", __FILE__, __LINE__, temp->np_count);
		flag = 1;
	} 
	
	else if (!strcmp(temp->np_list_head->name, nval)) {
		m = temp->np_list_head;
		n = m->next;
		temp->np_list_head = n;
		temp->np_count--;
		printf("\n> %s %d delNpFromApp() : temp->np_count = %d\n",__FILE__, __LINE__, temp->np_count);
		flag = 1;
	} 
	
	else {
		m = temp->np_list_head;
		b = m;
		
		if (temp->np_list_head == m && m->name == nval) {
			temp->np_list_head = m->next;
			temp->np_count--;
			printf("\n> %s %d delNpFromApp() : temp->np_count = %d\n",__FILE__, __LINE__, temp->np_count);
			flag = 1;
		}
		m = m->next;
		
		while (m != NULL) {
		
			if (strcmp(m->name, nval) == 0) {
				b->next = m->next;
				temp->np_count--;
				printf("\n> %s %d delNpFromApp() : temp->np_count = %d\n",__FILE__, __LINE__, temp->np_count);
				flag = 1;
				break;
			}
			b = m;
			m = m->next;
		}
	}
	if(flag == 0)
		return -1;
	
	np_tail = m;
		
	extr_kv = np_tail->key_val_ptr;
	while(extr_kv != NULL) {
		np_key_val_arr = (extr_kv)->key_val_arr;
	 	i = 0;
		while(np_key_val_arr[i++] != NULL) {
			free(np_key_val_arr[i]);
		}
		extr_kv_temp = extr_kv;
		extr_kv = extr_kv->next;
		free(extr_kv_temp);
	}
	free(np_tail->name);
	free(np_tail);	
		
	return 0;
}

/* Below is the code to test the list */

/*int main() {

	app_dcll l;
	char val[25], aval[25], nval[25];
	initApp(&l);
	
	int ch;
	while(1) {
		//printf("APP_DCLL : Enter Choice :\n0 - printApp\n1 - Add App\n2 - searchApp App\n3 - Delete App\n4 - Add NP to App\n5 - Delete NP from App\n");
		scanf("%d", &ch);
		switch(ch) {
	
			case 0 :	printApp(&l);
					break;
			case 1 :	//printf("APP_DCLL : Enter Val :\n");
					scanf(" %s", val);
					addAppNode(&l, val);
					printApp(&l);
					break;
			case 2 : 	//printf("APP_DCLL : Enter Val :\n");
					scanf(" %s", val);
					searchApp(&l, val);
					break;
			case 3 :	//printf("APP_DCLL : Enter Val :\n");
					scanf(" %s", val);
					delApp(&l, val);
					printApp(&l);
					break;
			case 4 :	//printf("APP_DCLL : Enter Val for App and NP:\n");
					scanf(" %s %s", aval, nval);
					//printf("APP_DCLL : Aval - %s| and nval - %s|\n", aval, nval);
					addNpToApp(&l, aval, nval);
					printApp(&l);
					break;
			case 5 :	//printf("APP_DCLL : Enter Val for App and NP:\n");
					scanf(" %s %s", aval, nval);
					delNpFromApp(&l, aval, nval);
					printApp(&l);
					break;
		}
	}
	return 0;

}

*/
