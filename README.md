# Dokumentácia k 2. projektu do predmetu IPK: Server s kalkulačkou

### Obash

1. [Základná teória](#basic_theory)
2. [Štruktúra programu](#program_structure)
3. [Testovanie](#testing)
4. [Zdroje](#sources)

## <a id="basic_theory">1. Základná teória</a>

### TCP

### UDP

## <a id="program_structure">2. Štruktúra programu</a>

Základná štruktúra:

1. ipkcpd.c
2. Makefile
3. modules/\*

### ipkcpd.c

Hlavný prvok programu využíva moduly definované v zložke <code>modules/headers/</code>
a implementované zložke <code>modules/cfiles/</code>. V tomto module sa nachádza funkcia main, ktorá overuje správnosť zadaných parametrov a spúšťa funcionalitu na základe nich.

### Makefile

Tento súbor slúži pre kompiláciu, testovanie a zipovanie programu do archívu.
Pre testovanie a základné použitie je možné zavolať v termináli <code>make help</code>

### modules/\*

V tejto zložke sa nachádzajú práve 2 podzložky - <code>modules/headers</code> a <code>modules/cfiles</code>. V zložke "headers" sú hlavičkové súbory jednotlivých modulov. Nachádzajú sa tu 5 hlavných moduly

- calculator.h
- fracion.h
- tcp_module.h
- udp_module.h
- hlavný modul ipkcpd.h (v tejto zložke sa nachádza kvôli lepšej organizácii a štruktúre projektu).

V zložke "cfiles" sa nachádza implementácia pre tieto hlavičkové súbory.

### calculator

Základná kalkulačka vyuźívajúca prefixovú notáciu. Požiadavnky zapísané v notácii ABNF:

<code>operator = "+" / "-" / "\_" / "/"

expr = "(" operator 2\_(SP expr) ")" / 1\*DIGIT

query = "(" operator 2\*(SP expr) ")"</code>

Princíp:
Hlavná funkcia kalkulačky je <code>frac_t get_result(char \*\* expr)</code>, ktorá ako jediný argument príjma výraz a výsledok vracia v tvare zlomku - typ frac_t.

Kalkulačka využíva najmä modul fraction.h. V tomto module sú implementované zlomky a základné operácie s nimi. Predávaný výraz si natokenizuje a dané operandy v tvare čísla si prevedie do tvaru zlomku, s ktorými potom ďalej počíta. Je to kvôli lepšej presnosti a minimalizácii chyby vzniknutej zaokrúhlením hodnoty na int.

Je navrhnutá tak, aby sa v budúcnosti ľahko dala rozšíriť o nové operácie, výpis zlomkov, podporu pre záporné čísla a pod.

V prípade neúspechu výpočtu vráti zlomok s menovateľom rovným 0, inak vráti platné kladné číslo v tvare zlomku.

### moduly spojenia

Moduly spojenia zabezpečujú spojenie podľa zadaného protokolu TCP alebo UDP. Prostredníctvom TCP je dostupná textová varianta kalkulačky a prostredníctvom UDP je dostupná jej binárna varianta.

## <a id="testing">3. Testovanie</a>

Testovanie programu prebiehalo popri jeho vytváraní. Každý modul bol testovaný hneď po pridaní do programu. Testované boli hlavne moudly **ipkcpd, tcp_module, udp_module, calculator**.

V ipkcpd bolo testované hlavne overovanie vstupných parametrov. Na to bol použitý script
<code>test_inputs.py</code>. (Tento script bol použitý aj pri testovaní klienta, pretože používajú rovnaké parametre).

Kalkulačka (modul <code>calculator.c calculator.h</code>), bol testovaný pomocou pri kompilácii definovaného makra TEST_CALC. Pri definícii tohto makra došlo k lokálnemu spusteniu kalkulačky, ktorá využila na testovanie vstupy zo súboru <code>calc_test.txt</code>. Pri jednotlivých výstupoch bola správnosť výsledku hodnotená programátorom. Výsledky splňovali zadané požiadavky. Spustenie testov pomocov Makefile:

<code>make run DEBUG_MACRO="TEST_CALC" ARGS="-h 127.0.0.1 -p 2023 -m udp"</code> - testy pre kalkulačku

<code>make arg_test</code> - testy pre správnosť overenia argumentov

Samotné spojenie TCP/UDP bolo testované najmä lokálne za použitia terminálu prípadne viacerých terminálov. Testovanie bolo uskutočnené hlavne prostredníctvom adresy localhostu - pre rýchle spustenie testovacieho prostredia:

UDP:
<code>make run ARGS="-h 127.0.0.1 -p 2023 -m udp"</code>

TCP:
<code>make run ARGS="-h 127.0.0.1 -p 2023 -m tcp"</code>

## <a id="sources">4. Zdroje</a>

<hr>

<a id="1">[1]</a> _C Program to Find GCD of two Numbers_ \[online\], https://www.programiz.com/c-programming/examples/hcf-gcd

<a id="2">[2]</a> _C Program to Find LCM of two Numbers_ \[online\], https://www.programiz.com/c-programming/examples/lcm

<a id="3">[3]</a> _C Program to Convert Integer to String and Vice-versa_ \[online\], https://www.sanfoundry.com/c-program-integer-to-string-vice-versa/

<a id="4">[4]</a> D. Dolejška, M. Koutenský, _Programování síťových aplikací IPK 2022/2023 L_ , \[online\]

<a id="4">[5]</a> _Using poll() instead of select()_, naposledy aktualizované 31.8.2021 ,\[online\],https://www.ibm.com/docs/en/i/7.1?topic=designs-using-poll-instead-select
