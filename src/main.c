#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/
void main(int argc, char** argv)
{
    FILE* fbench, * fvec, * ffau, * fres;             //file pointers used for .isc file, .vec file, .faults file and resultfile
    int dupli_max, Opt, Npi, Npo, Total, Tfs, orig_max, group;              //maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults

    NODE graph[Mnod], new_graph[Mnod];                         //structure used to store the ckt information in .isc file
    int a, b, c, d;                             //random variables
    char filename1[Mfnam];                     //filename variable used to hold primary file name
    char filename2[Mfnam];                     //filename variable used to hold secondary file name

    char* out_vec;                             //output vector
    char* pattern_list[Mpt];                   //pattern list

    // argv settings
    group = (argc > 2) ? atoi(argv[2]) : 1; // Default value is 1 if argv[2] is not present

    // clean the outputs directory
    system("rm -rf out/*");

    // Read the .bench file and store the information in graph structure
    sprintf(filename1, "bench/%s.bench", argv[1]);
    fbench = fopen(filename1, "r");                           //file pointer to open .bench file
    orig_max = readBench(graph, fbench);                //read .isc file and return index of last node in graph formed
    fclose(fbench);                            //close file pointer for .isc file

    // Print the original circuit
    PrintCircuit(graph, orig_max);                 //print all members of graph structure

    // Duplicate the circuit and print it
    dupli_max = duplicateCircuit(graph, new_graph, orig_max);
    PrintCircuit(new_graph, dupli_max);                 //print all members of graph structure

    // Write the .bench file for the duplicated circuit
    sprintf(filename1, "out/%s.bench", argv[1]);
    fbench = fopen(filename1, "w");
    writeBench(new_graph, fbench, dupli_max);

    // Write the .vec file for the duplicated circuit
    sprintf(filename1, "out/%s.fault", argv[1]);
    writeFaultFile(dupli_max, filename1);

    // Write all faulty circuits and run ATALANTA
    sprintf(filename1, "out/%s", argv[1]);
    writeAllErrors(new_graph, dupli_max, orig_max, filename1);
    runATALANTABatch(filename1);

    // Pick pattern groups of 1
    sprintf(filename1, "out/%s/", argv[1]);
    sprintf(filename2, "out/%s_g1.pattern", argv[1]);
    writePatterns(filename1, 500, 1, filename2);

    // Pick pattern groups of 2
    sprintf(filename2, "out/%s_g2.pattern", argv[1]);
    writePatterns(filename1, 500, 2, filename2);

    // Pick pattern groups of 3
    sprintf(filename2, "out/%s_g3.pattern", argv[1]);
    writePatterns(filename1, 500, 3, filename2);

    // Pick pattern groups of 4
    sprintf(filename2, "out/%s_g4.pattern", argv[1]);
    writePatterns(filename1, 500, 4, filename2);

    // Select unique patterns from group and run all faults
    getUniquePatterns(argv[1], pattern_list, group);
    runAllFaults(graph, orig_max, pattern_list, argv[1]);           //close file pointer for .out file

    ClearCircuit(graph, Mnod);                                      //clear memeory for all members of graph
    return;
}//end of main
/****************************************************************************************************************************/

