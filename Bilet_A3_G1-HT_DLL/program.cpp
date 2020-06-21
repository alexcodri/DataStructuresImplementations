#define _CRT_SECURE_NO_WARNINGS
#include "malloc.h"
#include "stdio.h"
#include "string.h"

struct Gradinita {
	char* cod;
	char* adresa;
	int numarClase;
	int numarCopiiPerClasa;
	int zileDeFunctionare;
};

void afisareGradinita(Gradinita gradinita) {
	printf("\n %s %s %d %d %d", gradinita.cod, gradinita.adresa, gradinita.numarClase,
		gradinita.numarCopiiPerClasa, gradinita.zileDeFunctionare);
}

Gradinita initGradinita(const char* cod, char* ad, int nrClase, int nrCopii, int zile) {
	Gradinita gradinita;

	gradinita.cod = (char*)malloc(sizeof(char) * (strlen(cod) + 1));
	strcpy(gradinita.cod, cod);

	gradinita.adresa = (char*)malloc(sizeof(char) * (strlen(ad) + 1));
	strcpy(gradinita.adresa, ad);

	gradinita.numarClase = nrClase;
	gradinita.numarCopiiPerClasa = nrCopii;
	gradinita.zileDeFunctionare = zile;
	
	return gradinita;
}

struct Element {
	Element* next;
	Gradinita gradinita;
};

Element* initElement(Gradinita gradinita) {
	Element* nou = (Element*)malloc(sizeof(Element));
	nou->gradinita = gradinita;
	nou->next = NULL;

	return nou;
}

int functieHash(int size,const char* cod) {
	return cod[0] % size;
}

struct Hashtable {
	int size;
	Element** elements;
};

Hashtable* initHt(int size) {
	Hashtable* ht = (Hashtable*)malloc(sizeof(Hashtable));
	ht->size = size;
	ht->elements = (Element**)malloc(sizeof(Element*) * size);
	for (int i = 0; i < ht->size; i++) {
		ht->elements[i] = NULL;
	}
	return ht;
}

void inserareHashTable(Hashtable*& ht, Gradinita gradinita) {
	int index = functieHash(ht->size, gradinita.cod);

	if (ht->elements) {

		if (ht->elements[index]) {
			Element* temp = ht->elements[index];
			while (temp->next) {
				temp = temp->next;
			}
			temp->next = initElement(gradinita);

		}
		else ht->elements[index] = initElement(gradinita);

	}
}

void afisareHashtable(Hashtable* ht) {
	if (ht) {
		if (ht->elements) {
			for (int i = 0; i < ht->size; i++) {
				Element* temp = ht->elements[i];
				while (temp) {
					afisareGradinita(temp->gradinita);
					temp = temp->next;
				}
			}
		}
	}
}


void citireGradiniteDingradinita(const char* numegradinita, Hashtable*& ht) {
	FILE* file = fopen(numegradinita, "r");
	while (!feof(file)) {
		Gradinita gradinita;

		char comma;
		char buffer[50];

		fscanf(file, "%[^,]s", &buffer);
		gradinita.cod = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(gradinita.cod, buffer);

		fscanf(file, "%c", &comma);
		fscanf(file, "%[^,]s", &buffer);
		gradinita.adresa = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(gradinita.adresa, buffer);
		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &gradinita.numarClase);
		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &gradinita.numarCopiiPerClasa);
		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &gradinita.zileDeFunctionare);
		fscanf(file, "%c", &comma);

		inserareHashTable(ht, gradinita);
	}
	fclose(file);
}

void modificareCheieGradinita(Hashtable*& ht, const char* codNou, const char* cod) {
	if (ht) {	
		int index = functieHash(ht->size, cod);

			Element* temp = ht->elements[index];
			while (temp->next) {
				if (strcmp(temp->next->gradinita.cod, cod) == 0) {
					Gradinita aux = initGradinita(codNou, temp->next->gradinita.adresa, temp->next->gradinita.numarClase,
						temp->next->gradinita.numarCopiiPerClasa, temp->next->gradinita.zileDeFunctionare);
					Element* tempElement = temp->next;
					temp->next = tempElement->next;
					free(tempElement->gradinita.adresa);
					free(tempElement->gradinita.cod);
					free(tempElement);

					inserareHashTable(ht, aux);
					break;
				}
				temp = temp->next;
			}
	}
}

struct Nod {
	Nod* next;
	Nod* prev;
	Gradinita gradinita;
};

struct ListaDubla {
	Nod* first;
	Nod* last;
};

ListaDubla inserareNod(ListaDubla nod, Gradinita gradinita) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->prev = nod.last;
	nou->gradinita = initGradinita(gradinita.cod, gradinita.adresa,
		gradinita.numarClase, gradinita.numarCopiiPerClasa, gradinita.zileDeFunctionare);

	if (nod.first == NULL) {
		nod.first = nod.last = nou; 
	}
	else {
		nod.last->next = nou;
		nod.last = nou;
	}
	return nod;

}

void afisareListaSD(ListaDubla nod) {
	Nod* temp = nod.first;
	while (temp) {
		afisareGradinita(temp->gradinita);
		temp = temp->next;
	}
}

void copiazaInListaDubla(Hashtable* ht, ListaDubla &lista, int numarCopii) {
	if (ht) {
		for (int i = 0; i < ht->size; i++) {
			Element* temp = ht->elements[i];
			while (temp) {
				if (temp->gradinita.numarCopiiPerClasa > numarCopii) {
					lista = inserareNod(lista, temp->gradinita);
				}
				temp = temp->next;
			}
		}
	}
}

void rupereListaDupaNrOre(ListaDubla &lista, ListaDubla& lista2, int numarOre) {
	Nod* temp = lista.first;
	int ok = 0;
	int count = 0;
	while (temp) {
		if (temp->gradinita.zileDeFunctionare == 7) {
			ok = 1;
		}

		if (ok == 1) {
			lista2 = inserareNod(lista2, temp->gradinita);
			count ++ ;
		}
		temp = temp->next;
	}

	Nod* tempLast = lista.last;

	while (count > 0) {
		Nod* aux = tempLast;
		tempLast = tempLast->prev;
		tempLast->next = NULL;
		lista.last = tempLast;
		free(aux->gradinita.adresa);
		free(aux->gradinita.cod);
		free(aux);
		count--;
	}
}

void dezalocareLista(ListaDubla &lista) {
	Nod* p = lista.first;
	while (p) {
		free(p->gradinita.adresa);
		free(p->gradinita.cod);
		Nod* temp = p;
		p = p->next;
		free(temp);
	}
	lista.first = NULL;
	lista.last = NULL;
}

void stergereLista(Element* &cap) {
	while (cap) {
		Element*aux = cap;
		cap = cap->next;
		free(aux->gradinita.cod);
		free(aux->gradinita.adresa);
		free(aux);
	}
}
	
void stergereHt(Hashtable*& ht) {
	if (ht->size > 0) {
		for (int i = 0; i < ht->size; i++) {
			stergereLista(ht->elements[i]);
		}
		free(ht->elements);
		ht->elements = NULL;
		ht->size = 0;
	}
}


void main() {

	const char numegradinita[20] = "gradinite.txt";
	int size = 5;
	Hashtable* ht = initHt(size);

	citireGradiniteDingradinita(numegradinita, ht);

	afisareHashtable(ht);
	printf("\n\n---2---");
	modificareCheieGradinita(ht, "HMHMH", "B345T");
	afisareHashtable(ht);

	printf("\n\n---3 Se copiaza doar gradinitele care au nr mediu de copii per clasa mai mare de 18.---");
	ListaDubla lista;
	lista.first = lista.last = NULL;
	copiazaInListaDubla(ht, lista, 18);
	afisareListaSD(lista);

	printf("\nCele doua structuri nu partajeaza zone de memorie, in ht fiind schimbat codul QWERT cu RERERE.");
	modificareCheieGradinita(ht, "RERERE", "QWERT");
	afisareListaSD(lista);

	ListaDubla lista2;
	lista2.first = lista2.last = NULL;
	

	printf("\n\n-----4 Lista se va rupe incepand cu gradinita care are disponibilitate 7 zile\n");
	rupereListaDupaNrOre(lista, lista2, 7);
	printf("Prima lista:");
	afisareListaSD(lista);
	printf("\nA doua lista:");
	afisareListaSD(lista2);
	
	printf("\n\n----5 Dezalocare structuri---");
	dezalocareLista(lista);
	dezalocareLista(lista2);
	stergereHt(ht);
}