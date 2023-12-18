## Proiect Nr. 2 - Dictionar

Sa se realizeze o aplicatie client-server care sa emuleze un dictionar.     \
Prin intermediul clientului se vor putea adauga cuvinte iar fiecarui        \
cuvant o singura definitie. Este posibila stergerea unui cuvant impreuna    \
cu definitia lui, o definitie nu poata fi stearsa ci doar inlocuita (este   \
posibil sa se introduca o definitie vida). Dictionarul trebuie initializat  \
explicit de catre client inainte de a se putea adauga cuvinte. Se           \
semnaleaza eroare in caz ca dictionarul nu a fost initializat.

Serverul va fi concurent

- a|A     <cuvant>      –   adauga cuvantul <cuvant>
- d       <cuvant>      -   adauga o definitie pentru cuvantul <cuvant>
- s       <cuvant>      -   sterge cuvantul cuvantul <cuvant> impreuna cu definitia lui
- i                     –   (re)initializeaza dictionarul (dictionar gol)

## adaugate in plus:
- w       <cuvant>      -   afiseaza toate cuvintele
- g       <cuvant>      -   afiseaza definitia cuvantului <cuvant>
- e                     -   iese din program

## Compilare

Pentru a compila proiectul, se va rula :
``` bash
make
```

Iar pentru a rula serverul :
``` bash
make run_server
```
si pentru a rula clientul :
``` bash
make run_client
```