#include "graph.h"

int duplicateCircuit(NODE* graph, NODE* new_graph, int tot) {
	int node_pointer_old, node_pointer_new, new_1, new_2;
	LIST* fin;
	LIST* fot;

	node_pointer_new = 0;

	for (int i = 0; i < Mnod; i++) {
		InitializeCircuit(new_graph, i);
	}

	for (node_pointer_old = 0; node_pointer_old <= tot; node_pointer_old++) {
		if (node_pointer_new < node_pointer_old) node_pointer_new = node_pointer_old;

		if (graph[node_pointer_old].Type == 0) {
			continue;
		} else if (graph[node_pointer_old].Type == INPT) {
			new_graph[node_pointer_old].Type = INPT;
		} else {
			copyNode(&new_graph[node_pointer_new], &graph[node_pointer_old], node_pointer_old);
			node_pointer_new++;
			copyNode(&new_graph[node_pointer_new], &graph[node_pointer_old], node_pointer_old);
			node_pointer_new++;
		}
	}

	for (int i = 0; i <= tot; i++) {
		if (graph[i].Type == INPT) {
			fot = graph[i].Fot;

			while (fot != NULL) {
				if (graph[fot->id].Type > INPT) {
					InsertList(&new_graph[i].Fot, mapNewtoOld(new_graph, fot->id, node_pointer_new, 0));
					InsertList(&new_graph[i].Fot, mapNewtoOld(new_graph, fot->id, node_pointer_new, 1));
				}

				new_graph[i].Nfo++;
				new_graph[i].Nfo++;
				fot = fot->next;
			}

		} else if (graph[i].Type > INPT) {
			new_1 = mapNewtoOld(new_graph, i, node_pointer_new, 0);
			new_2 = mapNewtoOld(new_graph, i, node_pointer_new, 1);

			fin = graph[i].Fin;

			while (fin != NULL) {
				if (graph[fin->id].Type == INPT) {
					InsertList(&new_graph[new_1].Fin, fin->id);
					InsertList(&new_graph[new_2].Fin, fin->id);
				} else if (graph[fin->id].Type > INPT) {
					InsertList(&new_graph[new_1].Fin, mapNewtoOld(new_graph, fin->id, node_pointer_new, 0));
					InsertList(&new_graph[new_2].Fin, mapNewtoOld(new_graph, fin->id, node_pointer_new, 1));
				}

				fin = fin->next;
			}

			fot = graph[i].Fot;

			while (fot != NULL) {
				if (graph[fot->id].Type > INPT) {
					InsertList(&new_graph[new_1].Fot, mapNewtoOld(new_graph, fot->id, node_pointer_new, 0));
					InsertList(&new_graph[new_2].Fot, mapNewtoOld(new_graph, fot->id, node_pointer_new, 1));
				}

				fot = fot->next;
			}
		}
	}
	printf("Total: %d\n", node_pointer_new);
	node_pointer_new = insertComparator(new_graph, node_pointer_new);
	printf("Total: %d\n", node_pointer_new);
	return node_pointer_new;
}

int insertComparator(NODE* graph, int tot) {
	int i = 0;
	int new_node_pointer = tot + 1;
	LIST* or_fin;

	or_fin = NULL;

	while (i <= tot) {
		if (graph[i].Po > 0) {
			// No need to initialize since we initialized MNod nodes in the previous function
			InsertList(&graph[new_node_pointer].Fin, i);
			InsertList(&graph[new_node_pointer].Fin, i + 1);
			InsertList(&graph[i].Fot, new_node_pointer);
			InsertList(&graph[i + 1].Fot, new_node_pointer);
			InsertList(&or_fin, new_node_pointer);

			graph[new_node_pointer].Type = XOR;
			graph[new_node_pointer].Nfi = 2;
			graph[new_node_pointer].Nfo = 1;

			graph[i].Po = 0;
			graph[i].Nfo++;
			graph[i + 1].Po = 0;
			graph[i + 1].Nfo++;

			new_node_pointer++;
			i += 2;
		} else {
			i++;
		}
	}

	graph[new_node_pointer].Type = OR;
	graph[new_node_pointer].Po = 1;

	while (or_fin != NULL) {
		InsertList(&graph[new_node_pointer].Fin, or_fin->id);
		InsertList(&graph[or_fin->id].Fot, new_node_pointer);
		graph[new_node_pointer].Nfi++;
		or_fin = or_fin->next;
	}

	return new_node_pointer;
}

int mapNewtoOld(NODE* graph, int new_id, int limit, int skip) {
	for (int i = 0; i <= limit; i++) {
		if (graph[i].Mark == new_id) {
			if (skip > 0) {
				skip--;
			} else {
				return i;
			}
		}
	}

	return -1;
}

void copyNode(NODE* dest, NODE* src, int src_id) {
	dest->Type = src->Type;
	dest->Po = src->Po;
	dest->Nfi = src->Nfi;
	dest->Nfo = src->Nfo;
	dest->Mark = src_id;
	dest->Cval = src->Cval;
	dest->Fval = src->Fval;
}

int readBench(NODE* graph, FILE* bench) {
	// read the isc file line by line
	char line[IscLineLen];
	int line_id = 0;
	int total = 0;
	int* tot = &total;
	int node_id = 0;
	int i;

	for (i = 0;i < Mnod;i++) {
		InitializeCircuit(graph, i);
	}

	while (fgets(line, sizeof(line), bench)) {
		printf("%s", line);
		LineToGate(line, graph, &node_id, tot);
	}
	printf("Total: %d\n", *tot);

	//Return the Maximum node of the Isc file
	return *tot;
}

void LineToGate(char* line, NODE* Node, int* node_id_ptr, int* tot) {
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
		Node[nid_tmp].Po = 1;
		if (nid_tmp > *tot) *tot = nid_tmp;

	} else {
		// Handle gates
		int fout = extractFout(line);
		char* name = extractName(line);
		char* fin = extractParenthesis(line);

		if (fout > *tot) *tot = fout;

		// Assign the type of the gate
		Node[fout].Type = AssignType(name);

		// Assign the fanin list
		char* token = strtok(fin, ",");
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

char* extractParenthesis(char* line) {
	char* start = strchr(line, '(');
	char* end = strchr(line, ')');

	if (start != NULL && end != NULL && end > start) {
		size_t length = end - start - 1;
		char* result = (char*)malloc(length + 1);
		if (result != NULL) {
			strncpy(result, start + 1, length);
			result[length] = '\0';
		}
		return result;
	}

	return NULL;
}//end of ExtractParenthesis

char* extractName(char* line) {
	char* start = strchr(line, '=');
	char* end = strchr(line, '(');

	if (start != NULL && end != NULL && end > start) {
		start++; // Move past the '=' character
		while (*start == ' ') start++; // Trim leading whitespaces

		size_t length = end - start;
		char* result = (char*)malloc(length + 1);
		if (result != NULL) {
			strncpy(result, start, length);
			result[length] = '\0';
		}
		return result;
	}

	return NULL;
}//end of ExtractName

int extractFout(char* line) {
	char* start = line;
	char* end = strchr(line, ' ');

	size_t length = end - start;
	char* result = (char*)malloc(length + 1);
	if (result != NULL) {
		strncpy(result, line, length);
		result[length] = '\0';
		int fout = atoi(result);
		free(result);
		return fout;
	}

	return -1; // Return an error code if allocation fails
}//end of ExtractFout

void writeBench(NODE* graph, FILE* bench, int max) {
	fprintf(bench, "#\n#\n#\n\n");

	// Write the primary inputs
	for (int i = 0; i <= max; i++) {
		if (graph[i].Type == INPT) {
			fprintf(bench, "INPUT(%d)\n", i);
		}
	}

	fprintf(bench, "\n");

	// Write the primary outputs
	for (int i = 0; i <= max; i++) {
		if (graph[i].Po == 1) {
			fprintf(bench, "OUTPUT(%d)\n", i);
		}
	}

	fprintf(bench, "\n");

	// Write the gates
	char* gate_type;
	for (int i = 0; i <= max; i++) {
		if (graph[i].Type > 1) {
			gate_type = invertType(graph[i].Type);

			fprintf(bench, "%d = %s(", i, gate_type);

			LIST* fin = graph[i].Fin;
			while (fin != NULL) {
				fprintf(bench, "%d", fin->id);
				fin = fin->next;
				if (fin != NULL) {
					fprintf(bench, ",");
				}
			}

			fprintf(bench, ")\n");
		}
	}
}

void injectError(NODE* graph, int node_id, int error, int tot) {
	int error_node_id = mapNewtoOld(graph, node_id, tot, 1);
	graph[error_node_id].Type = error;
}

void writeAllErrors(NODE* graph, int tot, int error_limit, char prefix[]) {
	int i, orig_type, fi_count;
	char filename[256];
	FILE* fbenchOut;

	for (int i = 0; i <= error_limit; i++) {
		if (graph[i].Type > INPT && mapNewtoOld(graph, i, tot, 0) > 0) {
			// mapnewtoold returns -1 if the node is not in the original circuit
			orig_type = graph[i].Type;
			fi_count = graph[i].Nfi;

			if (fi_count == 1) {
				for (int j = BUFF; j <= NOT; j++) {
					if (orig_type != j) {
						sprintf(filename, "%s_%d_to_%s.bench", prefix, i, invertType(j));
						fbenchOut = fopen(filename, "w");

						injectError(graph, i, j, tot);
						writeBench(graph, fbenchOut, tot);
						injectError(graph, i, orig_type, tot);

						fclose(fbenchOut);
					}

				}
			} else if (fi_count >= 2) {
				for (int j = AND; j <= XNOR; j++) {
					if (orig_type != j) {
						sprintf(filename, "%s_%d_to_%s.bench", prefix, i, invertType(j));
						fbenchOut = fopen(filename, "w");

						injectError(graph, i, j, tot);
						writeBench(graph, fbenchOut, tot);
						injectError(graph, i, orig_type, tot);

						fclose(fbenchOut);
					}

				}
			}
		}
	}
}
