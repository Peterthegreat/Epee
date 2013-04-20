Nume proiect: Epee
====



Scurta descriere:

	Framework de interfata capacitiva
	
	
Proiectul ofera utilizatorului posibilitatea de a transforma orice suprafata conductoare intr-un interfata fizica. Aceasta interfata poate avea orice forma si nu depinde de suprafata. Un microcontroller Atmega88 incarca un circuit RC cu un semnal PWM variabil in frecventa si masoara timpul de descarcare, care variaza in functie de capacitate. Anvelopa semnalului este variata de diverse gesturi, de persoana si de pozitie. Raspberry pi citeste aceste date si le clasifica folosind libraria "libsvm", generand la fiecare detectie o comanda. Ca exemplu, am implementat un player MP3 care poate fi controlat cu ajutorul catorva gesturi, acesta fiind folosit handsfree atunci cand alergi sau conduci.

Avantajul principal al acestei implementari este ca nu este nevoie decat de generarea unui singur semnal PWM si achizitia cu un convertor ADC. Acestea pot fi chiar un singur pin fizic al microcontrollerului, iar ca electrod poate fi folosita carcasa metalica a dispozitivului.

Marind performantele dispozitivului si profitand de propietatile conductoare ale corpului, se pot detecta chiar diferite gesturi si posturi ale unei persoane, sau recunoasterea mult mai multor gesturi si interactiuni.

Exemple de aplicatii: Sesizarea persoanelor care iti ating/zgarie masina, Controlul unui MP3 Player, Jocuri interactive cu mai multi participanti, Marirea paletei de gesturi recunoscute (alaturi de alte interfete capacitive: "click dreapta"), Interactivitate cu un lichid.


Categoria: Hardware


Componenta echipa: Cherciu Claudiu, Cherciu Costel 

Link Github: https://github.com/Peterthegreat/Eppe/


Raspberry Hack 2013
20.04.2013
