/***************************************************************************************************************************
Header Files
****************************************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <time.h>
#include <complex.h>
#include <assert.h>
#include <limits.h>
/***************************************************************************************************************************
Constant Declarations
****************************************************************************************************************************/
// VARIOUS CONSTANTS
#define Mfnam     200			// max size for a file name
#define Mnod    15000 		        // max number of nodes in a graph/node
#define Mlin      250			// max size of characters in a line
#define Mnam       25			// max size of a node name
#define Mtyp       10			// max type of nodes/gates
#define Mout       16		        // max node out degree (Nfo)
#define Min         9			// max node in degree (Nfi)
#define Mpi       233			// max number of primary inputs
#define Mpo       140			// max number of primary outputs
#define Mpt       500			// max number of input patterns in .vec file
#define Mft       10			// max number of stuck at faults in .faults file
#define Mfl       1000			// max number of fault lists read at a time
#define Mfpl      1000			// max number of faults per list
#define Mchf      50			// max number of characters in a fault name
#define Mflr      500			// max number of faults to check resolution
#define Mrd       250			// max number of retries for random fault picking
#define Muf       1000			// max number of unique faults
#define Mres      1000			// max resolution

// NODE TYPE CONSTANTS 
#define INPT 1				// Primary Input
#define AND  2				// AND 
#define NAND 3				// NAND 
#define OR   4				// OR 
#define NOR  5				// NOR 
#define XOR  6				// XOR 
#define XNOR 7				// XNOR 
#define BUFF 8				// BUFFER 
#define NOT  9				// INVERTER 
#define FROM 10				// STEM BRANCH

// User Defined Constants
#define TokLen      10       // max length of token
#define IscLineLen  100			// max length of bench file line
#define MAX_PATTERNS 100
#define IS_UNIX_LAB 0
#define FLIST_LINE_LEN 200

// File reading flags
#define BEGIN_PATTERN "Begin Pattern |"
#define END_PATTERN "End Pattern\n"
#define BEGIN_PO "Begin PO |"
#define END_PO "End PO\n"
#define DELIMITER '|'
/***************************************************************************************************************************
Structure Declarations
****************************************************************************************************************************/
//1.Stucture declaration for LIST
typedef struct LIST_type {
  int  id;		   //id for current element		
  struct LIST_type* next;  //pointer to next id element( if there is no element, then it will be NULL)		
} LIST;
//2.Stucture declaration for NODE
typedef struct NODE_type {
  char Name[Mnam];                      //name of the node
  int Type, Nfi, Nfo, Po;                  //type, nooffanins, nooffanouts,primaryo/p
  int Mark, Cval, Fval;                    //marker,correctvalue,faultvalue
  LIST* Fin, * Fot;                      //fanin members, fanout members 
} NODE;
//3.Stucture declaration for PATTERN




//4.Stucture declaration for FAULT




/***************************************************************************************************************************
Functions in given.c
****************************************************************************************************************************/
/***************************************************************************************************************************
LIST Structure Functions
****************************************************************************************************************************/
void InsertList(LIST**, int);
void PrintList(LIST*);
void FreeList(LIST**);
/***************************************************************************************************************************
 NODE Structure Functions
****************************************************************************************************************************/
int ReadIsc(FILE*, NODE*);
void InitializeCircuit(NODE*, int);
int AssignType(char*);
char* invertType(int);
void PrintCircuit(NODE*, int);
void ClearCircuit(NODE*, int);
/***************************************************************************************************************************
 PATTERN Structure Functions
****************************************************************************************************************************/

/***************************************************************************************************************************
User Defined Functions in user.c
****************************************************************************************************************************/
int duplicateCircuit(NODE*, NODE*, int);
void copyNode(NODE*, NODE*, int);
int insertComparator(NODE*, int);
int mapOldtoNew(NODE*, int, int, int);
int readBench(NODE*, FILE*);
void LineToGate(char*, NODE*, int*, int*);
char* extractParenthesis(char*);
char* extractName(char*);
int extractFout(char*);
void writeBench(NODE*, FILE*, int);
void writeAllErrors(NODE*, int, int, char[]);
void runATALANTA(char[], char[], char[]);
void writeFaultFile(int, char[]);
void runATALANTABatch(char[]);
void writePatterns(char[], int, int, char[]);
int ifPatternsExist(char*);
/****************************************************************************************************************************/
/**
 * Simulation related functions
 */
char* LogicSim(NODE*, int, char*);
void getUniquePatterns(char*, char* [], int);
void modifyType(NODE*, int, int);
void runAllFaults(NODE*, int, char* [], char*);
int parsePtrnChar(char);
char revertPtrnChar(int);
/****************************************************************************************************************************/
/**
 * Resolution related functions
 */
void reportResolutions(NODE*, int, int, char*);
int validFaultLists(char**, char**, char*, int);
void readSequence(char[], char[], char[]);
int markLists(char**, int*, char*, char**, char**);
void pickRandomFault(char**, char*, char*, int);
int buildUniqueFaultList(char**, char**, int);
int findCommonFaults(char**, int);
void dropFaults(char**, char**, char**, int, int);
