#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "malloc.h"
#include "string.h"

struct Librarie {
	char* denumire;
	char* cheie;
	int nrLinii;
	int sumaCheie;
};

int calculareSumaCheie(char* cheie) {
	int suma = 0;
	for (int i = 0; i < strlen(cheie) + 1; i++) {
		suma += cheie[i];
	}
	return suma;
}

Librarie initLibrarie(char* den, char* cheie, int nr, int suma) {
	Librarie librarie;
	librarie.denumire = (char*)malloc(sizeof(char) * (strlen(den) + 1));
	strcpy(librarie.denumire, den);
	librarie.cheie = (char*)malloc(sizeof(char) * (strlen(cheie) + 1));
	strcpy(librarie.cheie, cheie);
	librarie.nrLinii = nr;

	librarie.sumaCheie = calculareSumaCheie(librarie.cheie);

	return librarie;
}

void afisareLibrarie(Librarie librarie) {
	printf("\nDenumire %s, cheie %s, numar linii %d, suma cheie %d.", librarie.denumire,
		librarie.cheie, librarie.nrLinii, librarie.sumaCheie);
}

struct NodArbore {
	NodArbore* stanga;
	NodArbore* dreapta;
	Librarie librarie;
};

NodArbore* initNodArbore(Librarie librarie) {
	NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
	nou->dreapta = NULL;
	nou->stanga = NULL;
	nou->librarie = librarie;

	return nou;
}

NodArbore* inserareInArbore(NodArbore*& radacina, Librarie librarie) {
	if (radacina) {
		NodArbore* temp = radacina;
		while (1) {
			if (temp->librarie.sumaCheie > librarie.sumaCheie) {
				if (temp->stanga == NULL) {
					temp->stanga = initNodArbore(librarie);
					return radacina;
				}
				else temp = temp->stanga;
			}
			else if (temp->dreapta == NULL) {
				temp->dreapta = initNodArbore(librarie);
				return radacina;
			}
			else temp = temp->dreapta;
		}
	}
	else radacina = initNodArbore(librarie);
	return radacina;
}

void citireLibrariiDinFisierSiInserareInArbore(const char* numeFisier, NodArbore* &radacina) {
	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		Librarie librarie;
		char comma;
		char buffer[40];

		fscanf(file, "%[^,]s", &buffer);
		librarie.denumire = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(librarie.denumire, buffer);

		fscanf(file, "%c", &comma);
		fscanf(file, "%[^,]s", &buffer);
		librarie.cheie = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(librarie.cheie, buffer);
		
		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &librarie.nrLinii);
		fscanf(file, "%c", &comma);

		librarie.sumaCheie = calculareSumaCheie(librarie.cheie);

		inserareInArbore(radacina, librarie);
	}
	fclose(file);
}

void afisareArbore(NodArbore* radacina) {
	if (radacina) {
		afisareLibrarie(radacina->librarie);
		afisareArbore(radacina->stanga);
		afisareArbore(radacina->dreapta);
	}
}

void dezalocareArbore(NodArbore* radacina) {
	if (radacina) {
		dezalocareArbore(radacina->stanga);
		dezalocareArbore(radacina->dreapta);
		free(radacina);
	}
}

void copiereLibrarii(NodArbore* radacina, NodArbore*& radacinaCopie, int nr) {
	if (radacina) {
		if (radacina->librarie.nrLinii > nr) {
			inserareInArbore(radacinaCopie, radacina->librarie);
		}
		copiereLibrarii(radacina->stanga, radacinaCopie, nr);
		copiereLibrarii(radacina->dreapta, radacinaCopie, nr);
	}
}


void afisareVectorLibrarii(Librarie* vecLibrarii, int count) {
	for (int i = 0; i < count; i++) {
		afisareLibrarie(vecLibrarii[i]);
	}
}

void dezalocareVectorLibrarii(Librarie* &vecLibrarii, int& count) {
	for (int i = 0; i < count; i++) {
		free(vecLibrarii[i].cheie);
		free(vecLibrarii[i].denumire);
	}
	free(vecLibrarii);
	count = 0;
}

void numarElementeFaraDescendenti(NodArbore* radacina, int& nr, char** &vecChei) {
	if (radacina) {
		numarElementeFaraDescendenti(radacina->stanga, nr, vecChei);
		numarElementeFaraDescendenti(radacina->dreapta, nr, vecChei);
		if (radacina->dreapta == NULL && radacina->stanga == NULL) {
			nr++;
			vecChei = (char**)realloc(vecChei, sizeof(char*) * nr);
			vecChei[nr - 1] = (char*)malloc(sizeof(char) * (strlen(radacina->librarie.cheie) + 1));
			strcpy(vecChei[nr - 1], radacina->librarie.cheie);
		}
	}
}

void dezalocareVectori(char** &vec, int size) {
	for (int i = 0; i < size; i++) {
		free(vec[i]);
	}
	free(vec);
}

void main() {
	
	NodArbore* radacina = NULL;
	const char numeFisier[20] = "librarii.txt";
	citireLibrariiDinFisierSiInserareInArbore(numeFisier, radacina);
	printf("\n\tEx1.");
	afisareArbore(radacina);

	printf("\n\tEx2. Copierea librariilor se va face daca libraria are mai mult de 900 linii.");
	NodArbore* radacinaNoua = NULL;
	copiereLibrarii(radacina, radacinaNoua, 900);
	afisareArbore(radacinaNoua);

//	printf("\n\tEx3. Se vor sterge librariile care au suma cheii mai mare de 621.");

	printf("\n\tEx4.\nCheile elementelor care nu au descendenti:");
	
	char** vecChei = (char**)malloc(sizeof(char*));
	
	int nr = 0;
	numarElementeFaraDescendenti(radacina, nr,vecChei);
	char** vecCheiDescrescator = (char**)malloc(sizeof(char*)*nr);
	for (int i = 0; i < nr; i++) {
		vecCheiDescrescator[nr-i-1] = (char*)malloc(sizeof(char) * (strlen(vecChei[i]) + 1));
		strcpy(vecCheiDescrescator[nr-i-1], vecChei[i]);
	}

	for (int i = 0; i < nr; i++) {
		printf("%s ", vecCheiDescrescator[i]);
	}

	printf("\nEx5.\nDezalocari");
	dezalocareArbore(radacina);
	dezalocareArbore(radacinaNoua);
	dezalocareVectori(vecChei, nr);
	dezalocareVectori(vecCheiDescrescator, nr);
	nr = 0;
}