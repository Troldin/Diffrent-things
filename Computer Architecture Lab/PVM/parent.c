/* Sklad sekcji */
/* Marek Konopka */
/* Kamil Woldan */
#include <stdio.h>
#include <stdlib.h>
#include "pvm3.h"
int static kolumny = 3;
int static wiersze = 4;
main()
{
	int wynik = 0, ilhost, ilarch;
	int i, j, procnr, tmp = -1;
	int tidmaster, tid;
	int bytes, msgtag, tmp_tid;
	struct pvmhostinfo* info;
	/* wczytanie danych */
	int macierz_a[wiersze][kolumny] = { {1,2,3},{4,5,6},{7,8,9},{7,8,9} };
	int macierz_b[wiersze][kolumny] = { {1,2,3},{4,5,6},{7,8,9},{7,8,9} };
	printf("Macierze wejsciowe");
	printf("\n");
	for (i = 0; i < wiersze; i++) {
		for (j = 0; j < kolumny; j++) {
			printf("%d", macierz_a[i][j]);
			printf("+");
			printf("%d  ", macierz_b[i][j]);
		}
		printf("\n");
	}
	int macierz_wynik[wiersze][kolumny];
	pvm_config(&ilhost, &ilarch, &info);
	/* zg³oszenie procesu do systemu PVM */
	if ((tidmaster = pvm_mytid()) < 0) {
		pvm_perror("enroll");
		exit(1);
	}
	for (j = 0; j < ilhost; j++) {
		/* powo³anie nowego procesu-potomka o kodzie w pliku vect_srv */
		procnr = pvm_spawn("/home/pvm/pvm3/lab/child", 0, PvmTaskHost, info[0].hi_name, 1, &tid);
		tmp++;
		/* przes³anie danych pocz¹tkowych do procesu-potomka */
		pvm_initsend(PvmDataDefault);
		for (i = 0; i < kolumny; i++) {
			pvm_pkint(&macierz_a[j][i], 1, 1);
			pvm_pkint(&macierz_b[j][i], 1, 1);
		}
		pvm_send(tid, 100);
	}

	while (tmp != wiersze - 1) {
		/*Odebranie i zapisanie bufora*/
		int bufid = pvm_recv(-1, 200);
		for (j = 0; j < kolumny; j++) {
			pvm_upkint(&macierz_wynik[tmp][j], 1, 1);
		}
		/*Oczytanie informacji z bufora*/
		pvm_bufinfo(bufid, &bytes, &msgtag, &tmp_tid);
		tmp++;
		/*Wyczyszczenie bufora*/
		pvm_initsend(PvmDataDefault);
		/*Dodanie do bufora nastepnych danych*/
		for (i = 0; i < kolumny; i++) {
			pvm_pkint(&macierz_a[tmp][i], 1, 1);
			pvm_pkint(&macierz_b[tmp][i], 1, 1);
		}
		/*Wyslanie bufora*/
		pvm_send(tmp_tid, 100);
	}
	/* odczytanie danych z procesow-potomkow */
	for (i = 0; i < ilhost; i++) {
		pvm_recv(-1, 200);
		for (j = 0; j < kolumny; j++) {
			pvm_upkint(&macierz_wynik[i + tmp][j], 1, 1);
		}
	}
	printf("Macierz wynikowa \n");
	/* wypisanie wyników */
	for (i = 0; i < wiersze; i++) {
		for (j = 0; j < kolumny; j++) {
			printf("%d ", macierz_wynik[i][j]);
		}
		printf("\n");
	}
	/*Zakoczenie procesow*/
	for (i = 0; i < ilhost; i++)
		pvm_kill(tid);
	pvm_exit();
	exit(0);

}
