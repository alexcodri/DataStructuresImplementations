#define _CRT_SECURE_NO_WARNINGS

#include "malloc.h"
#include "stdio.h"
#include "string.h"

struct Task {
	int id;
	char* descriere;
	char* numeEchipa;
	int numarZile;
	int numarProgramatori;
};

Task initTask(int id, char* descriere, char* nume, int nrZile, int nrProgramatori) {
	Task task;

	task.id = id;
	task.numarProgramatori = nrProgramatori;
	task.numarZile = nrZile;

	task.descriere = (char*)malloc(sizeof(char) * (strlen(descriere) + 1));
	strcpy(task.descriere, descriere);

	task.numeEchipa = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(task.numeEchipa, nume);

	return task;
}

void afisareTask(Task task) {
	printf("\nID: %d, descriere %s, nume echipa %s, numar zile %d, numar programatori %d", task.id,
		task.descriere, task.numeEchipa, task.numarZile, task.numarProgramatori);
}

struct Nod {
	Task task;
	Nod* stanga;
	Nod* dreapta;
	int gradEchilibru;
};

Nod* initNod(Task task) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->dreapta = NULL;
	nou->stanga = NULL;
	nou->task = task;
	nou->gradEchilibru = 0;

	return nou;
}

void traversarePreordine(Nod* radacina) {
	if (radacina) {
		afisareTask(radacina->task);
		traversarePreordine(radacina->stanga);
		traversarePreordine(radacina->dreapta);
	}
}

int inaltimeArbore(Nod* radacina) {
	if (radacina) {
		int inaltimeStanga = inaltimeArbore(radacina->stanga);
		int inaltimeDreapta = inaltimeArbore(radacina->dreapta);

		return 1 + (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta);
	}
}

int gradEchilibru(Nod* radacina) {
	if (radacina) {
		int inaltimeStanga = inaltimeArbore(radacina->stanga);
		int inaltimeDreapta = inaltimeArbore(radacina->stanga);

		return inaltimeStanga - inaltimeDreapta;
	}
	else return 0;
}

Nod* rotireStanga(Nod* radacina) {
	if (radacina) {
		Nod* copilDreapta = radacina->dreapta;
		radacina->dreapta = copilDreapta->stanga;
		copilDreapta->stanga = radacina;

		radacina->gradEchilibru = gradEchilibru(radacina);
		copilDreapta->gradEchilibru = gradEchilibru(copilDreapta);
		return copilDreapta;
	}
	else return radacina;
}

Nod* rotireDreapta(Nod* radacina) {
	if (radacina) {
		Nod* copilStanga = radacina->stanga;
		radacina->stanga = copilStanga->dreapta;
		copilStanga->dreapta = radacina;

		radacina->gradEchilibru = gradEchilibru(radacina);
		copilStanga->gradEchilibru = gradEchilibru(radacina);

		return copilStanga;
	}
	else return radacina;
}

Nod* rotireStangaDreapta(Nod* radacina) {
	if (radacina) {
		radacina->stanga = rotireStanga(radacina->stanga);
		return rotireDreapta(radacina);
	}
	else return radacina;
}

Nod* rotireDreaptaStanga(Nod* radacina) {
	if (radacina) {
		radacina->dreapta = rotireDreapta(radacina->dreapta);
		return rotireStanga(radacina);
	}
	else return radacina;
}

Nod* echilibrare(Nod* radacina) {
	if (radacina) {
		if (radacina->gradEchilibru == -2) {
			return radacina->dreapta->gradEchilibru == -1 ? rotireStanga(radacina) : rotireDreaptaStanga(radacina);
		}
		else if (radacina->gradEchilibru == 2) {
			return radacina->stanga->gradEchilibru == 1 ? rotireDreapta(radacina) : rotireStangaDreapta(radacina);
		}
	}
	else return radacina;
}

Nod* inserareNod(Nod* &radacina, Task task) {
	if (radacina) {
		if (radacina->task.numarZile > task.numarZile) {
			radacina->stanga = inserareNod(radacina->stanga, task);
		}
		else radacina->dreapta = inserareNod(radacina->dreapta, task);
		return echilibrare(radacina);
	}
	else radacina =  initNod(task);

	return radacina;
}

void citireFisierSiInserareTask(const char* numeFisier, Nod* &radacina) {
	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		Task task;
		char comma;
		char buffer[50];

		fscanf(file, "%d", &task.id);

		fscanf(file, "%c", &comma);
		fscanf(file, "%[^,]s", &buffer);
		task.descriere = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(task.descriere, buffer);

		fscanf(file, "%c", &comma);
		fscanf(file, "%[^,]s", &buffer);
		task.numeEchipa = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(task.numeEchipa, buffer);

		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &task.numarZile);


		fscanf(file, "%c", &comma);
		fscanf(file, "%d", &task.numarProgramatori);
		fscanf(file, "%c", &comma);

		inserareNod(radacina, task);
	}
	fclose(file);
}

void dezalocareArbore(Nod* radacina) {
	if (radacina) {
		dezalocareArbore(radacina->stanga);
		dezalocareArbore(radacina->dreapta);
		free(radacina->task.numeEchipa);
		free(radacina->task.descriere);
		free(radacina);
	}
}

struct NodLista {
	NodLista* next;
	NodLista* prev;
	Task task;
};

NodLista* initNodLista(Task task) {
	NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
	nou->next = NULL;
	nou->prev = NULL;
	nou->task = initTask(task.id, task.descriere, task.numeEchipa, task.numarZile,task.numarProgramatori);
	
	return nou;
}

struct ListaDubla {
	NodLista* first;
	NodLista* last;
};

void afisareListaDubla(ListaDubla nod) {
	NodLista* cursor = nod.first;
	while (cursor) {
		afisareTask(cursor->task);
		cursor = cursor->next;
	}
}

void dezalocareListaDubla(ListaDubla nod) {
	while (nod.first) {
		NodLista* cursor = nod.first;
		nod.first = nod.first->next;
		free(cursor->task.descriere);
		free(cursor->task.numeEchipa);
		free(cursor);
	}
}

ListaDubla inserareInListaDubla(ListaDubla &lista, Task task) {
	NodLista* nou = initNodLista(task);
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

void adaugareInListaDubla(ListaDubla &lista, Nod* radacina) {
	if (radacina) {
		lista = inserareInListaDubla(lista, radacina->task);
		adaugareInListaDubla(lista, radacina->stanga);
		adaugareInListaDubla(lista, radacina->dreapta);
	}
}

NodLista* stergereNodDinLista(ListaDubla& lista, NodLista* nod, Task &taskAux) {
	
	if (lista.first->task.id == nod->task.id) {
		NodLista* aux = lista.first;
		taskAux = initTask(aux->task.id, aux->task.descriere,
			aux->task.numeEchipa,aux->task.numarZile, aux->task.numarProgramatori);
		lista.first = aux->next;
		lista.first->prev = NULL;
		aux->next = NULL;
		free(aux->task.descriere);
		free(aux->task.numeEchipa);
		free(aux);
		return lista.first;
	}
	else if (lista.last->task.id == nod->task.id){
		NodLista* aux = lista.last;
		taskAux = initTask(aux->task.id, aux->task.descriere,
			aux->task.numeEchipa, aux->task.numarZile, aux->task.numarProgramatori);
		lista.last = aux->prev;
		lista.last->next = NULL;
		aux->prev = NULL;
		free(aux->task.descriere);
		free(aux->task.numeEchipa);
		free(aux);
		return lista.last;
	}
	else {
		NodLista* cursor = lista.first;
		while (cursor->task.id != nod->task.id) {
			cursor = cursor->next;
		}
		NodLista* aux = cursor;
		taskAux = initTask(aux->task.id, aux->task.descriere,
			aux->task.numeEchipa, aux->task.numarZile, aux->task.numarProgramatori);
		cursor = cursor->prev;
		cursor->next = aux->next;
		aux->next->prev = cursor;
	
		free(aux->task.descriere);
		free(aux->task.numeEchipa);
		free(aux);
		return cursor;
	}
}

void extragereInVector(ListaDubla& lista, Task* &vecTask, int& count, int nrProgramatori, Task &aux) {
	NodLista* cursor = lista.first;
	while (cursor) {
		if (cursor->task.numarProgramatori == nrProgramatori) {
			cursor = stergereNodDinLista(lista, cursor, aux);
			count++;
			vecTask = (Task*)realloc(vecTask, count * sizeof(Task));
			vecTask[count - 1] = aux;
			cursor = cursor->next;
		}
		else {
			cursor = cursor->next;
		}
		
	}
}

void dezalocareVector(Task*& vecTask, int& count) {
	for (int i = 0; i < count; i++) {
		free(vecTask[i].descriere);
		free(vecTask[i].numeEchipa);
	}
	free(vecTask);
	count = 0;
}

void main() {
	Nod* radacina = NULL;
	const char numeFisier[20] = "taskuri.txt";
	citireFisierSiInserareTask(numeFisier, radacina);
	printf("\nEx1.\n");
	traversarePreordine(radacina);

	printf("\nEx2.\n");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	adaugareInListaDubla(lista, radacina);
	afisareListaDubla(lista);

	printf("\n\nEx4. Extragere taskuri in vector.\n");
	Task* vecTask = (Task*)malloc(sizeof(Task));
	int count = 0;
	Task aux;
	extragereInVector(lista, vecTask, count, 5, aux);
	for (int i = 0; i < count; i++) {
		afisareTask(vecTask[i]);
	}
	printf("\n");
	afisareListaDubla(lista);

	printf("\n\nEx.5\nDezalocari");
	dezalocareArbore(radacina);
	dezalocareListaDubla(lista);
	dezalocareVector(vecTask, count);
}