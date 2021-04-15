/* Sklad sekcji */
/* Marek Konopka */
/* Kamil Woldan */
#include <stdio.h>
#include "pvm3.h"
#include <stdlib.h>
int static kolumny = 4;
main()
{
	int macierz_a[kolumny], macierz_b[kolumny], wynik[kolumny];
	int masterid;
	while (1) {

		/* kto mnie stworzy³ */
		masterid = pvm_parent();
		/* przyjmij dane do obliczeñ */
		pvm_recv(-1, 100);
		for (int j = 0; j < kolumny; j++) {
			pvm_upkint(&macierz_a[j], 1, 1);
			pvm_upkint(&macierz_b[j], 1, 1);
		}
		/* obliczenia */
		for (int j = 0; j < kolumny; j++) {
			wynik[j] = macierz_a[j] + macierz_b[j];
		}
		/* zwróæ wyniki do procesu nadrzêdnego */
		pvm_initsend(PvmDataDefault);
		for (int j = 0; j < kolumny; j++) {
			pvm_pkint(&wynik[j], 1, 1);
		}
		pvm_send(masterid, 200);
	}
}