#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "malloc.h"
#include "string.h"

struct Angajat {
	int id;
	char* nume;
	char* adresa;
	int nrOreMuncite;
	int zileDeConcediuValabile;
};

Angajat initAngajat(int id, const char* nume, const char* adresa, int nrOreMuncite, int zileDeConcediuValabile) {
	Angajat angajat;

	angajat.id = id;
	angajat.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(angajat.nume, nume);

	angajat.adresa = (char*)malloc(sizeof(char) * (strlen(adresa) + 1));
	strcpy(angajat.adresa, adresa);

	angajat.nrOreMuncite = nrOreMuncite;
	angajat.zileDeConcediuValabile = zileDeConcediuValabile;

	return angajat;
}

void afisareAngajat(Angajat angajat) {
	printf("\n %d %s %s %d %d", angajat.id, angajat.nume, angajat.adresa,
		angajat.nrOreMuncite, angajat.zileDeConcediuValabile);
}

struct Nod {
	Nod* prev;
	Nod* next;
	Angajat angajat;
};

Nod* initNod(Angajat angajat) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->angajat = angajat;
	nou->next = NULL;
	nou->prev = NULL;

	return nou;
}

struct ListaDubla {
	Nod* first;
	Nod* last;
};

void adaugaInListaDubla(ListaDubla &lista, Angajat angajat) {
	Nod* nou = initNod(angajat);
	if (lista.first == NULL) {
		lista.first = lista.last = nou;
	}
	else {
		lista.last->next = nou;
		nou->prev = lista.last;
		lista.last = nou;
	}
}

void afisareListaSD(ListaDubla lista) {
	printf("\nAfisare stanga-dreapta");
	Nod* temp = lista.first;
	while (temp) {
		afisareAngajat(temp->angajat);
		temp = temp->next;
	}
}

void afisareListaDS(ListaDubla lista) {
	printf("\nAfisare dreapta-stanga");
	Nod* temp = lista.last;
	while (temp) {
		afisareAngajat(temp->angajat);
		temp = temp->prev;
	}
}

void dezalocareLista(ListaDubla& lista) {
	Nod* p = lista.first;
	while (p) {
		free(p->angajat.adresa);
		free(p->angajat.nume);
		Nod* temp = p;
		p = p->next;
		free(temp);
	}
	lista.first = NULL;
	lista.last = NULL;
}

void citireAngajatDinFisier(const char* numeFisier, ListaDubla &lista) {

	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		Angajat angajat;

		char comma;
		char buffer[50];

		fscanf(file, "%d", &angajat.id);
		fscanf(file, "%c", &comma);
		fscanf(file, "%[^,]s", &buffer);
		angajat.nume = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(angajat.nume, buffer);
		fscanf(file, "%c", &comma);
		fscanf(file, "%[^,]s", &buffer);
		angajat.adresa = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(angajat.adresa, buffer);
		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &angajat.nrOreMuncite);
		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &angajat.zileDeConcediuValabile);
		fscanf(file, "%c", &comma);

		adaugaInListaDubla(lista, angajat);
	}
	fclose(file);
}

void stergereAngajatiDupaOreMuncite(ListaDubla& lista, int numarOre) {
	Nod* temp = lista.first;
	while (temp) {
		if (temp->angajat.nrOreMuncite < numarOre) {
			if (temp->prev == NULL) {
				Nod* aux = temp;
				temp = temp->next;
				lista.first = temp;
				temp->prev = NULL;
				free(aux->angajat.adresa);
				free(aux->angajat.nume);
				free(aux);
			}
			else if (temp->next == NULL) {
				Nod* aux = temp;
				temp = temp->prev;
				lista.last = temp;
				temp->next = NULL;
				free(aux->angajat.adresa);
				free(aux->angajat.nume);
				free(aux);
			}
			else {
				Nod* aux = temp;
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				temp = temp->next;
				free(aux->angajat.adresa);
				free(aux->angajat.nume);
				free(aux);
			}
		}
		else temp = temp->next;
	}
}

struct NodArbore {
	Angajat angajat;
	NodArbore* stanga;
	NodArbore* dreapta;
};

NodArbore* initNodArbore(Angajat angajat) {
	NodArbore* nou = (NodArbore*)malloc(sizeof(Nod));
	nou->dreapta = NULL;
	nou->stanga = NULL;
	nou->angajat = initAngajat(angajat.id, angajat.nume, angajat.adresa, angajat.nrOreMuncite, angajat.zileDeConcediuValabile);
	return nou;
}

NodArbore* inserareInArbore(NodArbore* radacina, Angajat angajat) {
	if (radacina) {
		NodArbore* aux = radacina;
		while (1) {
			if (aux->angajat.id > angajat.id) {
				if (aux->stanga == NULL) {
					aux->stanga = initNodArbore(angajat);
					return radacina;
				}
				else aux = aux->stanga;
			}
			else if (aux->dreapta == NULL) {
				aux->dreapta = initNodArbore(angajat);
				return radacina;
			}
			else aux = aux->dreapta;
		}
	}
	else radacina = initNodArbore(angajat);
	return radacina;
}


void dezalocareArbore(NodArbore* &radacina) {
	if (radacina) {
		dezalocareArbore(radacina->stanga);
		dezalocareArbore(radacina->dreapta);
		free(radacina);
		radacina = NULL;
	}
}

void copiereAngajatiDupaZileDeConcediu(ListaDubla lista, NodArbore* &radacina, int zileConcediu) {
	Nod* temp = lista.first;

	while (temp) {
		if (temp->angajat.zileDeConcediuValabile > zileConcediu) {
			radacina = inserareInArbore(radacina, temp->angajat);
		}
		temp = temp->next;
	}
}

void traversarePreordine(NodArbore* radacina) {
	if (radacina) {
		afisareAngajat(radacina->angajat);
		traversarePreordine(radacina->stanga);
		traversarePreordine(radacina->dreapta);
	}
}

Angajat* traversareSiSalvareInVector(NodArbore* radacina, int id, Angajat* &vecAngajati, int &count) {
	if (radacina) {
		NodArbore* temp = radacina;
		if (temp->angajat.id == id) {
			return vecAngajati;
		}
		else if (temp->angajat.id > id && temp!=NULL) {
			count++;
			vecAngajati = (Angajat*)realloc(vecAngajati,  sizeof(Angajat) * count);
			vecAngajati[count - 1] = temp->angajat;
			vecAngajati = traversareSiSalvareInVector(radacina->stanga, id, vecAngajati, count);
		}
		else {
			count++;
			vecAngajati = (Angajat*)realloc(vecAngajati, sizeof(Angajat) * count);
			vecAngajati[count - 1] = temp->angajat;
			vecAngajati = traversareSiSalvareInVector(radacina->dreapta, id, vecAngajati, count);
		}
	}
}

void dezalocareVector(Angajat* vecAngajati, int count) {
	for (int i = 0; i < count; i++) {
		free(vecAngajati[i].adresa);
		free(vecAngajati[i].nume);
	}
	free(vecAngajati);
	count = 0;
}

void main() {

	const char* numeFisier = "angajati.txt";
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	printf("\n----1----");
	citireAngajatDinFisier(numeFisier, lista);
	afisareListaSD(lista);
	afisareListaDS(lista);
	printf("\n\n2 - Se vor sterge angajatii care au mai putin de 120 de ore lucrate.");
	stergereAngajatiDupaOreMuncite(lista, 120);
	afisareListaSD(lista);
	afisareListaDS(lista);

	printf("\n\n3 - Se vor adauga in BST doar angajatii cu mai mult de 14 zile de concediu.");
	NodArbore* radacina = NULL;
	copiereAngajatiDupaZileDeConcediu(lista, radacina, 14);
	traversarePreordine(radacina);

	printf("\n\n4 - Vectorul contine drumul pana la angajatul cu id-ul 5.");
	Angajat* vecAngajati = (Angajat*)malloc(sizeof(Angajat));
	int count = 0;
	vecAngajati = traversareSiSalvareInVector(radacina, 5, vecAngajati, count);
	for (int i = 0; i < count; i++) {
		afisareAngajat(vecAngajati[i]);
	}

	
	printf("\n5 - Dezalocare lista dublu inlantuita, bst si vector\n");
	dezalocareVector(vecAngajati, count);
	dezalocareArbore(radacina);
	dezalocareLista(lista);
}