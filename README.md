# Dokumentace

SpaceGame je vesmírná střílečka inspirovaná hrou Asteroids od známé firmy Atari, ve které ovládáte svou vlastní vesmírnou loď a snažíte se přežít v roji asteroidů. Těm se musíte vyhnout a pomocí raket všechny sestřelit, abyste se mohli dostat do další úrovně. Hra nemá konec, cílem je přežít co nejdéle.

Hra je naprogramována v jazyce C++ s použitím [GLFW](https://www.glfw.org/) a [glad](https://github.com/Dav1dde/glad) pro práci s OpenGL a pomocných knihoven [stb](https://github.com/nothings/stb) pro načítání textur a [glm](https://github.com/g-truc/glm) pro matematické operace.

## Sestavení

Jako systém pro sestavení byl použit [cmake](https://cmake.org/), který můžete použít pro kompilaci programu. Je potřeba mít k dispozici OpenGL a závislosti knihovny GLFW. Více informací na [webu GLFW o sestavení](https://www.glfw.org/docs/latest/build.html).

### Linux

Pro Linux byl vytvořen skript `build.sh`, který s využitím `cmake` a `make` program zkompiluje do adresáře `./build/bin/`. Ten pak lze spustit z příkazové řádky:

```shell
$ ./SpaceGame
```

### Windows

Sestavení na Windows můžete provést pomocí kompilátoru *MSVC* a *Visual Studia*. Můžete použít například *cmake GUI* nebo *PowerShell*:

```powershell
> mkdir build/
> cd build/
> cmake ../
```

Otevřete *solution* soubor `SpaceGame.sln` a zvolte konfiguraci. Sestavte projekt s názvem `ALL_BUILD`. V adresáři `./build/bin` by se měl objevit přeložený program (případně v nějakém podadresáři s názvem konfigurace).

## Ovládání a průběh hry

Po spuštění programu se objeví okno hry. Na začátku se Vaše vesmírná loď objeví ve středu okna a za krátkou dobu začnou ze stran obrazovky létat asteroidy.

Svou loď ovládejte pomocí šipek a mezerníku:

- **Šipka doleva** = otočení lodi proti směru hodinových ručiček.
- **Šipka doprava** = otočení lodi po směru hodinových ručiček.
- **Šipka nahoru** = pohyb lodi kupředu.
- **Mezerník** = střelba.

Pokud se raketou trefíte do asteroidu, tak zmizí. Po sestřelení posledního asteroidu se dostanete do další úrovně a ze stran obrazovky vyletí větší počet asteroidů, než v poslední úrovni.

Pokud Váši loď některý z asteroidů zasáhne, hra končí a po krátké pauze začínáte opět od první úrovně. Aktuální úroveň můžete sledovat na horním levém kraji obrazovky jako počet malých ikonek vesmírné lodi.

Pokud Vaše loď , raketa nebo některý z asteroidů přeletí okraj obrazovky, objeví se na jejím opačném konci se stejným směrem pohybu.

Hru můžete kdykoliv ukončit zavřením okna nebo stisknutím klávesy **ESC**.

## Technická dokumentace