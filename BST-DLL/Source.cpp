#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "malloc.h"
#include "string.h"

struct Camera {
	char* tipCamera;
	int etaj;
	int id;
	int numarDormitoare;
	char* numeCamera;
};

Camera initCamera(int id, int etaj, int nrDorm,const char* tip,const char* nume) {
	Camera camera;

	camera.id = id;
	camera.etaj = etaj;
	camera.numarDormitoare = nrDorm;

	camera.tipCamera = (char*)malloc(sizeof(char) * (strlen(tip) + 1));
	strcpy(camera.tipCamera, tip);

	camera.numeCamera = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(camera.numeCamera, nume);

	return camera;
}

void afisareCamera(Camera camera) {
	printf("ID %d, etaj %d, numar dormitoare %d, tip %s, nume: %s.\n", camera.id, camera.etaj, camera.numarDormitoare,
		camera.tipCamera, camera.numeCamera);
}

struct NodArbore {
	NodArbore* stanga;
	NodArbore* dreapta;
	Camera camera;
};

NodArbore* initNodArbore(Camera camera) {
	NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
	nou->stanga = NULL;
	nou->dreapta = NULL;

	nou->camera = camera;

	return nou;
}

NodArbore* inserareInArbore(NodArbore*& radacina, Camera camera) {
	if (radacina) {
		NodArbore* aux = radacina;

		while (1) {
			if (aux->camera.id > camera.id) {
				if (aux->stanga == NULL) {
					aux->stanga = initNodArbore(camera);
					return radacina;
				}
				else aux = aux->stanga;
			}
			else if (aux->dreapta == NULL) {
				aux->dreapta = initNodArbore(camera);
				return radacina;
			}
			else aux = aux->dreapta;
		}
	}
	else radacina = initNodArbore(camera);
	return radacina;
}

void citireCameraDinFisier(const char* numeFisier, NodArbore*& radacina) {
	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		Camera camera;

		char comma;
		char buffer[50];

		fscanf(file, "%d", &camera.id);
		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &camera.etaj);
		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &camera.numarDormitoare);
		fscanf(file, "%c", &comma);

		fscanf(file, "%[^,]s", &buffer);
		camera.tipCamera = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(camera.tipCamera, buffer);

		fscanf(file, "%c", &comma);
		fscanf(file, "%[^\n]s", &buffer);
		camera.numeCamera = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(camera.numeCamera, buffer);
		fscanf(file, "%c", &comma);

		radacina = inserareInArbore(radacina, camera);
	}
	fclose(file);
}

void traversarePreordine(NodArbore* radacina) {
	if (radacina) {
		afisareCamera(radacina->camera);
		traversarePreordine(radacina->stanga);
		traversarePreordine(radacina->dreapta);
	}
}

int numarulDeEtajeExistente(NodArbore* radacina, int& countEtaje) {
	if (radacina) {
		if (radacina->camera.etaj > countEtaje) {
			countEtaje = radacina->camera.etaj;
		}
		numarulDeEtajeExistente(radacina->stanga, countEtaje);
		numarulDeEtajeExistente(radacina->dreapta, countEtaje);	
		return countEtaje;
	}
}

void numarDeCamerePerEtaj(NodArbore* radacina, int* &vecEtaje) {
	if (radacina) {
		vecEtaje[radacina->camera.etaj]++;
		numarDeCamerePerEtaj(radacina->stanga, vecEtaje);
		numarDeCamerePerEtaj(radacina->dreapta, vecEtaje);
	}
}

struct NodListaDubla {
	NodListaDubla* next;
	NodListaDubla* prev;
	Camera camera;
};

struct ListaDubla {
	NodListaDubla* first;
	NodListaDubla* last;
};

NodListaDubla* initNod(Camera camera) {
	NodListaDubla* nou = (NodListaDubla*)malloc(sizeof(NodListaDubla));
	nou->next = NULL;
	nou->prev = NULL;
	nou->camera = initCamera(camera.id, camera.etaj, camera.numarDormitoare, camera.tipCamera, camera.numeCamera);

	return nou;
}

ListaDubla inserareInListaDubla(ListaDubla &lista, Camera camera) {
	NodListaDubla* nou = initNod(camera);
	nou->prev = lista.last;


	if (lista.first == NULL) {
		lista.first = lista.last = nou;
	}
	else {
		lista.last->next = nou;
		lista.last = nou;
	}
	return lista;
}

void adaugareInListaDubla(NodArbore* radacina, ListaDubla& lista, int etaj) {
	if (radacina) {
		if (radacina->camera.etaj >= etaj) {
			lista = inserareInListaDubla(lista, radacina->camera);
		}
		adaugareInListaDubla(radacina->stanga, lista, etaj);
		adaugareInListaDubla(radacina->dreapta, lista, etaj);
	}
}

void afisareListaDubla(ListaDubla lista) {
	NodListaDubla* cursor = lista.first;
	while (cursor) {
		afisareCamera(cursor->camera);
		cursor = cursor->next;
	}
}

void inserareNodInListaDubla(ListaDubla& lista, Camera camera, int id) {
	 NodListaDubla* cursor = lista.first;
	 while (cursor) {
		 if (cursor->camera.id == id) {
			 NodListaDubla* nou = initNod(camera);
			 NodListaDubla* nouNext = cursor->next;
			 cursor->next = nou;
			 nou->prev = cursor;
			 nouNext->prev = nou;
			 nou->next = nouNext;
			 break;
		 }
		 cursor = cursor->next;
	 }
}

void dezalocareArbore(NodArbore*& radacina) {
	if (radacina) {
		dezalocareArbore(radacina->stanga);
		dezalocareArbore(radacina->dreapta);
		free(radacina->camera.numeCamera);
		free(radacina->camera.tipCamera);
		free(radacina);
	}
}

void dezalocareLista(ListaDubla lista) {
	NodListaDubla* cursor = lista.first;
	while (cursor) {
		NodListaDubla* aux = cursor;
		cursor = cursor->next;
		free(aux->camera.numeCamera);
		free(aux->camera.tipCamera);
		free(aux);
	}
	free(cursor);
	lista.first = NULL;
	lista.last = NULL;

}

void main() {

	NodArbore* radacina = NULL;
	char numeFisier[50] = "hotel.txt";

	citireCameraDinFisier(numeFisier, radacina);
	traversarePreordine(radacina);
	int count = numarulDeEtajeExistente(radacina, count);

	int* vecEtaje = (int*)malloc(sizeof(int) * count);
	for (int i = 0; i < count+1; i++) {
		vecEtaje[i] = 0;
	}
	numarDeCamerePerEtaj(radacina, vecEtaje);
	printf("\n2 ----");
	for (int i = 0; i < count+1; i++) {
		printf("\n In cadrul etajului %d sunt %d camere.", i, vecEtaje[i]);
	}

	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	printf("\n\n3 - Inserarea in lista dubla doar a camerelor care sunt de la etajul 3 in sus.\n");
	adaugareInListaDubla(radacina, lista, 3);
	afisareListaDubla(lista);

	printf("\n4 - Inserare camera noua dupa camera cu id-ul 3\n");

	inserareNodInListaDubla(lista, initCamera(12, 2, 1, "Deluxe", "Camera12"),3);
	afisareListaDubla(lista);
	printf("\nDezalocari");
	dezalocareArbore(radacina);
	dezalocareLista(lista);
}