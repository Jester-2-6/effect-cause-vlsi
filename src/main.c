#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/
void main(int argc, char** argv)
{
    FILE* fisc, * fvec, * ffau, * fres;             //file pointers used for .isc file, .vec file, .faults file and resultfile
    int Max, Opt, Npi, Npo, Total, Tfs, orig_max;              //maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults

    NODE graph[Mnod], new_graph[Mnod];                         //structure used to store the ckt information in .isc file 
    //PATTERN vector[Mpt];                      //structure used to store the input vectors information in .vec file 

    //FAULT stuck[Mft];                      //structure used to store the faults information in .faults file
    int a, b, c, d;                             //random variables
    char filename[Mfnam];                     //filename used to store the name of the file


    //Read the .isc file and store the information in graph structure
    // fisc=fopen(argv[1],"r");                           //file pointer to open .isc file 
    // Max=0; 
    // Max=ReadIsc(fisc,graph);                 //read .isc file and return index of last node in graph formed
    // fclose(fisc);                            //close file pointer for .isc file
    // PrintCircuit(graph,Max);                 //print all members of graph structure

    // Read the .bench file and store the information in graph structure
    sprintf(filename, "../bench/%s.bench", argv[1]);
    fisc = fopen(filename, "r");                           //file pointer to open .bench file
    orig_max = readBench(graph, fisc);                //read .isc file and return index of last node in graph formed
    fclose(fisc);                            //close file pointer for .isc file

    PrintCircuit(graph, orig_max);                 //print all members of graph structure

    Max = duplicateCircuit(graph, new_graph, orig_max);
    PrintCircuit(new_graph, Max);                 //print all members of graph structure

    sprintf(filename, "../out/%s.fault", argv[1]);
    writeFaultFile(Max, filename);

    sprintf(filename, "../out/%s", argv[1]);
    writeAllErrors(new_graph, Max, orig_max, filename);
    runATALANTABatch(filename);

    //Read the .vec file and store the information in  vector structure

    //Opt=0; 
    //Opt=atoi(argv[3]);                          //getting the option from terminal for xval
    //fres=fopen(argv[2],"w");                           //file pointer to open .out file for printing results

    //Perform Logic Simulation for each Input vector and print the Pos .val in output file   

    //fclose(fres);                                                  //close file pointer for .out file

    ClearCircuit(graph, Mnod);                                      //clear memeory for all members of graph
    //for(a=0;a<Total;a++){ bzero(vector[a].piv,Mpi); }                //clear memeory for all members of vector
    return;
}//end of main
/****************************************************************************************************************************/

