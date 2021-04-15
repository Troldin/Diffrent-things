#include <iostream>
#include <chrono>
#include <random>
#include <limits>
#include <algorithm>
#include <string>
#include <iomanip>
#include <fstream>
#include <clocale>

using std::swap;


int losowa_liczba(int min, int max)
{
	static std::default_random_engine gen(std::random_device{}());
	static std::uniform_int_distribution<int> dist;
	return dist(gen, std::uniform_int_distribution<int>::param_type{ min, max });
}

void wypelnij(int* tablica, int rozmiar, int min = 0, int max = std::numeric_limits<int>::max())
{
	/*for (int i = 0; i < rozmiar; ++i)
		tablica[i] = losowa_liczba(min, max);
	
	// najmniejszy element na tab[0]
	/*
	int temp, l;
	for (l = 1; l < rozmiar; l++) {
		if (tablica[0] > tablica[l]) {
			temp = tablica[0];
			tablica[0] = tablica[l];
			tablica[l] = temp;
		}
	}
	*/
	// Tablica posortowana
	//for (int i = 0; i < rozmiar; ++i)
	//	tablica[i] = i;

	// Tablica z duplikatami
	for (int i = 0; i < rozmiar; ++i)
		tablica[i] = 0; 
	
	
}

bool jest_posortowane(int* tablica, int rozmiar)
{
	return std::is_sorted(tablica, tablica + rozmiar);
}

double mierz_czas(int* tablica, int rozmiar, void(*funkcja_sortujaca)(int*, int))
{
	auto start = std::chrono::high_resolution_clock::now();
	funkcja_sortujaca(tablica, rozmiar);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	return duration.count();
}

/*

	parametry:
	 - funkcja_sortujaca - wskanik na funkcj¹c¹ sortowanie, musi przyjmowaæ dwa parametry: adres pocz¹tku tablicy (int*) oraz jej rozmiar (int)
	 - nazwa - nazwa testowanej funkcji, tylko w celach wypisania
	 - output - strumien do ktorego beda zapisane wyniki, domyslnie std::cerr, przy ostatnim uruchomieniu warto nadpisac otwartym strumieniem plikowym, aby sobie zebrac wyniki do pliku
	 - dodatkowe_miejsce - liczba dodatkowych elementow tablicy zaalokowanych PRZED poczatkiem tablicy, przykladowo gdy =1, pierwszym indeksem tablicy jest -1, ale dane rozpoczynaja sie od indeksu 0, moze sie przydac do sortowania przez wstawianie z wartownikiem
*/
void eksperyment(void(*funkcja_sortujaca)(int*, int), const std::string& nazwa, std::ostream& output = std::cerr, int dodatkowe_miejsce = 0)
{
	//ustawienia
	const double limit_czasu = 30.0; //sekund
	const int powtorzen = 5;
	const int rozmiar_poczatkowy = 1 << 10;
	/////////////////////////////////////////
	const int szerokosc = 100;
	int gwiazdek = szerokosc - nazwa.length() - 2;
	if (gwiazdek < 0)
		gwiazdek = 0;
	int i = 0;
	output << " ";
	for (; i < gwiazdek / 2; ++i)
		output << '*';
	output << " " << nazwa << " ";
	for (; i < gwiazdek; ++i)
		output << '*';
	output << "\n\n";
	output.flush();

	output << std::setw(9) << "N";
	output << std::setw(1) << "";
	for (int nr = 0; nr < powtorzen; ++nr)
		output << std::setw(9) << nr + 1 << " ";
	output << std::setw(12) << "œredna" << " ";
	output << "\n";
	for (int rozmiar = rozmiar_poczatkowy; ; rozmiar *= 2)
	{
		output << std::setw(9) << rozmiar << ": ";
		output.flush();
		int* pamiec = new int[dodatkowe_miejsce + rozmiar];
		int* tablica = pamiec + dodatkowe_miejsce;
		double czas = 0.0;

		int* pattern = new int[rozmiar];


		for (int nr = 0; nr < powtorzen; ++nr)
		{
			wypelnij(tablica, rozmiar);
			for (int i = 0; i < rozmiar; ++i)
				pattern[i] = tablica[i];
			std::sort(pattern, pattern + rozmiar);
			double c = mierz_czas(tablica, rozmiar, funkcja_sortujaca);
			if (!jest_posortowane(tablica, rozmiar))
			{
				output << "Tablica nieposortowana!!\n";
				if (&output != &std::cerr)
					std::cerr << "Tablica nieposortowana!!\n";
				return;
			}
			if (!std::equal(pattern, pattern + rozmiar, tablica, tablica + rozmiar))
			{
				output << "Tablica zawiera inne wartosci niz powinna!!\n";
				if (&output != &std::cerr)
					std::cerr << "Tablica zawiera inne wartosci niz powinna!!\n";
				return;
			}
			czas += c;
			output.precision(6);
			output << std::fixed << c << "  ";
			output.flush();
		}
		czas /= powtorzen;
		output << std::setw(12) << std::fixed << czas << "\n";
		output.flush();
		delete[] pamiec;
		delete[] pattern;
		if (czas > limit_czasu)
			break;
	}
	output << "\n";
	output.flush();
}



/********************* LAB1 ***********************/
void sortowanie_babelkowe(int* tablica, int rozmiar) {
	int temp;
	for (int i = 0; i < rozmiar; i++) {
		for (int j = 1; j < rozmiar; j++) {
			if (tablica[j - 1] > tablica[j]) {
				temp = tablica[j - 1];
				tablica[j - 1] = tablica[j];
				tablica[j] = temp;
			}
		}
	}

}
void sortowanie_przez_proste_wybieranie(int* tablica, int rozmiar) {
	for (int i = 0; i < rozmiar - 1; i++) {
		int k = i;
		int x = tablica[i];
		for (int j = i + 1; j < rozmiar; j++) {
			if (tablica[j] < x) {
				k = j;
				x = tablica[j];
			}
		}
		tablica[k] = tablica[i];
		tablica[i] = x;
	}
}
void sortowanie_przez_proste_wstawianie(int* tablica, int rozmiar) {
	int j, x, i;
	for (i = 1; i < rozmiar; i++) {
		x = tablica[i];
		j = i - 1;
		while (j > -1 && x < tablica[j]) {
			tablica[j + 1] = tablica[j];
			j = j - 1;
		}
		tablica[j + 1] = x;

	}
}
/*Modifikacja sortowan poprzez usawienie minimum jako pierwszy element tablicy*/
void mod_sortowanie_przez_proste_wstawianie(int* tablica, int rozmiar) {

	int x, i, j;
	for (i = 2; i < rozmiar; i++) {
		x = tablica[i];
		j = i - 1;
		while (j > 0 && x < tablica[j]) {
			tablica[j + 1] = tablica[j];
			j = j - 1;
		}
		tablica[j + 1] = x;

	}
}
void mod_sortowanie_przez_proste_wybieranie(int* tablica, int rozmiar) {
	for (int i = 1; i < rozmiar - 1; i++) {
		int k = i;
		int x = tablica[i];
		for (int j = i + 1; j < rozmiar; j++) {
			if (tablica[j] < x) {
				k = j;
				x = tablica[j];
			}
		}
		tablica[k] = tablica[i];
		tablica[i] = x;
	}
}
void mod_sortowanie_babelkowe(int* tablica, int rozmiar) {
	
	int temp;
	for (int i = 1; i < rozmiar; i++) {
		for (int j = 2; j < rozmiar; j++) {
			if (tablica[j - 1] > tablica[j]) {
				temp = tablica[j - 1];
				tablica[j - 1] = tablica[j];
				tablica[j] = temp;
			}
		}
	}

}

/********************* LAB2 ***********************/
void sortowanie_shella(int*tablica, int rozmiar) {
	int h = 1;
	while (h < rozmiar / 9) {
		h = 3 * h + 1;
	}
	while (h >= 1) {
		for (int i = h; i < rozmiar; i++) {
			int x = tablica[i];
			int j;
			j = i;
			while ((j >= h) && (x < tablica[j - h])) {
				tablica[j] = tablica[j - h];
				j = j - h;
			}
			tablica[j] = x;
		}
		h /= 3;
	}
}
/*Sortowanie szybkie*/
int sortowanie_szybkie_2(int *tablica, int lewy, int prawy) {
	int t, s, temp;
	if (lewy < prawy) {
		t = tablica[lewy];
		s = lewy;
		for (int i = lewy + 1; i <= prawy; i++) {
			if (tablica[i] < t) {
				s = s + 1;
				temp = tablica[i];
				tablica[i] = tablica[s];
				tablica[s] = temp;
			}
		}
		temp = tablica[lewy];
		tablica[lewy] = tablica[s];
		tablica[s] = temp;
		sortowanie_szybkie_2(tablica, lewy, s - 1);
		sortowanie_szybkie_2(tablica, s + 1, prawy);
	}
	return *tablica;
}
void sortowanie_szybkie(int*tablica, int rozmiar) {
	*tablica = sortowanie_szybkie_2(tablica, 0, rozmiar - 1);
}

int sortowanie_szybkie_mid_2(int tablica[], int lewy, int prawy) {
	int i = lewy;
	int j = prawy;
	int temp;
	int s = tablica[(lewy + prawy) / 2];

	while (i <= j) {
		while (tablica[i] < s)
			i++;
		while (tablica[j] > s)
			j--;
		if (i <= j) {
			temp = tablica[i];
			tablica[i] = tablica[j];
			tablica[j] = temp;
			i++;
			j--;
		};
	}
		if (lewy < j)
			sortowanie_szybkie_mid_2(tablica, lewy, j);
		if (i < prawy)
			sortowanie_szybkie_mid_2(tablica, i, prawy);

		
	
	
	return *tablica;
}
void sortowanie_szybkie_mid(int*tablica, int rozmiar) {
	*tablica = sortowanie_szybkie_mid_2(tablica, 0, rozmiar - 1);
}
/********************/
void sort_std(int *tablica, int rozmiar) {
	std::sort(tablica, tablica + rozmiar);
}

/********************* LAB3 ***********************/
/*Sortowanie kopcowe*/
void kopiec_w_dol(int *tablica, int rozmiar, int i) {
	int largest;
	int l = i * 2 + 1;
	int r = i * 2 + 2;
	if ((l < rozmiar) && (tablica[l] > tablica[i]))
		largest = l;
	else largest = i;
	if ((r < rozmiar) && (tablica[r] > tablica[largest]))
		largest = r;
	if (i != largest) {
		int temp = tablica[i];
		tablica[i] = tablica[largest];
		tablica[largest] = temp;
		kopiec_w_dol(tablica, rozmiar, largest);
	}
}
void buduj_kopiec(int *tablica, int rozmiar) {
	int i;
	for (i = (rozmiar - 2) / 2; i >= 0; i--) {
		kopiec_w_dol(tablica, rozmiar, i);
	}
}
void sortowanie_kopcowe(int *tablica, int rozmiar) {
	int temp, i;
	buduj_kopiec(tablica, rozmiar);
	for (i = rozmiar - 1; i > 0; i--) {
		temp = tablica[i];
		tablica[i] = tablica[0];
		tablica[0] = temp;
		kopiec_w_dol(tablica, i, 0);

	}
}


int main()
{
	setlocale(LC_ALL, "");
	std::ofstream wyniki("wyniki.txt", std::ofstream::app);
	//std::ostream& output = std::cerr;
	std::ostream& output = wyniki; //zmienic na = wyniki aby zapisywalo do pliku

	//eksperyment(sortowanie_babelkowe, "Sortowanie babelkowe", output);
	//eksperyment(sortowanie_przez_proste_wybieranie, "Sortowanie przez proste wybieranie", output);
	//eksperyment(sortowanie_przez_proste_wstawianie, "Sortowanie przez proste wstawianie", output);
	//eksperyment(sortowanie_shella, "Sortowanie Shella", output);
	//eksperyment(sortowanie_szybkie, "Sortowanie szybkie", output);
	//eksperyment(sort_std, "Sortowanie std::sort", output);
	//eksperyment(sortowanie_kopcowe, "Sortowanie kopcowe", output);

	/********************************************/

	//eksperyment(sortowanie_babelkowe, "Sortowanie babelkowe - dane posortowane", output);
	//eksperyment(sortowanie_przez_proste_wybieranie, "Sortowanie przez proste wybieranie - dane posortowane", output);
	//eksperyment(sortowanie_przez_proste_wstawianie, "Sortowanie przez proste wstawianie - dane posortowane", output);
	//eksperyment(sortowanie_shella, "Sortowanie Shella - dane posortowane", output);
	//eksperyment(sortowanie_szybkie_mid, "Sortowanie szybkie - dane posortowane", output);
	//eksperyment(sortowanie_kopcowe, "Sortowanie kopcowe - dane posortowane", output);
	//eksperyment(sort_std, "Sortowanie std::sort - dane posortowane", output);

	/***********************************************/

	//eksperyment(sortowanie_babelkowe, "Sortowanie babelkowe - duplikaty", output);
	//eksperyment(sortowanie_przez_proste_wybieranie, "Sortowanie przez proste wybieranie - duplikaty", output);
	//eksperyment(sortowanie_przez_proste_wstawianie, "Sortowanie przez proste wstawianie - duplikaty", output);
	//eksperyment(sortowanie_shella, "Sortowanie Shella - duplikaty", output);
	//eksperyment(sortowanie_szybkie_mid, "Sortowanie szybkie - duplikaty", output);
	//eksperyment(sortowanie_kopcowe, "Sortowanie kopcowe - duplikaty", output);
	eksperyment(sort_std, "Sortowanie std::sort - duplikaty", output);
	

	/***********************************************/

	//eksperyment(mod_sortowanie_babelkowe, "Sortowanie babelkowe - minimum  pierwszym elementem tablicy", output);
	//eksperyment(mod_sortowanie_przez_proste_wstawianie, "Sortowanie przez proste wstawianie - minimum  pierwszym elementem tablicy", output);
	//eksperyment(mod_sortowanie_przez_proste_wybieranie, "Sortowanie przez proste wybieranie - minimum  pierwszym elementem tablicy", output);
	

	return 0;
}