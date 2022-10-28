Ekaterina Mashchenko, Katarzyna Chodak
<br><br><h1> Baza danych filmów </h1>

<h3> Cel i możliwości </h3>
Zaprojektowana przez autorki baza przechowuje dane symulowanego portalu do oceny filmów. Dwie najważniejsze tabele to tabela użytkowników - którzy mogą dodawać filmy do ulubionych oraz je oceniać i komentować - oraz tabela filmów. Przygotowana baza pozwala na przeglądanie różnorodnych informacji na temat filmów: zarówno statystyk, dotyczących np. czasu trwania poszczególnych filmów, ale również przeglądania aktorów biorących w nich udział, wygranych nagród, czy wyszukiwania filmów po gatunku czy tematyce.

<h3> Główne założenia </h3>
Do zaprojektowania i stworzenia bazy przyjęto szereg założeń celem optymalizacji pracy autorek i działania bazy, a jednocześnie zachowania ogromnej różnorodności w przemyśle filmowym:
<ul>
<li> nazwa filmu ma maksymalną długość 100 znaków; </li>
<li> każdy identyfikator filmu może być wspomniany jeden lub więcej razy w tabelach Movies & Locations, Movies & Colors, Movies & Languages, Movies & Topics, Movies & Genres, Movies & Companies; </li>
<li> dana persona może występować w jednym filmie w charakterze aktora/aktorki lub reżysera; </li>
</ul>

<h3> Ograniczenia przyjęte przy projektowaniu </h3>

Zaprojektowana i wykonana przez 2 osoby baza nie jest zbyt obszerna - znajduje się w niej 50 najlepiej ocenianych filmów (na dzień 13.02.2022) według serwisu <strong> IMDb </strong> oraz związanych z nimi ludzi kina: reżyserów oraz aktorów wcielających się w role głównych postaci. Nie zawiera ona również wszystkich informacji na temat każdego filmu, a jedynie wybrane, uznane za najważniejsze przez projektantki. Dodatkowo, przyjęto pewne uproszczenia w informacji na temat nagród filmowych, jako że w rzeczywistości jest ich bez liku, jak i również są one przyznawane w wielu kategoriach - w bazie jednak odnotowane jest osiągnięcie jedynie w przypadku wygrania głównej nagrody konkursu. Również w przypadku osób przemysłu filmowego przyjęto założenie, że dana nagroda w danym roku mogła zostać przyznana jedynie jednej osobie..</p>

<h3> Diagram ER - schemat bazy danych </h3>
Diagram ER przygotowano przy pomocy aplikacji <strong> lucid.app. </strong>

![This is an image](https://lucid.app/lucidchart/7925d6ed-8bda-4c83-90be-d83c6eff89fd/edit?viewport_loc=-50%2C-142%2C2559%2C1036%2C0_0&invitationId=inv_04eacf7d-6155-4d6c-9c57-30712cb60927)
