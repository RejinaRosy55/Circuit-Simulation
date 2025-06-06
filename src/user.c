#include "graph.h"

// Truth table for AND gate
int and_LUT[5][5] = {	//0 1  X  D  Db
						{0, 0, 0, 0, 0},  //0
						{0, 1, X, D, Db},  //1
						{0, X, X, X, X},  //X
						{0, D, X, D, 0},  //D
						{0, Db, X, 0, Db} //Db
					};

// Truth table for OR gate
int or_LUT[5][5] =  {
						{0, 1, X, D, Db},
						{1, 1, 1, 1, 1},
						{X, 1, X, X, X},
						{D, 1, X, D, 1},
						{Db, 1, X, 1, Db}
					};

// Truth table for NOT gate
int not_LUT[5] = {1, 0, X, Db, D};

void num_input_output(int *Npi, int *Npo, int Max, NODE *graph, LIST **ip_list, LIST **op_list){
	int count, i;
	for(i=0;i<=Max;i++){
		if(graph[i].Type == INPT){
			*Npi +=1;
			InsertList(ip_list,i);			
		}
		if(graph[i].Po == 1){	
			*Npo+=1;
			InsertList(op_list,i);
		}
		
	}
	 //PrintList(*ip_list);
}

PATTERN* readVector(int *Npt, FILE *fvec){

	char line[Mlin];
	static PATTERN vector[Mpt];
	int line_count = 0;
	int j;

	// Read each line from the fvec
	while (fgets(line, sizeof(line), fvec) != NULL) {
		// Remove the newline character if present
		if (line[strlen(line) - 1] == '\n') {
			line[strlen(line) - 1] = '\0';
		}

		// Copy the line to the array
		for (j = 0; j < strlen(line); j++) {
			if (line[j]=='0'){
				vector[line_count].pattern[j]= 0;				
			}
			else if(line[j]=='1'){
				vector[line_count].pattern[j]= 1;
			}
			else if(line[j]=='x' || line[j]=='X'){
				vector[line_count].pattern[j]= X;
			}
			else {
				printf("Error:Invalid Character in readvector function \n");
				exit(1);
			}
		}
		line_count++;
	}
	*Npt = line_count;
	return vector;
}

void readFault(int *Nft, FILE *ffau, FAULT **stuck){
	char line[Mlin];
	int i=0; 
	int node, stuck_at;

	//Read the file line by line and store node/value in fault structure
	while(fscanf(ffau,"%d /%d", &node, &stuck_at)==2 || fscanf(ffau,"%d/%d", &node, &stuck_at)==2){
		stuck[i] = (FAULT *)malloc(sizeof(FAULT));  // Allocate memory for each FAULT
		stuck[i]->node = node;
		stuck[i]->stuck_at = stuck_at;
		// printf("Node:%d Fault:%d \n", stuck[i].node, stuck[i].stuck_at);
		i++;
	}
	*Nft = i;
}

void and_function(NODE *graph, int j, LIST *temp){
	graph[j].Cval = 1;
	while(temp!= NULL){
	graph[j].Cval = and_LUT[graph[j].Cval][graph[temp->id].Cval];
	temp = temp->next;
	}
}

void or_function(NODE *graph, int j, LIST *temp){
	graph[j].Cval = 0;
	while(temp!= NULL){
		graph[j].Cval = or_LUT[graph[j].Cval][graph[temp->id].Cval];
		temp=temp->next;
	}
}

void nand_function(NODE *graph, int j, LIST *temp){
	graph[j].Cval =  1;
	while(temp!= NULL){
		graph[j].Cval = and_LUT[graph[j].Cval][graph[temp->id].Cval];
		temp = temp->next;
	}
	graph[j].Cval = not_LUT[graph[j].Cval];
}

void nor_function(NODE *graph, int j, LIST *temp){
	graph[j].Cval = 0;
	while(temp!= NULL){
		graph[j].Cval = or_LUT[graph[j].Cval][graph[temp->id].Cval];
		temp=temp->next;
	}
	graph[j].Cval = not_LUT[graph[j].Cval];
}

void buf_from_function(NODE *graph, int j, LIST *temp){
	graph[j].Cval = graph[temp->id].Cval ;
}

void not_function(NODE *graph, int j, LIST *temp){
	graph[j].Cval = not_LUT[graph[temp->id].Cval];
}


void logicSim(PATTERN *vector, NODE *graph, int Max, LIST *ip_list, LIST *op_list, int Npt, int Npi, int Nft, FAULT **stuck, FILE *fres){
	int i,j,k,l;
	for(k=0; k<Nft; k++){
		fprintf(fres,"%d/%d\n",stuck[k]->node,stuck[k]->stuck_at);
		for(i=0; i<Npt; i++){
			int bit_idx = 0;

			//fprintf(fres, "Input Pattern: ");
			for(l=0; l<Npi; l++){
				if(vector[i].pattern[l]==2){
					fprintf(fres, "x");
				}
				else
				{fprintf(fres, "%d", vector[i].pattern[l]);}
			}
			fprintf(fres, " ");
			
			//Logic and Fault Simulation
			for(j=0; j<=Max; j++){
				if(graph[j].Type ==0 ){ continue;}
			
				LIST *temp =graph[j].Fin;  
				if (graph[j].Type == INPT) {
						graph[j].Cval = vector[i].pattern[bit_idx++];
				}

				else if(graph[j].Type == AND){and_function(graph, j, temp);}
				
				else if(graph[j].Type == NAND){nand_function(graph, j, temp);}	

				else if(graph[j].Type == OR){or_function(graph, j, temp);}

				else if(graph[j].Type == NOR){nor_function(graph, j, temp);}

				else if(graph[j].Type == BUFF){buf_from_function(graph, j, temp);}

				else if(graph[j].Type == NOT){not_function(graph, j, temp);}

				else if(graph[j].Type == FROM){buf_from_function(graph, j, temp);}
				
				if(j==stuck[k]->node){
					if ((stuck[k]->stuck_at == 0) && (graph[j].Cval == 1)) {
						graph[j].Cval = D;
						}
					else if ((stuck[k]->stuck_at == 1) && (graph[j].Cval == 0)) {
						graph[j].Cval = Db;
						}
					}
			}
			// printf("Working on pattern %d\n", i);
			// PrintCircuit(graph, Max);
			// getchar();
			writeResult(graph, ip_list, op_list, vector, fres, i);
		}
		fprintf(fres,"\n");
	}
}

void writeResult(NODE *graph, LIST *ip_list, LIST *op_list, PATTERN *vector, FILE *fres, int i){
	fprintf(fres, "%d: ",i);
	LIST *temp=NULL;
	// temp = ip_list;
	// PrintList(temp);
	// while(temp!=NULL){
	// 	if(vector[i].pattern[temp->id] == 2){
	// 		fprintf(fres, "x");
	// 		printf("x");
	// 	}
	// 	else
	// 	{fprintf(fres, "%d",vector[i].pattern[temp->id]);
	// 	printf("%d",vector[i].pattern[temp->id]);
	// 	}
	// 	temp = temp->next;
	// }
	// fprintf(fres, " ");
	temp = op_list;
	int a=0;
	while(temp!=NULL){
		if(graph[temp->id].Cval == 2){
			fprintf(fres, "x");
		}
		else if(graph[temp->id].Cval == 3){
			fprintf(fres, "D");
			a=1;
		}
		else if(graph[temp->id].Cval == 4){
			fprintf(fres, "d");
			a=1;
		}
		else{
		fprintf(fres, "%d",graph[temp->id].Cval);
		}
		temp = temp->next;
	}
	if(a==0){fprintf(fres, " (Fault Not Detected)");}
	else if(a==1){fprintf(fres, " (Fault Detected)");}
	fprintf(fres,"\n");			
}


