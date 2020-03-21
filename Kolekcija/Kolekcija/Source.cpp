#include<iostream>
using namespace std;
#pragma warning (disable:4996)
char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T, int max>
class Kolekcija
{
	T* _elementi[max];//staticki niz pokazivaca na T
	int _trenutnoElemenata;
public:
	//za ispis
	friend ostream & operator<< <>(ostream& COUT, Kolekcija&kolekcija);
	//prondaji element
	int GetTrenutnoElemenata()const { return _trenutnoElemenata; }
	//Implementirati potrebne funkcije za rad sa kolekcijom.
	Kolekcija(){
		for (size_t i = 0; i < max; i++){
			_elementi[i] = nullptr;
		}
		_trenutnoElemenata = 0;
	}

	Kolekcija(const Kolekcija& k){
		for (size_t i = 0; i < k._trenutnoElemenata; i++){
			if (k._elementi[i] != nullptr)
				_elementi[i] = new T(*k._elementi[i]);
			else
				_elementi[i] = nullptr;
		}

		_trenutnoElemenata = k._trenutnoElemenata;
	}

	Kolekcija& operator=(const Kolekcija& k){
		if (this != &k){
			for (size_t i = 0; i < _trenutnoElemenata; i++){
				delete _elementi[i];
			}


			for (size_t i = 0; i < k._trenutnoElemenata; i++){
				if (k._elementi[i] != nullptr)
					_elementi[i] = new T(*k._elementi[i]);
				else
					_elementi[i] = nullptr;
			}

			_trenutnoElemenata = k._trenutnoElemenata;
		}

		return *this;
	}

	~Kolekcija(){
		for (size_t i = 0; i < _trenutnoElemenata; i++){
			delete _elementi[i];
			_elementi[i] = nullptr;
		}
	}
	T *operator[](int i) {
		return _elementi[i];
	}
	//1. Kolekcija može biti puna
	//2. Sprijeciti dodavanje istih elemenata
	bool ADD(const T& el) {
		if (_trenutnoElemenata == max)
			return false;

		//Provjera duplih elem
		for (size_t i = 0; i < _trenutnoElemenata; i++) 
			//Ako je T instanca vaše klase, ovdje se mora preklopiti operator==
			if (*_elementi[i] == el) 
				return false;
			
		
		//Za T se poziva kons. kopije
		_elementi[_trenutnoElemenata++] = new T(el);

		//Drugi nacin
		//_elementi[_trenutnoElemenata] = new T;//default. kons
		//*_elementi[_trenutnoElemenata] = el;//operator=
		return true;
	}

	bool RemoveByIndex(int index){//od 0 do _trenutno-1
		if (index < 0 || index >= _trenutnoElemenata)
			return false;

		delete _elementi[index];
		for (size_t i = index; i < _trenutnoElemenata - 1; i++){
			_elementi[i] = _elementi[i + 1];
		}
		_elementi[_trenutnoElemenata - 1] = nullptr;

		_trenutnoElemenata--;
	}

	bool RemoveByElement(const T& el){
		for (size_t i = 0; i < _trenutnoElemenata; i++){
			if (_elementi[i] == el)
				return RemoveByIndex(i);
		}

		return false;
	}

	void Info(){
		for (size_t i = 0; i < _trenutnoElemenata; i++){
			//operator<< za T
			cout << *_elementi[i] << endl;
		}
	}
};
template<class T, int max>
ostream & operator<< (ostream& COUT, Kolekcija<T, max>&kolekcija) {
	for (size_t i = 0; i < kolekcija.GetTrenutnoElemenata(); i++){
		COUT << *kolekcija._elementi[i];
	}
	return COUT;
}

class Prisustvo{
	char* _brojIndeksa;
	int _brojSati;
public:
	int GetBrojSati()const { return _brojSati; }
	char* GetBrojIndeksa()const { return _brojIndeksa; }
	//move konst
	/*const Prisustvo(const Prisustvo && p) {
		this->_brojSati = p._brojSati;
		this->_brojIndeksa = p._brojIndeksa;
		*p._brojIndeksa = NULL;
	}*/
	Prisustvo() {
		_brojIndeksa = nullptr;
		_brojSati = 0;
	}
	//Osnovne funkcije za rad s klasom
	Prisustvo(const char* brojIndeksa, int brojSati) :
		_brojSati(brojSati)
	{
		int size = strlen(brojIndeksa) + 1;
		_brojIndeksa = new char[size];
		strcpy_s(_brojIndeksa, size, brojIndeksa);
	}

	Prisustvo(const Prisustvo& p) : _brojSati(p._brojSati)
	{
		_brojIndeksa = AlocirajNizKaraktera(p._brojIndeksa);
	}

	~Prisustvo()
	{
		delete[] _brojIndeksa;
		_brojIndeksa = nullptr;
	}

	//Preklopljeni operator + 
	friend Prisustvo operator+(const Prisustvo&, int);
	//i += za uvecavanje broja sati 
	//prisustva za vrijednost proslijedenog parametra
	bool operator+=(int brojSati) {
		//if (_brojSati != 0)
		//	return false;
		_brojSati += brojSati;
		return true;
	}
	friend bool operator==(const Prisustvo&, const Prisustvo&);
	friend bool operator==(const Prisustvo&, const char*);
	friend ostream& operator<<(ostream&, const Prisustvo&);
	bool  operator<(Prisustvo& p) {
		int indeks2 = atoi(p._brojIndeksa);
		int indeks1 = atoi(_brojIndeksa);
		return indeks1 < indeks2;
	}
	//operator =
	 Prisustvo operator=(const Prisustvo& p) {
		if (this != &p) {
			delete[] _brojIndeksa;
			_brojIndeksa = AlocirajNizKaraktera(p._brojIndeksa);
			_brojSati = p._brojSati;
		}
		return *this;
	}
};
Prisustvo operator+(const Prisustvo& p, int brojSati) {
	Prisustvo temp(p);
	temp._brojSati += brojSati;
	return temp;
}
bool operator==(const Prisustvo& p1, const Prisustvo& p2)
{
	return strcmp(p1._brojIndeksa, p2._brojIndeksa) == 0;
}
bool operator==(const Prisustvo& p1, const char* p2)
{
	return strcmp(p1._brojIndeksa, p2) == 0;
}
ostream& operator<<(ostream& cout, const Prisustvo& p)
{
	cout << "Broj sati: " << p._brojSati << endl;
	if (p._brojIndeksa != nullptr)
		cout << "Broj indeksa: " << p._brojIndeksa << endl;

	return cout;

}
void Sortiraj(Prisustvo *p, int brojac) {
	for (size_t i = 1; i < brojac; i++) {
		int j = i;
		while (j > 0 && p[j] < p[j - 1]) {
			Prisustvo temp = p[j];
			p[j] = p[j - 1];
			p[j - 1] = temp;
			j--;
		}
	}
}
class Nastava
{
	char _datum[12];
	char _tipNastave; //P ili V
	int _odrzanoSati;
	Prisustvo* _prisutni;
	int _brojStudenata;
public:
	Nastava operator=(Nastava & desna) {
		if (this != &desna) {
			_tipNastave = desna._tipNastave;
			_odrzanoSati = desna._odrzanoSati;
			_brojStudenata = desna._brojStudenata;
			delete[] _prisutni;
			_prisutni = new Prisustvo[_brojStudenata];
			for (size_t i = 0; i < _brojStudenata; i++){
				_prisutni[i] = desna._prisutni[i];
			}
		}
		return *this;
	}
	//get Broj indeksa
	Nastava operator+= (int brojSati) {
		_prisutni->operator+=(brojSati);
		return *this;
	}
	//operator ==
	friend bool operator==(const Nastava&n1, const Nastava&n2);
	//Konstruktor sa osnovnim atributima klase
	Nastava(const char* datum, const char tipNastave, int sati) :
		_tipNastave(tipNastave), _odrzanoSati(sati), _brojStudenata(0)
	{
		strncpy_s(_datum, datum, _TRUNCATE);
		_prisutni = nullptr;
	}
	//kons kopije
	Nastava(const Nastava&nastava) :
		_tipNastave(nastava._tipNastave), _odrzanoSati(nastava._odrzanoSati)
	{
		strncpy_s(_datum, nastava._datum, _TRUNCATE);
		_prisutni = new Prisustvo[nastava._brojStudenata];
		for (size_t i = 0; i < nastava._brojStudenata; i++){
			_prisutni[i] = nastava._prisutni[i];
		}
		_brojStudenata = nastava._brojStudenata;
	}
	//Destruktor
	~Nastava() {
		delete[] _prisutni;
		_prisutni = nullptr;
	}
	//Preklopljeni operator+= za dodavanje objekata tipa Prisustvo u niz _prisutni.
	//Nije moguce dodati više puta prisustvo za istog studenta 
	//(preklopiti operator == u klasi Prisustvo).
	//Elementi u svakom momentu treba da budu sortirani po broju indeksa studenta 
	//(preklopiti operator < u klasi Prisustvo).
	bool operator+=(Prisustvo &desni) {
		for (size_t i = 0; i < _brojStudenata; i++) {
			if (_prisutni[i] == desni)
				return false;
		}
		Prisustvo *temp = new Prisustvo[_brojStudenata + 1];
		for (size_t i = 0; i < _brojStudenata; i++) {
			temp[i] = _prisutni[i];
		}
		temp[_brojStudenata] = desni;
		delete[] _prisutni;
		_prisutni = temp;
		_brojStudenata++;
		Sortiraj(_prisutni, _brojStudenata);
		return true;
	}
	//Preklopljeni operator-= za uklanjanje prisustva za studenta sa
	//proslijedjenim brojem indeksa. Nakon uklanjanja,
	//potrebno je sacuvati redoslijed dodavanja, 
	//odnosno podaci ostaju sortirani po broju indeksa.
	bool operator-=(const char * brojIndeksa) {
		int indeks = -1;
		for (size_t i = 0; i < _brojStudenata; i++) {
			if (_prisutni[i] == brojIndeksa) {
				indeks = i;
				break;
			}
		}
		if (indeks == -1)
			return false;
		for (size_t i = indeks; i < _brojStudenata - 1; i++) {
			_prisutni[i] = _prisutni[i + 1];
		}
		_brojStudenata--;
		return true;
	}
	//Preklopljeni operator [] koji na osnovu broja indeksa provjerava da li je 
	//odredeni student prisustvovao nastavi i vraca njegove podatke o prisustvu.
	Prisustvo & operator[](const char *brojIndeksa) {
		int indeks = -1;
		for (size_t i = 0; i < _brojStudenata; i++) {
			if (_prisutni[i] == brojIndeksa) {
				return _prisutni[i];
			}
		}
		//throw exception("Greska. Ovaj student ne  postoji u prisustvu.");
	}
	//Preklopljeni operator () koji pronalazi studenta sa 
	//proslijedenim brojem indeksa i uvecava mu broj sati prisustva
	//za vrijednost drugog parametra. Ako je studentu vec evidentiran
	//odrzani broj sati nastave prekinuti funkciju.
	bool operator()(const char *brojIndeksa, int brojSati) {
		int indeks = -1;
		for (size_t i = 0; i < _brojStudenata; i++) {
			if (_prisutni[i] == brojIndeksa) {
				if (_prisutni[i].GetBrojSati() == 0)
					if (_prisutni[i] += brojSati)
						return true;
			}
		}
		cout << "Student ili ne postoji ili je njogv broj sati unsesn vec." << endl;
		return false;
	}
	//Preklopljeni operator + koji zbraja dva objekta tipa Nastava 
	//na nacin da kreira novi objekat na osnovu lijevog operanda
	//i dodaje mu prisutne studente desnog operanda.
	//Funkciju napustiti ukoliko objekti koji ucestvuju u operaciji sabiranja
	//nisu istog tipa nastave. 
	//Sprijeciti dodavanje prisustva za istog studenta više puta.
	//Operator implementirati kao globalnu funkciju.
	friend Nastava operator+(Nastava&, Nastava&);
	//operator ispisa
	friend ostream&operator<<(ostream&cout, Nastava &n);
	//dobvljanje tipa nastave
	char GetTipNastave()const { return _tipNastave; }
	//operator =
	
};
bool operator==(const Nastava&n1, const Nastava&n2) {
	if ((n1._tipNastave == n2._tipNastave) && strncmp(n1._datum, n2._datum, _TRUNCATE) == 0)
			for (size_t i = 0; i < n1._brojStudenata; i++)
				for (size_t j = 0; j < n2._brojStudenata; j++)
						if (n1._odrzanoSati > n1._prisutni[i].GetBrojSati())
							return true;
	return false;
}

Nastava operator+(Nastava& lijevi, Nastava&desni) {
	if ((lijevi.GetTipNastave() == desni.GetTipNastave()))
		throw exception("Greska");

	Nastava temp(lijevi);

	for (size_t i = 0; i < desni._brojStudenata; i++) {
		temp += desni._prisutni[i];
	}
	return temp;
}
ostream&operator<<(ostream&cout, Nastava &n) {
	cout << "Datum " << n._datum << " je odrzano sati " << n._odrzanoSati << " iz " << n._tipNastave << endl;
	for (size_t i = 0; i < n._brojStudenata; i++) {
		cout << n._prisutni[i];
	}
	return cout;
}
class Predmet{
	char* _naziv;
	int _godinaStudija;
	Kolekcija<Nastava, 45> _odrzanaNastava;
public:
	Predmet(const char* naziv, int godinaStudija) : _godinaStudija(godinaStudija){
		_naziv = AlocirajNizKaraktera(naziv);
	}
	~Predmet(){
		delete[] _naziv;
		_naziv = nullptr;
	}
	//Funkciju za evidenciju održane nastave na predmetu.
	/*Funkciju koja na osnovu datuma i tipa održane nastave dodaje prisustvo studentu sa proslijedenim
	brojem indeksa i brojem sati.
	Voditi racuna da broj sati prisustva ne može biti veci od broja sati održane nastave.*/
	void DodajNastavu(const Nastava& Nastava) {
		_odrzanaNastava.ADD(Nastava);
	}
	/*Funkciju koja na osnovu datuma i tipa održane nastave uklanja prisustvo studentu sa proslijedenim brojem indeksa.*/
	bool UkloniStudenta(const char *indeks) {

	}
	/*Funkciju koja na osnovu broja indeksa studenta vraca sve podatke o njegovom prisustvu na nastavi*/
	/*Prisustvo NadjiStudenta(const char*indeks) {
		return _odrzanaNastava.Pronadji(indeks);
	}*/
	/*Funkciju koja na osnovu tipa nastave (P ili V) vrši ispis prisustva za sve studente (sumarno) u datom formatu:
	IB140001 30/40 75%, gdje je prva kolona "broj indeksa", druga kolona "broj sati prisustva / ukupan broj sati održane nastave" i
	treca kolona "procenat prisustva studenta na nastavi".*/

	friend ostream& operator<<(ostream&, Predmet&);
};

ostream& operator<<(ostream& cout, Predmet& p)
{
	cout << "Predmet: " << p._naziv << endl;
	cout << "Godina studija: " << p._godinaStudija << endl;
	cout << p._odrzanaNastava;
	return cout;

}

int main()
{
	//Testirati sve dostupne funkcionalnosti klasa Prisustvo i Nastava.
	Prisustvo p1("2255", 2);
	Prisustvo p2("2037", 1);

	/*Kolekcija<Prisustvo, 30>* kolekcija = new Kolekcija<Prisustvo, 30>;
	kolekcija->AddElement(p1);
	kolekcija->AddElement(p2);*/
	//kolekcija->Info();
	//Kolekcija<int, 10> brojevi;
	//for (size_t i = 0; i < 10; i++)
	//{
	//	brojevi.AddElement(i);
	//}
	//brojevi.Info();
	//Kolekcija<int, 10> brojevi2 = brojevi;//kons. kopije
	//brojevi2.Info();

	//Kolekcija<int, 10> brojevi3;
	//brojevi3 = brojevi2;//operator= za Kolekciju
	//brojevi3.Info();


	//brojevi3.RemoveByIndex(0);
	//cout << "Nakon uklanjanja.\n";
	//brojevi3.Info();

	Prisustvo p("170218", 0);
	cout << p + 20;
	Prisustvo p3("170211", 20);
	Nastava nastava("12", 'P', 20), nastava2("13.10.1998", 'V', 21);
	nastava += p;
	nastava += p3;
	nastava2 += p3;
	nastava2 += p2;
	cout << "ISPIS" << endl;
	cout << nastava << endl;
	/*nastava -= "170211";
	cout << "POSLIJE" << endl;
	cout << nastava;*/
	/*cout << "PODACI O TRAZENOM STUDENTU PO INDEKSU" << endl;
	cout << nastava["170218"];
	if (nastava("170218", 20))
		cout << "Dodan broj sati" << endl;
	else cout << "Ovaj student vec ima unesen broj sati" << endl;*/
	cout << "\n ISPIS DRUGE NASTAVE " << endl;
	cout << nastava2;
	cout << "Nakon + operatora dodavanja na nastavu " << endl;
	Nastava n(nastava + nastava2);
	cout << n;
	Predmet predmet("PROGRAMIRANJE", 2);
	cout << "PRVA" << endl;
	predmet.DodajNastavu(n);
	cout << predmet;
	system("pause");
	return 0;
}

