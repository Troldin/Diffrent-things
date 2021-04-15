#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <algorithm>
//////////////////////// LAB 1 - 2 //////////////////////

int hash_count = 0;
int h(int x, int i, int T)
{
	hash_count++;
	int hash;
	//hash=(x + i) % T;
	hash = ((x % T) + (i * (((x/T) % (T/2))*2)+1))% T;
	return hash;
}
bool insert(int x, int* &tablica, int &size)
{
	int hash;
	for (int kolizje = 0; kolizje < size; kolizje++)
	{
		hash = h(x, kolizje, size);
		if (tablica[hash] == -1)
		{
			tablica[hash] = x;
			return true;
		}
	}
	return false;
}
bool find(int x, int* tablica, int size)
{
	int hash;
	for (int i = 0; i < size; i++)
	{
		hash = h(x, i, size);
		if (tablica[hash] == x)
		{
			return true;
		}
		if (tablica[hash] == -1)
		{
			return false;
		}
	}
	return false;
}
int losowa_liczba(int min = 0, int max = std::numeric_limits<int>::max())
{
	static std::default_random_engine gen(std::random_device{}());
	static std::uniform_int_distribution<int> dist;
	return dist(gen, std::uniform_int_distribution<int>::param_type{ min, max });
}
int main()
{
	std::default_random_engine gen(std::random_device{}());
	int size_tab_random = 10000;
	int * tab_random = new int[size_tab_random];
	for (int i = 0; i < size_tab_random; i++)
	{
		tab_random[i] = losowa_liczba();
	}
	int size_tab_hash = 1048576;
	int * tab_hash = new int[size_tab_hash];
	for (int i = 0; i < size_tab_hash; i++)
	{
		tab_hash[i] = -1;
	}
	//////////////////////////  LAB 1 /////////////////////////////
	std::cerr << "//////////// WSTAWIANIE //////////////// \n";
	for (int pomiar = 0; pomiar < 100; pomiar++)
		{
		
		if (pomiar % 10 == 0) {
			std::cerr << "Pomiar dla " << pomiar << " % \n";
		}
			auto start = std::chrono::high_resolution_clock::now();
		
			for (int i =0; i < 10000; i++)
			{
				insert(tab_random[i], tab_hash, size_tab_hash);
			}
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::micro> duration = end - start;
			if (pomiar %10 ==0 ){
				std::cerr << " - sredni czas wstawiania jednego elementu: " << duration.count()/10000 << "us. \n";
				std::cerr << " - srednia liczba wywolan funkcji mieszajacej na element : " << hash_count/10000<< "\n";
			}
			hash_count = 0;
			
		}

	delete[] tab_random;
	//////////////////////////  LAB 2 /////////////////////////////
	std::cerr << "//////////// WYSZUKIWANIE //////////////// \n";
	for (int i = 1; i < 11; i++) {
		std::cerr << "Pomiar dla " << i << "0 % \n";
		for (int j = 0; j < size_tab_hash; j++)
		{
			tab_hash[j] = -1;
		}
		int *tab_wpisanych = new int[i*(size_tab_hash/10)];
		int liczba;

		for (int j = 0; j < (size_tab_hash / 10); j++) {
			liczba = losowa_liczba();
			insert(liczba, tab_hash, size_tab_hash);
			tab_wpisanych[j] = liczba;
		}

		shuffle(tab_wpisanych, tab_wpisanych + (size_tab_hash / 10), gen);
		auto start = std::chrono::high_resolution_clock::now();
		for (int j = 0; j < 10000; j++)
		{
			find(tab_wpisanych[j], tab_hash, size_tab_hash);
		}
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::micro> duration = end - start;
		std::cerr << " - sredni czas wyszukiwania jednego elementu  " << duration.count() / 10000 << "us \n";
		std::cerr << " - srednia liczba wywolan funkcji mieszajacej na element : " << hash_count / 10000<< "\n";
		delete[] tab_wpisanych;
		hash_count = 0;
		}

	delete[] tab_hash;
	
	return 1;
}
//////////////////////// LAB 3 //////////////////////
/* int hash_count = 0;
int restruct_count = 0;
int h(int x, int i, int T)
{
	hash_count++;
	int hash;
	hash=(x + i) % T;
	//hash = ((x % T) + (i * (((x / T) % (T / 2)) * 2) + 1)) % T;
	return hash;
}
void restruct(int* &tab_hash, int &T, int &current);
bool insert(int x, int* &tab_hash, int &T, int &tab_hash_current, int w_wypelnienia)
{
	int hash;
	if (tab_hash_current *100 / T >= w_wypelnienia)
	{
		restruct(tab_hash, T, tab_hash_current);
	}
	for (int i = 0; i < T; i++)
	{
		hash = h(x, i, T);
	if (tab_hash[hash] == -1)
		{
			tab_hash[hash] = x;
			tab_hash_current++;
			return true;
		}
	}
	return false;
}
bool find(int x, int* tab_hash, int T)
{
	int hash;
	for (int i = 0; i < T; i++)
	{
		hash = h(x, i, T);
		if (tab_hash[hash] == x)
		{
			return true;
		}
		if (tab_hash[hash] == -1)
		{
			return false;
		}
	}
	return false;
}
void restruct(int* &tab_hash, int &T, int &current)
{
	restruct_count++;
	T = T * 2;
	current = 0;
	int * temp_tab_hash = new int[T];
	for (int i = 0; i < T; i++)
	{
		temp_tab_hash[i] = -1;
	}
	for (int i = 0; i < T / 2; i++)
	{
		if (tab_hash[i] != -1)
		{
			insert(tab_hash[i], temp_tab_hash, T, current, 200);
		}
	}
	delete[] tab_hash;
	tab_hash = temp_tab_hash;
}
int losowa_liczba(int min = 0, int max = std::numeric_limits<int>::max())
{
	static std::default_random_engine gen(std::random_device{}());
	static std::uniform_int_distribution<int> dist;
	return dist(gen, std::uniform_int_distribution<int>::param_type{ min, max });
}
int main()
{
	std::default_random_engine gen(std::random_device{}());
	int all_restruct = 0;
	int all_hash = 0;
	int all_hash_insert = 0;
	int all_hash_find = 0;
	int tab_hash_current = 0;// Aktualna liczba elementów w tablicy mieszaj¹cej
	int all_w_restruct = 0;
	int size_tab_random = 1000000;
	int * tab_random = new int[size_tab_random];
	for (int i = 0; i < size_tab_random; i++)
	{
		tab_random[i] = losowa_liczba();
	}
	
	for (int w_wype³nienia = 60; w_wype³nienia < 100; w_wype³nienia = w_wype³nienia + 10) {
		std::cerr << "Graniczny wspolczynnik wypelnienia " << w_wype³nienia << " \n";
		int size_tab_hash = 1024;	//Aktualny rozmiar fizyczny tablicy mieszaj¹cej
		int * tab_hash = new int[size_tab_hash];
		for (int i = 0; i < size_tab_hash; i++)
		{
			tab_hash[i] = -1;
		}
		std::cout << "WSTAWIANIE \n";
		std::cout.width(10);
		std::cout << "Pomiar";
		std::cout.width(25);
		std::cout << "Czas dla 10k[us]";
		std::cout.width(25);
		std::cout << "Liczba wywolan hash()";
		std::cout.width(30);
		std::cout << "Liczba restrukturyzacji";
		std::cout.width(35);
		std::cout << "Liczba restrukturyzacji dotychczas" << "\n";


		for (int pomiar = 1; pomiar < 101; pomiar++)
		{

			auto start = std::chrono::high_resolution_clock::now();


			for (int i = (pomiar-1) * 10000; i < (pomiar-1) * 10000 + 10000; i++)
			{
				insert(tab_random[i], tab_hash, size_tab_hash, tab_hash_current, w_wype³nienia);
			}


			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::micro> duration = end - start;;
			
			all_restruct += restruct_count;
			all_w_restruct += restruct_count;
			all_hash_insert += hash_count;
			std::cout.width(10);
			std::cout  << pomiar;
			std::cout.width(25);
			std::cout  << duration.count();
			std::cout.width(25);
			std::cout  << hash_count;
			std::cout.width(30);
			std::cout  << restruct_count;
			std::cout.width(35);
			std::cout <<  all_w_restruct << "\n";

				
			restruct_count = 0;
			hash_count = 0;
		}


		std::cout << "WYSZUKIWANIE \n";
		std::cout.width(10);
		std::cout << "Pomiar";
		std::cout.width(25);
		std::cout << "Czas dla 10k[us]";
		std::cout.width(25);
		std::cout << "Liczba wywolan hash() \n";
		std::cout.width(30);
		shuffle(tab_random, tab_random + size_tab_random, gen);
		for (int pomiar = 1; pomiar < 101; pomiar++)
		{
			auto start = std::chrono::high_resolution_clock::now();
				
			for (int i = (pomiar-1) * 10000; i < (pomiar-1) * 10000 + 10000; i++)
			{
				find(tab_random[i], tab_hash, size_tab_hash);
			}
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::micro> duration = end - start;
			all_hash_find += hash_count;
			std::cout.width(10);
			std::cout << pomiar;
			std::cout.width(25);
			std::cout << duration.count();
			std::cout.width(25);
			std::cout << hash_count << "\n";
			hash_count = 0;

		}
			
		all_hash = all_hash + all_hash_find + all_hash_insert;
  
		all_w_restruct = 0;
		}


	std::cerr << " Liczba restukturyzacji dla calego programu - " << all_restruct << "\n";
	std::cerr << " Liczba wywolan funckji mieszajacej dla calego programu - " << all_hash << "\n";




	
	return 1;
}*/


