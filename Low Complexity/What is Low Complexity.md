<h1> Złożoność lingwistyczna DNA </h1>

<h3> Cele i możliwości </h3>

Celem projektu jest utworzenie uproszczonej wersji programu **seg** - narzędzia bioinformatycznego, wykorzystanego dla predykcji LCR. 
<br> Program jest wywoływany z linii poleceń, odczytuje sekwencję z pliku w formacie FASTA. Użytkownik ma dwa parametry, które może (nie) podawać w różnej kolejności:
 - *w* - szerokość okna w którym liczona jest wartość lc, okno przesuwa się o 1 nukleotyd (domyślnie 12).
 -  *t* - próg od 0 do 1, od którego uznajemy region za lc (domyślnie 0.6).
 -  Opcja *help* wyświetli się, jeśli podać samą nazwę skryptu.

Przykład uruchomienia:
<pre><code>g++ -o lc lc.cpp
./lc example.fasta -w 9 -t 0.875
</code></pre>

Program powinien zwrócić te regiony w postaci podsekwencji w formacie poniżej. Sekwencje, które nakładają się na siebie, są połączone przy wypisywaniu.

<pre><code>>id seq 20:40 (położenie)
ACGACGAAAAA...
</code></pre>

<h3> Czym jest LCR? </h3>
Regiony o niskiej złożoności (LCRs) są fragmentami sekwencji aminokwasowych wyróżniającymi się nierównomiernym wykorzystaniem puli znaków alfabetu LCR odgrywają istotną rolę podczas ekspresji białek heterologicznych.

<h3> Jak obliczyć złożoność lingwistyczną? </h3>

*lc(s) = syb(s) / m(a,n),* gdzie
<br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
*lc* - złożoność lingwistyczna
<br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
*s* - sekwencja
<br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
*n* - długość sekwencji
<br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
*a* - moc alfabetu
<br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
*sub(s)* - liczba unikalnych podciągów łańcucha
<br> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
*man(a,n)* - maksymalna możliwa liczba unikalnych podciągów z wykorzystaniem całego alfabetu

<br> Przykład. Niech *s* = ATTTGGATT, moc alfabetu DNA *(a)* = 4. Wtedy *sub(s)* = 35, a *m(a,n)* = 40, więc *lc* = 0.875.

<h3> ASD (Algorytmy i struktury danych) </h3>

Dla obliczenia liczebnika trzeba wyznaczyć liczbę podsłów. Istnieją struktury danych, które mogą nam pomóc w rozwiązaniu problemu z lepszą złożonością czasową, między innymi, **tablice sufiksowe.** Używając tablicy sufiksowej, otrzymamy odpowiedź ze wzoru mianownik w czasie **O(n)** - liniowa złożoność.

Obliczenia składają się z dwóch części:
1. Ze wzoru *n * (n + 1) / 2*, gdzie *n* - liczba liter w słowie (szerokość okna) wyliczamy liczbę wszystkich podslów (nie unikatowych też).
2. Odejmujemy liczbę duplikatów - sumę elementów tablicy *lcp  (Longest Common Prefix)*.

Tablica *lcp* jest wyznaczana z *tablicy sufiksowej*. Poniżej przedstawiono pseudokody **algorytmów Karkkainena-Sandersa** (konstrukcja *tablicy sufiksowej* w liniowym czasie) oraz **Kasai** (konstrakcja *tablicy prefiksów* w liniowym czasie).
<br><br>
<img width="25%" align = "left" src="https://user-images.githubusercontent.com/80549277/198854676-8ec491f7-427c-4e33-9bc6-f27b212340ac.png" /> 
<img width="40%" src="https://user-images.githubusercontent.com/80549277/198854936-14f7fb2a-4caf-49e9-8cad-f92dd0bbb3d7.png" />
<br>

<br> Mianownik oblicza się za pomocą wariancji.
<pre><code>int MaxSubstring(int length) {
  int max_substring = 0;
  for (int i = 1; length >= 1; i++, length--) max_substring += min(length, int(pow(4, i)));
  return max_substring;
}
</code></pre>
