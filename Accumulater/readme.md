Az array_accumulater osztály template megkapja template paraméterként, hogy
azok a tömbök, amelyekkel dolgozni fog, milyen típusú elemeket tárolnak.
Konstruktor paraméterként megkap egy megfelelő típusú tömböt, aminek hatására
a tömbben az elemek értéke az adott indexig tartó eredeti tömbelemek
részletösszege lesz, tehát a tömb 0 indexű eleme megmarad, a tömb 1 indexű
eleme a a tömb 0 és 1 indexű elemének az összege lesz, és így tovább. Amikor a
tömb(ök)ről lekerül az array_accumulater objektum, a tömb(ök) visszaáll(nak)
az eredeti állapotba. Az add művelettel további tömbök adhatóak az
accumulaterhez, ilyenkor az összegzés az előző tömb végétől folytatódik az
összegzés. Opcionális template paraméterként az összeadás művelet tetszőleges
bináris műveletre cserélhető. asd