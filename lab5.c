#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHANGE (10.0/86400.0)		/* 10kg/day */
	/* seconds in a day is 24 hours * 60 minutes * 60 seconds */
//linked list implementation
	struct linkedlist{
	char data[1024]; // pointer to store unlimited number of userID's
	float  *weightdata;
	int    timedata;
	int numofweight;
	struct linkedlist *next;
	}
	*head , *current ,*last ; /*the head of the list points to null*/
int main() {
    char linearray[1025];
	char *line;  //token
	int lasttime = 0;
	int tokens;
	int timestamp;
	int duration;
	char userID[1024];
	//char lastuserID[180] = "";
	float weight;
	float lastweight;
	float change;
	float changePerTime;
	//int  numofweight; /*counter for the the weight change*/
	 int counter = 0;
	 int numofweight2 = 0;
	/*intialiszing the nodes to store the data*/
	struct linkedlist *current ;
	/* dummy node*/
	head = (struct linkedlist*)calloc(1,sizeof(struct linkedlist));
	strcpy(head ->data ,"");
	/*intialised the weightdata because its a pointer*/
	head-> weightdata = NULL;//(float*)realloc(head-> weightdata, sizeof(float)*(1+head ->numofweight));
	//head -> weightdata[head->numofweight] = 0.0f;
	head -> timedata = 0;
	head -> next = NULL;
	last = head;
	
	while (fgets(linearray,1024,stdin) != NULL) {
		line = linearray;
		line = strtok(line," \n");	/* read number for timestamp */
		if (line == NULL || sscanf(line,"%d", &timestamp) < 1 || timestamp == 0) {
			printf("Invalid time\n");
			continue;
			}
		line = strtok(NULL," \n");	/* start reading userID */
		if (line == NULL || isdigit(line[0]) || line[0] == '.') {
			printf("Illegal userID\n");
			continue;
			}
		strcpy(userID,line);		/* initialize userID */
		strcat(userID," ");			/* append 1 space to normalize white space */
		do {
			line = strtok(NULL," \n");
			if (line != NULL && !isdigit(line[0]) && line[0] != '.') {
				strcat(userID,line);						 /* valid -> append */
				strcat(userID," ");
				}
			} while (line != NULL && !isdigit(line[0]) && line[0] != '.');
		/* there will be one space too much at the end here. Fix that */
		userID[strlen(userID) - 1] = '\0';
		if (strlen(userID) > 179) {
			printf("Illegal userID\n");
			continue;
			}
		if (line != NULL)		/* in this case current token is a number, so we read weight */
			tokens = sscanf(line,"%f",&weight);
		if (line == NULL || tokens < 1 || weight < 30.0 || weight > 300.0) {
			printf("Illegal weight\n");
			continue;
			}
		if (lasttime >= timestamp) {
			printf("Nonmonotonic timestamps\n");
			continue;
			}
		
			else {
				counter++;
				for(current = head -> next; current != NULL ; current = current -> next){
					if(strcmp(current -> data ,userID) == 0){
					duration = timestamp - current->timedata;
					change = weight - current -> weightdata[current -> numofweight];
					changePerTime = change / duration;
					//current->numofweight++;
					//printf("Weight index before comparing :  %d\n", current->numofweight);
						if(changePerTime < -MAX_CHANGE || changePerTime > MAX_CHANGE)
						//printf("%f\n", current -> weightdata[current->numofweight]);
						printf("Suspiciously large weight change\n");
							else/*else for change in weight over time*/
							printf("OK\n");
							current ->timedata = timestamp;
							/*if they are the same userid i increase the count and store it in the weight pointer array*/
							current ->numofweight++;
							/*store the number of weight in array pointer by reintialising the weightdata after every run*/
							current-> weightdata = (float*)realloc(current-> weightdata, sizeof(float)*(1+current->numofweight));
							current -> weightdata[current->numofweight] = weight;
							//printf("Execute %d\n", current->numofweight); 
							
							break;
							}	
						}
					if(current == NULL){
						current =(struct linkedlist*)calloc(1,sizeof(struct linkedlist));
						strcpy(current ->data , userID);
						current->timedata = timestamp;
						current-> weightdata = (float*)realloc(current-> weightdata, sizeof(float)*(1+current->numofweight));
						current -> weightdata[current->numofweight] = weight;
						//current->numofweight++;
						//printf("newUser Count count %d\n", current->numofweight);
						current -> next = NULL;
						last->next = current;
						last = current;
						printf("OK newuser\n");
					}
				}
					
					lasttime = timestamp;
					lastweight = weight;
					//strcpy(lastuserID,userID);
		
		
		}
	    /*resetting the line after every input*/ 
			timestamp = 0;
			weight = 0.0f;
			strcpy(userID ,"");
			strcpy(linearray,"");
			
		/*the graph representation code*/
		numofweight2 = current ->numofweight;
		//printf("the number of valid weights %d\n", counter);
		//int p = 0;
		//for(p = 0 ; p<= numofweight2 ; p++){
		//printf(" data at %d %d \n", p , (int)(current->weightdata[p]/30));
		//}
		
		//printf("the number of weight %d \n" , numofweight2);
		int rows  = numofweight2+1;
		int col = 0;
		char barchart[11][rows+1];
		for (col = 0 ; col <= 10 ; col++){
		 barchart[col][0] = '|'; 
		}
		for(rows= 0 ; rows<=numofweight2+1 ; rows++){
		 barchart[0][rows] = '-';
		}
		barchart[0][0] = '+';
		/*start at the bottom and print a star*/
		for(rows= 1 ; rows<=numofweight2+1 ; rows++){
			if( rows -1 <= 0){
				printf("No weight data, please check input bye\n");
				exit(0);
			}	
			int stars = (int)(current->weightdata[rows-1]/30);
			for(col = 1 ; col <= stars ; col++){
				barchart[col][rows]= '*'	;
			}
			/*print the spaces after printing the stars*/
			for(col = stars+1 ; col <= 11 ; col++){
				barchart[col][rows]= ' '	;
	
			}
		}
		/*prints bargraph reversed*/
		for(col = 10 ; col >=0 ; col--){
			for(rows = 0 ; rows <=numofweight2+1 ; rows++)
				printf("%c",barchart[col][rows]); 
				printf("\n");
			}
			return 0;
}/*end of main*/     
  
