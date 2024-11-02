#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/
void main(int argc, char** argv)
{
    FILE* fbench, * fvec, * ffau, * fres;             //file pointers used for .isc file, .vec file, .faults file and resultfile
    int Max, Opt, Npi, Npo, Total, Tfs, orig_max;              //maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults

    NODE graph[Mnod], new_graph[Mnod];                         //structure used to store the ckt information in .isc file 
    //PATTERN vector[Mpt];                      //structure used to store the input vectors information in .vec file 

    //FAULT stuck[Mft];                      //structure used to store the faults information in .faults file
    int a, b, c, d;                             //random variables
    char filename1[Mfnam];                     //filename variable used to hold primary file name
    char filename2[Mfnam];                     //filename variable used to hold secondary file name

    char* out_vec;                             //output vector

    //Read the .isc file and store the information in graph structure
    // fisc=fopen(argv[1],"r");                           //file pointer to open .isc file 
    // Max=0; 
    // Max=ReadIsc(fisc,graph);                 //read .isc file and return index of last node in graph formed
    // fclose(fisc);                            //close file pointer for .isc file
    // PrintCircuit(graph,Max);                 //print all members of graph structure

    // clean the outputs directory
    system("rm -rf out/*");

    // Read the .bench file and store the information in graph structure
    sprintf(filename1, "bench/%s.bench", argv[1]);
    fbench = fopen(filename1, "r");                           //file pointer to open .bench file
    orig_max = readBench(graph, fbench);                //read .isc file and return index of last node in graph formed
    fclose(fbench);                            //close file pointer for .isc file

    PrintCircuit(graph, orig_max);                 //print all members of graph structure

    Max = duplicateCircuit(graph, new_graph, orig_max);
    PrintCircuit(new_graph, Max);                 //print all members of graph structure

    // sprintf(filename1, "out/%s.bench", argv[1]);
    // fbench = fopen(filename1, "w");
    // writeBench(new_graph, fbench, Max);

    // sprintf(filename1, "out/%s.fault", argv[1]);
    // writeFaultFile(Max, filename1);

    // sprintf(filename1, "out/%s", argv[1]);
    // writeAllErrors(new_graph, Max, orig_max, filename1);
    // runATALANTABatch(filename1);

    // sprintf(filename1, "out/%s/", argv[1]);
    // sprintf(filename2, "out/%s_g1.pattern", argv[1]);
    // writePatterns(filename1, 500, 1, filename2);

    // sprintf(filename2, "out/%s_g2.pattern", argv[1]);
    // writePatterns(filename1, 500, 2, filename2);

    // sprintf(filename2, "out/%s_g3.pattern", argv[1]);
    // writePatterns(filename1, 500, 3, filename2);

    // sprintf(filename2, "out/%s_g4.pattern", argv[1]);
    // writePatterns(filename1, 500, 4, filename2);

    out_vec = LogicSim(graph, orig_max, "1111x");
    printf("Output: %s\n", out_vec);

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

