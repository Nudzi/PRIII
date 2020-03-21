#include <iostream>
#include <vector>
#include <thread>
#include <exception>
#include <mutex>
#include <chrono>
#include <string>
#include <iterator>
using namespace std;

const char *crt = "\n-------------------------------------------\n";
enum OznakaKrvneGrupe { O, A, B, AB };
const char* OznakaKrvneGrupeString[] = { "O", "A", "B", "AB" };

class Datum {
	int _dan, _mjesec, _godina;
public:
	int sumaDana() const{ return _dan + _mjesec * 30 + _godina * 365; }
	friend int operator-(const Datum&d1, const Datum&d2) {
		return d1.sumaDana() - d2.sumaDana();
	}
	string ToString(string separator = ".") {
		return  to_string(_dan) + separator + to_string(_mjesec )+ separator + to_string(_godina);
	}
	Datum(int dan, int mjesec, int godina) :
		_dan(dan), _mjesec(mjesec), _godina(godina) {}

};

template <class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutnoElemenata;
	int _keyElement;
	// kljucni element, a podrazumijevano T1 predstavlja kljucni element i tada je vrijednost 1, a u slucaju da je T2 onda ce vrijednost biti 2
public:
	void zamjena(int j) {
		T1 temp = _elementi1[j];
		_elementi1[j] = _elementi1[j - 1];
		_elementi1[j - 1] = temp;
		T2 privr = _elementi2[j];
		_elementi2[j] = _elementi2[j - 1];
		_elementi2[j - 1] = privr;
		
	}
	void Sort(string key) {
		for (size_t i = 1; i < _trenutnoElemenata; i++) {
			int j = i;
			if (key == "DESC") {
				if (_keyElement == 1) {
					while (j > 0 && _elementi1[j] > _elementi1[j - 1]) {
						zamjena(j);
						j--;
					}
				}
				else if (_keyElement == 2) {
					while (j > 0 && _elementi2[j] > _elementi2[j - 1]) {
						zamjena(j); j--;
					}
				}
			}
			else if (key == "ASC") {
				if (_keyElement == 1) {
					while (j > 0 && _elementi1[j] < _elementi1[j - 1]) {
						zamjena(j); j--;
					}
				}
				else if (_keyElement == 2) {
					while (j > 0 && _elementi2[j] < _elementi2[j - 1]) {
						zamjena(j); j--;
					}
				}
			}
		}
	}
	Kolekcija(int keyElement = 1) {
		_trenutnoElemenata = 0;
		_keyElement = keyElement;
	}
	bool AddElement(T1 ele1, T2 ele2) {
		if (_trenutnoElemenata == max)
			return false;
		_elementi1[_trenutnoElemenata] = ele1;
		_elementi2[_trenutnoElemenata] = ele2;
		_trenutnoElemenata++;
		return true;
	}

	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}
	void SetKeyElement(int keyElement) {
		if (keyElement < 1 || keyElement>2)
			throw exception("Nedozvoljena vrijednost!");
		_keyElement = keyElement;
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1 GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return _elementi1[lokacija];
	}
	T2 GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception("Nepostojeca lokacija!");
		return _elementi2[lokacija];
	}
	friend ostream &operator<<(ostream &COUT, const Kolekcija &obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
};

class KrvnaGrupa {
	/*postoje 4 krvne grupe koje su navedene u enumeracije OznakaKrvneGrupe, 
	pri cemu svaka od navedenih moze imati pozitivan (+) i negativan (-) Rh faktor*/
	OznakaKrvneGrupe _oznaka;//npr: AB
	char _rhFaktor; // + ili -
					/*prethodno pomenuto je izuzetno bitno iz razloga sto postoje jasna pravila vezana za darivanje krvi tj. koji primalac moze primiti krv od kojeg donatora sto je prikazano u tabeli, a naredna dva vector-a su zaduzena da cuvaju informacije o tome, npr. za krvnu grupu A+ vector donatori ce cuvati vrijednosti: A+ i AB+ */
	vector<KrvnaGrupa> _donatori; //krvne grupe kojima odredjena krvna grupa moze donirati krv.
	vector<KrvnaGrupa> _primaoci; //krvne grupe od kojih odredjena krvna grupa moze primiti krv.
public:
	friend bool operator==(const KrvnaGrupa&k1, const KrvnaGrupa&k2) {
		return (k1._oznaka == k2._oznaka && k1._rhFaktor == k2._rhFaktor);
	}
	KrvnaGrupa(const KrvnaGrupa&k) {
		_oznaka = k._oznaka;
		_rhFaktor = k._rhFaktor;
		for (size_t i = 0; i < k._donatori.size(); i++){
			_donatori.push_back(k._donatori[i]);
		}
		for (size_t i = 0; i < k._primaoci.size(); i++) {
			_primaoci.push_back(k._primaoci[i]);
		}
	}
	/*
	Ispis podataka o krvnoj grupi treba biti u formatu:
	-------------------------------------------
	Krvna grupa -> 0+
	-------------------------------------------
	Donori  ->  0+, A+, B+, AB+
	Primaoci ->	0+, 0-
	-------------------------------------------
	*/
	string ToString() {
		string ispis = crt;
		ispis += "Krvna grupa -> " + string(OznakaKrvneGrupeString[_oznaka]) + _rhFaktor + crt + "Donori -> ";
		for (size_t i = 0; i < _donatori.size(); i++){
			if (i != _donatori.size() - 1)
				ispis += string(OznakaKrvneGrupeString[_donatori[i]._oznaka]) + _donatori[i]._rhFaktor + ", ";
			else if (i == _donatori.size() - 1)
				ispis += string(OznakaKrvneGrupeString[_donatori[i]._oznaka]) + _donatori[i]._rhFaktor + "\n";
		}
		ispis += "Primaoci -> ";
		for (size_t i = 0; i < _primaoci.size(); i++) {
			if (i != _primaoci.size() - 1)
				ispis += string(OznakaKrvneGrupeString[_primaoci[i]._oznaka]) + _primaoci[i]._rhFaktor + ", ";
			else if (i == _primaoci.size() - 1)
				ispis += string(OznakaKrvneGrupeString[_primaoci[i]._oznaka]) + _primaoci[i]._rhFaktor + crt;
		}
		return ispis;
	}
	KrvnaGrupa(OznakaKrvneGrupe oznaka, char rhFaktor) :
		_oznaka(oznaka), _rhFaktor(rhFaktor) {}
	void SetDonatori(vector<KrvnaGrupa> donatori) {
		for (size_t i = 0; i < donatori.size(); i++)
			_donatori.push_back(donatori[i]);
	}
	void SetPrimaoci(vector<KrvnaGrupa> primaoci) {
		for (size_t i = 0; i < primaoci.size(); i++)
			_primaoci.push_back(primaoci[i]);
	}
};

//Klasu proglasiti apstraktnom
class Osoba {
protected:
	char* _imePrezime;
	KrvnaGrupa _krvnaGrupa;
public:
	Osoba &operator=(const Osoba&o) {
		if (this != &o) {
			delete[] _imePrezime;
			int size = strlen(o._imePrezime) + 1;
			_imePrezime = new char[size];
			strcpy_s(_imePrezime, size, o._imePrezime);
			_krvnaGrupa = o._krvnaGrupa;
		}return *this;
	}
	KrvnaGrupa GetKrvnaGrupa() { return _krvnaGrupa; }
	char* GetIme() const { return _imePrezime; }
	virtual void Info() = 0 {}
	Osoba(const Osoba&o):_krvnaGrupa(o._krvnaGrupa){
		int size = strlen(o._imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, o._imePrezime);
	}
	Osoba(const char* imePrezime, KrvnaGrupa krvnaGrupa) : _krvnaGrupa(krvnaGrupa) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
	}
	~Osoba() {
		delete[] _imePrezime;
		_imePrezime = nullptr;
	}
};

class Donator :  public Osoba {
	//u slucaju da broj telefona nije u validnom formatu potrebno ga je postaviti na podrazumijevanu vrijednost: 000-000-000
	string _telefon; //regex: 000/000-000 ili 000-000-000
	Datum _datumPosljednjegDoniranja;
	bool _podsjetiMe;
	bool _kontaktirajMe;

public:
	bool getKontaktiranje() { return _kontaktirajMe; }
	string getBr() { return _telefon; }
	Datum GetdatumPosljednjegDoniranja() { return _datumPosljednjegDoniranja; }
	virtual void Info() {

	}
	Donator(const char* imePrezime, KrvnaGrupa krvnaGrupa, string telefon, Datum dpd, bool remind = true, bool contact = true)
		: Osoba(imePrezime, krvnaGrupa), _datumPosljednjegDoniranja(dpd) {
		//if (ValidanFormat(telefon))
			_telefon = telefon;
		//else
			_telefon = "000-000-000";
	}
};

class Zahtjev {
	string _ustanova;
	Datum _datumZahtjeva;
	KrvnaGrupa _krvnaGrupa;
	double _kolicina;
public:
	string GetUstanova() { return _ustanova; }
	Datum GetDatumZahtjeva() const{ return _datumZahtjeva; }
	KrvnaGrupa GetKG()const { return _krvnaGrupa; }
	double GetKolicina() const{ return _kolicina; }
	Zahtjev(string ustanova, Datum datum, KrvnaGrupa krvnaGrupa, double kolicina) :
		_ustanova(ustanova), _datumZahtjeva(datum), _krvnaGrupa(krvnaGrupa), _kolicina(kolicina) { }

	friend ostream& operator<<(ostream& COUT,  Zahtjev& obj) {
		COUT << "Ustanova: " << obj._ustanova << endl;
		COUT << "Datum: " << obj._datumZahtjeva.ToString() << endl;
		//COUT << "Krvna grupa: " << obj._krvnaGrupa.ToString() << endl;
		COUT << "Kolicina: " << obj._kolicina << endl;
		return COUT;
	}
};

class TransfuzijskiCentar {
	//stanje zaliha za svaku pojedinu krvnu grupu
	Kolekcija<KrvnaGrupa*, double, 8> _zalihe;
	//evidentira svaku donaciju krvi
	Kolekcija<Osoba *, double, 100> _donacije;
	vector<Zahtjev> _zahtjevi;
public:
	bool daLiPosjedujeKolicine(const Zahtjev&z) {
		for (int i = 0; i < _zalihe.GetTrenutno(); i++){
			if (z.GetKG() == *_zalihe.GetElement1(i) && z.GetKolicina() >= _zalihe.GetElement2(i))
				return true;
		}
		return false;
	}
	/*ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
	a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati 
	(poslati SMS poruku sa odgovarajucim sadrzajem)
	sve donore koji zadovoljavaju sljedece uslove:
	- mogu donirati krv zahtijevanoj krvnoj grupi
	- oznacili su da zele biti kontaktirani
	- nisu davali krv u posljednja 3 mjeseca
	*/
	void moguciDonatori(const Zahtjev&z) {
		mutex locker;
		bool n = false;
		for (size_t i = 0; i < _donacije.GetTrenutno(); i++) {
			locker.lock();
			Donator *d = dynamic_cast<Donator*>(_donacije.GetElement1(i));
			if (d->GetKrvnaGrupa() == z.GetKG() && d->getKontaktiranje() && (z.GetDatumZahtjeva() - d->GetdatumPosljednjegDoniranja() < 90)) {
				cout << "Saljem SMS na broj telefona..." << endl;
				cout << "Broj Telefona -> " << d->getBr() << endl;
				cout << "Potrebne zalihe krvi.." << endl;
				n = true;
			}
			locker.unlock();
		}
		if (!n)
			cout << "Niko nije pronadjen." << endl;
	}
	//printa zahvalnice (zahvaljujemo se ime i prezime donatoru na ukupno doniranih X doza krvi)
	//za TOP 2 donatora krvi
	void GetZahvalniceZahvalnice(int brojNajboljih) {
		_donacije.SetKeyElement(2);
		_donacije.Sort("DESC");
		for (size_t i = 0; i < 2; i++){
			cout << "Zahvaljujemo " << _donacije.GetElement1(i)->GetIme() << " na doniranih " << _donacije.GetElement2(i) << " kolicina krvi." << endl;
		}
	}
	bool DodajZahtjev(const Zahtjev&z) {
		if (daLiPosjedujeKolicine(z)) {
			_zahtjevi.push_back(z);
			return true;
		}
		thread tr(&TransfuzijskiCentar::moguciDonatori, this, z);
		tr.join();
		return false;
	}
	/*
	prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
	ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
	takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije
	*/
	//datum donacije, donator, kolicina
	void AddDonaciju(Datum  datumDonacije,  Osoba *donator, double kolicina) {
		bool imaKrvna = false;
		for (size_t i = 0; i < _zalihe.GetTrenutno(); i++){
			if (*_zalihe.GetElement1(i) == donator->GetKrvnaGrupa()) {
				_zalihe.SetElement2(i, _zalihe.GetElement2(i) + kolicina);
				imaKrvna = true;
			}
		}
		if (!imaKrvna)
			_zalihe.AddElement(new KrvnaGrupa(donator->GetKrvnaGrupa()), kolicina);

		for (size_t i = 0; i < _donacije.GetTrenutno(); i++){
			if (strcmp(_donacije.GetElement1(i)->GetIme(), donator->GetIme()) == 0) {
				_donacije.SetElement2(i, _donacije.GetElement2(i) + kolicina);
				dynamic_cast<Donator*>(_donacije.GetElement1(i))->GetdatumPosljednjegDoniranja() = datumDonacije;
				return;
			}

		}
		_donacije.AddElement(new Donator(*dynamic_cast<Donator*>(donator)), kolicina);
	}
	~TransfuzijskiCentar() {
		for (size_t i = 0; i < _zalihe.GetTrenutno(); i++)
			delete _zalihe.GetElement1(i);
		for (size_t i = 0; i < _donacije.GetTrenutno(); i++)
			delete _donacije.GetElement1(i);
	}
};

void main() {

	Datum dat_12_01_2018(12, 1, 2018), dat_01_02_2018(1, 2, 2018);
	cout << dat_01_02_2018.ToString() << endl; //podrazumijevani separator je tacka ".", pa ispisuje 1.2.2018
	cout << dat_12_01_2018.ToString("/") << endl; //separator je proslijedjen, pa ispisuje 12/1/2018

	Kolekcija<int, int, 10> kolekcija1;
	for (size_t i = 0; i < kolekcija1.GetMax(); i++)
		if (!kolekcija1.AddElement(i, 170000 + i))
			cout << "Elementi " << i << " i " << 170000 + i << " nisu dodati u kolekciju" << endl;

	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl << crt;

	kolekcija1.Sort("ASC"); //kljucne rijeci za sortiranje su ASC i DESC, a sortiranje se vrsi prema kljucnom elementu
	cout << kolekcija1 << endl << endl << endl;
	kolekcija1.Sort("DESC");
	cout << kolekcija1 << endl;

	kolekcija1.SetKeyElement(2);//postavlja kljucni element na T2

	kolekcija1.Sort("DESC"); //sada se sortiranje vrsi po elementu T2
	cout << kolekcija1 << endl;

	KrvnaGrupa
		O_poz(O, '+'), O_neg(O, '-'),
		A_poz(A, '+'), A_neg(A, '-'),
		B_poz(B, '+'), B_neg(B, '-'),
		AB_poz(AB, '+'), AB_neg(AB, '-');

	vector<KrvnaGrupa> donori_O_poz{ O_poz, A_poz, B_poz, AB_poz };
	O_poz.SetDonatori(donori_O_poz);
	//ili krace napisano
	O_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, O_neg});
	A_poz.SetDonatori(vector<KrvnaGrupa>{A_poz, AB_poz});
	A_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, A_poz, O_neg, A_neg});

	B_poz.SetDonatori(vector<KrvnaGrupa>{B_poz, AB_poz});
	B_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, B_poz, O_neg, B_neg});
	/*
	Ispis podataka o krvnoj grupi treba biti u formatu:
	-------------------------------------------
	Krvna grupa -> 0+
	-------------------------------------------
	Donori  ->  0+, A+, B+, AB+
	Primaoci ->	0+, 0-
	-------------------------------------------
	*/
	cout << O_poz.ToString() << endl;

	Osoba * jasmin = new Donator("Jasmin Azemovic", B_poz, "061-111-222", Datum(12, 2, 2017), true, true);
	Osoba * adel = new Donator("Adel Handzic", A_neg, "061-222-333", Datum(9, 1, 2017), true, true);
	Osoba * goran = new Donator("Goran Skondric", B_neg, "061-333-444", Datum(9, 3, 2017), true, true);


	TransfuzijskiCentar tcMostar;
	/*
	prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
	ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
	takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije
	*/
	//datum donacije, donator, kolicina
	tcMostar.AddDonaciju(Datum(20, 5, 2017), jasmin, 2.5);
	tcMostar.AddDonaciju(Datum(20, 5, 2017), adel, 3);
	tcMostar.AddDonaciju(Datum(6, 5, 2017), goran, 1.2);


	tcMostar.AddDonaciju(Datum(10, 9, 2017), jasmin, 2);
	tcMostar.AddDonaciju(Datum(18, 10, 2017), adel, 1.8);
	tcMostar.AddDonaciju(Datum(15, 9, 2017), goran, 3.8);

	Zahtjev zahtjev_0_poz("Tranfuziologija KCUS", Datum(18, 2, 2018), O_poz, 15),
		zahtjev_0_neg("Tranfuziologija Bakir Nakas", Datum(20, 2, 2018), O_neg, 8);

	/*ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
	a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati (poslati SMS poruku sa odgovarajucim sadrzajem)
	sve donore koji zadovoljavaju sljedece uslove:
	- mogu donirati krv zahtijevanoj krvnoj grupi
	- oznacili su da zele biti kontaktirani
	- nisu davali krv u posljednja 3 mjeseca
	*/
	tcMostar.DodajZahtjev(zahtjev_0_poz);
	tcMostar.DodajZahtjev(zahtjev_0_neg);

	//printa zahvalnice (zahvaljujemo se ime i prezime donatoru na ukupno doniranih X doza krvi) za TOP 2 donatora krvi
	tcMostar.GetZahvalniceZahvalnice(2);

	delete adel;
	delete jasmin;
	delete goran;

	system("pause>0");
}