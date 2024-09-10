#include "graph.h"

void duplicateCircuit(NODE* graph, int num) {
	int node_pointer_old, node_pointer_new;

	// TODO: Implement this function
}

void readBench(NODE* graph, FILE* bench) {
	// read the isc file line by line
	char line[IscLineLen];
	int line_id = 0;
	int tot = 0;
	int node_id = 0;

	while (fgets(line, sizeof(line), bench)) {
		graph = LineToGate(line, graph, &node_id, &tot);
	}
	printf("Total: %d\n", tot);

	//Return the Maximum node of the Isc file
	return tot;
}

NODE *LineToGate(char *line, NODE *Node, int *node_id_ptr, int *tot) {
	int node_id_temp;
	// skip comment lines
	if (line[0] == '\n' || line[0] == '#') {
		return Node;
	} else if (line[0] == 'I') {
		node_id_temp = atoi(extractParenthesis(line));
		Node[node_id_temp].Type = INPT;
	} else if (line[0] == 'O') {
		node_id_temp = atoi(extractParenthesis(line));
		Node[node_id_temp].Type = OUTPUT;
	} else {
		int fout = extractFout(line);
		char *name = extractName(line);
		char *fin = extractParenthesis(line);
	}

	return Node;
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
