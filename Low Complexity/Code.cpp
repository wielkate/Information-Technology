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
 *
 * long
*/

#include <iostream>
#include <unordered_set>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

//================================================== funkcja wylicza mianownik, przyjmujac szerokosc okna jako argument
int MaxSubstring(int length) {
  int max_substring = 0;
  for (int i = 1; length >= 1; i++, length--) max_substring += min(length, int(pow(4, i)));
  return max_substring;
}

//if (sequance_char.length() == 1) return vector<int>{0}; // przyspieszenie, mononukleotyd ma {0}

int GetMax(const vector<int> &array) {
  int maximum = -1;
  for (auto i : array) if (i > maximum) maximum = i;
  return maximum;
}

inline bool Compare2(int i, int j, int k, int l) {
  return (i < j || i == j && k < l);
}

inline bool Compare3(int i, int j, int k, int l, int m, int n) {
  return Compare2(i, j, k, l) || (i == j && k == l && m < n);
}

vector<int> RadixSort(vector<int> numerical_long, vector<int> index, int num_tripletes, int max_element, int position) {

  vector<int> count_sort(max_element + 1);
  vector<int> sorted_triplets(num_tripletes);

  for (int i = 0; i < num_tripletes; i++) count_sort[numerical_long[index[i] + position]]++;
  for (int i = 1; i <= max_element; i++) count_sort[i] += count_sort[i - 1];
  for (int i = num_tripletes - 1; i >= 0; i--)
    sorted_triplets[--count_sort[numerical_long[index[i] + position]]] = index[i];

  return sorted_triplets;
}

bool NameTriplets(int tripletNumber, vector<int> numerical_long, vector<int> sorted, vector<int> &lex_name) {
  int name = 1;
  bool is_unique = true;
  for (int i = 1; i < tripletNumber; i++) {
    if ((numerical_long[sorted[i]] == numerical_long[sorted[i - 1]]) &&
        (numerical_long[sorted[i] + 1] == numerical_long[sorted[i - 1] + 1]) &&
        (numerical_long[sorted[i] + 2] == numerical_long[sorted[i - 1] + 2]))
      is_unique = false;
    else name++;

    lex_name[i] = name;
  }
  return is_unique;
}

vector<int> ConcatenateTripletNames(int n0, int num_triplets, vector<int> sorted_tripletrs, vector<int> lex_name) {

  vector<int> lex_name_sorted(num_triplets);

  for (int i = 0; i < num_triplets; i++) {
    if (sorted_tripletrs[i] % 3 == 1) lex_name_sorted[(sorted_tripletrs[i] - 1) / 3] = lex_name[i];
    if (sorted_tripletrs[i] % 3 == 2) lex_name_sorted[(sorted_tripletrs[i] - 2) / 3 + n0] = lex_name[i];
  }

  return lex_name_sorted;
}

void TransformToA12(int n0, int num_triplets, vector<int> suffix_array, vector<int> &A12) {
  for (int i = 0; i < num_triplets; i++) {
    if (suffix_array[i] < n0) A12[i] = 1 + 3 * suffix_array[i];
    else A12[i] = 2 + 3 * (suffix_array[i] - n0);
  }
}

void deriveA0(int num_triplets, vector<int> &A12, vector<int> &A0) {
  for (int i = 0, j = 0; i < num_triplets; i++)
    if (A12[i] % 3 == 1) A0[j++] = A12[i] - 1;
}

void Merge(int n,
           int tripletNumber,
           vector<int> numerical_string,
           vector<int> &A12,
           vector<int> &A0,
           vector<int> &resultSA) {
  vector<int> R12(numerical_string.size());
  int n0 = (int) ceil(n / 3.0);

  int index0 = 0;
  int index12 = 0;
  int m = 0;
  bool end = false;

  if (n % 3 == 1) index12 = 1;

  for (int i = 0; i < tripletNumber; i++) R12[A12[i]] = i + 1;

  for (int i = 0; i < tripletNumber + n0; i++) {
    if (index0 == n0) {
      while (index12 < tripletNumber) resultSA[m++] = A12[index12++];
      end = true;
    }
    if (end) break;

    if (index12 == tripletNumber) {
      while (index0 < n0) resultSA[m++] = A0[index0++];
      end = true;
    }
    if (end) break;

    if (A12[index12] % 3 == 1) {
      if (Compare2(numerical_string[A0[index0]],
                   numerical_string[A12[index12]],
                   R12[A0[index0] + 1],
                   R12[A12[index12] + 1])) {
        resultSA[m++] = A0[index0++];
      } else resultSA[m++] = A12[index12++];
    } else {
      if (Compare3(numerical_string[A0[index0]],
                   numerical_string[A12[index12]],
                   numerical_string[A0[index0] + 1],
                   numerical_string[A12[index12] + 1],
                   R12[A0[index0] + 2],
                   R12[A12[index12] + 2])) {
        resultSA[m++] = A0[index0++];
      } else resultSA[m++] = A12[index12++];
    }
  }
}

vector<int> MakeSuffixArray(vector<int> numerical, int max_element) {
  int n = numerical.size(); // liczba elementow wektora
  int n0 = (int) ceil(n / 3.0);
  int n1 = (int) ceil((n - 1) / 3.0);
  int n2 = (int) ceil((n - 2) / 3.0);
  int num_triplet = n0 + n2;

  // dopelnienie zerami
  if (n % 3 == 0 || n % 3 == 2) numerical.insert(numerical.cend(), {0, 0});
  else numerical.insert(numerical.cend(), {0, 0, 0});

  // tablica indelsow ktore nie sa podzielone przez 3
  vector<int> index;
  for (int i = 0; i < numerical.size() - 2; i++) if (i % 3 != 0) index.push_back(i);

  vector<int> sorted_triplets; // posortowane trojki

  // radix sort (pozycja od konca)
  sorted_triplets = RadixSort(numerical, index, num_triplet, max_element, 2);
  sorted_triplets = RadixSort(numerical, sorted_triplets, num_triplet, max_element, 1);
  sorted_triplets = RadixSort(numerical, sorted_triplets, num_triplet, max_element, 0);

  // nazwy trojek
  vector<int> lex_name(num_triplet);
  lex_name[0] = 1;

  bool is_unique = NameTriplets(num_triplet, numerical, sorted_triplets, lex_name);

  // laczenie nazw w dobrym porzadku
  vector<int> lex_name_sorted;
  lex_name_sorted = ConcatenateTripletNames(n0, num_triplet, sorted_triplets, lex_name);

  // budowa tablicy sufiksowej
  vector<int> suffix_array(num_triplet);

  if (!is_unique) {
    int maximum = GetMax(lex_name_sorted);
    suffix_array = MakeSuffixArray(lex_name_sorted, maximum);
  } else for (int i = 0; i < num_triplet; i++) suffix_array[lex_name_sorted[i] - 1] = i;

  // transformacja suffix w A12
  vector<int> A12(num_triplet);
  TransformToA12(n0, num_triplet, suffix_array, A12);

  // wyorebnienie A0 z A12
  vector<int> A0(n0);
  deriveA0(num_triplet, A12, A0);

  A0 = RadixSort(numerical, A0, n0, max_element, 0); // sortowanie radix

  // Polaczenie A12 and A0 w wynikowa tablice
  vector<int> suffix_array_exit(n0 + num_triplet - (n0 - n1));
  Merge(n, num_triplet, numerical, A12, A0, suffix_array_exit);

  return suffix_array_exit;
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
  string line; // wiersz w firmacie fasta
  string sequence; // sekwencja oryginalna
  string cut_sequence; // sekwencja po wprowadzeniu szerokosci okna
  unordered_set<string> result; // set z uwzglednieniem progu zlozonosci
  long location[2] = {-1, -1}; // tablica przechowuje polozenie
  float lc; // zlozonosc lingwistyczna
  int max_substring; // mianownik (liczba podciagow alfabetu)
  int unique; // licznik (liczba unikatowych podciagow)

  // wprowadzenie wartosci defaultowych
  int wide = 12; // szerokosc okna (wartosc programu seg)
  float threshold = 0.6; // prog (wartosc = 2.5 / 4.322 z programu seg)

  //===================================================================== opcja "help" jesli wpisac sama nazwe programu
  if (argc == 1) {
    cout << "Usage: /lc4 <file> <options>\n"
            "\t<file> - filename containing fasta-formatted sequence\n"
            "\t<options>\n"
            "\t\t-w <window> OPTIONAL window size (default 12)\n"
            "\t\t-t <threshold> OPTIONAL low complexity (default 0.6)\n";

    return 0;
  }

  for (int i = 2; i < argc; i += 2) { // nadanie wartosci argumentom
    if ((string) argv[i] == "-w" && stoi(argv[i + 1]) > 0) wide = stoi(argv[i + 1]);
    else if ((string) argv[i] == "-t" && stof((string)(argv[i + 1])) > 0 && stof((string)(argv[i + 1])) <= 1)
      threshold = stof((string)(argv[i + 1]));

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
  getline(file_from, line); // przeczytac komentarz >

  while (getline(file_from, line)) sequence += line;
  file_from.close();

  if (wide > sequence.length()) {
    cout << "Szerokosc okna jest wieksza od dlugosci sekwencji" << endl;
    return -1;
  }

  //================================================================================= algorytm
  for (long window_start = 0; window_start + wide <= sequence.length();
       window_start++) { // wprowadzenie przesuwania

    cut_sequence = sequence.substr(window_start, wide); // ograniczenie sekwencji do rozmiaru okna
    vector<int> seq_numerical;

    if (cut_sequence.length() != 1) {

      for (char &i : cut_sequence) seq_numerical.push_back(i - 'A'); // przepisac na numery alfabetu
      int maximum = GetMax(seq_numerical); // maksymalny numer w sekwencji (A == 0, C == 2, G == 6, T == 19)
      // podwojne uzycie funlcji, aby nie tworzyc dodatkowych zmienncyh
      unique -= Kasai(cut_sequence, MakeSuffixArray(seq_numerical, maximum));
    } // inaczej odejmujemy zero, czyli nic nie robimy

    // jesli region ma niska zlozonosc, to dodaje zbior do tablicy wynikowej
    lc = float(unique) / float(max_substring); // rzutowanie typu
    if (lc >= 0 && lc <= threshold && !result.count(cut_sequence)) {
      //cout << endl << "TO JEST LC == " << lc << endl;
      result.insert(cut_sequence);

      //================================================================================= wypisywanie z polaczeniem
      // jesli wartosci polozenia sa defaultowe (nielogiczne), to ustwic nowe
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

  if (lc <= threshold && !(location[0] == -1 || location[1] == -1)) { // wypisac pozostalosc z buforu, jesli ostatnia zlozonosc byla dobra
    cout << ">id " << string(argv[1]).substr(0, string(argv[1]).find_last_of('.')) << " " << location[0] + 1 << ":"
         << location[1] << endl;
    cout << sequence.substr(location[0], location[1] - location[0]) << endl;
  }

  return 0;
}
