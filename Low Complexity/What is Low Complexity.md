<h1> Złożoność lingwistyczna DNA </h1>

<h3> Cele i możliwości </h3>

Celem projektu jest utworzenie uproszczonej wersji programu **seg** - narzędzia bioinformatycznego, wykorzystanego dla predykcji LCR. 
<br> Program jest wywoływany z linii poleceń, odczytuje sekwencję z pliku w formacie FASTA. Użytkownik ma dwa parametry, które może podawać w różnej kolejności:
 - *w* - szerokość okna w którym liczona jest wartość lc, okno przesuwa się o 1 nukleotyd.
 -  *t* - próg od 0 do 1, od którego uznajemy region za lc.
 -  Opcja *help* wyświetli się, jeśli podać samą nazwę skryptu

Program powinien zwrócić te regiony w postaci podsekwencji w formacie poniżej. Sekwencje, które nakładają się na siebie, są połączone przy wypisywaniu.
<br> >id seq 20:40 (położenie)
<br> ACGACGAAAAA...

<h3> Czym jest LCR? </h3>
Regiony o niskiej złożoności (LCRs) są fragmentami sekwencji aminokwasowych wyróżniającymi się nierównomiernym wykorzystaniem puli znaków alfabetu.LCR odgrywają istotną rolę podczas ekspresji białek heterologicznych.

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

<br> Przykład. Niech *s* = ATTTGGATT, moc alfabrtu DNA *(a)* = 4. Wtedy *sub(s)* = 35, a *m(a,n)* = 40, więc *lc* = 0.875.

<h3> Wykorzystane algorytmy </h3>

Dla obliczenia liczebnika trzeba rozwiązać problem "Wyznaczanie liczby podsłów







