#include "lectures.h"

void initLectures(Lecture *listLectures)
{
  listLectures[0].titlu = "Ion";
  listLectures[0].anAparitie = "1920";
  listLectures[0].autor = "Liviu Rebreanu";
  listLectures[0].curentLiterar = "Realism Interbelic";
  listLectures[0].temaOperei = "Problematica Pamantului";
  listLectures[0].elemStr = "Roman obiectiv, 2 parti, simetrie incipit-final";
  listLectures[0].scena1= "Hora din sat";
  listLectures[0].scena2= "Sarutatea pamantului";


  listLectures[1].titlu = "Ultima noapte";
  listLectures[1].anAparitie = "1930";
  listLectures[1].autor = "Camil Petrescu";
  listLectures[1].curentLiterar = "Modernism interbelic, roman psihologic";
  listLectures[1].temaOperei = "Conditia intelectualului lucid";
  listLectures[1].elemStr = "Roman subiectiv, memorie afectiva, fluxul constiintei";
  listLectures[1].scena1= "Cina de la unchiul Tache";
  listLectures[1].scena2= "Capitolul 'ne-a acoperit pamantul lui Dumnezeu'";


  listLectures[2].titlu = "Enigma Otiliei";
  listLectures[2].anAparitie = "1938";
  listLectures[2].autor = "George Calinescu";
  listLectures[2].curentLiterar = "Realism, elem moderniste";
  listLectures[2].temaOperei = "Arivismul(mostenirea), paternicia, iubirea adolescentina, conditia femeii";
  listLectures[2].elemStr = "Roman obiectiv, balzacian, simetrie incipit-final, pluriperspectivism";
  listLectures[2].scena1= "Descrierea strazii si a casei lui mos Costache";
  listLectures[2].scena2= "Jocl de table";


  listLectures[3].titlu = "Morometii";
  listLectures[3].anAparitie = "I-1995, II-1967"; // Corectat anul de aparitie pentru I-ul volum
  listLectures[3].autor = "Marin Preda";
  listLectures[3].curentLiterar = "Realism postbelic";
  listLectures[3].temaOperei = "Destramarea familiei taranesti traditionale, timpul, criza comunicarii";
  listLectures[3].elemStr = "Roman obiectiv, focalizare multipla";
  listLectures[3].scena1= "Cina in familie";
  listLectures[3].scena2= "Taierea salcamului";


  listLectures[4].titlu = "Baltagul";
  listLectures[4].anAparitie = "1930";
  listLectures[4].autor = "Mihail Sadoveanu";
  listLectures[4].curentLiterar = "Realism, traditionalism";
  listLectures[4].temaOperei = "Monografia lumii pastorale arhaice, calatoria initiatica, iubirea conjugala";
  listLectures[4].elemStr = "Roman obiectiv, structura tripla (asteptarea, cautarea, infaptuirea dreptatii), elemente de roman politist";
  listLectures[4].scena1= "Pregatirea Vitoriei de drum";
  listLectures[4].scena2= "Praznicul final si demascarea ucigasilor";


  listLectures[5].titlu = "O scrisoare pierduta";
  listLectures[5].anAparitie = "1884";
  listLectures[5].autor = "Ion Luca Caragiale";
  listLectures[5].curentLiterar = "Realism Clasic";
  listLectures[5].temaOperei = "Satirizarea moravurilor politice si sociale ale vremii";
  listLectures[5].elemStr = "Comedie de moravuri, 4 acte, conflict comic, bazat pe pierderea scrisorii";
  listLectures[5].scena1= "Numararea steagurilor de catre branzovenescu si farfuridi";
  listLectures[5].scena2= "Discursurile electorale ale lui farfuridi si catavencu"; // Corectat numele lui Farfuridi


  listLectures[6].titlu = "Povestea lui Harap Alb";
  listLectures[6].anAparitie = "1877";
  listLectures[6].autor = "Ion Creanga";
  listLectures[6].curentLiterar = "Basm cult";
  listLectures[6].temaOperei = "Maturizarea eroului";
  listLectures[6].elemStr = "Basm cult, umanizarea fantasticului";
  listLectures[6].scena1= "Scena din fantana";
  listLectures[6].scena2= "Intalnirea cu personajele himerice (Gerila, flamanzila, etc)";


  listLectures[7].titlu = "Moara cu noroc";
  listLectures[7].anAparitie = "1881";
  listLectures[7].autor = "Ioan Slavici";
  listLectures[7].curentLiterar = "Realism Clasic";
  listLectures[7].temaOperei = "Consecintele dorintei de avutie";
  listLectures[7].elemStr = "Nuvela psihologica realista, incipit-final moralizator (vorbele babei)";
  listLectures[7].scena1= "Aparitia lui Lica Samadaul";
  listLectures[7].scena2= "Uciderea Anei de catre Ghita";


  listLectures[8].titlu = "Eu nu strivesc Corola";
  listLectures[8].anAparitie = "1919";
  listLectures[8].autor = "Lucian Blaga";
  listLectures[8].curentLiterar = "Modernism (expresionism)";
  listLectures[8].temaOperei = "Cunoasterea luciferica (prin mister), iubirea, rolul poetului";
  listLectures[8].elemStr = "Arta poetica, vers liber, metafora revelatorie, antiteza, ingambament";
  listLectures[8].scena1= "Invoca lumina pentru a sublinia misterul";
  listLectures[8].scena2= "Compara lumina cu o floare care nu e strivita";


  listLectures[9].titlu = "Floare Albastra";
  listLectures[9].anAparitie = "1873";
  listLectures[9].autor = "Mihai Eminescu";
  listLectures[9].curentLiterar = "Romantism";
  listLectures[9].temaOperei = "Iubirea si natura, conditia omului de geniu, tema evaziunii";
  listLectures[9].elemStr = "Idila, elegie, antiteza, natura edenica si cosmica";
  listLectures[9].scena1= "Dialogul dintre vocea feminina (natura) si cea masculina (geniul)";
  listLectures[9].scena2= "Intoarcerea la realitate si melancolie";


  listLectures[10].titlu = "Plumb";
  listLectures[10].anAparitie = "1916";
  listLectures[10].autor = "George Bacovia";
  listLectures[10].curentLiterar = "Simbolism";
  listLectures[10].temaOperei = "Spleen-ul, moartea, singuratatea, angoasa existentiala, conditia poetului";
  listLectures[10].elemStr = "Poem simbolist, simetrie, laitmotivul 'plumb', cromatica sumbra, muzicalitate";
  listLectures[10].scena1= "Descrierea spatiului funebru si a conditiei eului liric";
  listLectures[10].scena2= "Starea de angoasa si izolare, repetarea 'plumb'";


  listLectures[11].titlu = "Luceafarul";
  listLectures[11].anAparitie = "1883";
  listLectures[11].autor = "Mihai Eminescu";
  listLectures[11].curentLiterar = "Romantism";
  listLectures[11].temaOperei = "Conditia geniului, iubirea, cosmogonia, raportul om-univers";
  listLectures[11].elemStr = "Poem filozofic, epopee, alegorie, antiteza, simetrie compozitionala";
  listLectures[11].scena1= "Nasterea Luceafarului din cer si mare";
  listLectures[11].scena2= "Dialogul Hyperion - Demiurg";


  listLectures[12].titlu = "Testament";
  listLectures[12].anAparitie = "1927";
  listLectures[12].autor = "Tudor Arghezi";
  listLectures[12].curentLiterar = "Modernism";
  listLectures[12].temaOperei = "Conditia poetului, rolul poeziei, estetica uratului";
  listLectures[12].elemStr = "Arta poetica, metafora, oximoron (veninul strans l-am preschimbat in miere), adresare directa";
  listLectures[12].scena1= "Referinta la \"graiul cu-ndemnuri pentru vite\" si transformarea lui in carte";
  listLectures[12].scena2= "Mentionarea \"zdrentelor\" si a \"buruienilor\" transformate in frumusete";


  listLectures[13].titlu = "Maitreyi";
  listLectures[13].anAparitie = "1933";
  listLectures[13].autor = "Mircea Eliade";
  listLectures[13].curentLiterar = "Modernism, roman al experientei, roman exotic";
  listLectures[13].temaOperei = "Iubirea exotica, initierea, cunoasterea prin experienta, sacrul si profanul";
  listLectures[13].elemStr = "Roman subiectiv, jurnal, insertii de jurnale, narator-personaj, ambiguitate";
  listLectures[13].scena1= "Intalnirea lui Allan cu Maitreyi si intrarea in casa Sen";
  listLectures[13].scena2= "Ceremonia logodnei tainice (juramantul iubirii)";


  listLectures[14].titlu = "Riga Crypto";
  listLectures[14].anAparitie = "1926";
  listLectures[14].autor = "Ion Barbu";
  listLectures[14].curentLiterar = "Modernism (ermetism)";
  listLectures[14].temaOperei = "Cunoasterea, incompatibilitatea, tema iubirii imposibile, conditia geniului";
  listLectures[14].elemStr = "Balada culta, poem alegoric, limbaj ermetic, dualitatea regnului mineral si vegetal";
  listLectures[14].scena1= "Nunta in povestea spusa de menestrel (Cripto si Enigel)";
  listLectures[14].scena2= "Transformarea lui Cripto in ciuperca otravitoare, esecul iubirii";
}
