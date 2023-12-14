#include <string>
#include <stdlib.h>
#include "hdf5.h"
#include <cstring>
#include <iostream>

using namespace std;


//Creates the groups in the file


/**
	Okay
*/
hid_t newGroup(hid_t file, string groupLoc) {
	hid_t group;
	group = H5Gcreate2(file, (groupLoc).c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);


	return group;
}

//coord[0] = x, coord[1] = y, coord[2] = z, coord[3] = index;
vector <double> * getCoords (unsigned long numNodes) {
	double * coordinates;
	unsigned long nodeInd;
	vector <double> **coordsVec;
	coordsVec = (vector **) malloc(3 * sizeof (vector *));
	for(int aux = 0; aux < 3; aux++) {
		coordsVec = (vector *) malloc(numNodes * sizeof(vector));
	}
	for (nodeInd = 0; nodeInd < numNodes; nodeInd++ ) {
		coordinates = GlobalModel.Coords.GetNodeCoordinates(nodeInd)
		coordsVec[0].push_back(coordinates[0]); //x
		coordsVec[1].push_back(coordinates[1]); //y
		coordsVec[2].push_back(0); //z
	}

	return coordsVec;
}


//checar
vector <unsigned long int> * getIndex (unsigned long numNodes) {
	unsigned long int index;
	vector <unsigned long int> indexVec;
	for(index = 0; index < numNodes; index++) {
		indexvec.push_back(index);
	}

	return indexVec;
}

/**
	Okay
*/
hid_t newDataset(hid_t file, string target, string type, hsize_t *dimsf, int RANK, int dimsSize) {
	try {
		hid_t dataspace, datatype, dataset;
		herr_t status;
		// int RANK = 1;
		// dimsf[0] = size;
		for (int aux = 0; aux < dimsSize; aux++) {
			dimsf[aux] = size[aux];
		}

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
		else {
			printf("newDataset: ERROR: only int and double are supported!\n");
		}
		status = H5Tset_order(datatype, H5T_ORDER_LE);
		dataset = H5Dcreate(file, (target).c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

		return dataset;
	}
	catch (...) {
		cout << "ERROR: could not create a new dataset (not to be confused with "write")." << endl;
	}
}


/**
	Okay
*/
void writeDataset(hid_t dataset, vector array, string type) {
	try {
		herr_t status;
		if (type == "int") {
			status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, array);
		}
		else if (type == "double") {
			status = H5Dwrite(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, array);
		}
		else if (type == "long") {
			status = H5Dwrite(dataset, H5T_NATIVE_ULONG, H5S_ALL, H5S_ALL, H5P_DEFAULT, array)
		}
		else {
			printf("writeDataset: ERROR: only int, double and long int are supported!\n");
		}
	}
	catch (...) {
		cout << "ERROR: dataset could not be written. Please check if the array passed to the 'write dataset' function is correct." << endl;
	}

}


/**
	Okay
*/
//Creates the basic file structure
hid_t newFile(string fileName) {
	hid_t file;

    file = H5Fcreate((fileName).c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT); 

	return file;
}

/**
	Okay
*/
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


/**
	Okay
*/
void close(hid_t target, string type) {
	herr_t status;
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
 
	else {
		printf("Closing of " + type + " unsuccessful.");
	}
}

/*Descobrir: Onde tirar as informações referentes a:
	- Nome dos atributos; - R:Nomes arbitrários
	- Quantidade de nós; -	Método dedicado;
	- Quantidade de steps; - Método dedicado;
	- Número de elementos. -  GlobalModel.Groups[GroupNumber].GetNumberElements();
	- Coordenadas -	
	- Onde extrair os vetores de coord
	- Onde tirar os vetores Double e Int
	- Quando os vetores têm três componentes*/

//- Nós: GlobalModel.eqs.getTotalNumberDOFs() / GlobalModal.getDimensions();

//- Steps: SolvParam.nts();

//GlobalModel.Coords.GetNodeCoordinates(Node); - 3 vetores para (x, y, z) de cada nó

void printGidH5(string fileName, Vector Vector, unsigned long int numNodes, unsigned long int numSteps) {

	int defaultDimsf[1] = {numNodes};
	int defaultSize = 1;
	int rank = 1;

	vector <double> coordsVec = getCoords(numNodes);
	vector <unsigned long> indexVec = getIndex(numNodes);

	//descobrir numGroups
	int aux, aux1, auxN = 0, numAttr = 10, numGroups = numNodes * numSteps;
    hid_t file = newFile(fileName);
	//criação dos grupos
	hid_t meshes = newGroup(file, "/Meshes/");
	hid_t submesh = newGroup(file, "/Meshes/1/");
    hid_t coord = newGroup(file, "Meshes/1/Coordinates/");
    hid_t elems = newGroup(file, "Meshes/1/Elements/");
    hid_t res = newGroup(file, "/Results/");
	hid_t *subRes = (hid_t *) malloc(numGroups * sizeof(hid_t));

	for (aux = 0; aux < numGroups; aux++) {
		subRes[aux] = newGroup(file, "/Results/" + to_string(aux + 1) );
	}

	//Criação dos atributos
	newAttribute(file, "GiD Post Results File", "1.1", 4);
	newAttribute(submesh, "Color", "0.0392157 1 1 1", 16);
	newAttribute(submesh, "Dimension", "3",  2 );
	newAttribute(submesh, "ElemType", "Quadrilateral", 14);
	newAttribute(submesh, "Name", "Group_0", 8);
	newAttribute(submesh, "Nnode", "4", 2);

	string attrName[] = {"Analysis", "Step", "ResultType", "ResultLocation", "NumComponents", "Name", "Component 1", "Component 2", "Component 3", "Component 4"};	
	
	//Descobrir de onde extrair esses valores
	string attrVals[] = {"Velocity", "", "Vector", "OnNodes", "4", "NamesAttr", "X", "Y", "Z", "C4"};
	string namesAttr[] = {"Stress", "NodalPath", "ScalarValues", "Astar_Values", "Theta", "VonMises"};



	/**
		Okay
	*/
	for (aux = 0; aux < numGroups; aux++) {
		for (aux1 = 0; aux1 < numAttr; aux1++) {
			if(aux % 2 == 0 && aux1 == numAttr - 1) {
				break;
			}
			if (aux % 2 == 0) {
				attrVals[4] = "3";
			}
			if (aux < 6) {
				attrVals[1] = "1";
			}
			else if (aux < 12) {
				attrVals[1] = "2";
			}
			else if (aux < 18){
				attrVals[1] = "3";
			}
			if(auxN == 6) {
				auxN = 0;
				
			}

			//nomes dos atributos - MUDAR
			attrVals[5] = namesAttr[auxN];
			newAttribute(subRes[aux], attrName[aux1], (char *) (attrVals[aux1]).c_str(), (attrVals[aux1]).length());
			auxN++;
		}
	}


	/*******************************EVERYTHING WORKS*********************************/
	//Writing to the datasets
	hid_t *coordDataset, *elemDataset, *resDataset;
	
	// /Meshes/1/Coordinates/

	//Descobrir de onde tirar
	int numCoord = 4; //1 para o índice + 3 para as coordenadas (x, y, z)
	coordDataset = (hid_t *) malloc(numCoord * sizeof(hid_t));


	//Coordinates

	/*
		Checar se é "long"
	*/
	for (aux = 0; aux < numCoord; aux++) {
		if (aux > 0) {
			for (aux1 = 0; aux < numNodes; aux++) {
				nodeCoords = GlobalModel.Coords.GetNodeCoordinates(aux1);
				coordDataset[aux] = newDataset(file, "Meshes/1/Coordinates/" + to_string(aux + 1) + "/", "double", &defaultDimsf, rank, defaultSize);
				writeDataset(coordDataset[aux], coordsVec, "double");
				close(coordDataset[aux], "dataset");
			}
		}
		else {
			coordDataset[aux] = newDataset(file, "Meshes/1/Coordinates/" + to_string(aux + 1) + "/", "int", &defaultDimsf, rank, defaultSize);
			writeDataset(coordDataset[aux], indexVec, "long");
			close(coordDataset[aux], "dataset");
		}
	}

	//For the actual values
	for (aux = 0; aux < numNodes; aux++) {
		if (aux == 0) {
			coordDataset[aux] = newDataset(file, "Meshes/1/Coordinates/" + to_string(aux + 1) + "/", "int", &defaultDimsf, rank, defaultSize);
			writeDataset(coordDataset[aux], VECTOR, "long");
			close(coordDataset[aux], "dataset");
		}
		else {
			nodeCoords = GlobalModel.Coords.GetNodeCoordinates(aux);
			coordDataset[aux] = newDataset(file, "Meshes/1/Coordinates/" + to_string(aux + 1) + "/", "double", &defaultDimsf, rank, defaultSize);
			writeDataset(coordDataset[aux], VECTOR, "long");
		}
	}

	// /Meshes/1/Elements/

	//descobrir o group number
	unsigned long int numElems, elemOrder, *incid, incidences[numElemNodes][numElems];
	double *elemCoords; 

	for(unsigned long groupNumber = 0; groupNumber < GlobalModel.Groups.GetNumberFEGroups(), groupNumber++) {
		
		numElems = GlobalModel.Groups[GroupNumber].GetNumberElements();


		vector<unsigned long int> indexVecElem;
		vector<unsigned long int> indexVecElemOnes;

		for(int elem = 0; elem < numElems; aux++) {
			elemOrder = GlobalModel.Groups[groupNumber].GetMesh(SOLUTION).GetElementOrder(elem);
			elemCoords = GlobalModel.Groups[groupNumber].GetMesh(MeshType).GetElementCoordinates(elem, numCoords);
			incid = GlobalModel.Groups[groupNumber].GetMesh(SOLUTION).GetElementIncidence(elem, numElemNodes);
			for (unsigned long int incidAux = 0; incidAux < numElemNodes; incidAux++) {
				incidences[incidAux][elem] = incid[incidAux] + 1;
			}							
			indexVecElem.push_back(ElemNumber+1);
			indexVecElemOnes.push_back(1);														
		}

		
		elemDataset = (hid_t *) malloc(numElems * sizeof(hid_t));
		hsize_t nodesPerElem = (unsigned long int) numNodes;
		hsize_t dims2[2] = {numElems, 1};
		hsize_t dims3[2] = {1, numElems};
		int elemsSize = 2;
		int elemsDim = 2;

		//corrigir os datasets
		for (unsigned long int aux = 0; aux < nodesPerElem + 2; aux++) {
			if (aux + 1 == 1) {
				elemDataset[aux] = newDataset(file, "Meshes/1/Elements/" + to_string(aux + 1), "long", &dims2, elemsDim, elemsSize);
				writeDataset(elemDataset, indexVecElem.front(), "long");
			}
			else if (aux + 1 > 1 && aux < nodesPerElem + 2) {
				elemDataset[aux] = newDataset(file, "Meshes/1/Elements/" + to_string(aux + 1), "long", &dims2, elemsDim, elemsSize);
				writeDataset(elemDataset, incidences[i-1][0], "long");
			}
			else {
				elemDataset[aux] = newDataset(file, "Meshes/1/Elements/" + to_string(aux + 1), "long", &dims2, elemsDim, elemsSize);
				writeDataset(elemDataset, indexVecElemOnes.front(), "long");			
			}
		}
	}

	// /Results/
	int numRes = numNodes + 1, numResGroup = numNodes * numSteps;
	resDataset = (hid_t *) malloc(numRes * sizeof(hid_t));

	
	for (aux = 0; aux < numResGroup; aux++) {
		for (aux1 = 0; aux1 < numRes; aux1++) {
			//initDatasets(&testArr,"Results/");
			//condição do número de componentes = 3		
			if(aux % 2 == 0  && aux1 == 4) {
				break;
			}
			else if (aux1 == 0) {
				resDataset[aux1] = newDataset(file, "Results/" + to_string(aux + 1) + "/" + to_string(aux1 + 1)+ "/", "long", numNodes);
				writeDataset(resDataset[aux1], vectorResultsIndex, "long");
				close(resDataset[aux1], "dataset");
			}
			else{
				resDataset[aux1] = newDataset(file, "Results/" + to_string(aux + 1) + "/" + to_string(aux1 + 1) + "/", "double", numNodes);
				writeDataset(resDataset[aux1], vectorResults, "double");
				close(resDataset[aux1], "dataset");			
			}
		}

	}
	//Closing every single hid_t
	close(submesh, "group");
	close(meshes, "group");
    close(elems, "group");
    close (coord, "group");
    close (res, "group");
    close(file, "file");
	
	for (aux = 0; aux < numGroups; aux++) {
		close(subRes[aux], "group");
	}
	free(subRes);
}

