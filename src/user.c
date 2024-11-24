#include "graph.h"
#include <dirent.h>

int andGate[5][5] = {
	{0, 0, 0, 0, 0},
	{0, 1, 2, 3, 4},
	{0, 2, 2, 2, 2},
	{0, 3, 2, 3, 0},
	{0, 4, 2, 0, 4}
};

int orGate[5][5] = {
	{0, 1, 2, 3, 4},
	{1, 1, 1, 1, 1},
	{2, 1, 2, 2, 2},
	{3, 1, 2, 3, 1},
	{4, 1, 2, 1, 4}
};

int notGate[5] = { 1, 0, 2, 4, 3 };

int duplicateCircuit(NODE* graph, NODE* new_graph, int tot) {
	int node_pointer_old, node_pointer_new, new_1, new_2, i;
	LIST* fin;
	LIST* fot;

	node_pointer_new = 0;

	for (i = 0; i < Mnod; i++) {
		InitializeCircuit(new_graph, i);
	}

	for (node_pointer_old = 0; node_pointer_old <= tot; node_pointer_old++) {
		if (node_pointer_new < node_pointer_old) node_pointer_new = node_pointer_old;

		if (graph[node_pointer_old].Type == 0) {
			continue;
		} else if (graph[node_pointer_old].Type == INPT) {
			copyNode(&new_graph[node_pointer_new], &graph[node_pointer_old], node_pointer_old);
		} else {
			copyNode(&new_graph[node_pointer_new], &graph[node_pointer_old], node_pointer_old);
			node_pointer_new++;
			copyNode(&new_graph[node_pointer_new], &graph[node_pointer_old], node_pointer_old);
			node_pointer_new++;
		}
	}

	for (i = 0; i <= tot; i++) {
		if (graph[i].Type == INPT) {
			fot = graph[i].Fot;

			while (fot != NULL) {
				if (graph[fot->id].Type > INPT) {
					InsertList(&new_graph[i].Fot, mapOldtoNew(new_graph, fot->id, node_pointer_new, 0));
					InsertList(&new_graph[i].Fot, mapOldtoNew(new_graph, fot->id, node_pointer_new, 1));
				}

				new_graph[i].Nfo++;
				new_graph[i].Nfo++;
				fot = fot->next;
			}

		} else if (graph[i].Type > INPT) {
			new_1 = mapOldtoNew(new_graph, i, node_pointer_new, 0);
			new_2 = mapOldtoNew(new_graph, i, node_pointer_new, 1);

			fin = graph[i].Fin;

			while (fin != NULL) {
				if (graph[fin->id].Type == INPT) {
					InsertList(&new_graph[new_1].Fin, fin->id);
					InsertList(&new_graph[new_2].Fin, fin->id);
				} else if (graph[fin->id].Type > INPT) {
					InsertList(&new_graph[new_1].Fin, mapOldtoNew(new_graph, fin->id, node_pointer_new, 0));
					InsertList(&new_graph[new_2].Fin, mapOldtoNew(new_graph, fin->id, node_pointer_new, 1));
				}

				fin = fin->next;
			}

			fot = graph[i].Fot;

			while (fot != NULL) {
				if (graph[fot->id].Type > INPT) {
					InsertList(&new_graph[new_1].Fot, mapOldtoNew(new_graph, fot->id, node_pointer_new, 0));
					InsertList(&new_graph[new_2].Fot, mapOldtoNew(new_graph, fot->id, node_pointer_new, 1));
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
	int i;
	int new_node_pointer = tot + 1;
	LIST* or_fin;

	or_fin = NULL;
	i = 0;

	while (i <= tot) {
		if (graph[i].Po > 0) {
			// No need to initialize since we initialized MNod nodes in the previous function
			InsertList(&graph[new_node_pointer].Fin, i);
			InsertList(&graph[i].Fot, new_node_pointer);
			graph[i].Po = 0;
			graph[i].Nfo++;

			if (graph[i].Type > INPT) {
				InsertList(&graph[new_node_pointer].Fin, i + 1);
				InsertList(&graph[i + 1].Fot, new_node_pointer);
				graph[i + 1].Po = 0;
				graph[i + 1].Nfo++;

				graph[new_node_pointer].Nfi = 2;
			} else {
				graph[new_node_pointer].Nfi = 1;
			}

			InsertList(&or_fin, new_node_pointer);

			graph[new_node_pointer].Type = XOR;
			graph[new_node_pointer].Nfo = 1;


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

int mapOldtoNew(NODE* graph, int old_id, int limit, int skip) {
	int i;

	for (i = 0; i <= limit; i++) {
		if (graph[i].Mark == old_id) {
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
		Node[nid_tmp].Type = INPT; // To handle direct outputs
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
	int i;

	fprintf(bench, "#\n#\n#\n\n");

	// Write the primary inputs
	for (i = 0; i <= max; i++) {
		if (graph[i].Type == INPT) {
			fprintf(bench, "INPUT(%d)\n", i);
		}
	}

	fprintf(bench, "\n");

	// Write the primary outputs
	for (i = 0; i <= max; i++) {
		if (graph[i].Po == 1) {
			fprintf(bench, "OUTPUT(%d)\n", i);
		}
	}

	fprintf(bench, "\n");

	// Write the gates
	char* gate_type;
	for (i = 0; i <= max; i++) {
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

			// for 1 input XOR and XNOR gates, add a comma and repeat the first input
			if ((graph[i].Type == XOR || graph[i].Type == XNOR) && graph[i].Nfi == 1) {
				fprintf(bench, ",%d", graph[i].Fin->id);
			}

			fprintf(bench, ")\n");
		}
	}
}

void writeAllErrors(NODE* graph, int tot, int error_limit, char prefix[]) {
	int i, j, new_index, orig_type, fi_count;
	char filename[Mfnam];
	FILE* fbenchOut;

	sprintf(filename, "mkdir %s", prefix);
	system(filename);

	for (i = 0; i <= error_limit; i++) {
		new_index = mapOldtoNew(graph, i, tot, 1);

		if (new_index == -1 || graph[i].Type <= INPT) {
			continue;
		}

		// mapnewtoold returns -1 if the node is not in the original circuit
		orig_type = graph[new_index].Type;
		fi_count = graph[new_index].Nfi;

		if (fi_count == 1) {
			for (j = BUFF; j <= NOT; j++) {
				if (orig_type != j) {
					sprintf(filename, "%s/%d_to_%s.bench", prefix, i, invertType(j));
					fbenchOut = fopen(filename, "w");

					graph[new_index].Type = j;
					writeBench(graph, fbenchOut, tot);
					graph[new_index].Type = orig_type;

					fclose(fbenchOut);
				}

			}

		} else if (fi_count >= 2) {
			for (j = AND; j <= XNOR; j++) {
				// XOR and XNOR gates can only have 2 inputs due to atanlanta limitations
				if ((j == XOR || j == XNOR) && fi_count != 2) {
					continue;
				}
				if (orig_type != j) {
					sprintf(filename, "%s/%d_to_%s.bench", prefix, i, invertType(j));
					fbenchOut = fopen(filename, "w");

					graph[new_index].Type = j;
					writeBench(graph, fbenchOut, tot);
					graph[new_index].Type = orig_type;

					fclose(fbenchOut);
				}

			}
		}
	}
}

void runATALANTA(char bench[], char error[], char result[]) {
	char command[256];

	if (IS_UNIX_LAB) {
		sprintf(command, "/opt/net/apps/atalanta/atalanta -D %d -f %s -t %s %s", MAX_PATTERNS, error, result, bench);
	} else {
		sprintf(command, "/home/codespace/Atalanta/atalanta -D %d -f %s -t %s %s", MAX_PATTERNS, error, result, bench);
	}

	system(command);
}

void writeFaultFile(int end_node_id, char filename[]) {
	FILE* faultOut;
	faultOut = fopen(filename, "w");

	fprintf(faultOut, "%d /0\n", end_node_id);
	fclose(faultOut);
}

void runATALANTABatch(char prefix[]) {
	DIR* dir;
	struct dirent* entry;
	char bench_file[2 * Mfnam];
	char error_file[2 * Mfnam];
	char result_file[2 * Mfnam];

	sprintf(error_file, "%s.fault", prefix);

	if ((dir = opendir(prefix)) != NULL) {
		while ((entry = readdir(dir)) != NULL) {
			// Skip if the file extension is not .bench
			if (strstr(entry->d_name, ".bench") == NULL || strstr(entry->d_name, ".test") != NULL) {
				continue;
			}

			sprintf(bench_file, "%s/%s", prefix, entry->d_name);
			sprintf(result_file, "%s/%s.test", prefix, entry->d_name);
			runATALANTA(bench_file, error_file, result_file);
		}
		closedir(dir);
	}
}

void select_random_patterns(const char* filename, int patterns_per_fault, FILE* outfile) {
	char line[256];
	char patterns[MAX_PATTERNS][256];
	int pattern_count, i;
	char* colon_pos = strchr(line, ':');
	int selected[MAX_PATTERNS] = { 0 };
	FILE* file;

	file = fopen(filename, "r");
	pattern_count = 0;

	// Read the file and extract patterns
	while (fgets(line, sizeof(line), file)) {
		if (strstr(line, ":") && strstr(line, "x")) {
			sscanf(line, "%*d: %s %*d", patterns[pattern_count++]);
		}
	}
	fclose(file);

	// Randomly select patterns_per_fault number of patterns
	srand(time(NULL));

	if (pattern_count <= patterns_per_fault) {
		// Print all available patterns if they are less than or equal to patterns_per_fault
		for (i = 0; i < pattern_count; i++) {
			fprintf(outfile, "%s\n", patterns[i]);
		}
	} else {
		// Randomly select patterns_per_fault number of patterns without repetition
		for (i = 0; i < patterns_per_fault; i++) {
			int index;
			do {
				index = rand() % pattern_count;
			} while (selected[index]); // Repeat until an unselected pattern is found

			selected[index] = 1; // Mark the pattern as selected
			fprintf(outfile, "%s\n", patterns[index]);
		}
	}

	fprintf(outfile, "\n");
}

void writePatterns(char path_prefix[], int fault_count, int patterns_per_fault, char outfile[]) {
	DIR* dir;
	struct dirent* entry;
	char* test_files[fault_count];
	char fname[256];
	int file_count, i;

	file_count = 0;

	if ((dir = opendir(path_prefix)) != NULL) {
		while ((entry = readdir(dir)) != NULL && file_count < fault_count) {
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
				continue;
			}
			if (strstr(entry->d_name, ".test")) {
				sprintf(fname, "%s%s", path_prefix, entry->d_name);
				if (!ifPatternsExist(fname)) continue;
				test_files[file_count] = malloc(strlen(path_prefix) + strlen(entry->d_name) + 2);
				sprintf(test_files[file_count], "%s%s", path_prefix, entry->d_name);
				file_count++;
			}
		}
		closedir(dir);
	}

	srand(time(NULL));
	FILE* outfile_ptr = fopen(outfile, "w");

	for (i = 0; i < file_count; i++) {
		select_random_patterns(test_files[i], patterns_per_fault, outfile_ptr);
	}

	fclose(outfile_ptr);

	// Free allocated memory
	for (i = 0; i < file_count; i++) {
		free(test_files[i]);
	}
}

int ifPatternsExist(char* filename) {
	FILE* file;
	char line[256];

	file = fopen(filename, "r");

	while (fgets(line, sizeof(line), file)) {
		if (strstr(line, ":") && strstr(line, "x")) {
			fclose(file);
			return 1;
		}
	}
	fclose(file);
	return 0;
}

int parsePtrnChar(char ptrn_char) {
	int val;
	val = ptrn_char - '0';

	switch (val) {
	case 0: return 0;
	case 1: return 1;
	default: return 2;
	}
}

char revertPtrnChar(int ptrn_num) {
	switch (ptrn_num) {
	case 0: return '0';
	case 1: return '1';
	default: return 'x';
	}
}

char* LogicSim(NODE* graph, int Tgat, char* pattern) {
	int node_index, pi_index, po_index, node_result, ones_count;
	char* output_vector = (char*)malloc(Tgat * sizeof(char));

	NODE* curent_node = NULL;
	LIST* current_fanin = NULL;

	node_index = 0;
	pi_index = 0;
	po_index = 0;
	node_result = 0;

	while (node_index <= Tgat) {
		curent_node = &graph[node_index];
		switch (curent_node->Type) {
		case INPT:
			curent_node->Cval = parsePtrnChar(pattern[pi_index]);
			pi_index++;
			break;

		case FROM:
			curent_node->Cval = graph[curent_node->Fin->id].Cval;
			break;

		case BUFF:
			curent_node->Cval = graph[curent_node->Fin->id].Cval;
			break;

		case NOT:
			curent_node->Cval = notGate[graph[curent_node->Fin->id].Cval];
			break;

		case AND:
			node_result = 1;
			current_fanin = curent_node->Fin;

			while (current_fanin != NULL) {
				node_result = andGate[node_result][graph[current_fanin->id].Cval];
				current_fanin = current_fanin->next;
			}
			curent_node->Cval = node_result;
			break;

		case NAND:
			node_result = 1;
			current_fanin = curent_node->Fin;

			while (current_fanin != NULL) {
				node_result = andGate[node_result][graph[current_fanin->id].Cval];
				current_fanin = current_fanin->next;
			}
			curent_node->Cval = notGate[node_result];
			break;

		case OR:
			node_result = 0;
			current_fanin = curent_node->Fin;

			while (current_fanin != NULL) {
				node_result = orGate[node_result][graph[current_fanin->id].Cval];
				current_fanin = current_fanin->next;
			}
			curent_node->Cval = node_result;
			break;

		case NOR:
			node_result = 0;
			current_fanin = curent_node->Fin;

			while (current_fanin != NULL) {
				node_result = orGate[node_result][graph[current_fanin->id].Cval];
				current_fanin = current_fanin->next;
			}
			curent_node->Cval = notGate[node_result];
			break;

		case XOR:
			ones_count = 0;
			current_fanin = curent_node->Fin;

			while (current_fanin != NULL) {
				if (graph[current_fanin->id].Cval == 1) ones_count++;
				else if (graph[current_fanin->id].Cval == 2) {
					ones_count = -1;
					break;
				}
				current_fanin = current_fanin->next;
			}

			if (ones_count == -1) curent_node->Cval = 2;
			else curent_node->Cval = (ones_count % 2) ? 1 : 0;
			break;

		case XNOR:
			ones_count = 0;
			current_fanin = curent_node->Fin;

			while (current_fanin != NULL) {
				if (graph[current_fanin->id].Cval == 1) ones_count++;
				else if (graph[current_fanin->id].Cval == 2) {
					ones_count = -1;
					break;
				}
				current_fanin = current_fanin->next;
			}

			if (ones_count == -1) curent_node->Cval = 2;
			else curent_node->Cval = (ones_count % 2) ? 0 : 1;
			break;

		default:
			break;
		}
		node_index++;
	}

	for (node_index = 0; node_index <= Tgat; node_index++) {
		if (graph[node_index].Po == 1) {
			output_vector[po_index] = revertPtrnChar(graph[node_index].Cval);
			po_index++;
		}
	}
	output_vector[po_index] = '\0';

	return output_vector;
}

void getUniquePatterns(char* prefix, char* pattern_list[], int group) {
	char filename[Mfnam];
	sprintf(filename, "out/%s_g%d.pattern", prefix, group);

	FILE* fp = fopen(filename, "r");
	char line[Mlin];
	int pattern_index, duplicate;

	pattern_index = 0;
	duplicate = 0;

	while (fgets(line, sizeof(line), fp) && pattern_index < Mpt) {
		if (line[0] == '\n' || line[0] == '\0') {
			continue;
		}

		for (int i = 0; i < pattern_index; i++) {
			if (strcmp(pattern_list[i], line) == 0) {
				duplicate = 1;
				break;
			}
		}

		if (duplicate) {
			duplicate = 0;
			continue;
		}

		line[strlen(line) - 1] = '\0';
		pattern_list[pattern_index] = malloc(Mpi * sizeof(char));
		strcpy(pattern_list[pattern_index], line);
		pattern_index++;
	}
}

void modifyType(NODE* graph, int node_id, int fault_type) {
	graph[node_id].Type = fault_type;
}

void runAllFaultsold(NODE* graph, int max, char* pattern_list[], char* prefix) {
	int node_id, fault_type, pattern_index, original_type;
	char* sim_result;
	char filename[Mfnam];
	FILE* fp;

	sprintf(filename, "out/%s_all.result", prefix);
	fp = fopen(filename, "w");

	for (node_id = 0; node_id <= max; node_id++) {
		if (graph[node_id].Type > INPT && graph[node_id].Type < BUFF) {
			for (fault_type = AND; fault_type <= XNOR; fault_type++) {
				original_type = graph[node_id].Type;
				if (original_type == fault_type) {
					continue;
				}

				modifyType(graph, node_id, fault_type);
				for (pattern_index = 0; pattern_index < Mpt; pattern_index++) {
					if (pattern_list[pattern_index] == NULL) break;

					sim_result = LogicSim(graph, max, pattern_list[pattern_index]);
					fprintf(
						fp,
						"Node: %d, Fault: %s, Pattern: %s -> %s\n",
						node_id,
						invertType(fault_type),
						pattern_list[pattern_index],
						sim_result
					);
				}
				modifyType(graph, node_id, original_type);
			}

		} else if (graph[node_id].Type == BUFF || graph[node_id].Type == NOT) {
			for (fault_type = BUFF; fault_type <= NOT; fault_type++) {
				original_type = graph[node_id].Type;
				if (original_type == fault_type) {
					continue;
				}

				modifyType(graph, node_id, fault_type);
				for (pattern_index = 0; pattern_index < Mpt; pattern_index++) {
					if (pattern_list[pattern_index] == NULL) break;

					sim_result = LogicSim(graph, max, pattern_list[pattern_index]);
					fprintf(
						fp,
						"Node: %d, Fault: %s, Pattern: %s -> %s\n",
						node_id,
						invertType(fault_type),
						pattern_list[pattern_index],
						sim_result
					);
				}
				modifyType(graph, node_id, original_type);
			}
		}
	}
}

int detectedAtPo(int ff_val, int f_val) {
	if (ff_val == 48 && f_val == 49) {
		return 1;
	} else if (ff_val == 49 && f_val == 48) {
		return 1;
	} else {
		return 0;
	}
}

void runAllFaults(NODE* graph, int max, char* pattern_list[], char* prefix) {
	int node_id, fault_type, pattern_index, original_type, po_index;
	char* sim_result, * ff_output;
	char filename[Mfnam];
	int po_count;
	FILE* fp;

	po_count = 0;

	for (node_id = 0; node_id <= max; node_id++) {
		if (graph[node_id].Po == 1) po_count++;
	}

	char** fault_lists;

	fault_lists = (char**)malloc(FLIST_LINE_LEN * sizeof(char*));
	for (po_index = 0; po_index < po_count; po_index++) {
		fault_lists[po_index] = (char*)malloc((max + 1) * sizeof(char*));
		for (int node_id = 0; node_id <= max; node_id++) {
			fault_lists[po_index] = NULL;
		}
	}

	sprintf(filename, "out/%s_all.result", prefix);
	fp = fopen(filename, "w");

	for (pattern_index = 0; pattern_index < Mpt; pattern_index++) {
		if (pattern_list[pattern_index] == NULL) break;

		ff_output = LogicSim(graph, max, pattern_list[pattern_index]);
		fprintf(fp, "%s%s%s\n", BEGIN_PATTERN, pattern_list[pattern_index], DELIMITER);

		for (node_id = 0; node_id <= max; node_id++) {
			if (graph[node_id].Type > INPT && graph[node_id].Type < BUFF) {
				for (fault_type = AND; fault_type <= XNOR; fault_type++) {
					original_type = graph[node_id].Type;
					if (original_type == fault_type) continue;

					modifyType(graph, node_id, fault_type);
					sim_result = LogicSim(graph, max, pattern_list[pattern_index]);
					modifyType(graph, node_id, original_type);

					for (po_index = 0; po_index < po_count; po_index++) {
						if (detectedAtPo(ff_output[po_index], sim_result[po_index])) {
							if (fault_lists[po_index] == NULL) {
								fault_lists[po_index] = (char*)malloc(FLIST_LINE_LEN * sizeof(char));
								sprintf(
									fault_lists[po_index],
									"N%d_F%d%s\n",
									node_id,
									fault_type,
									DELIMITER
								);
							} else {
								sprintf(
									fault_lists[po_index],
									"%sN%d_F%d%s\n",
									fault_lists[po_index],
									node_id,
									fault_type,
									DELIMITER
								);
							}
						}
					}
				}
			} else if (graph[node_id].Type == BUFF || graph[node_id].Type == NOT) {
				for (fault_type = BUFF; fault_type <= NOT; fault_type++) {
					original_type = graph[node_id].Type;
					if (original_type == fault_type) continue;

					modifyType(graph, node_id, fault_type);
					sim_result = LogicSim(graph, max, pattern_list[pattern_index]);
					modifyType(graph, node_id, original_type);

					for (po_index = 0; po_index < po_count; po_index++) {
						if (detectedAtPo(ff_output[po_index], sim_result[po_index])) {
							if (fault_lists[po_index] == NULL) {
								fault_lists[po_index] = (char*)malloc(FLIST_LINE_LEN * sizeof(char));
								sprintf(
									fault_lists[po_index],
									"N%d_F%d%s\n",
									node_id,
									fault_type,
									DELIMITER
								);
							} else {
								sprintf(
									fault_lists[po_index],
									"%sN%d_F%d%s\n",
									fault_lists[po_index],
									node_id,
									fault_type,
									DELIMITER
								);
							}
						}
					}
				}
			}
		}

		for (po_index = 0; po_index < po_count; po_index++) {
			fprintf(fp, "%s%d%s:\n", BEGIN_PO, po_index, DELIMITER);
			for (int node_id = 0; node_id < max; node_id++) {
				if (fault_lists[po_index] != NULL) {
					fprintf(fp, "%s", fault_lists[po_index]);
					fault_lists[po_index] = NULL;
				}
			}
			fprintf(fp, "%s\n", END_PO);
		}

		fprintf(fp, "%s\n", END_PATTERN);
	}
}
