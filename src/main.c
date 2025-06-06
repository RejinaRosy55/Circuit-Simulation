#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/
void main(int argc,char **argv)
{
FILE *fisc,*fvec,*ffau,*fres;             //file pointers used for .isc file, .vec file, .faults file and resultfile
int Max,Opt,Npi=0,Npo=0,Total,Tfs,Tpt=0;              //maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults

NODE graph[Mnod];                         //structure used to store the ckt information in .isc file 
PATTERN *vector;                      //structure used to store the input vectors information in .vec file 
LIST *ip_list = NULL, *op_list = NULL;
char benchmark[15];
char benchmark1[15];
FAULT *stuck[Mft];                      //structure used to store the faults information in .faults file
int a,b,c,d;                             //random variables
int Npt;                //Number of input patterns
int Nft;                //Number of stuck at fault

sprintf(benchmark,"%s.isc", argv[1]);
printf("%s", benchmark);
//Read the .isc file and store the information in graph structure
fisc=fopen(benchmark,"r");                           //file pointer to open .isc file 
Max=0; 
Max=ReadIsc(fisc,graph);                 //read .isc file and return index of last node in graph formed
fclose(fisc);                            //close file pointer for .isc file

//Read the .vec file and store the information in  vector structure
benchmark[0]='\0';
// printf("\nCleared Benchmark: %s \n", benchmark);
sprintf(benchmark,"%s.vec", argv[1]);
fvec = fopen(benchmark, "r");
if (fvec == NULL){
    printf("%s does not exist\n", benchmark);
    exit(0);
}
vector = readVector(&Npt, fvec);
fclose(fvec);
num_input_output(&Npi, &Npo, Max, graph, &ip_list, &op_list);

//Read the .fault file 
sprintf(benchmark,"%s.faults", argv[1]);
printf(" %s\n", benchmark);
ffau = fopen(benchmark, "r");
readFault(&Nft, ffau, stuck);
fclose(ffau);

benchmark1[0]='\0';
// printf("\nCleared Benchmark: %s \n", benchmark1);
sprintf(benchmark1," %sout.vec", argv[1]); 

fres = fopen(benchmark1, "w");
fprintf(fres, "D represents 1/0 and d represents 0/1 \n \n");
// printf("%s\n",benchmark1);
logicSim(vector, graph, Max, ip_list, op_list, Npt, Npi, Nft, stuck, fres);
fclose(fres);

//Opt=0; 
//Opt=atoi(argv[3]);                          //getting the option from terminal for xval
//fres=fopen(argv[2],"w");                           //file pointer to open .out file for printing results

//Perform Logic Simulation for each Input vector and print the Pos .val in output file   

// free(stuck);
ClearCircuit(graph,Mnod);                                      //clear memeory for all members of graph
//for(a=0;a<Total;a++){ bzero(vector[a].piv,Mpi); }                //clear memeory for all members of vector
return;
}//end of main
/****************************************************************************************************************************/

