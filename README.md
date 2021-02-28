Zadanie Naliczanie Opłat Drogowych

Należy napisać program, który wypisuje podsumowanie przejechanych przez pojazdy
odległości drogami płatnymi. Program czyta dane ze standardowego wejścia.
Program analizuje dane wejściowe linia po linii. Program ignoruje puste
linie. Dla każdej niepustej linii program decyduje, czy jest ona poprawna.
Poprawna linia zawiera informację o wjeździe pojazdu na drogę płatną lub
zjeździe pojazdu z takiej drogi albo polecenie do wykonania. Elementy składowe
poprawnej linii oddziela się białymi znakami. Białe znaki mogą wystąpić
w dowolnej ilości również na początku i końcu linii.

Informacja o ruchu pojazdu zawiera jego numer rejestracyjny, oznaczenie drogi,
kilometraż węzła wjazdu na tę drogę lub zjazdu z tej drogi. Numer rejestracyjny
pojazdu jest to napis składający się z dużych i małych liter alfabetu
angielskiego oraz cyfr od 0 do 9, zawierający od 3 do 11 znaków. Oznaczenie
drogi składa się z litery A (autostrada) lub S (droga ekspresowa) i numeru
drogi, który jest liczbą dziesiętną z zakresu od 1 do 999 bez zer wiodących.
Kilometraż węzła wjazdu na drogę lub zjazdu z drogi podaje się w kilometrach
z dokładnością do 100 metrów, z obowiązkową częścią ułamkową oddzieloną od
części całkowitej przecinkiem, bez dodatkowych zer wiodących. Informacje o ruchu
pojazdów pojawiają się na wejściu programu w takiej kolejności, jak pojazdy
przejeżdżały przez kolejne węzły. Przykłady informacji o ruchu pojazdów:

W1234567 A1 234,0
12P2000 A10 734,1
ABC S90 0,0
eLo S10 0,9

Polecenie do wykonania składa się ze znaku zapytania, po którym może wystąpić
oznaczenie drogi lub numer rejestracyjny pojazdu. Wynikiem polecenia bez
parametrów jest wypisanie dla każdego pojazdu informacji o liczbie kilometrów
przejechanych poszczególnymi kategoriami dróg oraz dla każdej drogi łącznej
liczby kilometrów przejechanych nią przez pojazdy. Jeśli polecenie ma
parametr, to program wypisuje tylko informacje dla pojazdu o podanym numerze
rejestracyjnym lub dla podanej drogi. Jeśli podany parametr jest zarówno numerem
rejestracyjnych, jak i oznaczeniem drogi, to program wypisuje obie informacje
w takiej właśnie kolejności. Polecenie wykonywane jest na podstawie dotychczas
zebranych danych. Jeśli nie ma danych do wypisania, program niczego nie
wypisuje. Po poleceniu mogą się pojawić następne dane o ruchu pojazdów i kolejne
polecenia. Program wypisuje wynik działania polecenia na standardowe wyjście.
Przykłady poleceń:

?
?A12
? WA123

Program wypisuje informacje o pojazdach posortowane leksykograficznie według ich
numerów rejestracyjnych. Program wypisuje informacje o drogach posortowane
według ich numerów, a przy jednakowych numerach według ich kategorii (autostrada
przed drogą ekspresową).

Program wypisuje informacje o numerze i zawartości błędnej linii na standardowe
wyjście diagnostyczne. Zawartość błędnej linii wypisuje dokładnie tak, jak ją
wczytał, z zachowaniem wszystkich białych znaków. Program liczy również puste,
zignorowane linie.

Jeśli w danych wejściowych brakuje informacji o zjeździe pojazdu, to program
wypisuje informację o błędnej linii zawierającej niesparowaną informację
o wjeździe.
