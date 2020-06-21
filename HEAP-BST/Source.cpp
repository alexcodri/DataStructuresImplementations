#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "malloc.h"
#include "string.h"

struct Platforma {
	float pretLicenta;
	char* denumire;
	int valabilitateInLuni;
	char** platformeDisponibile;
	int numarPlatforme;
};

Platforma initPlatforma(float pret, char* denumire, int valabilitate, char** platforme, int nrPlatforme) {
	Platforma platforma;

	platforma.pretLicenta = pret;
	platforma.denumire = (char*)malloc(sizeof(char) * (strlen(denumire) + 1));
	strcpy(platforma.denumire, denumire);

	platforma.valabilitateInLuni = valabilitate;
	platforma.numarPlatforme = nrPlatforme;

	platforma.platformeDisponibile = (char**)malloc(sizeof(char*) * nrPlatforme);
	for (int i = 0; i < nrPlatforme; i++) {
		platforma.platformeDisponibile[i] = (char*)malloc(sizeof(char) * (strlen(platforme[i]) + 1));
		strcpy(platforma.platformeDisponibile[i], platforme[i]);
	}

	return platforma;
}

void afisarePlatforma(Platforma platforma) {
	printf("\n %.2f, %s, %d, %d platforme: ", platforma.pretLicenta, platforma.denumire,
		platforma.valabilitateInLuni, platforma.numarPlatforme);
	
	for (int i = 0; i < platforma.numarPlatforme; i++) {
		printf(" %s", platforma.platformeDisponibile[i]);
	}
}

struct Heap {
	Platforma* platforme;
	int size;
};

Heap initHeap(int size) {
	Heap heap;
	heap.size = size;

	heap.platforme = (Platforma*)malloc(sizeof(Platforma) * size);
	return heap;
}
//MAXHEAP
//void filtrare(Heap heap, int pozitie) {
//	if (heap.size > 0) {
//		int fiuS = 2 * pozitie + 1;
//		int fiuD = 2 * pozitie + 2;
//
//		int pozitieMax = pozitie;
//
//		if (fiuS < heap.size && heap.platforme[pozitieMax].pretLicenta < heap.platforme[fiuS].pretLicenta) {
//			pozitieMax = fiuS;
//		}
//		if (fiuD < heap.size && heap.platforme[pozitieMax].pretLicenta < heap.platforme[fiuD].pretLicenta) {
//			pozitieMax = fiuD;
//		}
//
//		if (pozitieMax != pozitie) {
//			Platforma aux = heap.platforme[pozitie];
//			heap.platforme[pozitie] = heap.platforme[pozitieMax];
//			heap.platforme[pozitieMax] = aux;
//			if (2 * pozitieMax + 1 < heap.size) {
//				filtrare(heap, pozitieMax);
//			}
//		}
//	}
//}
//MINHEAP
void filtrare(Heap heap, int pozitie) {
	if (heap.size > 0) {
		int fiuS = 2 * pozitie + 1;
		int fiuD = 2 * pozitie + 2;

		int pozitieMax = pozitie;

		if (fiuS < heap.size && heap.platforme[pozitieMax].pretLicenta > heap.platforme[fiuS].pretLicenta) {
			pozitieMax = fiuS;
		}
		if (fiuD < heap.size && heap.platforme[pozitieMax].pretLicenta > heap.platforme[fiuD].pretLicenta) {
			pozitieMax = fiuD;
		}

		if (pozitieMax != pozitie) {
			Platforma aux = heap.platforme[pozitie];
			heap.platforme[pozitie] = heap.platforme[pozitieMax];
			heap.platforme[pozitieMax] = aux;
			if (2 * pozitieMax + 1 < heap.size) {
				filtrare(heap, pozitieMax);
			}
		}
	}
}

void  afisareHeap(Heap heap) {
	for (int i = 0; i < heap.size; i++) {
		afisarePlatforma(heap.platforme[i]);
	}
}

void citirePlatformaDinFisier(const char* numeFisier, Heap heap) {
	FILE* file = fopen(numeFisier, "r");
	int count = 0;
	while (!feof(file)) {
		Platforma platforma;

		char comma;
		char buffer[50];

		fscanf(file, "%f", &platforma.pretLicenta);
		fscanf(file, "%c", &comma);
		fscanf(file, "%[^,]s", &buffer);
		platforma.denumire = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(platforma.denumire, buffer);
		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &platforma.valabilitateInLuni);
		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &platforma.numarPlatforme);

		platforma.platformeDisponibile = (char**)malloc(sizeof(char*) * platforma.numarPlatforme);
		for (int i = 0; i < platforma.numarPlatforme; i++) {
			fscanf(file, "%c", &comma);
			fscanf(file, "%[^,]s", &buffer);
			platforma.platformeDisponibile[i] = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
			strcpy(platforma.platformeDisponibile[i], buffer);
		}
		fscanf(file, "%c", &comma);

		heap.platforme[count] = platforma;
		count++;

	}
}

void aplicareFiltrare(Heap heap) {
	for (int i = (heap.size - 1) / 2; i >= 0; i--) {
		filtrare(heap, i);
	}
}

void modificarePlatforma(Heap &heap, int pretNou) {
	heap.platforme[0].pretLicenta = pretNou;
	aplicareFiltrare(heap);
}

struct NodArbore {
	Platforma platforma;
	NodArbore* stanga;
	NodArbore* dreapta;
};

NodArbore* initNod(Platforma platforma) {
	NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
	nou->dreapta = NULL;
	nou->stanga = NULL;
	nou->platforma = initPlatforma(platforma.pretLicenta, platforma.denumire,
		platforma.valabilitateInLuni, platforma.platformeDisponibile, platforma.numarPlatforme);
	return nou;
}

NodArbore* inserareInArbore(NodArbore* radacina, Platforma platforma) {
	if (radacina) {
		NodArbore* aux = radacina;
		while (1) {
			if (aux->platforma.valabilitateInLuni > platforma.valabilitateInLuni) {
				if (aux->stanga == NULL) {
					aux->stanga = initNod(platforma);
					return radacina;
				}
				else aux = aux->stanga;
			}
			else if (aux->dreapta == NULL) {
				aux->dreapta = initNod(platforma);
				return radacina;
			}
			else aux = aux->dreapta;
		}
	}
	else radacina = initNod(platforma);
	return radacina;
}

void copiereInBST(NodArbore*& radacina, Heap heap, int numarPlatformeDeCopiat, int minValabilitateLicenta) {
	int count = 0;
	for (int i = 0; i < heap.size; i++) {
		if (count < numarPlatformeDeCopiat && heap.platforme[i].valabilitateInLuni > minValabilitateLicenta) {
			radacina = inserareInArbore(radacina, heap.platforme[i]);
			count++;
		}
	}
}

void traversarePreordine(NodArbore* radacina) {
	if (radacina) {
		afisarePlatforma(radacina->platforma);
		traversarePreordine(radacina->stanga);
		traversarePreordine(radacina->dreapta);
	}
}

void afisareUnSingurDescendent(NodArbore* radacina) {
	if (radacina) {
		if (radacina->stanga != NULL && radacina->dreapta == NULL) {
			afisarePlatforma(radacina->platforma);
			afisareUnSingurDescendent(radacina->stanga);
		}
		else if (radacina->stanga == NULL && radacina->dreapta != NULL) {
			afisarePlatforma(radacina->platforma);
			afisareUnSingurDescendent(radacina->dreapta);
		}
		else {
			afisareUnSingurDescendent(radacina->stanga);
			afisareUnSingurDescendent(radacina->dreapta);
		}
	}
}

void dezalocareHeap(Heap heap) {
	if(heap.platforme != NULL && heap.size >0)
	free(heap.platforme);

}

void dezalocareBST(NodArbore*& radacina) {
	if(radacina) {
		dezalocareBST(radacina->stanga);
		dezalocareBST(radacina->dreapta);
		free(radacina);
		radacina = NULL;
	}
}

void main() {
	Heap heap = initHeap(10);
	const char* numeFisier = "platforme.txt";
	citirePlatformaDinFisier(numeFisier, heap);
	afisareHeap(heap);
	printf("\nFiltrare");
	aplicareFiltrare(heap);
	afisareHeap(heap);

	printf("\n-2---");
	modificarePlatforma(heap, 6666.0);
	afisareHeap(heap);
	
	NodArbore* radacina = NULL;
	printf("\n3 - Se vor adauga in BST doar 4 platforme care au valabilitatea mai mare de 9 luni.");
	copiereInBST(radacina, heap, 4, 9);
	traversarePreordine(radacina);
	printf("\n4 -----");
	afisareUnSingurDescendent(radacina);

	printf("\n5 - dezalocari");
	dezalocareHeap(heap);
	dezalocareBST(radacina);
}

