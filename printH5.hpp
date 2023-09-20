
#include <string>
#include <stdlib.h>
#include "hdf5.h"
#include <cstring>
#include <cstring>

using namespace std;

typedef struct array {
    int *datasetInt;
    double *datasetDouble;
} vector;


array newArray() {
    array vector;
    vector.datasetInt = NULL;
    vector.datasetDouble = NULL;

    return vector;
}

//Função cria estruturas de dados teste e as inicializa.
hid_t newDataset(hid_t file, string target, int numElements, string type) {
	int RANK = 1;
	hsize_t dimsf[1];
	dimsf[0] = 1;


	hid_t datatype;

	hid_t dataset;

	//creates a dataspace (dataset shape)
    hid_t dataspace = H5Screate_simple(RANK, dimsf, NULL);

    //datatype = H5Tcopy(H5T_NATIVE_DOUBLE);
    herr_t status = H5Tset_order(datatype, H5T_ORDER_LE);

	if (type == "int") {
    	//defines the datatype (description of the data in a dataset)
		datatype = H5Tcopy(H5T_NATIVE_INT);
	} 
	else if (type == "double") {
		datatype = H5Tcopy(H5T_NATIVE_DOUBLE);
	}
	else {
		printf("Você passou um tipo inválido.\n");
		exit(1);
	}

	dataset = H5Dcreate(file, (target).c_str() , datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);


    return dataset;
}


//Creates the groups in the file
hid_t newGroup(hid_t file, string groupLoc) {
	hid_t group;
	group = H5Gcreate2(file, (groupLoc).c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);


	return group;
}


//Creates the basic file structure
hid_t newFile(string fileName) {
	hid_t file;

    file = H5Fcreate((fileName).c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT); 

	return file;
}


void writeDataset(hid_t dataset, array vector, hid_t group, string type) {
	hid_t dataspace, datatype;
	herr_t status;
	int RANK = 1;
	hsize_t dimsf[1];
	dimsf[0] = 1;


	dataspace = H5Screate_simple(RANK, dimsf, NULL);
	//creates a dataspace (dataset shape)
	if(type == "int") {
		datatype = H5Tcopy(H5T_NATIVE_INT);
		status = H5Tset_order(datatype, H5T_ORDER_LE);
		status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, vector.datasetInt);
	}
	else if(type == "double") {
		datatype = H5Tcopy(H5T_NATIVE_DOUBLE);
		status = H5Tset_order(datatype, H5T_ORDER_LE);
		status = H5Dwrite(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vector.datasetDouble);
	}
}


void newAttribute(hid_t target, string attrName, char attrValue[], int size) {
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
	ret = H5Awrite(attribute, attrType, attrValue);
	ret = H5Aclose(attribute);
	ret = H5Sclose(aidType);
	ret = H5Tclose(attrType);
} 



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

	else {
		printf("Something went wrong. Check the type you wrote!");
	}
}

void executor(string fileName) {
    hid_t file = newFile(fileName);

	//criação dos grupos

	hid_t meshes = newGroup(file, "/Meshes/");

	hid_t submesh = newGroup(file, "/Meshes/1/");
    
    hid_t coord = newGroup(file, "Meshes/1/Coordinates/");

    hid_t elems = newGroup(file, "Meshes/1/Elements/");

    hid_t res = newGroup(file, "/Results/");

	//Criação dos atributos

	newAttribute(file, "GiD Post Results File", "1.1", 4);

	close(submesh, "group");
	close(meshes, "group");
    close(elems, "group");
    close (coord, "group");
    close (res, "group");
    close(file, "file");

}