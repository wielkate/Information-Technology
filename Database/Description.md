Ekaterina Mashchenko, Katarzyna Chodak
<br><br><h1> Baza danych filmów </h1>

<h3> Cel i możliwości </h3>
Zaprojektowana przez autorki baza przechowuje dane symulowanego portalu do oceny filmów. Dwie najważniejsze tabele to tabela użytkowników - którzy mogą dodawać filmy do ulubionych oraz je oceniać i komentować - oraz tabela filmów. Przygotowana baza pozwala na przeglądanie różnorodnych informacji na temat filmów: zarówno statystyk, dotyczących np. czasu trwania poszczególnych filmów, ale również przeglądania aktorów biorących w nich udział, wygranych nagród, czy wyszukiwania filmów po gatunku czy tematyce.

<h3> Główne założenia </h3>
Do zaprojektowania i stworzenia bazy przyjęto szereg założeń celem optymalizacji pracy autorek i działania bazy, a jednocześnie zachowania ogromnej różnorodności w przemyśle filmowym:

- nazwa filmu ma maksymalną długość 100 znaków;
- każdy identyfikator filmu może być wspomniany jeden lub więcej razy w tabelach Movies & Locations, Movies & Colors, Movies & Languages, Movies & Topics, Movies & Genres, Movies & Companies;
- dana persona może występować w jednym filmie w charakterze aktora/aktorki lub reżysera;

<h3> Ograniczenia przyjęte przy projektowaniu </h3>

Zaprojektowana i wykonana przez 2 osoby baza nie jest zbyt obszerna - znajduje się w niej 50 najlepiej ocenianych filmów (na dzień 13.02.2022) według serwisu <img width="50px" src="https://user-images.githubusercontent.com/80549277/198831938-8e35ec55-39d9-44d7-84a9-97df404f9c0e.png" /> oraz związanych z nimi ludzi kina: reżyserów oraz aktorów wcielających się w role głównych postaci. Nie zawiera ona również wszystkich informacji na temat każdego filmu, a jedynie wybrane, uznane za najważniejsze przez projektantki. Dodatkowo, przyjęto pewne uproszczenia w informacji na temat nagród filmowych, jako że w rzeczywistości jest ich bez liku, jak i również są one przyznawane w wielu kategoriach - w bazie jednak odnotowane jest osiągnięcie jedynie w przypadku wygrania głównej nagrody konkursu. Również w przypadku osób przemysłu filmowego przyjęto założenie, że dana nagroda w danym roku mogła zostać przyznana jedynie jednej osobie..</p>

<h3> Diagram ER - schemat bazy danych </h3>
Diagram ER przygotowano przy pomocy aplikacji 
<a href="https://www.frontlive.pl/css-houdini/](https://lucid.app/lucidchart/7925d6ed-8bda-4c83-90be-d83c6eff89fd/edit?viewport_loc=-50%2C-142%2C2559%2C1036%2C0_0&invitationId=inv_04eacf7d-6155-4d6c-9c57-30712cb60927"> lucid.app. </a>

Na schemacie zaznaczono klucz główny poprzez **wytłuszczenie** nazwy kolumny tabeli, a klucz obcy poprzez dopisek ‘(FK)’ oraz odpowiednie połączenie tabel. Przy łączeniu tabel przyjęto konwencję *crow’s foot*.

![Baza filmów](https://user-images.githubusercontent.com/80549277/198832142-fab83cc8-7f70-4ed2-9031-3438ab726bdd.jpeg)

Wyszczególnione związki encji:
-	Film ----- *MA* ----- gatunki
-	Film ----- *KRĘCONY W* ----- lokalizacjach
-	Film ----- *W* ----- językach
-	Film ----- *MA* ----- rodzaje
-	Film ----- *MA* ----- tematy
-	Film ----- *MA* ----- konwencję kolorystyczną
-	Film ----- *OCENIANY PRZEZ* ----- użytkowników
-	Aktor/reżyser ----- *UCZESTNICZY* ----- w filmie
-	Film ----- *WYGRYWA* ----- nagrody
-	Aktor/reżyser ----- *WYGRYWA* ----- nagrody
-	Użytkownik ----- *MA* ----- znajomych (innych użytkowników)

<h3> Dodatkowe więzy integralności danych </h3>
Na diagramie ER nie zaznaczono związku pomiędzy tabelą *Currency Conversion* a pozostałymi tabelami. Tabela *Currency Conversion* wykorzystywana jest do funkcji, która pozwala na ocenę budżetu (w tabeli *Movies* kwoty są przedstawione w dolarach amerykańskich).

<h3> Oprogramowanie bazy </h3>
W celu zachowania przejrzystości bazy, cały skrypt tworzący indeksy, widoki, funkcje i wyzwalacze znajduje się w osobnym pliku. Poniżej znajdują się jedynie fragmenty kodu odnoszące się do omawianych w danym momencie poleceń.

<h3> Opis utworzonych indeksów </h3>
Ze względu na brak potrzeby tworzenia indeksów dla tej bazy, przygotowano przykładowe indeksy, które nie są jednak nigdzie wykorzystywane.

<pre><code>CREATE INDEX ndx_movie
ON Movies (Movie_Name, Duration, IMDB_Rating)
</code></pre>

<pre><code>CREATE INDEX ndx_person
ON People (Name, Country)
</code></pre>

<h3> Opis stworzonych widoków </h3>
Poniżej przedstawiono przygotowane widoki:

<br> **1.** Widok "Ranking użytkowników". 
<br> Przedstawia listę filmów ocenionych przez użytkowników oraz wylicza ich średnią ocenę.

<pre><code>CREATE VIEW Ranking AS
	SELECT Movie_Name, AVG(Rate) AS Rating FROM Movies 
	JOIN [User's Ratings] ON Movies.MovieID=[User's Ratings].MovieID
	GROUP BY Movies.Movie_Name
  
SELECT * FROM Ranking
ORDER BY Rating DESC
</code></pre>

<br> **2.** Widok "Nagrodzeni aktorzy". 
<br> Przedstawia imię i nazwisko aktora/reżysera oraz liczbę przyznanych mu nagród.

<pre><code>CREATE VIEW Actors AS
	SELECT Name, COUNT(AwardID) AS Number_of_awards FROM People
	JOIN [People & Awards] ON People.PersonID = [People & Awards].PersonID
	WHERE Deleted_at IS NULL
	GROUP BY People.Name
  
SELECT * FROM Actors
ORDER BY Number_of_awards DESC
</code></pre>

<br> **3.** Widok "Czas trwania filmów".

<pre><code>CREATE VIEW Durations AS
	SELECT Movie_Name, Duration, IMDB_Rating FROM Movies
  
SELECT * FROM Durations
ORDER BY Duration ASC
</code></pre>

<h3> Opis procedur składowanych </h3>
Poniżej przedstawiono przygotowane procedury składowane:

<br> **1.** Procedura "Most expensive movie" dla tabeli *Movies*. 
<br> Pozwala na szybkie wyszukanie filmu o największym budżecie.

<pre><code>IF OBJECT_ID('Most_expensive_movie') IS NOT NULL DROP PROC Most_expensive_movie

GO
CREATE PROC Most_expensive_movie
AS
	SELECT Movie_Name AS 'The most expensive movie' FROM Movies 
	WHERE [Budget ($)] = (SELECT MAX([Budget ($)]) FROM Movies)
</code></pre>

<br> **2.** Procedura "Most_movies" dla tabeli *People*. 
<br> Pozwala na wyszukanie najbardziej płodnego aktora lub reżysera.

<pre><code>IF OBJECT_ID('Most movies') IS NOT NULL DROP PROC [Most movies]

GO
CREATE PROC [Most movies]
AS
	SELECT TOP 1 WITH TIES P1.Name, COUNT(P2.PersonID) AS [Number of movies] 
	FROM People P1 JOIN [People and Movies] P2 ON P1.PersonID = P2.PersonID 
	GROUP BY P1.Name
	ORDER BY [Number of movies] DESC
</code></pre>

<br> **3.** Procedura "Lista na litere" dla tabeli *People*. 
<br> Pozwala na wyszukanie osób z tabeli People, których imię zaczyna się na zadaną literę.

<pre><code>IF OBJECT_ID('lista_na_litere') IS NOT NULL DROP PROC lista_na_litere

CREATE PROC lista_na_litere ( @litera_1 CHAR )
AS
SELECT Name FROM People
WHERE Name LIKE @litera_1 + '%' 
</code></pre>

<br> **4.** Procedura "FilmyZkraju" dla tabeli *Movies*. 
<br> Pozwala na wyszukanie filmów, które chociaż po części zostały wyprodukowane w danym kraju.

<pre><code>IF OBJECT_ID('FilmyZkraju') IS NOT NULL DROP PROC FilmyZkraju

CREATE PROC FilmyZkraju ( @country NVARCHAR(50) )
AS
SELECT Movie_Name 
FROM Movies M1 JOIN [Movies and Locations] M2
ON M1.MovieID = M2.MovieID
WHERE M2.Country = @country 
</code></pre>

<br> **5.** Procedura "FilmNaTemat" dla tabeli *Movies*. 
<br> Pozwala na wyszukanie filmów o danej tematyce.

<pre><code>IF OBJECT_ID('FilmNaTemat') IS NOT NULL DROP PROC FilmNaTemat

CREATE PROC FilmNaTemat ( @temat NVARCHAR(50) )
AS
SELECT Movie_Name 
FROM Movies M1 JOIN [Movies and Topics] M2
ON M1.MovieID = M2.MovieID
WHERE M2.Topic = @temat 
</code></pre>

<br> **6.** Procedura "MoviesRated" dla tabeli *Movies*. 
<br> Pozwala na sprawdzenie, które filmy zostały ocenione przez danego użytkownika.

<pre><code>IF OBJECT_ID(MoviesRated') IS NOT NULL DROP PROC MoviesRated

CREATE PROC MoviesRated (@UserID INT) AS
	SELECT Movie_Name, [User's Ratings].UserID FROM Movies
	JOIN [User's Ratings] ON Movies.MovieID = [User's Ratings].MovieID
	WHERE (@UserID = UserID)
</code></pre>

<h3> Opis wyzwalaczy </h3>
Poniżej przedstawiono przygotowane wyzwalacze:

<br> **1.** Wyzwalacz INSTEAD OF DELETE do tabeli *People*
<br> Poniższy wyzwalacz pozwala na pseudo-usunięcie osoby z tabeli *People* - dodawana jest adnotacja w kolumnie Deleted_at o usunięciu danego dnia i danej godzinie.

<pre><code>IF OBJECT_ID('On_Delete', 'TR') IS NOT NULL DROP TRIGGER On_Delete

GO
CREATE TRIGGER On_Delete ON People
INSTEAD OF DELETE
AS
	UPDATE People
	SET Deleted_at = CURRENT_TIMESTAMP
	WHERE (PersonID IN (SELECT PersonID FROM deleted D WHERE (PersonID = D.PersonID)))
GO
  
DELETE FROM People WHERE PersonID = 10
</code></pre>

<br> **2.** Wyzwalacz AFTER UPDATE do tabeli *People*.
<br> Poniższy wyzwalacz pozwala na śledzenie zmian dokonywanych w tabeli *People* - po aktualizacji danych dodawana jest adnotacja o dacie i godzinie zmiany.

<pre><code>IF OBJECT_ID('On_Update_P', 'TR') IS NOT NULL DROP TRIGGER On_Update_P

GO
CREATE TRIGGER On_Update_P ON People
AFTER UPDATE
AS
	UPDATE People
	SET Updated_at = CURRENT_TIMESTAMP
	WHERE (PersonID IN (SELECT PersonID FROM deleted D WHERE (PersonID = D.PersonID)))
GO

UPDATE People
SET Date_of_death = '2022-02-13'
WHERE PersonID = 5
</code></pre>

<br> **3.** Wyzwalacz AFTER UPDATE do tabeli *Movies*
<br> Ten wyzwalacz również pozwala na śledzenie zmian, ale tym razem dla tabeli *Movies*.

<pre><code>IF OBJECT_ID('On_Update_M', 'TR') IS NOT NULL DROP TRIGGER On_Update_M

GO
CREATE TRIGGER On_Update_M ON Movies
AFTER UPDATE
AS
	UPDATE Movies
	SET Updated_at = CURRENT_TIMESTAMP
	WHERE (MovieID IN (SELECT MovieID FROM deleted D WHERE (MovieID = D.MovieID)))
GO

UPDATE Movies
SET Budget = 19000000
WHERE MovieID = 15
</code></pre>

<h3> Skrypt tworzący bazę danych </h3>
W celu zachowania przejrzystości opisu bazy, skrypt znajduje się w oddzielnym <a href="https://www.frontlive.pl/css-houdini/](https://github.com/wielkate/Information-Technology/blob/main/Database/SQL%20tables.MYSQL"> pliku.</a>

<h3> Typowe zapytania </h3>
Typowe zapytania zostały przedstawione jako przykładowe zapytania przy omówieniu poszczególnych W celu zachowania przejrzystości opisu bazy, skrypt znajduje się w oddzielnym <a href="https://www.frontlive.pl/css-houdini/](https://github.com/wielkate/Information-Technology/blob/main/Database/SQL%20tables.MYSQL](https://github.com/wielkate/Information-Technology/blob/main/Database/Function%2C%20view%2C%2C%20trigger.MYSQL"> funkcji i procedur.</a>
