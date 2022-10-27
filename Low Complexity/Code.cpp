// Ekaterina Mashchenko

#include <iostream>
#include <algorithm>
#define SIZE 1000000

using namespace std;

string dictionary[SIZE]; // definicja tablicy poza mainem z powodu rozmiaru

int main() {

  // definicja zmiennych
  string sequence; // sekwencja
  unsigned int num_substring = 0; // licznik (liczba unikatowych podciagow sekwencji)
  unsigned int from = 0;
  unsigned int distance = 1; // zaczynajac od mononukleotydu
  unsigned int iterator = 1; // numer kroku, liczac od 1
  unsigned int cardinality; // moc alfabetu
  unsigned int max_substring = 0; // mianownik (liczba podciagow alfabetu)
  unsigned int variance; // wariancja z powtorzeniami
  unsigned int counter = 0; // licznik slow w jednym cyklu
  float lc; // zlozonosc linfwustyczna

  // wczytanie lancucha
  cout << "Prosze podac sekwencje DNA oraz moc alfabetu: " << endl;
  cin >> sequence >> cardinality;

  // wzor na max liczbe podlancychow: n * (n + 1) / 2, gdzie n - dlugosc lancucha
  // jakby wszystkie litery byly rozne
  unsigned int all_substring = sequence.length() * (sequence.length() + 1) / 2;

  // wszystkie kombinacje w podanej sekwencji
  while (distance <= sequence.length()) {
	dictionary[num_substring] = sequence.substr(from, distance);
	num_substring++;
	from++;
	counter++;

	if (distance + from > sequence.length()) { // doszlismy do konca

	  variance = pow(cardinality, iterator); // wszystkie kombinacji alfabetu
	  max_substring += min(variance, counter); // m(n,a,k) --> m(9,4,9) = 4, m(9,16,8) = 8, m(9,64,7) = 7
	  counter = 0; // wyzerowanie licznika
	  from = 0; // wyzerowanie poczatku
	  iterator++; // nastepny krok
	  distance++; // wydluzenie dystancu, czyli dodanie nukleotydu
	}
  } // na koncu all_substring == num_substring

  // sortowanie w czasie n * O(log n) za pomoca biblioteki <algorithm>
  sort(dictionary, dictionary + num_substring);

  // unikatowosc
  for (int i = 1; i < all_substring; i++) if (dictionary[i] == dictionary[i - 1]) num_substring--;

  lc = float (num_substring) / float  (max_substring); // rzutowanie typi
  cout << "Calkowita liczba roznych podciagow lancucha = " << num_substring << endl;
  cout << "Maksymalna liczba roznych podciagow lancucha = " << max_substring << endl;
  cout << "Zlozonosc lingwistyczna = " << lc << endl;

  return 0;
}
