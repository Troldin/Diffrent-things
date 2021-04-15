#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <conio.h>
using namespace std;
struct lista_w {
	int w;
	double o;
	lista_w *nastepny;
};
const int infinity = 2147483647;
void argumenty_wiersza_polecen(int argc, char *argv[]);
bool if_int(string x, int i);
int zliczanie_linii(fstream &plik);
int zliczanie_wierzcho³kow(string *tab, int x);

int main(int argc, char ** argv) {
	argumenty_wiersza_polecen(argc, argv);
	int lk;                                       /*liczba krawêdzi*/
	int lw;                                       /*liczba wierzcho³ków*/
	int x, y;                                     /*wierzcho³ek startowy, koncowy, dlugosc*/
	double d;
	int j, u;                                     /*zmienne pomocnicze algorytmu*/
	int ls;                                       /*Ilosc wierzcholkow do obliczenia*/
	int ws  ;                                     /*wiercho³ek startowy*/
	bool *QS;                                     /*Wskaznik zbioru QS*/
	int *droga, *pop;                             /*wskaznik drogi oraz poprzedników*/
	lista_w *pointer, *pointer2;                  /*wskaznik do struktury*/
	lista_w **graf;                               /* graf w postaci list sasiedztwa*/
	string plik_graf = "C:\\Users\\Kamil\\source\\repos\\Projekt_Algorytm_Dijkstry\\Pliki\\plik_graf.txt";
	string plik_szukanych = "C:\\Users\\Kamil\\source\\repos\\Projekt_Algorytm_Dijkstry\\Pliki\\plik_szukanych.txt";
	string plik_wynik = "C:\\Users\\Kamil\\source\\repos\\Projekt_Algorytm_Dijkstry\\Pliki\\plik_wynik.txt";
	string z;
	string linia;
	int sptr = 0;
	int *S;

	/*Otwieranie plike z grafem*/

	fstream o_plik_graf;
otwieranie1:
	o_plik_graf.open(plik_graf, ios::in || ios::out);
	if (!o_plik_graf.is_open()) {
		cout << "Brak pliku z grafem o podanej nazwie, podaj nowa sciezke(podwójne \\)" << endl;
		cin >> plik_graf;
		goto otwieranie1;
	}

	lk = zliczanie_linii(o_plik_graf);

	/*Inicjalizacja tablicy z grafem*/

	string *dane_graf = new string[lk];

	/*zapisanie danych grafu z pliku do tablicy*/

	for (int i = 0; i < lk; i++) {
		getline(o_plik_graf, linia);

		if (linia.empty()) {
			i--;
			continue;
		}

		dane_graf[i] = linia;
	}
	o_plik_graf.close();


	lw = zliczanie_wierzcho³kow(dane_graf, lk);
	

	graf = new lista_w *[lw];
	for (int i = 0; i < lw; i++) {
		graf[i] = NULL;

	}

	/*Podzia³ stringa na osobne dane i przypisanie ich do zmiennych*/

	for (int i = 0; i < lk; i++) {
		string str = dane_graf[i];
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
		string split = str.substr(0, str.find("-"));
		str.erase(0, str.find("-"));
		x = stoi(split);
		char compare = str[1];
		if (compare == '>') {
			z = "->";
			str.erase(0, 2);
		}
		else {
			z = "-";
			str.erase(0, 1);
		}
		split = str.substr(0, str.find(":"));
		y = stoi(split);
		str.erase(0, str.find(":") + 1);
		if (!if_int(str, i)) {
			system("pause");
			exit(0);
		};
		d = stod(str);
		if (d < 0) cout << "Dlugosc krawedzi musi byc dodatnia" << endl;
		/*Przypisanie wczytanych danych*/
		pointer = new lista_w;
		pointer->w = y;
		pointer->o = d;
		pointer->nastepny = graf[x];
		graf[x] = pointer;
		char znak = z[1];
		if (znak != '>') {
			pointer = new lista_w;
			pointer->w = x;
			pointer->o = d;
			pointer->nastepny = graf[y];
			graf[y] = pointer;
		}
	}
	delete[] dane_graf;
	

	/*Otwieranie pliku z lista wierzcholkow dla ktroych liczymy odleglosci*/
	fstream o_plik_szukanych;
otwieranie2:
	o_plik_szukanych.open(plik_szukanych, ios::in || ios::out);
	if (!o_plik_szukanych.is_open()) {
		cout << "Brak pliku z szukanymi nazwie, podaj nowa sciezke (podwójne \\)" << endl;
		cin >> plik_szukanych;
		goto otwieranie2;
	}

	ls = zliczanie_linii(o_plik_szukanych);
	string *dane_szukanych = new string[ls];
	for (int i = 0; i < ls; i++) {
				getline(o_plik_szukanych, linia);

				if (linia.empty()) {
					i--;
					continue;
				}

				dane_szukanych[i] = linia;
			}

	o_plik_szukanych.close();

	/*Otwieranie pliku przeznaczonego na zapis*/
	ofstream o_plik_wynik;
	o_plik_wynik.open(plik_wynik, ios::trunc || ios::out);
		if (!o_plik_wynik.is_open()) {
			cout << "Brak pliku z wynikiem, nacisnij dowolny klawisz by zamknac program (podwójne \\)" << endl;
			system("pause");
			exit(0);
		}
	/*Obliczenia oraz zapis do pliku*/
	for (int k = 0; k < ls; k++) {
		string str = dane_szukanych[k];
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
		string split = str.substr(0, str.find(" "));
		int ws = stoi(split);
				
	/*Sprawdzenie czy wierzcholek startowy nalezy do grafu*/

		if (ws >= lw || ws < 0) {
			o_plik_wynik << endl << "Brak wierzcholka " << ws << " w grafie" << endl;
			goto test;
		}
				
	/*Tworzenie i inicjowanie tablic dynamicznych algorytmu*/
		droga = new int[lw];
		pop = new int[lw];
		QS = new bool[lw];
		S = new int[lw];


		for (int i = 0; i < lw; i++) {
			droga[i] = infinity;
			pop[i] = -1;
			QS[i] = false;
		}

		droga[ws] = 0;
	/*Algorytm Djikstry*/
	for (int i = 0; i < lw; i++){

		for (j = 0; QS[j]; j++);

		for (u = j++; j < lw; j++)
			if (!QS[j] && (droga[j] < droga[u])) u = j;

		QS[u] = true;

		for (pointer = graf[u]; pointer; pointer = pointer->nastepny)
			if (!QS[pointer->w] && (droga[pointer->w] > droga[u] + pointer->o)){
				droga[pointer->w] = droga[u] + pointer->o;
				pop[pointer->w] = u;
			}
		}
	/*Zapis wyniku do pliku*/
		o_plik_wynik << endl << "Wierzcholek startowy :  " << dane_szukanych[k] << endl << endl;
		for (int i = 0; i < lw; i++){

			for (j = i; j > -1; j = pop[j]) S[sptr++] = j;

	/* Wyœwietlamy œcie¿kê, pobieraj¹c wierzcho³ki ze stosu*/

			while (sptr) {
				if (S[--sptr] != ws) {
					o_plik_wynik << " -> " << S[sptr];
				}
				else o_plik_wynik << S[sptr];
			}

	/*Wartosc wyliczonej drogi*/

			o_plik_wynik << " : " << droga[i] << endl;
		}

	/*czyszczenie/zerowanie pamieci*/
		sptr = 0;
		delete[] droga;
		delete[] pop;
		delete[] QS;
		delete[] S;
					
	/*Dziwne cos zastepujace breaka*/
	test:
		cout << "";
	}
		o_plik_wynik.close();
	/*Czyszczenie listy sasiedztwa zawierajacej graf*/
	for (int i = 0; i < lw; i++){
		pointer = graf[i];
		while (pointer){
			pointer2 = pointer;
			pointer = pointer->nastepny;
			delete pointer2;
		}
	}
	delete[] dane_szukanych;
}
	

bool if_int(string x, int i) {
	if (x.empty()) {
		cout << "Bledny zapis krawedzi numer (brak wartosci) " << i +1 << endl;
		return false;
	}
	for (int n = 0; n < x.length(); n++) {

			if (x[n] > '9' || x[n] < '0') {
				cout << "Bledny zapis (wartosc nie jest liczba), wiersz numer " << i +1 << endl;
				 return false;
			};
			return true;
		}
	
}
int zliczanie_linii(fstream &plik) {
	string linia;
	int x;
	for (x = 0; !plik.eof(); x++) {
		getline(plik, linia);
		if (linia.empty()) {
			x--;
			continue;
		}
	}
	plik.clear();
	plik.seekg(0, plik.beg);
	return x;
}
int zliczanie_wierzcho³kow(string *tab, int n) {
	string z;
	int x, y;
	int ilosc = 0;
	int *w_tab = new int[n];
	for (int i = 0; i < n; i++) {
		w_tab[i] = 0;
	}
	for (int i = 0; i < n; i++) {
		string str = tab[i];
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
		if (str[0] == '-') {
			cout << "Bledny zapis krawedzi numer (wartosc ujemna)" << i << endl;
			break;
		}
		string split = str.substr(0, str.find("-"));
		str.erase(0, str.find("-"));
		if (!if_int(split, i)) {
			system("pause");
			exit(0);
		}
		x = stoi(split);
		char compare = str[1];
		if (compare == '>') {
			z = "->";
			str.erase(0, 2);
		}
		else {
			z = "-";
			str.erase(0, 1);
		}
		split = str.substr(0, str.find(":"));
		if (!if_int(split, i)) {
			system("pause");
			exit(0);
		};
		y = stoi(split);
		if (w_tab[x] == 0)w_tab[x] = 1; 
		if (w_tab[y] == 0)w_tab[y] = 1; 
	}
	for (int i = 0; i < n; i++) {
		if (w_tab[i] == 1) ilosc++;
	}
	delete[] w_tab;
	return ilosc ;
}
void argumenty_wiersza_polecen(int argc, char *argv[]) {
	int i;
	for (i = 0; i < argc; ++i);
	if (argv[1] == "-h" || i != 4) {
		
		cout << "Program obslguje trzy przelaczniki (wszystkie sa wymagane) : -g, -w, -o.Uzyj ich by uruchomic program. Wprowadz nowe przelaczniki " << endl;
		system("pause");
		exit(0);
	}
	if (argv[1] == "-g" && argv[2] == "-w" && argv[3] == "-o")return;
	if (argv[1] == "-g" && argv[3] == "-w" && argv[2] == "-o")return;
	if (argv[2] == "-g" && argv[1] == "-w" && argv[3] == "-o")return; 
	if (argv[2] == "-g" && argv[3] == "-w" && argv[1] == "-o")return;
	if (argv[3] == "-g" && argv[2] == "-w" && argv[1] == "-o")return;
	if (argv[3] == "-g" && argv[1] == "-w" && argv[2] == "-o")return;
	
}

/*C:\Users\Kamil\source\repos\Projekt_Algorytm_Dijkstry\Debug\Projekt_Algorytm_Dijkstry.exe*/

