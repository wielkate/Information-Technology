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
#include <cmath>

using namespace std;

/* wprowadzenie argumentow z koncoli - dazymy do formatu file.cpp seq.fasta -w 10 -t 0.9 > out.txt
 * Have 6 arguments:
 * i = 0 => file.cpp
 * i = 1 => seq.fasta
 * i = 2 => -w
 * i = 3 => 10
 * i = 4 => -t
 * i = 5 => 0.9
 * Z powyzszego wydruku widzimy, ze sa potezebne argumenty o indeksach 1 (nazwa pliku do odczytu),
 * 3 jako szerokosc okna oraz 5 jako prog od ktorego uznajemy zlozonosc za niska */

int main(int argc, char **argv) {

  //================================================================================= definicja zmiennych
  fstream file_from; // plik dla odczytu
  string sequence; // sekwencja oryginalna
  string cut_sequence; // sekwencja po wprowadzeniu szerokosci okna
  unordered_set<string> result; // set z uwzglednieniem progu zlozonosci
  unsigned int variance; // wariancja z powtorzeniami
  float lc; // zlozonosc lingwistyczna

  //================================================================================= odczyt pliku
  while (true) {
	file_from.open(string(argv[1]).c_str(), fstream::in);

	if (file_from.good()) {
	  getline(file_from, sequence); // fasta jest zapisany w jednej linii
	  sequence = sequence.substr(sequence.find_last_of(' ') + 1); // sekwecja jest ostatnim slowem
	  file_from.close();
	  break;
	}
	  cout << "Plik nie istnieje, prosze sprobowac ponownie" << endl;
	  cin >> argv[1];
	  file_from.close();
  }

  //================================================================================= sprawdzenie poprawnosci danych
	while (!(stoi(argv[3]) <= sequence.length() && stoi(argv[3]) >= 1)) {
	  cout << "Szerokosc nie moze byc mniejsza od jedynki ani wieksza od dlugosci sekwencji = " << sequence.length() << endl;
	  cin >> argv[3];
	}
	while (!(stof(argv[5]) <= 1 && stof(argv[5]) >= 0)) {
	  cout << "T miesci sie w przedziale od 0 do 1" << endl;
	  cin >> argv[5];
	}

  //================================================================================= algorytm
  for (int window_start = 0; window_start + stoi(argv[3]) <= sequence.length(); window_start++) { // wprowadzenie przesuwania
	cut_sequence = sequence.substr(window_start, stoi(argv[3])); // ograniczenie sekwencji do rozmiaru okna

	// inicjalizacja zostala przeniesiona tu dla wyzerowania zmiennych
	unordered_set<string> substring; // set z uwzglednieniem unikatowosci podlancuchow
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

	// jesli region ma niska zlozonosc, to dodaje zbior do tablicy wynikowej z jednoczesnym wypisywaniem
	lc = float(substring.size()) / float(max_substring); // rzutowanie typu
	if (lc <= stof((argv[5])) && !result.count(cut_sequence)) {
	  result.insert(cut_sequence);
	  // Przykladowy wyglad : widoczna jest nazwa pliku bez rozszerzenia, polozenie liczy sie od 1
	  // >id seq 50:58
	  // AAAA ...
	  cout << ">id " << string(argv[1]).substr(0,string(argv[1]).find_last_of('.')) << " " << window_start + 1 << ":" << window_start + stoi(argv[3]) << endl;
	  cout << cut_sequence << endl;
	}
  } // koniec algorytmu

  return 0;
}
