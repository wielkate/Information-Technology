/* Ekaterina Mashchenko
 *
 * Sens programu polega na implementacji algorytmu, obliczajacego zlozonosc lingwistyczna sekwencji DNA
 * ze wzoru lc(s) = sub(s) / m(a,n), gdzie
 * licznik - liczba unikatowych podciagow podanej sekwencji
 * mianownik - maksymalna liczba podciagow o dlugosci sekwencji, wykorzystujacych caly alfabet
 *
 * Idea: Uzycie tablic sufiksowych
 * Przy duzych ilosciach informacji lepiej od drzew sufiksowych wykazuja sie tablicy
 * Tworzenie tablicy sufiksowej zajmuje O(nlogn) czasu, co mozna zredukowac do O(n)
 * Algorytm Kasai w czasie liniowym tworzy tablice lcp (longest common prefix)
 *
 * Obliczenie licznika jako roznicy wszystkich podciagow (nieunikatowych tez) a duplikatow
 * Duplikaty wyliczaja sie jako suma elementow tablicy lcp
 *
 * Zlozonosc czasowa: O(nlogn), gdzie n jest dlugoscia sekwencji
 * Zlozonosc pamieciowa: ?
 *
 * Sekwencja jest wczytywana z pliku.
 * Sprawdzenie poprawnosci przelacznikow, zalresow parametrow
*/

#include <iostream>
#include <unordered_set>
#include <fstream>
#include <string>
#include <cmath>
#include <cstring>
#include <vector>
#include <map>

using namespace std;

//================================================== funkcja wylicza mianownik, przyjmujac szerokosc okna jako argument
int MaxSubstring(int length) {
  int max_substring = 0;
  for (int i = 1; length >= 1; i++, length--) max_substring += min(length, int(pow(4, i)));
  return max_substring;
}

//======================================================= funkcja tworzy tablice sufiksowa, przyjmijac sekwencje liter
vector<int> SuffixArray(string &sequance_char) {

  if (sequance_char.length() == 1) return vector<int>{0}; // przyspieszenie, mononukleotyd ma {0}

  vector<int> sequence; // sekwencja liczb
  for (char &i : sequance_char) sequence.push_back(i - 'A'); // przepisac na numery alfabetu
  sequence.push_back(0); // dla ulatwienia dodajemy zerowy symbol na koniec

  //============================================================ definicja zmiennych
  int length = (int) sequence.size();
  int counter = 0;  // zmienna pomocnicza, licznik dla sortowania
  int num_class = 0;  // liczba klas rownowaznosci

  vector<int> c(length); // klasa rownowaznosci
  vector<int> p(length); // permutacja

  map<int, vector<int> > map_map; // mapa - pojemnik majacy sklad <klucz - wartosc>
  for (int i = 0; i < length; i++) map_map[sequence[i]].push_back(i); // wypelnic mape

  //=================================================================== faza zerowa
  for (auto &item_map : map_map) { // przejscie przez mape
    // przejscie przez wektor (drugi argument)
    for (int item_vector : item_map.second) c[item_vector] = num_class, p[counter++] = item_vector;
    num_class++;
  }

  for (int iterator = 1; num_class < length; iterator++) { // dopoki wszystkie sufiksy nie sa unikatowe

    vector<vector<int> > array(num_class);  // tablica dla sortowania liczeniem
    vector<int> c_new(length);  // nowe klasy rownowaznosci
    int distance = (1 << iterator) / 2; // 1 --> 1, 2 --> 2, 3 --> 4, 4 --> 8
    int num_class_new = counter = 0;  // nowa liczba klas

    for (int i = 0; i < length; i++) {
      int k = (p[i] - distance + length) % length;
      array[c[k]].push_back(k);
    }

    for (int i = 0; i < num_class; i++) {
      for (size_t j = 0; j < array[i].size(); j++) {
        // jesli sufix zaczyna nowa klase rownowaznosci
        if (j == 0 || c[(array[i][j] + distance) % length] != c[(array[i][j - 1] + distance) % length]) num_class_new++;
        c_new[array[i][j]] = num_class_new - 1;
        p[counter++] = array[i][j];
      }
    }
    c = c_new;
    num_class = num_class_new;
  }

  return vector<int>(p.begin() + 1, p.end()); // zwracanie tablicy sufiksowej
}

//=================================================================================== algorytm Kasai tworzy tablice lcp
int Kasai(string seq, vector<int> suffix_array) {

  if (seq.length() == 1) return 0; // przyspieszenie dla mononukleotydu

  int length_seq = seq.size();
  vector<int> lcp(length_seq, 0); // tablica lcp
  vector<int> rank(length_seq, 0); // tablica odwrotna do sufiksowej

  for (int i = 0; i < length_seq; i++) rank[suffix_array[i]] = i;

  for (int i = 0, k = 0; i < length_seq; i++, k ? k-- : 0) {
    if (rank[i] == length_seq - 1) {
      k = 0;
      continue;
    }

    int j = suffix_array[rank[i] + 1];
    while (i + k < length_seq && j + k < length_seq && seq[i + k] == seq[j + k]) k++;
    lcp[rank[i]] = k;
  }

  int sum = 0;
  for (auto item : lcp) sum += item;

  return sum; // suma elementow (duplikaty)
}

/* wprowadzenie argumentow z koncoli z uwzglednieniem opcji (szerokosc oraz prog)
 * czyli mozna ich nie podawac albo podawac w roznej lolejnosci
 * dazymy do formatu file.cpp seq.fasta -w 10 -t 0.9 > out.txt
 * Have 6 arguments:
 * i = 0 => file.cpp
 * i = 1 => seq.fasta
 * i = 2 => -w
 * i = 3 => 10
 * i = 4 => -t
 * i = 5 => 0.9
 * Z powyzszego wydruku widzimy, ze sa potezebne argumenty o indeksach nieparzystych :
 * nazwa pliku, szerokosc okna oraz prog, od ktorego uznajemy zlozonosc za niska */

int main(int argc, char **argv) {

  //============================================================================================ definicja zmiennych
  fstream file_from; // plik dla odczytu
  string sequence; // sekwencja oryginalna
  string cut_sequence; // sekwencja po wprowadzeniu szerokosci okna
  unordered_set<string> result; // set z uwzglednieniem progu zlozonosci
  int location[2] = {-1, -1}; // tablica przechowuje polozenie
  float lc; // zlozonosc lingwistyczna
  int max_substring; // mianownik (liczba podciagow alfabetu)
  int unique; // licznik (liczba unikatowych podciagow)

  // wprowadzenie wartosci defaultowych
  int wide = 12; // szerokosc okna (wartosc programu seg)
  float threshold = 0.6; // prog (wartosc = 2.5 / 4.322 z programu seg)

  for (int i = 2; i < argc; i += 2) { // nadanie wartosci argumentom
    if ((string)argv[i] == "-w" &&  stoi(argv[i + 1]) > 0) wide = stoi(argv[i + 1]);
    else if ((string)argv[i] == "-t" && (float(stoi(argv[i + 1])) >= 0 || float(stoi(argv[i + 1])) <= 1))
      threshold = float(stoi(argv[i + 1]));
    else {
      cout << "Niepoprawny przelacznik albo niedozwolona wartosc parametru" << endl
           << "Kontynuować z wartosciami domyslnymi? ( yes / no) ";
      cin >> sequence;

      if (sequence == "no") return -1; // blad, koniec programu
      wide = 12;
      threshold = 0.6;
    }
  }

  // szerokosc okna jest jednoczesnia dlugoscia cut, wiec ze wzoru n * (n + 1) / 2 znajdziemy liczbe wszystkich podciagow
  unique = wide * (wide + 1) / 2;
  max_substring = MaxSubstring(wide);

  //================================================================================= odczyt pliku
  file_from.open(string(argv[1]).c_str(), fstream::in);
  getline(file_from, sequence); // fasta jest zapisany w jednej linii
  sequence = sequence.substr(sequence.find_last_of(' ') + 1); // sekwecja jest ostatnim slowem
  file_from.close();

  if (wide > sequence.length()) {
    cout << "Szerokosc okna jest wieksza od dlugosci sekwencji" << endl;
    return -1;
  }

  //================================================================================= algorytm
  for (int window_start = 0; window_start + wide <= sequence.length(); window_start++) { // wprowadzenie przesuwania

    cut_sequence = sequence.substr(window_start, wide); // ograniczenie sekwencji do rozmiaru okna
    unique -= Kasai(cut_sequence, SuffixArray(cut_sequence)); // podwojne uzycie funlcji, aby nie tworzyc dodatkowych zmienncyh

    // jesli region ma niska zlozonosc, to dodaje zbior do tablicy wynikowej
    lc = float(unique) / float(max_substring); // rzutowanie typu
    if (lc <= threshold && !result.count(cut_sequence)) {
      result.insert(cut_sequence);

      //================================================================================= wypisywanie z polaczeniem
      // jesli wartosci polozenia sa defaultowe, to ustwic nowe
      if (location[0] == -1 && location[1] == -1) {
        location[0] = window_start;
        location[1] = window_start + wide;
      }
        // jesli sekwencji sie pokrywaja, to wydluzyc koniec [0,10), [5,20) --> [0,20)
      else if (window_start < location[1]) location[1] = window_start + wide; // nie wchodzi

      else { // inaczej wypisac poprzednika i zdefiniowac polozenie na nowo
        cout << ">id " << string(argv[1]).substr(0, string(argv[1]).find_last_of('.')) << " " << location[0] + 1 << ":"
             << location[1] << endl;
        cout << sequence.substr(location[0], location[1] - location[0]) << endl;

        location[0] = window_start;
        location[1] = window_start + wide;
      }
    }
  } // koniec algorytmu

  if (lc <= threshold) { // wypisac pozostalosc z buforu, jesli ostatnia zlozonosc byla dobra
    cout << ">id " << string(argv[1]).substr(0, string(argv[1]).find_last_of('.')) << " " << location[0] + 1 << ":"
         << location[1] << endl;
    cout << sequence.substr(location[0], location[1] - location[0]) << endl;
  }

  return 0;
}
