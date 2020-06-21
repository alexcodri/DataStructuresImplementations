#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "malloc.h"
#include "string.h"

struct AparatElectrocasnic {
	int id;
	float pret;
	char* denumire;
	char* dataCumpararii;
};

AparatElectrocasnic initAparatElectrocasnic(int id, float pret, char* den, char* data) {
	AparatElectrocasnic aparat;

	aparat.id = id;
	aparat.pret = pret;
	aparat.denumire = (char*)malloc(sizeof(char) * (strlen(den) + 1));
	strcpy(aparat.denumire, den);

	aparat.dataCumpararii = (char*)malloc(sizeof(char) * (strlen(data) + 1));
	strcpy(aparat.dataCumpararii, data);
	
	return aparat;
}

void afisareAparatElectrocasnic(AparatElectrocasnic aparat) {
	printf("\nID %d, pret %.2f, denumire %s, data cumpararii %s.", aparat.id, aparat.pret,
		aparat.denumire, aparat.dataCumpararii);
}

struct NodArbore {
	AparatElectrocasnic aparat;
	NodArbore* stanga;
	NodArbore* dreapta;
};

NodArbore* initNodArbore(AparatElectrocasnic aparat) {
	NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
	nou->stanga = NULL;
	nou->dreapta = NULL;
	nou->aparat = aparat;

	return nou;
}

NodArbore* inserareInArbore(NodArbore* radacina, AparatElectrocasnic aparat) {
	if (radacina) {
		NodArbore* aux = radacina;
		while (1) {
			if (aux->aparat.pret > aparat.pret) {
				if (aux->stanga == NULL) {
					aux->stanga = initNodArbore(aparat);
					return radacina;
				}
				else aux = aux->stanga;
			}
			else if (aux->dreapta == NULL) {
				aux->dreapta = initNodArbore(aparat);
				return radacina;
			}
			else aux = aux->dreapta;
		}
	}
	else radacina = initNodArbore(aparat);
	return radacina;
}

void citireAparateDinFisierSiInserareInArbore(const char* numeFisier, NodArbore* &radacina) {
	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		AparatElectrocasnic aparat;
		char comma;
		char buffer[50];

		fscanf(file, "%d", &aparat.id);
		fscanf(file, "%c", &comma);
		fscanf(file, "%f", &aparat.pret);
		fscanf(file, "%c", &comma);
		fscanf(file, "%[^,]s", &buffer);

		aparat.denumire = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(aparat.denumire, buffer);

		fscanf(file, "%c", &comma);
		fscanf(file, "%[^\n]s", &buffer);

		aparat.dataCumpararii = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(aparat.dataCumpararii, buffer);
		fscanf(file, "%c", &comma);

		radacina = inserareInArbore(radacina, aparat);
	}
	fclose(file);
}

void afisareArbore(NodArbore* radacina) {
	if (radacina) {
		afisareAparatElectrocasnic(radacina->aparat);
		afisareArbore(radacina->stanga);
		afisareArbore(radacina->dreapta);
	}
}

void dezalocareArbore(NodArbore* radacina) {
	if (radacina) {
		dezalocareArbore(radacina->stanga);
		dezalocareArbore(radacina->dreapta);
		free(radacina->aparat.denumire);
		free(radacina->aparat.dataCumpararii);
		free(radacina);
	}
}

float gasesteCelMaiMicPret(NodArbore* radacina,float &pret) {
	if (radacina) {
		if (pret == 0) {
			pret = radacina->aparat.pret;
		}
		if (radacina->aparat.pret < pret) {
			pret = radacina->aparat.pret;
		}
		gasesteCelMaiMicPret(radacina->stanga, pret);
		gasesteCelMaiMicPret(radacina->dreapta, pret);
	}
	return pret;
}

float gasesteCelMaiMarePret(NodArbore* radacina, float &pret){
	if (radacina) {
		if (radacina->aparat.pret > pret) {
			pret = radacina->aparat.pret;
		}
		gasesteCelMaiMarePret(radacina->stanga, pret);
		gasesteCelMaiMarePret(radacina->dreapta, pret);
	}
	return pret;
}

NodArbore* stergere_nod_radacina(NodArbore*& radacina)
{
	NodArbore* nod = radacina;
	if (nod->dreapta)
	{
		radacina = nod->dreapta;
		if (nod->stanga)
		{
			NodArbore* temp = nod->dreapta;
			while (temp->stanga != NULL)
				temp = temp->stanga;
			temp->stanga = nod->stanga;
		}
	}
	else if (nod->stanga != NULL) {
		radacina = nod->stanga;
	}
	else {
		radacina = NULL;
	}
	free(nod);
	return radacina;
}

void stergereNod(NodArbore* &radacina, float pret) {
	if (radacina) {
		if (radacina->aparat.pret == pret) {
			AparatElectrocasnic aux = radacina->aparat;
			stergere_nod_radacina(radacina);
			printf("\nNodul sters continea aparatul:");
			afisareAparatElectrocasnic(aux);
		}
		else {
			NodArbore* nodCurent = radacina;
			if (pret < nodCurent->aparat.pret) {
				stergereNod(nodCurent->stanga, pret);
			}
			else {
				stergereNod(nodCurent->dreapta, pret);
			}
		}
	}
}

void modificarePret(NodArbore*& radacina, float pretMax, float pretMin, AparatElectrocasnic& aux) {
	if (radacina) {
		if (radacina->aparat.pret == pretMax) {
			 aux = radacina->aparat;
			stergere_nod_radacina(radacina);
			aux.pret = pretMin - 1;
		}
		else {
			NodArbore* nodCurent = radacina;
			if (pretMax < nodCurent->aparat.pret) {
				modificarePret(nodCurent->stanga, pretMax,pretMin, aux);
			}
			else {
				modificarePret(nodCurent->dreapta, pretMax,pretMin, aux);
			}
		}
	}

}

struct NodLista {
	NodLista* next;
	AparatElectrocasnic aparat;
};

NodLista* initNodLista(AparatElectrocasnic aparat) {
	NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
	nou->next = NULL;
	nou->aparat = initAparatElectrocasnic(aparat.id, aparat.pret, aparat.denumire, aparat.dataCumpararii);

	return nou;
}

NodLista* adaugareInLista(NodLista* &cap, AparatElectrocasnic aparat) {
	if (cap) {
		NodLista* cursor = cap;
		while (cursor->next) {
			cursor = cursor->next;
		}
		cursor->next = initNodLista(aparat);
		return cap;
	}
	else cap = initNodLista(aparat);
	return cap;
}

void afisareLista(NodLista* cap) {
	if (cap) {
		NodLista* cursor = cap;
		while (cursor) {
			afisareAparatElectrocasnic(cursor->aparat);
			cursor = cursor->next;
		}
	}
}

void dezalocareLista(NodLista* cap) {
	if (cap) {
		while (cap) {
			NodLista* cursor = cap;
			cap = cap->next;
			free(cursor->aparat.dataCumpararii);
			free(cursor->aparat.denumire);
			free(cursor);
		}
	}
}

void adaugaInLista(NodLista*& cap, NodArbore* radacina, int id) {
	if (radacina) {
		if (radacina->aparat.id > id) {
			cap = adaugareInLista(cap, radacina->aparat);
		}
		adaugaInLista(cap, radacina->stanga, id);
		adaugaInLista(cap, radacina->dreapta, id);
	}
}

void main() {
	NodArbore* radacina = NULL;
	const char numeFisier[20] = "aparate.txt";
	citireAparateDinFisierSiInserareInArbore(numeFisier, radacina);
	printf("\n\tEx1.");
	afisareArbore(radacina);

	printf("\n\n\tEx2.");
	float pretMin = 0;
	printf("\n Pretul cel mai mic din arbore este: %.2f.", gasesteCelMaiMicPret(radacina,pretMin));
	printf("\n\tEx3.");
	float pretMax = 0;
	pretMax = gasesteCelMaiMarePret(radacina, pretMax);
	stergereNod(radacina, pretMax);
	afisareArbore(radacina);
	printf("\n\tEx4");
	pretMax = 0;
	pretMax = gasesteCelMaiMarePret(radacina, pretMax);
	AparatElectrocasnic aux;
	modificarePret(radacina, pretMax, pretMin, aux);
	inserareInArbore(radacina, aux);
	afisareArbore(radacina);

	printf("\n\tEx5.");
	printf("\nSe vor adauga in lista toate aparatele care au id-ul mai mare ca 6.");
	NodLista* cap = NULL;
	adaugaInLista(cap, radacina, 6);
	afisareLista(cap);

	printf("\n\tEx6. Dezalocari.");
	dezalocareArbore(radacina);
	dezalocareLista(cap);
}