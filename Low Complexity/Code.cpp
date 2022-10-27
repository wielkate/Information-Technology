/* Ekaterina Mashchenko
 *
 * Sens programu polega na implementacji algorytmu, obliczajacego zlozonosc lingwistyczna sekwencji DNA
 * ze wzoru lc(s) = sub(s) / m(a,n), gdzie
 * licznik - liczba unikatowych podciagow podanej sekwencji
 * mianownik - maksymalna liczba podciagow o dlugosci sekwencji, wykorzystujacych caly alfabet
 *
 * Idea: Uzycie hashowania
 * Zlożonosc czasowa: O(n * m), gdzie m jest dlugoscia najdlluzszego ciagu.
 *
 * Sekwencja jest wczytywana z pliku i zapisywana do innego pliku (dowolne rozszerzenia)
 * Dodatkowo sprawdza sie, czy prog, szerokosc okna, nazwa pliku wejsciowego sa poprawne
*/

#include <iostream>
#include <unordered_set>
#include <fstream>
#include <string>

using namespace std;

struct STRUCTURE {
  int wide; // szerokosc okna
  float threshold; // prog, od ktorego uznajemy lc
};

STRUCTURE parameters;

//================================================================================= funkcja sprawdza poprawnosc danych
void Check(int length) {

  while (!(parameters.wide <= length && parameters.wide >= 1)) {
	cout << "Szerokosc nie moze byc ani ujemna, ani wieksza od dlugosci sekwencji = " << length << endl;
	cin >> parameters.wide;
  }
  while (!(parameters.threshold <= 1 && parameters.threshold >= 0)) {
	cout << "T miesci sie w przedziale od 0 do 1" << endl;
	cin >> parameters.threshold;
  }
}

int main() {

  //================================================================================= definicja zmiennych
  fstream file_from, file_to; // pliki dla odczytu i zapisu
  string file_name; // nazwa pliku
  string sequence; // sekwencja
  string cut_sequence; // sekwencja po wprowadzeniu szerokosci okna
  unordered_set<string> result; // set dla wynikowych lancuchow

  unsigned int variance; // wariancja z powtorzeniami
  float lc; // zlozonosc lingwistyczna

  //================================================================================= odczyt pliku
  while (true) {
	cout << "Prosze podac istniejaca nazwe pliku z rozszerzeniem: " << endl;
	cin >> file_name;
	file_from.open(file_name.c_str(), fstream::in | fstream::_Nocreate );

	if (file_from.good()) {
	  getline(file_from, sequence); // fasta jest zapisany w jednej linii
	  sequence = sequence.substr(sequence.find_last_of(' ') + 1); // sekwecja jest ostatnim slowem
	  file_from.close();
	  break;
	}
	file_from.close();
  }

  //================================================================================= wprowadzenie parametrow
  cout << "Wprowadzic parametry W oraz T" << endl;
  cin >> parameters.wide >> parameters.threshold;
  Check(sequence.length());

  //================================================================================= algorytm

  for (int window_start = 0; window_start + parameters.wide <= sequence.length();
	   window_start++) { // wprowadzenie przesuwania
	cut_sequence = sequence.substr(window_start, parameters.wide); // ograniczenie sekwencji do rozmiaru okna

	// inicjalizacja zostala przeniesiona tu dla wyzerowania zmiennych
	unordered_set<string> substring; // set dla podciagow
	unsigned int distance = 1; // liczba nukleotydow, zaczynajac od mononukleotydu
	unsigned int max_substring = 0; // mianownik (liczba podciagow alfabetu)
	unsigned int counter = 0; // licznik slow w jednym cyklu

	// wszystkie kombinacje w podanej sekwencji
	for (int from = 0; distance <= cut_sequence.length(); from++) {

	  // jesli podlancuch jest unikalny, to dodac go do hasha
	  if (!substring.count(cut_sequence.substr(from, distance))) substring.insert(cut_sequence.substr(from, distance));
	  counter++;

	  if (distance + from >= cut_sequence.length()) { // doszlismy do konca
		variance = pow(4, distance++); // wszystkie kombinacji, zakladam alfabet A = {A,T(U),G,C}
		max_substring += min(variance, counter); // m(n,a,k) --> m(9,4,9) = 4, m(9,16,8) = 8, m(9,64,7) = 7
		counter = 0; // wyzerowanie licznika
		from = -1; // wyzerowanie poczatku
	  }
	}

	// jesli region ma niska zlozonosc, to dodaje sie do tablicy wynikowej
	lc = float(substring.size()) / float(max_substring); // rzutowanie typu
	if (lc <= parameters.threshold && !result.count(cut_sequence)) result.insert(cut_sequence);
  } // koniec algorytmu

  //================================================================================= zapis pliku
  cout << "Prosze wpisac nazwe pliku wynikowego " << endl;
  cin >> file_name;
  // otwieranie nieistniejacego pliku spowoduje jego utworzenie
  file_to.open(file_name.c_str(), fstream::out | fstream::app);
  for (const auto &item : result) file_to << item << endl;
  file_to.close();

  return 0;
}
