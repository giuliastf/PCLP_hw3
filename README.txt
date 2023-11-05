
main:
(82-103)
bi_num1, bi_num2 - retine nr  de biti de pe o linie din matricea de pixeli
pix_pad1, pix_pad2 - retine nr de pixeli+padding de pe o linie din matricea de pixeli
width - latimea dreptunghiului
line_width - grosimea liniei
height - inaltimea dreptunghiului
x1,x2,y1,y2,x3,y3,x,y - coordonate
FILE* in - fisierul din care se citeste,cu numele in char *input_filename 
FILE* out - fisierul in care scriem/modificam, cu numele in char *output_filenam
char *action - comanda (quit, save, edit,..)
char * extra - urmatoarul cuvand dupa comanda (...)
bmp_fileheader fileheader_1, fileheader_2; - structurile fileheader pt cele 2 imagini
bmp_infoheader infoheader_1, infoheader_2; - structurile infoheader pt cele 2 fisiere/imagini
pixel **pixel_matrix_1, **pixel_matrix_2 - matricile de pixeli(adica structuri ce retin culorile) scoase din cele 2 imagini, 
pixel color - structura ce retine culoarea setata
i,j,k,l - contori
structura pixel retine culorile pixelului blue, green,red declarate cu uint8_t(folosesc biblioteca stdint.h) , deoarece iau valori de la 0 la 255, adica au nevoie de 8 biti de stocare

(92-95) alocari dinamice
(97-101) verific daca a mers alocarea pentru action
103 - cat timp am ce citi (iau cate un cuvant) adica functia scanf este != NULL
//task1     
 EDIT (action=="edit")   // edit img1.bmp
 (103-130)
- citesc cu scanf numele pozei de editat (img1.bmp)
- deschid cu fopen in (fisierul numit img1.bmp)
- verific cu if(in==NULL), daca a mers deschiderea imaginii
-fiindca o imagine bmp este formata astfel: fileheader, infoheader, iar inceputul matricei cu pixeli este memorat in imageDataOffset din fileheader
 citesc cu fread fileheaderul lui img1 ca un bloc si infoheaderul lui img1  
 aflu nr de biti de pe o linie din matrice(bi_num1), cu tot cu cei de padding cu ajutorul formulei: bi_num1=(31+24*latimea lui img1) / 32 *4
 aflu nr de pixeli(+padding) de pe o linie(pix_pad), impartind bi_num1 la dimensiunea unui pixel
 pozitionez cursorul la inceputul matricei de pixel cu fseek
-(116) aloc dinamic memorie pt liniile matricea de pixeli 
 verific cu urmatorul if verif daca a mers alocarea
- (122) cu un for parcurg liniile din matricea de pixeli, iar fiecare linie va fi alocata dinamic cu malloc(pixel_pad(pixeli si padding pe linie)* dimensiunea unui pixel)
 verific cu if daca a mers alocarea anterioara
 tot in acest for voi citi acum cu fread fiecare linie din matricea

 SAVE (action=="save")
 (132-138)
- citesc fisierul de salvat, verific daca  s a putut crea(practic fac o copie la ce era in edit)
- copiez fileheader ul si infoheader ul imaginii
- intr un for execut copierea matricei de pixel

QUIT (action=="quit")
(145-154)
-inchid fisierul out, unde am lucrat/scris
-eliberez action
-in for eliberez matricea pe linii
-eliberez vectorul de pointeri din matrice

INSERT(action=="insert") insert img2 y x
-reatribui lui in, un alt fisier, img2(imaginea de inserat)
-ca la edit, dar pt img2 citesc fieheader, infoheader,matricea de pixel_matrix_2
-(183-193):parte de inserare propriu zisa
 in 2 for-uri parcurg matricea imaginii din edit (img1), plecand de la x pana la inaltime img1 si a imaginii2, respectiv de la y pana la latimea lui img1 si a lui img2.
 aici voi atribui pixelului de pe pozitia obtinuta in foruri , valoarea pixelului din a doua imagine(in ordine plecand de la 0,0) 
 contorii k si l sunt folositi pt matricea 2 de pixeli si ma ajuta sa o parcurg in ordine

 SET draw color (196-200)
 -citesc culorile alese si le memorez in structura de tip pixel, numita color
 
 SET line line_width
 -citesc grosimea liniei

DRAW line
-citesc coordonatele celor doua puncte, y1 x1 y2 x2 si apelez functia line
  (22-59)
  void line:
  aflu mai intai maximul si minimul dintre x1 si x2 , respectiva y1 si y2
  in functie de diferenta cea mai mare(max_x-min_x sau max_y-y_min) aflu fie j (care parcurge coloanele) in functie de i (parcurge liniile), fie invers
  urmeaza sa fac linia cu grosimea dorita, deci practic in jurul punctului meu construiesc o matrice patratica (width x width)

DRAW rectangle
- apelez functia line dand ca parametri colturile dreptunghiului, practic fac fiecare latura pe rand

DRAW triangle
- ca la dreptunghi 

FILL
-apelez functia fill
 (62-77)
 void fill
 parametrul aux din functia fill, este o copie a pixelul initial m[x][y], cu culoarea initiala
 pixelul m[x][y] primeste valorile de la structura color
 mai intai pentru fiecare caz(sus jos dreapta, stanga)verific de fiecare data daca ma x si y se afla in interiorul matricei, la fiecare apelez
 daca se respecta acest lucru se compara pixelul cu culoarea initial aux, cu cei din jurul lui cu ajutorul functiei cmp_pixel
 daca s a gasit un pic=xel de aceeasi culoare cu cel initial, se reapeleaza functia fill cu coordonatele acestui pixel

 (56-60)
 functia cmp pixel, compara doi pixeli, adica verifica daca culorile sunt egale