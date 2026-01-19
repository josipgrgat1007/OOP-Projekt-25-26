# Blackjack - C++ konzolna igra

## Opis
Projekt je konzolna implementacija igre Blackjack napisana u C++.
Igra podržava više igrača, AI igrače, split i double down prema osnovnim pravilima Blackjacka.

## Kako pokrenuti projekt

### Opcija 1: Pokretanje izvršne datoteke (Windows)
1. Otvoriti folder "Blackjack"
2. Pokrenuti "Blackjack.exe"

### Opcija 2: Kompajliranje iz izvornog koda
Potreban je C++ kompajler s podrškom za C++17.

Primjer (g++):
```bash
g++ *.cpp -o blackjack
```
Nakon toga pokrenuti:
./blackjack (Linux/macOS) ili blackjack.exe (Windows)

---

## Kontrole u igri

- **h** - Hit (povlačenje karte)
- **s** - Stand (završetak poteza)
- **d** - Double down (udvostručavanje uloga, jedna dodatna karta)
- **p** - Split (ako je dopušten prema pravilima)

---

## Pravila (sažeto)

- Cilj je postići zbroj karata što bliži 21 bez prelaska
- Dealer igra nakon svih igrača
- Ako igrač prijeđe 21 -> bust
- Ako dealer prijeđe 21 -> svi preostali igrači pobjeđuju
- **Blackjack**
    - Blackjack vrijedi samo ako igrač ima točno dvije karte
    - Isplata za Blackjack je x2.5
    - Blackjack nakon splita se ne računa kao pravi Blackjack
- **Split**
    - Moguć kada igrač ima dvije iste karte ili dvije karte iste vrijednosti (Npr. K i Q)
    - Potrebno je uložiti još jedan isti ulog
    - Nakon splita, svaka ruka se igra zasebno
    - Nakon splita dopušteni su samo hit i stand
- **Double down**
    - Ulog se udvostručuje
    - Igrač povlači točno jednu dodatnu kartu
    - Nakon toga igrač automatski završava potez

---

## AI igrači
Ako je broj ljudskih igrača manji od maksimalnog broja (5),
igra automatski dodaje AI igrače.

AI igrači samostalno donose odluke na temelju vrijednosti karata

Postoje dva stila igre AI igrača:
- Safe AI - igra oprezno i češće bira stand
- Aggressive AI - češće bira hit i double down
AI koriste iste mehanike kao ljudski igrači

---

## Ovisnosti
Projekt nema vanjskih ovisnosti. Koriste se standardne C++ biblioteke.

---

## Mogući problemi i rješenja

- Ako se program ne pokrene dvostrukim klikom, potrebno ga je pokrenuti iz terminala
- Neispravan unos s tipkovnice može zahtijevati ponovni unos
- Ako se broj karata u špilu smanji ispod dozvoljenog praga, špil se automatski ponovno miješa.
- Projekt je namijenjen izvođenju u konzoli

---

## Napomena
Projekt je izrađen u edukativne svrhe za potrebe kolegija.