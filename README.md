# Szachy online Jakub Jarmużek 151818

## Opis:
Projekt ten skupia się na implementacji gry w szachy, która może być rozgrywana między dwoma graczami za pomocą klienta i serwera napisanych w języku C++. Gra wykorzystuje protokół TCP do komunikacji między klientem a serwerem.

## Struktura projektu:

client.cpp: Klient gry w szachy, który łączy się z serwerem i umożliwia graczom komunikację oraz ruchy.
server.cpp: Serwer gry w szachy, który obsługuje połączenia dwóch graczy, zarządza stanem gry i przekazuje informacje między graczami.
utility.hh: Plik nagłówkowy zawierający przydatne funkcje pomocnicze, definicje stałych oraz funkcje do obsługi błędów.
game.hh: Klasa ChessGame reprezentująca logikę gry w szachy.
## Uruchamianie:

Kompilacja:
 - Skompiluj klienta za pomocą komendy: g++ client.cpp -o client -std=c++11
 - Skompiluj serwer za pomocą komendy: g++ server.cpp -o server -std=c++11
   
Uruchomienie:
 - Uruchom serwer, podając jako argumenty adres IP i numer portu: ./server <server_ip> <server_port>
 - Uruchom dwóch klientów, podając jako argumenty adres IP i numer portu: ./client <server_ip> <server_port>
## Użyte rozwiązania:

Protokół TCP: Gry w szachy wymagają niezawodnego połączenia między klientem a serwerem, dlatego użyto protokołu TCP, który gwarantuje dostarczenie danych w kolejności.

Select i Komunikacja Wieloklientowa: Serwer obsługuje wielu klientów jednocześnie za pomocą funkcji select. Umożliwia to równoczesną obsługę wielu połączeń, bez potrzeby wielu wątków.

Timeout dla Select: Wprowadzono timeout dla funkcji select, aby cykl gry mógł być przerwany po określonym czasie, co może zapobiec zawieszeniu programu.

Obsługa Błędów: Funkcje pomocnicze obsługują błędy poprzez wyświetlanie komunikatów na standardowym wyjściu błędów i zakończenie programu z kodem błędu.

Struktura Klienta i Serwera: Klient i serwer są odrębnymi programami, co pozwala na łatwe rozszerzanie funkcji i umożliwia uruchamianie klientów na różnych maszynach.

## Instrukcja Użytkowania:

Uruchom serwer, podając jako argumenty adres IP i numer portu.
Uruchom co najmniej dwóch klientów, podając jako argumenty adres IP i numer portu.
Obserwuj komunikaty serwera dotyczące połączeń i rozgrywki.
Wprowadzaj ruchy szachowe w formie "startRow startCol destRow destCol" na żądanie klienta.
Kontynuuj grę, aż jedno z graczy zwycięży lub zakończ grę manualnie.
## Zawartość Plików:

 - client.cpp: Implementacja klienta gry w szachy.
 - server.cpp: Implementacja serwera gry w szachy.
 - utility.hh: Funkcje pomocnicze, definicje stałych i obsługa błędów.
 - game.hh: Klasa reprezentująca logikę gry w szachy.
## Wnioski:
Projekt ten demonstruje praktyczne zastosowanie komunikacji międzyprocesowej w grach sieciowych. Implementacja gry w szachy obejmuje zarówno obsługę wielu klientów, jak i skomplikowane zasady ruchów szachowych. Wykorzystanie protokołu TCP, funkcji select oraz funkcji pomocniczych pozwala na stabilną i płynną rozgrywkę między graczami. Struktura projektu umożliwia także rozbudowę i modyfikacje w przyszłości.
