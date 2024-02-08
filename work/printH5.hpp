
#include <string>
#include <stdlib.h>
#include "hdf5.h"
#include <cstring>
#include <vector>
#include <iostream>

using namespace std;


typedef struct h5Data {
	int dims;
	unsigned long numNodes;
	unsigned long *index;
	unsigned long **incidences;
	unsigned long **elements;
	double **coords;
} h5Data;

h5Data * newh5Data (unsigned long numNodes, int dims) {
	h5Data *h5;
	h5 = (*h5) malloc(sizeof(h5));

	h5 -> numNodes = numNodes;
	h5 -> dims = dims;

	h5 -> index = (unsigned long *) malloc(sizeof(unsigned long) * numNodes);
	for (int i = 0; i < numNodes; i++) {
		h5 -> index[i] = i;
	}

	h5 -> incidences = (unsigned long **) malloc(sizeof(unsigned long *) * numNodes);
	for (int i = 0; i < numNodes; i++) {
		h5->incidences[i] = (unsigned long *) malloc(sizeof(unsigned long));
	}

	h5 -> elements = (unsigned long **) malloc(sizeof(unsigned long *) * numNodes);
	for (int i = 0; i < numNodes; i++) {
		h5->elements[i] = (unsigned long *) malloc(sizeof(unsigned long) );
	}


	h5 -> coords = (double **) malloc(sizeof(double *) * numNodes);

	for (int i = 0; i < dims; i++) {
		h5->coords[i] = (double *) malloc(sizeof(double));
	}
}

void freeh5Data(h5Data * h5) {
	free(h5->index);
	for (int i = 0; i < h5 -> numNodes; i++) {
		free(h5 -> incidences[i]);
		free(h5 -> elements[i]);
		free(h5-> coords[i]);
	}
	free(h5);
}

//Creates the groups in the file
hid_t newGroup(hid_t file, string groupLoc) {
	hid_t group;
	group = H5Gcreate2(file, (groupLoc).c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);


	return group;
}

hid_t newDataset(hid_t file, string target, string type, int size) {
	hid_t dataspace, datatype, dataset;
	herr_t status;
	hsize_t dimsf[0]; //consertar
	int RANK = 1;
	dimsf[0] = size;
	
	dataspace = H5Screate_simple(RANK, dimsf, NULL);
	//creates a dataspace (dataset shape)
	if (type == "int") {
		datatype = H5Tcopy(H5T_NATIVE_INT);
	}
	else if (type == "double") {
		datatype = H5Tcopy(H5T_NATIVE_DOUBLE);
	}
	else if (type == "long") {
		datatype = H5Tcopy(H5T_NATIVE_ULONG);
	}
	status = H5Tset_order(datatype, H5T_ORDER_LE);
	dataset = H5Dcreate(file, (target).c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	return dataset;

}

struct ** allocate ()

//Will be using templates for this as I don't want to rewrite the same pieces of code for different types
void writeDataset(hid_t dataset, vector <T> &array, string type) {
	herr_t status;
	if (type == "int") {
		status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &array);
	}
	else if (type == "double") {
		status = H5Dwrite(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, &array);
	}
	else if (type == "long") {
		status = H5Dwrite(dataset, H5T_NATIVE_ULONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, &array);
	}	
}

//Creates the basic file structure
hid_t newFile(string fileName) {
	hid_t file;
	file = H5Fcreate((fileName).c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	return file;
}


void newAttribute(hid_t target, string attrName, string attrValue, int size) {
	hid_t attribute, aidType, attrType; //attribute, dataspace identifire for the attribute, attrtype;
	herr_t ret; //return attribute

	aidType = H5Screate(H5S_SCALAR);
	attrType = H5Tcopy(H5T_C_S1);
	H5Tset_size(attrType, size);
	H5Tset_strpad(attrType, H5T_STR_NULLTERM);

	/// Atribute "GiD Post Results File" Value = "0.0392157 1 1 1"
	attribute = H5Acreate2(target, (attrName).c_str(), attrType, aidType, H5P_DEFAULT,
						H5P_DEFAULT); /// Set the atribute name and types             

	/// Writes string attribute.
	ret = H5Awrite(attribute, attrType, attrValue.c_str());
	ret = H5Aclose(attribute);
	ret = H5Sclose(aidType);
	ret = H5Tclose(attrType);
} 



void close(hid_t target, string type) {
	herr_t status;
	try {
		if (type == "dataspace") {
			status = H5Sclose(target);
		}

		else if(type == "datatype") {
			status = H5Tclose(target);
		}

		else if(type == "file") {
			status = H5Fclose(target);		
		}
		else if(type == "group") {
			status = H5Gclose(target);		
		}
		else if(type == "dataset") {
			status = H5Dclose(target);
		}
	} 
	catch (...) {
		cout << "Closing unsuccessful." << endl;
	}
	
}

// int getDimsFromFile(const char *FEM_LOC) {
// 	FILE *fem = fopen(FEM_LOC, "r");
// 	int dims;
// 	char start[] = "*DIMENSION";
// 	char empty[] = "";
// 	int found = 0;

// 	if (fem == NULL) {
// 		cout << ".fem file is empty!" << endl;
// 		fclose(fem);
// 		return dims;
// 	}

// 	fseek(fem, 0, SEEK_END);
// 	long size = ftell(fem);
// 	fseek(fem, 0, SEEK_SET);
// 	char *buffer = (char *) malloc(sizeof(char) * (size + 1));
	
// 	if (buffer == NULL) {
// 		perror ("Memory allocation error");
// 		fclose(fem);
// 		return dims;
// 	}

// 	fread(buffer, 1, size, fem);
// 	buffer[size] = '\0';
// 	fclose(fem);
// 	char *token = strtok(buffer, " ");
// 	token = strtok(NULL, " ");
// 	dims = strtol(token, NULL, 10);
// 	free(buffer);

// 	return dims;
// }

vector <vector <unsigned long int>> getIncidencesFromFile(const char *FEM_LOC) {
	FILE *fem = fopen(FEM_LOC, "r");
	vector <vector <unsigned long int>> incidences;
	char start[] = "*INCIDENCES";
	char empty[] = "";
	int found = 0;
	if (fem == NULL) {
        perror("Memory allocation error");
		fclose(fem);
		return incidences;
	}

	fseek(fem, 0, SEEK_END);
	long size = ftell(fem);
	fseek(fem, 0, SEEK_END);


	char *buffer = (char *) malloc(sizeof(char) * (size + 1));

	if (buffer == NULL) {
        perror("Memory allocation error");
		fclose(fem);
		return incidences;
	}

	fread(buffer, sizeof(char), size, fem);
	buffer[size] = '\0';
	fclose(fem);

	char * tokenln = strtok(buffer, "\n");
	tokenln = strtok(NULL, "\n");
	tokenln = strtok(NULL, "\n");
	char * tokenCoords;
	char * coordPtr;
	unsigned long value;
	unsigned long i = 0;
	unsigned long j = 0;
	
	while (tokenln != NULL) {
		tokenCoords = strtok(tokenln, "");
		while (tokenCoords != NULL) {
			value = strtol(tokenCoords, &coordPtr, 10);
			value = strtol(NULL, &coordPtr, 10);
			incidences[i].push_back(value);
			printf("%lu\n", &value);
		}
		i++;
	}

    free(buffer); 
    return incidences;
}

unsigned long int getNodesFromFile(const char *FEM_LOC) {
	FILE *fem = fopen(FEM_LOC, "r");
	unsigned long int nodes;
	char start[] = "*COORDINATES";
	char empty[] = "";
	int found = 0;

	if (fem == NULL) {
		cout << ".fem file is empty!" << endl;	
		fclose(fem);
		return nodes;	
	}

	fseek(fem, 0, SEEK_END);
    long size = ftell(fem);
    fseek(fem, 0, SEEK_SET);
    char *buffer = (char *)malloc(sizeof(char) * (size + 1));

    if (buffer == NULL) {
        perror("Memory allocation error");
        fclose(fem);
        return nodes; 
    }
    fread(buffer, sizeof(char), size, fem);
    buffer[size] = '\0'; 
    fclose(fem);
	char *token = strtok(buffer, "\n"); //delim is set to \n
	char *strptr;
	token = strtok(NULL, "\n");
	nodes = strtol(token, &strptr, 10);
	free(buffer);

	cout << "Nodes = " << nodes << endl;

	return nodes;
}



double ** getCoordsFromFile(unsigned long int numNodes, int dims, const char *FEM_LOC) {

	FILE *fem = fopen(FEM_LOC, "r");
    char start[] = "*COORDINATES";
	char empty[] = "";
    int found = 0; 

    if (fem == NULL) {
        cout << ".fem file is empty!" << endl;
        fclose(fem);
        return coords; 
    }
	
    fseek(fem, 0, SEEK_END);
    long size = ftell(fem);
    fseek(fem, 0, SEEK_SET);
    char *buffer = (char *)malloc(sizeof(char) * (size + 1));

	double **coords = (double **) malloc(sizeof(double *) * numNodes);

	for (int i = 0; i < numNodes; i++) {
		coords[i] = (double *) malloc(sizeof(double) * dims);
	}

    if (buffer == NULL) {
        perror("Memory allocation error");
        fclose(fem);
        return coords; 
    }

    fread(buffer, sizeof(char), size, fem);
    buffer[size] = '\0'; 
    fclose(fem);

    char *tokenln = strtok(buffer, "\n");
	char *tokenCoord = strtok(tokenln, "");
	char *coordEnd;
	int auxi, auxj = 0;

    while(tokenCoord != NULL) {
		if (strcmp(tokenCoord, "\n") != 0) {
			coords[auxi][auxj] = strtod(tokenCoord, &coordEnd);
			tokenCoord = strtok(NULL, "");
			auxj++;
		}
		else {
			tokenCoord = strtok(NULL, "");
			tokenCoord = strtok(NULL, "");
			auxj = 0;
			auxi++;
		}
	}
	printf("%s", buffer);
    free(buffer); 
    return coords;
}

void printGidH5(string fileName, const char* FEM_LOC, const char* DEF_LOC) {
	unsigned long numNodes = getNodesFromFile(FEM_LOC);;
	int aux, aux1, auxN = 0, numAttr = 10, numGroups = 18;
	int dims = 2;
	h5Data h5Data = newh5Data(numNodes, dims;)
	h5Data->coords = getCoordsFromFile(numNodes, FEM_LOC);
	h5Data->incidences = getIncidencesFromFile(FEM_LOC);

    hid_t file = newFile(fileName);
	//criação dos grupos
	hid_t meshes = newGroup(file, "/Meshes/");
	hid_t submesh = newGroup(file, "/Meshes/1/");
    hid_t coord = newGroup(file, "Meshes/1/Coordinates/");
    // hid_t elems = newGroup(file, "Meshes/1/Elements/");
    // hid_t res = newGroup(file, "/Results/");
	// hid_t *subRes = (hid_t *) malloc(numGroups * sizeof(hid_t));
	// for (aux = 0; aux < numGroups; aux++) {
	// 	subRes[aux] = newGroup(file, "/Results/" + to_string(aux + 1) );
	// }

	//Criação dos atributos
	newAttribute(file, "GiD Post Results File", "1.1", 4);
	// newAttribute(submesh, "Color", "0.0392157 1 1 1", 16);
	// newAttribute(submesh, "Dimension", "3",  2 );
	// newAttribute(submesh, "ElemType", "Quadrilateral", 14);
	// newAttribute(submesh, "Name", "Group_0", 8);
	// newAttribute(submesh, "Nnode", "4", 2);

	string attrName[] = {"Analysis", "Step", "ResultType", "ResultLocation", "NumComponents", "Name", "Component 1", "Component 2", "Component 3", "Component 4"};	
	string attrVals[] = {"Velocity", "", "Vector", "OnNodes", "4", "NameGoesHere", "X", "Y", "Z", "C4"};
	string namesAttr[] = {"Stress", "NodalPath", "ScalarValues", "Astar_Values", "Theta", "VonMises"};

	// for (aux = 0; aux < numGroups; aux++) {
	// 	for (aux1 = 0; aux1 < numAttr; aux1++) {
	// 		if(aux % 2 == 0 && aux1 == numAttr - 1) {
	// 			break;
	// 		}
	// 		if (aux % 2 == 0) {
	// 			attrVals[4] = "3";
	// 		}
	// 		if (aux < 6) {
	// 			attrVals[1] = "1";
	// 		}
	// 		else if (aux < 12) {
	// 			attrVals[1] = "2";
	// 		}
	// 		else if (aux < 18){
	// 			attrVals[1] = "3";
	// 		}
	// 		if(auxN == 6) {
	// 			auxN = 0;
				
	// 		}
	// 		attrVals[5] = namesAttr[auxN];
	// 		newAttribute(subRes[aux], attrName[aux1], (char *) (attrVals[aux1]).c_str(), (attrVals[aux1]).length());
	// 		auxN++;
	// 	}
	// }

	//Writing to the datasets
	hid_t *coordDataset, *elemDataset, *resDataset;
	
	// /Meshes/1/Coordinates/
	coordDataset = (hid_t *) malloc(numNodes * sizeof(hid_t));
	

	for (aux = 0; aux < numNodes; aux++) {
		coordDataset[aux] = newDataset(file, "Meshes/1/Coordinates/" + to_string(aux + 1) + "/", "double", 2);
		writeDataset(coordDataset[aux], h5Data->coords[aux], "double");
		close(coordDataset[aux], "dataset");
	}

	// // /Meshes/1/Elements/
	// int numElem = 6;
	// elemDataset = (hid_t *) malloc(numElem * sizeof(hid_t));

	// for (aux = 0; aux < numElem; aux++) {
	// 	initDatasets(&testArr,"Meshes/1/Elements/" + to_string(aux + 1) + "/");
	// 	elemDataset[aux] = newDataset(file, "Meshes/1/Elements/" + to_string(aux + 1) + "/", "long int", 2);
	// 	writeDataset(elemDataset[aux], testArr, "int");
	// 	close(elemDataset[aux], "dataset");
	// }

	// // /Results/
	// int numRes = 5, numResGroup = 18;
	// resDataset = (hid_t *) malloc(numRes * sizeof(hid_t));

	
	// for (aux = 0; aux < numResGroup; aux++) {
	// 	for (aux1 = 0; aux1 < numRes; aux1++) {
	// 		initDatasets(&testArr,"Results/");
	// 		//condição do número de componentes = 3		
	// 		if(aux % 2 == 0  && aux1 == 4) {
	// 			break;
	// 		}
	// 		else if (aux1 == 0) {
	// 			resDataset[aux1] = newDataset(file, "Results/" + to_string(aux + 1) + "/" + to_string(aux1 + 1)+ "/", "long int", 6);
	// 			writeDataset(resDataset[aux1], testArr, "int");
	// 			close(resDataset[aux1], "dataset");
	// 		}
	// 		else{
	// 			resDataset[aux1] = newDataset(file, "Results/" + to_string(aux + 1) + "/" + to_string(aux1 + 1) + "/", "double", 6);
	// 			writeDataset(resDataset[aux1], testArr, "double");
	// 			close(resDataset[aux1], "dataset");			
	// 		}
	// 	}

	// }
	//Closing every single hid_t
	close(submesh, "group");
	close(meshes, "group");
    // close(elems, "group");
    close (coord, "group");
    // close (res, "group");
    close(file, "file");
	
	// for (aux = 0; aux < numGroups; aux++) {
	// 	close(subRes[aux], "group");
	// }
	// free(subRes);
}

