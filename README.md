# p2nprobe

**Martin Slezák (xsleza26)<br />**
18.10.2024

## Obsah
- [Popis programu](#popis-programu)
    - [Flows](#flows)
    - [Timeouty](#timeouty)
    - [Export](#export)
- [Příklad spuštění](#příklad-spuštění)
- [Seznam odevzdaných souborů](#seznam-odevzdaných-souborů)

## Popis programu

Program analyzuje komunikaci z daného `.pcap` souboru a exportuje pomocí
protokolu Netflow ve verzi 5.

### Flows

Analyzována je pouze komunikace TCP, ostatní provoz se neanalyzuje. Jednotlivé
pakety se rozdělí do takzvaných flows, kdy flow je jednoznačně určena zdrojovou
IP adresou a portem paketu, cílovou IP adresou a portem paketu a také typem IP
služby (ToS). Flow také obsahuje další informace o skupině paketů, jako
například počet paketů, velikost a tak dále.

### Timeouty

Exportér podporuje dva typy timeoutů - aktivní a neaktivní. Aktivní timeout
ukončí flow po daném čase od prvního přijatého paketu a flow je následně
exportována. Neaktivní timeout ukončí flow po daném čase, kdy do flow nepřišel
žádný paket.

### Export

Po vypršení jednoho z časových limitů nebo po přečtení celého souboru se flows
exportují. Flows se exportují v maximálním množství, aby se naplnil paket -
tedy 29 + hlavička. Pouze po přečtení celého `.pcap` souboru se odešle zbytek
flows.

## Příklad spuštění

```
./p2nprobe localhost:2055 pcap_soubor.pcap -a 30 -i 15
```

V případě, že běh programu narazí na chybu, bude na standardní chybový výstup
chyba vypsána.

## Seznam odevzdaných souborů
