# Dokumentace

*SpaceGame* je vesmírná střílečka inspirovaná hrou Asteroids od známé firmy Atari, ve které ovládáte svou vlastní vesmírnou loď a snažíte se přežít v roji asteroidů. Těm se musíte vyhnout a pomocí raket všechny sestřelit, abyste se mohli dostat do další úrovně. Hra nemá konec, cílem je přežít co nejdéle.

Program je napsaný v jazyce C++ s použitím [GLFW](https://www.glfw.org/) a [glad](https://github.com/Dav1dde/glad) pro práci s OpenGL a pomocných knihoven [stb](https://github.com/nothings/stb) pro načítání textur a [glm](https://github.com/g-truc/glm) pro matematické výpočty.

## Sestavení

Jako systém pro sestavení byl použit [cmake](https://cmake.org/), který můžete použít pro kompilaci programu. Je potřeba mít k dispozici OpenGL verze alespoň 3.3 a případně některé závislosti knihovny GLFW. Více informací na [webu GLFW o kompilaci](https://www.glfw.org/docs/latest/compile.html).

Hra se po sestavení skládá ze spustitelného programu `SpaceGame` (případně `SpaceGame.exe`) a adresáře `res/`, který obsahuje další zdroje a musí být ponechán ve stejném adresáři jako `SpaceGame`, jinak program nebude fungovat.

### Linux

Pro Linux byl vytvořen skript `build.sh`, který s využitím `cmake` a `make` program zkompiluje do adresáře `./build/bin/`.

```shell
$ ./build.sh
```

Hru pak lze spustit z příkazové řádky:

```shell
$ cd /build/bin/
$ ./SpaceGame
```

### Windows

Sestavení na Windows můžete provést pomocí kompilátoru *MSVC* a *Visual Studia*. Můžete použít například *cmake GUI* nebo *PowerShell*:

```powershell
> mkdir build/
> cd build/
> cmake ../
```

Otevřete *solution* soubor `SpaceGame.sln` a zvolte požadovanou konfiguraci. Sestavte projekt s názvem `ALL_BUILD`. V adresáři `./build/bin` by se měl objevit přeložený program `SpaceGame.exe` (případně v nějakém podadresáři s názvem konfigurace).

## Ovládání a průběh hry

Po spuštění programu se objeví okno hry. Na začátku se Vaše vesmírná loď nachází ve středu okna a za krátkou dobu začnou ze stran obrazovky létat asteroidy.

Svou loď ovládejte pomocí šipek a mezerníku:

- **Šipka doleva** = otočení lodi proti směru hodinových ručiček.
- **Šipka doprava** = otočení lodi po směru hodinových ručiček.
- **Šipka nahoru** = pohyb lodi kupředu.
- **Mezerník** = střelba.

Asteroid zasáhnutý raketou je zničen a zmizí. Po sestřelení posledního asteroidu se dostanete do další úrovně a ze stran vyletí větší počet asteroidů, než jaký byl v předchozí úrovni.

Pokud Váši loď některý z asteroidů zasáhne, hra končí a po krátké pauze začínáte opět od první úrovně. Aktuální úroveň můžete sledovat na levém horním okraji obrazovky jako počet malých ikonek vesmírné lodi.

Pokud Vaše loď , raketa nebo některý z asteroidů přeletí okraj obrazovky, objeví se na jejím opačném konci se stejným směrem pohybu.

Hru můžete kdykoliv ukončit zavřením okna nebo stisknutím klávesy **ESC**.

## Vývojová dokumentace

Všechny třídy a namespace jsou rozděleny do hlavičkových (`.hpp`) a zdrojových (`.cpp`) souborů. Vlastní kód se nachází v adresáři `src/`, použité knihovny jsou v adresářích `lib/` a `include/`.

### Abstrakce OpenGL

Knihovna GLFW je použita pro vytvoření okna a získání OpenGL kontextu. Aby byla práce s OpenGL snazší, byly její funkce seskupeny do tříd. Příkladem jsou třídy `Shader` pro kompilaci a používání GLSL programů a `Texture2D` pro generování textur.

Třída `Renderer` slouží pro nakreslení tvaru na obrazovku. Omezil jsem se pouze na kreslení dvou trojúhelníků jako jednoho obdélníku (quad), který je pokryt barvou nebo texturou. Tvar se tak nemusí reprezentovat seznamem vrcholů, stačí načíst texturu, kde je celý tvar jako jeden obrázek. To se nakonec neukázalo jako nejlepší nápad, protože do programu musely být stejně zaneseny podrobnější souřadnice vrcholů kvůli kolizím.

### Hierarchie `GameObject`

Všechny třídy reprezentující herní objekty (vesmírnou loď, asteroidy a rakety) jsou odvozeny z jediné třídy `GameObject`. Ta obsahuje všechny potřebné údaje pro nakreslení objektu na herní scénu. Má jedinou abstraktní metodu `void update(float deltaTime)`, kterou ostatní doplňují o výpočet svého pohybu.

### `Game`

Chod hry řídí třída `Game` zahrnující konstanty pro velikosti okna, obrázků, rychlosti objektů, a další. Po inicializaci běží cyklus, ve které se neustále aktualizují veškeré objekty ve hře, kontrolují se kolize a případně se mění stav hry.

S přibývajícím počtem funkcí a konstant se třída `Game` postupně stávala nepřehlednou. Některé funkce byly kvůli tomu vyčleněny do dalších tříd a namespaců, jako například třídy `Window` pro vytváření okna, `ResourceManager` pro ukládání textur a shaderů a namespace `rnd` pro generování náhodných čísel.

### Pohyb hráče

Vesmírná loď má v každém okamžiku vektor rychlosti, který určuje její rychlost (délka vektoru) a směr. Samotné otáčení lodi tento směr nijak nemění. Při zmáčknutí šipky nahoru (pohyb kupředu) na vesmírnou loď zapůsobí síla směrem, kam je zrovna natočená. Hmotnost lodi zanedbávám (m = 1kg) a rychlost počítám podle vzorce F = m * a = m * v / dt. Vektor rychlosti vypočtený podle této síly přičtu k aktuální rychlosti lodi.

Pro zpomalení hráče jsem se původně snažil přidat odporovou a později i třecí sílu, ale ty nedosahovaly dobrých výsledků, parametry byly málo čitelné a špatně se nastavovaly. Nakonec nejlépe zafungoval koeficient *decay* (v programu nastaven na hodnotu 0,99), kterým se při zavolání metody `update` vektor rychlosti vynásobí. Na tento koeficient jsem nepřišel sám, ale našel jej vícekrát zmíněný na různých fórech na internetu (např. na [gamedev.net](https://www.gamedev.net/forums/topic/188760-2d-spaceship-movement/)), kde byl řešen stejný problém.

### Detekce kolizí

Každý objekt ve hře má tvar mnohoúhelníku. Kolize mezi těmito mnohoúhelníky jsou zjišťovány hledáním průsečíků stran (úseček). Toto řešení nefunguje pro případ, kdy je mnohoúhelník celý obsažen v jiném mohoúhelníku, ale ukázal se jako dostačující. Algoritmus pro hledání průsečíku dvou úseček byl převzatý z [geeksforgeeks](https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/).

Původně byly kolize detekovány pomocí algoritmu [ray-casting](https://en.wikipedia.org/wiki/Point_in_polygon#Ray_casting_algorithm) a zjišťováním, jestli nějaký bod jednoho mnohoúhelníku leží v jiném. Nepodařilo se mi ale algoritmus dostatečně správně implementovat a kvůli chybám v přesnosti výpočtu docházelo například ke kolizím objektů ve stejné výšce, i když byly na opačných stranách obrazovky.

## Možná vylepšení

Po technické stránce by mohl mít program lepší objektový návrh. Pokud by se přidávaly další scény (jako například menu), hodilo by se mít třídu `GameScene` obsahující prvky dané scény. Třída `Game` by pak pouze přepínala scény a vše ostatní delegovala na aktuální scénu.

`Renderer` používá jeden buffer s obdélníkem a kreslí jej se statickým nastavením (`GL_STATIC_DRAW`). Herní objekty se pak musí kreslit po jednom a s jejich větším počtem může dojít k výraznému zpomalení. Místo toho by pro dynamické kreslení mohl existovat `BatchRenderer`, který pro každý objekt dostane jeho `Model` s vrcholy, které postupně střádá v bufferu a na konci všechny objekty nakreslí jedním zavoláním `draw`.

Hra by se dala rozšířit o další prvky, jako třeba nepřátele, bonusy, různé úrovně a další.

