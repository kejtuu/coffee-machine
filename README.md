# ☕ Coffee Machine – projekt zaliczeniowy

Prosta gra konsolowa w C++, symulująca ekspres do kawy.  
Projekt zaliczeniowy ze studiów, pokazujący podstawy programowania obiektowego.  

## Mechanizmy OOP
- **Klasy i konstruktory** – np. `Coffee` z polami prywatnymi i metodami publicznymi  
- **Dziedziczenie** – `Coffee` → `Latte`, `Espresso`, `Cappuccino`, `BialaKawa`  
- **Kompozycja i agregacja** – `CoffeeShop` zarządza kawami i kubkami  

## Struktura projektu
- `Console` – obsługa konsoli  
- `Coffee` + pochodne – napoje  
- `Cup` – kubki  
- `CoffeeShop` – kolekcja kaw i kubków  
- `View` – rysowanie ekspresu i paska energii  
- `TheGame` – logika i pętla gry  
