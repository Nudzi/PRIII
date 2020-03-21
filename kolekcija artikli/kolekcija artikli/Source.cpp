#include<iostream>
using  namespace std;
#pragma warning(disable:4996)
const char * crt = "\n----------------------------------------------------\n";
char * AlocirajCHar(const char *naziv) {
	int vel = strlen(naziv) + 1;
	char *temp = new char[vel];
	strcpy_s(temp, vel, naziv);
	return temp;
}
template<class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int * _trenutno;
public:
	Kolekcija(const Kolekcija&k) {
		_trenutno = new int(*k._trenutno);
		_elementi1 = new T1[*_trenutno];
		_elementi2 = new T2[*_trenutno];
		for (int i = 0; i < *_trenutno; i++) {
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}
	//operator=
	T2 & operator=(int b) {
		*_elementi2 = b;
		return *_elementi2;
	}
	Kolekcija & operator=(Kolekcija<T1, T2>&k) {
		if (this != &k) {
			delete[] _elementi1;
			delete[] _elementi2;
			//delete _trenutno;
			_trenutno = new int(*k._trenutno);
			_elementi1 = new T1[*_trenutno];
			_elementi2 = new T2[*_trenutno];
			for (int i = 0; i < *_trenutno; i++) {
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];
			}
		}
		return *this;
	}
	friend ostream& operator<< <>(ostream& cout, Kolekcija&k);
	int GetTrenutno()const { return *_trenutno; }
	T2 operator*(float i) {
		cout << *_elementi2;
		return *_elementi2 * i;
	}
	T1  getElement1(int i)const { return _elementi1[i]; }
	T2  getElement2(int i)const { return _elementi2[i]; }
	T2 operator[](int i) {
		return _elementi2[i];
	}
	T2 operator+=(int i) {
		return *_elementi2 += i;
	}
	void AddElement(const T1 &element1, const T2 &element2) {
		T1 *t1 = new T1[*_trenutno + 1];
		T2 *t2 = new T2[*_trenutno + 1];

		for (int i = 0; i < *_trenutno; i++)
		{
			t1[i] = _elementi1[i];
			t2[i] = _elementi2[i];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = t1;
		_elementi2 = t2;

		_elementi1[*_trenutno] = element1;
		_elementi2[*_trenutno] = element2;

		(*_trenutno)++;
	}
	Kolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = new int(0);
	}
	//1. potrebne funkcije	
	~Kolekcija() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}
};
template<class T1, class T2>
ostream& operator<< (ostream& cout, Kolekcija<T1, T2>&k) {
	for (size_t i = 0; i < k.GetTrenutno(); i++) {
		cout << k.getElement1(i) << " " << k.getElement2(i) << endl;
	}
	return cout;
}
class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	Datum(const Datum&d) {
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
	}
	Datum& operator=(const Datum&d) {
		if (this != &d) {
			*_dan = *d._dan;
			*_mjesec = *d._mjesec;
			*_godina = *d._godina;
		}
		return *this;
	}
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, const Datum &obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}
	//1. potrebne funkcije

};
class Osoba {
protected:
	char * _imePrezime;
	Datum * _datumRodjenja;
public:
	friend ostream& operator<<(ostream&cout, Osoba& o) {
		cout << o._imePrezime << " (" << *o._datumRodjenja << ")" << endl;
		return cout;
	}
	Osoba & operator=(Osoba&o) {
		if(this != &o){
			delete[] _imePrezime;
			_imePrezime = AlocirajCHar(o._imePrezime);
			delete _datumRodjenja;
			_datumRodjenja = new Datum(*o._datumRodjenja);
		}
		return *this;
	}
	Osoba(const char * imePrezime, Datum datum) {
		_imePrezime = new char[strlen(imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
		_datumRodjenja = new Datum(datum);
	}
	Osoba(const Osoba & obj) {
		_imePrezime = new char[strlen(obj._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
		_datumRodjenja = new Datum(*obj._datumRodjenja);
	}
	~Osoba() { delete[]_imePrezime; delete _datumRodjenja; }
	//1. potrebne funkcije
};

class Kupac {
	Osoba *_kupac;
	//svaki kupac ima popust koji mu se zaracunava prilikom svake kupovine
	//pocetni popust je 0.10%
	//svaka kupovina u iznosu 100 - 500KM povecava iznos popusta za 0.15%
	//svaka kupovina u iznosu 500 - 1000KM povecava iznos popusta za 0.25%
	//svaka kupovina u iznosu vecem od 1000KM povecava iznos popusta za 0.30%
	float _odobreniPopust;
public:
	friend ostream& operator<<(ostream& cout, Kupac&k) {
		cout << "Popust " << k._odobreniPopust << endl;
		cout << *k._kupac;
		return cout;
	}
	Kupac(const Kupac&k) {
		_odobreniPopust = k._odobreniPopust;
		_kupac = new Osoba(*k._kupac);
	}
	Kupac & operator=(Kupac&k) {
		if (this != &k) {
			delete _kupac;
			_kupac = new Osoba(*k._kupac);
			_odobreniPopust = k._odobreniPopust;
		}
		return *this;
	}
	Kupac(const char* ime, const Datum& d) :_kupac(new Osoba(ime,d)) {
		_odobreniPopust = 0.1;
	}
	~Kupac() {
		delete _kupac; _kupac = nullptr;
		_odobreniPopust = 0;
	}
};
class Artikal {
	char* _barCode;
	char * _naziv;
	float _cijena;
public:
	float GetCijena()const { return _cijena; }
	friend bool operator==(Artikal&, Artikal&);
	Artikal & operator=(Artikal& a) {
		if (this != &a) {
			delete[] _barCode;
			delete[] _naziv;
			_barCode = AlocirajCHar(a._barCode);
			_naziv = AlocirajCHar(a._naziv);
			_cijena = a._cijena;

		}
		return *this;
	}
	Artikal(const char * naziv="--", float cijena=0, const char * barCode=00000000) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
			_cijena = cijena;
		_barCode = AlocirajCHar(barCode);
	}
	~Artikal() { delete[]_naziv; _naziv = nullptr; }
	friend ostream& operator<< (ostream &COUT, const Artikal &obj) {
		COUT << obj._naziv << " " << obj._cijena << " KM, kod: " << obj._barCode << endl; 
		return COUT; 
	}
	friend ostream& operator<< (ostream &COUT, const Artikal *obj) {
		COUT << obj->_naziv << " " << obj->_cijena << " KM, kod: " << obj->_barCode << endl;
		return COUT;
	}
	//1. potrebne funkcije

};
bool operator==(Artikal& lijevi, Artikal& desni) {
	return strcmp(lijevi._barCode, desni._barCode) == 0;
}
float returnCijena(Artikal*a) {
	return a->GetCijena();
}
class Racun {
	//format broja racuna: RN6005491
	//osigurati automatsko generisanje broja racuna pocevsi od RN0000001
	char _brojRacuna[10];
	Datum _datumKreiranja;
	//prvi argument je Artikal, a drugi kolicina/broj artikala koju je kupac uzeo
	Kolekcija<Artikal *, int> * _stavkeRacuna;
	Kupac * _kupac;
public:
	void Sortiraj() {
		for (size_t i = 0; i < _stavkeRacuna->GetTrenutno(); i++) {
			int j = i;
			while (j > 0 && (_stavkeRacuna->getElement2(j) * returnCijena(_stavkeRacuna->getElement1(j))) <
				(_stavkeRacuna->getElement2(j - 1) * returnCijena(_stavkeRacuna->getElement1(j - 1)))) {
				Artikal a = *_stavkeRacuna->getElement1(j);
				int b = _stavkeRacuna->getElement2(j);
				*_stavkeRacuna->getElement1(j) = *_stavkeRacuna->getElement1(j - 1);
				_stavkeRacuna[j] = _stavkeRacuna[j - 1];
				*_stavkeRacuna->getElement1(j - 1) = a;
				_stavkeRacuna[j] = b;
				j--;
			}
		}
	}
	Racun(const Racun&r) {
		strncpy(_brojRacuna, r._brojRacuna, 10);
		_datumKreiranja = r._datumKreiranja;
		_kupac = new Kupac(*r._kupac);
		_stavkeRacuna = new Kolekcija<Artikal*, int>;
		for (size_t i = 0; i < r._stavkeRacuna->GetTrenutno(); i++) {
			_stavkeRacuna->AddElement(new Artikal(*r._stavkeRacuna->getElement1(i)), r._stavkeRacuna->getElement2(i));
		}
	}
	friend ostream&operator<<(ostream&cout, Racun&r) {
		cout << r._brojRacuna << " (" << r._datumKreiranja << ")" << endl;
		cout << "Kupac " << *r._kupac;
		cout << *r._stavkeRacuna;
		return cout;
	}
	void DodajArtikal(Artikal&a, int broj) {
		for (int i = 0; i < _stavkeRacuna->GetTrenutno(); i++){
			if (*_stavkeRacuna->getElement1(i) == a) {
				_stavkeRacuna[i] += broj;
				return;
			}
		}
		_stavkeRacuna->AddElement(new Artikal(a), broj);
	}
	Racun(const char *broj, const Datum&d, Kupac* k):_datumKreiranja(d), _kupac(new Kupac(*k)) {
		strncpy(_brojRacuna, broj, 10);
		_stavkeRacuna = new Kolekcija<Artikal*, int>;
	}
	~Racun() {
		delete _kupac; _kupac = nullptr;
		delete _stavkeRacuna; _stavkeRacuna = nullptr;
	}
	float UkupanIznosRacuna() {
		float suma = 1, cijena = 0;
		for (size_t i = 0; i < _stavkeRacuna->GetTrenutno(); i++){
			suma = _stavkeRacuna->getElement2(i) * returnCijena(_stavkeRacuna->getElement1(i));
			cijena += suma;
			suma = 1;
		}
		if (cijena < 100)
			return cijena;
		if (cijena > 99 && cijena < 499)
			return cijena - 0.15;
		if (cijena > 499 && cijena < 999)
			return  cijena - 0.25;
		if (cijena > 999)
			return cijena - 0.30;

	}
	/*
	1. potrebne (de)konstruktor i druge funkcije
	2. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom. 
	Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko korisnik vise puta 
	pokusa dodati isti artikal.
	3. operator<< - ispisuje racun u formatu prikazanom na slici(nije obavezno da bude identican, ali je svakako pozeljno).Nakon ispisa svakog racuna, ukoliko su ispunjeni definisani uslovi, kupcu se uvecava popust.
	4. Sortiraj - sortira artikle (rastuci) po kolicini novca kojom ucestvuju u ukupnom iznosu racuna
	*/
};
int brojRacuna = 100001;

const char *GenerisiSljedeciBrojRacuna(){
	char *temp = new char[strlen("RN000001") + 1];
	if (brojRacuna < 1000000) {
		char buffer[10];
		itoa(brojRacuna, buffer, 10);
		strcpy(temp, "RN");
		strcat(temp, buffer);
		temp[2] = temp[2] - 1;
		brojRacuna++;
	}
	return temp;
}
void main() {
#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
//#pragma region TestiranjeKolekcije
//	const int brojElemenata = 15;
//	Kolekcija<int, int> kolekcija1;
//	for (int i = 0; i < brojElemenata; i++)
//		kolekcija1.AddElement(i, i);
//	cout << kolekcija1.getElement1(0) << " " << kolekcija1.getElement2(0) << endl;
//	cout << kolekcija1 << endl;
//	Kolekcija<int, int> kolekcija2;
//	kolekcija2 = kolekcija1;
//	cout << kolekcija2 << crt;
//	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
//		cout << "ISTI BROJ ELEMENATA" << endl;
//	Kolekcija<int, int> kolekcija3(kolekcija2);
//	cout << kolekcija3 << crt;
//#pragma endregion
#pragma region TestiranjeKupca
	Kupac jasmin("Jasmin Azemovic", Datum(15, 2, 1980));
	Kupac adel("Adel Handzic", Datum(15, 10, 1981));
	Kupac jasmin2 = jasmin;
	cout << jasmin << endl << jasmin2 << endl;
	jasmin = jasmin2;
	cout << jasmin << endl << jasmin2 << endl;
#pragma endregion
#pragma region TestiranjeArtikla
	Artikal sokCppyNarandza("Cappy narandza", 2.10, "699511236545");
	Artikal cokoladaMilkaMilkins("Milka Milkins", 2.55, "622445875463");
	Artikal jogurtMeggle("Meggle jogurt", 2.55, "155338774458");

	cout << sokCppyNarandza << cokoladaMilkaMilkins << jogurtMeggle << endl;
	sokCppyNarandza = cokoladaMilkaMilkins;
	cout << sokCppyNarandza << endl;
	if (sokCppyNarandza == cokoladaMilkaMilkins)
		cout << "Artikli su isti" << endl;
#pragma endregion
#pragma region TestiranjeRacuna
	/*GenerisiSljedeciBrojRacuna - globalna funkcija koja generise i vraca naredni broj 
	racuna u prethodno definisanom formatu*/
	Racun racun1(GenerisiSljedeciBrojRacuna(), danas, &jasmin);

	racun1.DodajArtikal(cokoladaMilkaMilkins, 15);
	racun1.DodajArtikal(sokCppyNarandza, 20);
	racun1.DodajArtikal(jogurtMeggle, 20);

	//cout << racun1 << endl;

	Racun racun2(GenerisiSljedeciBrojRacuna(), sutra, &jasmin);
	racun2.DodajArtikal(cokoladaMilkaMilkins, 15);
	racun2.DodajArtikal(sokCppyNarandza, 20);
	racun2.DodajArtikal(jogurtMeggle, 20);
	//cout << racun2 << endl;

	Racun racun3(racun2);
	cout << racun3 << endl;
	cout << racun3.UkupanIznosRacuna();
	racun3.Sortiraj();
	cout << racun3;
#pragma endregion
	system("pause");
}
//popust mozete izracunati kao procenat od ukupne sume racuna (pozeljna opcija) 
//ili kao vrijednost u KM-ovima (kao u nastavku)
/*
FITStore::Racun::RN000001
| RB Artikal Cijena Kolicina Ukupno |
| 1. Milka Milkins 2.55 15 38.25 |
| 2. Cappy narandza 2.1 20 42 |
| 3. Meggle jogurt 2.55 20 51 |
| Suma:131.25KM
| Popust : 0.1KM
| Za platiti : 131.15KM


FITStore::Racun::RN000002
| RB Artikal Cijena Kolicina Ukupno |
| 1. Milka Milkins 2.55 15 38.25 |
| 2. Cappy narandza 2.1 20 42 |
| 3. Meggle jogurt 2.55 20 51 |
| Suma:131.25KM
| Popust : 0.25KM
| Za platiti : 131KM


FITStore::Racun::RN000002
| RB Artikal Cijena Kolicina Ukupno |
| 1. Milka Milkins 2.55 15 38.25 |
| 2. Cappy narandza 2.1 20 42 |
| 3. Meggle jogurt 2.55 20 51 |
| Suma:131.25KM
| Popust : 0.4KM
| Za platiti : 130.85KM
*/