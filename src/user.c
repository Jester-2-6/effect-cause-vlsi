#include "graph.h"

void duplicateCircuit(NODE* graph, int num) {
	int node_pointer_old, node_pointer_new;

	// TODO: Implement this function
}

int readBench(NODE* graph, FILE* bench) {
	// read the isc file line by line
	char line[IscLineLen];
	int line_id = 0;
	int total = 0;
	int *tot = &total;
	int node_id = 0;
	int i;

	for(i=0;i<Mnod;i++) { 
		InitializeCircuit(graph,i); 
	} 

	while (fgets(line, sizeof(line), bench)) {
		LineToGate(line, graph, &node_id, tot);
	}
	printf("Total: %d\n", *tot);

	//Return the Maximum node of the Isc file
	return *tot;
}

void LineToGate(char *line, NODE *Node, int *node_id_ptr, int *tot) {
	int nid_tmp;
	// skip comment lines
	if (line[0] == '\n' || line[0] == '#') {
		return;
	} else if (line[0] == 'I') {
		// Handle inputs
		nid_tmp = atoi(extractParenthesis(line));
		Node[nid_tmp].Type = INPT;
		if (nid_tmp > *tot) *tot = nid_tmp;
	} else if (line[0] == 'O') {
		// Handle outputs
		nid_tmp = atoi(extractParenthesis(line));
		Node[nid_tmp].Type = OUTPUT;
		if (nid_tmp > *tot) *tot = nid_tmp;
	} else {
		// Handle gates
		int fout = extractFout(line);
		char *name = extractName(line);
		char *fin = extractParenthesis(line);

		if (fout > *tot) *tot = fout;
		
		// Assign the type of the gate
		Node[fout].Type = AssignType(name);

		// Assign the fanin list
		char *token = strtok(fin, ",");
		while (token != NULL) {
			int fin_id = atoi(token);
			
			InsertList(&Node[fout].Fin, fin_id);
			InsertList(&Node[fin_id].Fot, fout);
			Node[fout].Nfi++;
			Node[fin_id].Nfo++;

			token = strtok(NULL, ",");
		}
	}
}//end of LineToGate

char *extractParenthesis(char *line) {
	char *start = strchr(line, '(');
	char *end = strchr(line, ')');

	if (start != NULL && end != NULL && end > start) {
		size_t length = end - start - 1;
		char *result = (char *)malloc(length + 1);
		if (result != NULL) {
			strncpy(result, start + 1, length);
			result[length] = '\0';
		}
		return result;
	}

	return NULL;
}//end of ExtractParenthesis

char *extractName(char *line) {
	char *start = strchr(line, '=');
	char *end = strchr(line, '(');

	if (start != NULL && end != NULL && end > start) {
		start++; // Move past the '=' character
		while (*start == ' ') start++; // Trim leading whitespaces

		size_t length = end - start;
		char *result = (char *)malloc(length + 1);
		if (result != NULL) {
			strncpy(result, start, length);
			result[length] = '\0';
		}
		return result;
	}

	return NULL;
}//end of ExtractName

int extractFout(char *line) {
	char *start = line;
	char *end = strchr(line, ' ');

	size_t length = end - start;
	char *result = (char *)malloc(length + 1);
	if (result != NULL) {
		strncpy(result, line, length);
		result[length] = '\0';
		int fout = atoi(result);
		free(result);
		return fout;
	}

	return -1; // Return an error code if allocation fails
}//end of ExtractFout
