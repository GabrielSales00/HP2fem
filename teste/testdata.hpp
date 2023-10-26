
#include <string>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>

using namespace std;

struct array {
	int * arrayInt;
	double * arrayDouble;
};

void initDatasets(struct array *d, string target) {
	int aux, SIZE;
	if (target == "Meshes/1/Coordinates/1/") {
		SIZE = 6;
		int v [] = {
			1, 2, 3, 4, 5, 6
		};
		d -> arrayInt = (int *) malloc(sizeof(int) * SIZE);
		for (aux = 0; aux < SIZE; aux++) {
			d->arrayInt[aux] = v[aux];
		}

	}
	else if(target == "Meshes/1/Coordinates/4/") {
		SIZE = 6;
		d->arrayDouble = (double *) malloc(sizeof(double) * SIZE);
		for(aux = 0; aux < SIZE; aux++) {
			d -> arrayDouble[aux] = 0;
		}
	}
	else if(target == "Meshes/1/Coordinates/3/") {
		SIZE = 6;
		double v [] = {
			0.0,
			0.0,
			1.0,
			1.0,
			2.0,
			2.0
		};
		d -> arrayDouble = (double *) malloc(sizeof(double) * SIZE);
		for (aux = 0; aux < SIZE; aux++) {
			d->arrayDouble[aux] = v[aux];
		}

	}
	else if(target == "Meshes/1/Coordinates/2/") {		
		SIZE = 6;
		double v [] = {
			0.0,
			1.0,
			0.0,
			1.0,
			0.0,
			1.0
		};
		d -> arrayDouble = (double *) malloc(sizeof(double) * SIZE);
		for (aux = 0; aux < SIZE; aux++) {
			d->arrayDouble[aux] = v[aux];
			printf("%.2f\n", d->arrayDouble[aux]);
		}
	}
	else if(target == "Meshes/1/Elements/1/") {
		SIZE = 2;
		int v [] = {
        1, 2
    	};
		d -> arrayInt = (int *) malloc(sizeof(int) * SIZE);
		for (aux = 0; aux < SIZE; aux++) {
			d->arrayInt[aux] = v[aux];
		}
	}	
	else if (target == "Meshes/1/Elements/2/") {
		SIZE = 2;
		int v [] = {
		1, 3
    	};
		d -> arrayInt = (int *) malloc(sizeof(int) * SIZE);
		for (aux = 0; aux < SIZE; aux++) {
			d->arrayInt[aux] = v[aux];
		}
	}
	else if (target == "Meshes/1/Elements/3/") {
		SIZE = 2;
		int v [] = {
        2, 4
    	};
		d -> arrayInt = (int *) malloc(sizeof(int) * SIZE);
		for (aux = 0; aux < SIZE; aux++) {
			d->arrayInt[aux] = v[aux];
		}
	}

	else if (target == "Meshes/1/Elements/4/") {
		SIZE = 2;
		int v [] = {
        4, 6
    	};
		d -> arrayInt = (int *) malloc(sizeof(int) * SIZE);
		for (aux = 0; aux < SIZE; aux++) {
			d->arrayInt[aux] = v[aux];
		}
	}
	else if (target == "Meshes/1/Elements/5/") {
		SIZE = 2;
		int v [] = {
        3, 5
    	};
		d -> arrayInt = (int *) malloc(sizeof(int) * SIZE);
		for (aux = 0; aux < SIZE; aux++) {
			d->arrayInt[aux] = v[aux];
		}
	}
	else if (target == "Meshes/1/Elements/6/") {
		SIZE = 2;
		d->arrayInt = (int *) malloc(sizeof(int) * SIZE);
		for (aux = 0; aux < SIZE; aux++) {
			d->arrayInt[aux] = 1;
		}
	}
	else if(target == "Results/") {
		SIZE = 6;
		d->arrayInt = (int *) malloc(sizeof(int) * SIZE);
		d->arrayDouble = (double *) malloc(sizeof(double) * SIZE);
		for(aux = 0; aux < SIZE; aux++) {
			d -> arrayInt[aux] = aux + 1;
			d -> arrayDouble[aux] = (rand() % 100) + 1;

		}	
	}
		
}