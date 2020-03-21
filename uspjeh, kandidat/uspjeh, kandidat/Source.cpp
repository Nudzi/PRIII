#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

#pragma warning(disable:4996)
const char *crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const char* eRazredchar[] { "I", "II", "III", "IV" };
const float minimalan_prosjek = 4.5;

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int * _trenutno;
public:
	void SortOpadajuciByT2() {
		for (size_t i = 1; i < (*_trenutno); i++){
			int j = i;
			while (j > 0 && _elementi2[j] > _elementi2[j - 1]) {
				T1 temp = _elementi1[j];
				_elementi1[j] = _elementi1[j - 1];
				_elementi1[j - 1] = temp;
				T2 temp2 = _elementi2[j];
				_elementi2[j] = _elementi2[j - 1];
				_elementi2[j - 1] = temp2;
				j--;
			}
		}
	}
	Kolekcija(const Kolekcija&k) {
		_trenutno = new int(*k._trenutno);
		_elementi1 = new T1[*_trenutno];
		_elementi2 = new T2[*_trenutno];
		for (size_t i = 0; i < *_trenutno; i++) {
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}
	Kolekcija & operator=(const Kolekcija&k) {
		if (this!= &k) {
			delete[] _elementi1;
			delete[] _elementi2;
			*_trenutno = *k._trenutno;
			_elementi1 = new T1[*_trenutno];
			_elementi2 = new T2[*_trenutno];
			for (size_t i = 0; i < *_trenutno; i++){
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];
			}
		}
		return *this;
	}
	void AddElement(const T1&el1, const T2&el2) {
		T1 * temp1 = new T1[(*_trenutno) + 1];
		T2 * temp2 = new T2[(*_trenutno) + 1];
		for (size_t i = 0; i < *_trenutno; i++){
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;
		_elementi1[*_trenutno] = el1;
		_elementi2[*_trenutno] = el2;
		(*_trenutno)++;
	}
	Kolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = new int(0);
	}
	~Kolekcija() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}
	T1& getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() { return *_trenutno; }
	friend ostream& operator<< (ostream &COUT, Kolekcija &obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	friend ostream& operator<< (ostream &COUT, Kolekcija *obj) {
		for (size_t i = 0; i < *obj->_trenutno; i++)
			COUT << obj->getElement1(i) << " " << obj->getElement2(i) << endl;
		return COUT;
	}
};

class DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
public:
	friend bool operator==(const DatumVrijeme&d, const DatumVrijeme&d2) {
		return *d._dan == *d2._dan && *d._mjesec == *d2._mjesec && *d._godina == *d2._godina;
	}
	int GetSumaMinuta() { return *_sati * 60 + *_minuti; }
	int GetSumaDana() { return *_dan + *_mjesec * 30 + *_godina *365; }
	DatumVrijeme & operator = (const DatumVrijeme &d) {
		if (this != &d) {
			*_dan = *d._dan;
			*_mjesec = *d._mjesec;
			*_godina = *d._godina;
			*_sati = *d._sati;
			*_minuti = *d._minuti;
		}	return *this;
	}
	char* ToCharArray() const{
		stringstream ss;
		ss << *_dan << "." << *_mjesec << "." << *_godina << " " << *_sati << ":" << *_minuti;
		return AlocirajNizKaraktera(ss.str().c_str());
	}
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	DatumVrijeme(const DatumVrijeme &d) :DatumVrijeme(*d._dan, *d._mjesec, *d._godina, *d._sati, *d._minuti) {}
	friend ostream& operator<< (ostream &COUT, DatumVrijeme &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
		return COUT;
	}
};

class Predmet {
	char * _naziv;
	int _ocjena;
	string _napomena;
public:
	int GetOcjena()const { return _ocjena; }
	Predmet(const Predmet&p) {
		_ocjena = p._ocjena;
		_napomena = p._napomena;
		_naziv = AlocirajNizKaraktera(p._naziv);
	}
	Predmet & operator=(const Predmet&p) {
		if (this != &p) {
			delete[]  _naziv;
			_ocjena = p._ocjena;
			_napomena = p._napomena;
			_naziv = AlocirajNizKaraktera(p._naziv);
		}
		return *this;
	}
	Predmet(const char * naziv = "No name", int ocjena = 1, string napomena = "") :_ocjena(ocjena), _napomena(napomena) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}
	friend ostream& operator<< (ostream &COUT, Predmet &obj) {
		COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
		return COUT;
	}
	friend ostream& operator<< (ostream &COUT, Predmet *obj) {
		COUT << "\nNaziv " << obj->_naziv << " (" << obj->_ocjena << ") \n";
		if (obj->_napomena == "")
			return COUT;
		COUT << "Napomena " << obj->_napomena << endl;
		return COUT;
	}
	char * GetNaziv() const{ return _naziv; }
	string GetNapomena() { return _napomena; }
	void DodajNapomenu(string napomena) {
		_napomena += " " + napomena;
	}
};
struct Uspjeh {
	eRazred _razred;
	//datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Kolekcija<Predmet*, DatumVrijeme> * _predmeti;
public:
	float vratiSumuOcjena() {
		float suma = 0;
		for (size_t i = 0; i < _predmeti->getTrenutno(); i++){
			suma += _predmeti->getElement1(i)->GetOcjena();
		}
		if (suma == 0)
			return suma;
		return suma / _predmeti->getTrenutno();
	}
	Uspjeh(const Uspjeh & u) {
		_razred = u._razred;
		_predmeti = new Kolekcija<Predmet*, DatumVrijeme>(*u._predmeti);
	}
	DatumVrijeme GetZadnjiPredmetDatum()const {
			return _predmeti->getElement2(_predmeti->getTrenutno() - 1);
	}
	eRazred GetRazred() { return _razred; }
	bool GetPredmetPoNazivu(const char *naziv) {
		for (size_t i = 0; i < _predmeti->getTrenutno(); i++) {
			if (strcmp(_predmeti->getElement1(i)->GetNaziv(), naziv) == 0)
				return true;
		}
		return false;
	}
	Uspjeh(eRazred razred) :_razred(razred), _predmeti(nullptr) {
		_predmeti = new Kolekcija<Predmet*, DatumVrijeme>();
	}
	Uspjeh(eRazred razred, const Predmet &p, const DatumVrijeme &dv): Uspjeh(razred){
		_predmeti->AddElement(new Predmet(p), dv);
	}
	~Uspjeh() {
		for (size_t i = 0; i < _predmeti->getTrenutno(); i++){
			delete _predmeti->getElement1(i);
		}
		delete _predmeti; _predmeti = nullptr;
	}
	Kolekcija<Predmet*, DatumVrijeme> * GetPredmeti() { return _predmeti; }
	friend ostream& operator<< (ostream &COUT, const Uspjeh &obj) {
		COUT << "Razred " << eRazredchar[obj._razred - 1] << *obj._predmeti << endl;
		return COUT;
	}
};

class Kandidat {
	char * _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh *> _uspjeh;
public:
	Uspjeh * operator()(eRazred razred) {//vraca uspjeh kandidata ostvaren u prvom razredu
		for (size_t i = 0; i < _uspjeh.size(); i++) {
			if (_uspjeh[i]->GetRazred() == razred) {
				return _uspjeh[i];
			}
		}
		return nullptr;
	}
	/*pretpostavka je da na nivou jednog razreda kandidati imaju 6 predmeta.
	upravo zbog toga, nakon evidentiranja 6 predmeta na nivou jednog razreda kandidatu se salje mail
	sa porukom: "evidentirali ste uspjeh za X razred". ukoliko je prosjek na nivou tog razreda veci
	od minimalan_prosjek
	kandidatu se salje SMS sa porukom: "svaka cast za uspjeh 4.X". slanje poruka i emailova implemenitrati koristeci
	zasebne thread-ove.
	*/
	mutex locker;
	void posaljiEmail(eRazred razred, float moj_prosjek ) {
		locker.lock();
		cout << "Evidentirali ste uspjeh  za " << eRazredchar[razred - 1] << " razred.";
		if (minimalan_prosjek < moj_prosjek)
			cout << "\nSvaka cast za uspjeh " << moj_prosjek << endl;
		locker.unlock();
	}
	float prosjekRazreda(eRazred razred) {
		for (size_t i = 0; i < _uspjeh.size(); i++){
			if (_uspjeh[i]->GetRazred() == razred) {
				return _uspjeh[i]->vratiSumuOcjena();
			}
		}
		return 0;
	}
	bool viseOd6Predmeta(eRazred razred);
	/*
	- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, 
	razmak izmedju dodavanja pojedinih predmeta mora biti najmanje minut).
	*/
	bool AddPredmet(eRazred razred, const Predmet&p, DatumVrijeme&d) {
		if (viseOd6Predmeta(razred) == true)
			return false;

		
		for (size_t i = 0; i < _uspjeh.size(); i++) {
			if (_uspjeh[i]->GetRazred() == razred && _uspjeh[i]->GetPredmetPoNazivu(p.GetNaziv())) 
				return false;
			if (_uspjeh[i]->GetRazred() == razred) {
				if (d == _uspjeh[i]->GetZadnjiPredmetDatum()) {
					if (d.GetSumaMinuta() - _uspjeh[_uspjeh.size() - 1]->GetZadnjiPredmetDatum().GetSumaMinuta() < 1)
						return false;
				}
				if (d.GetSumaDana() < _uspjeh[_uspjeh.size() - 1]->GetZadnjiPredmetDatum().GetSumaDana())
					return false;
			}
			if (_uspjeh[i]->GetRazred() == razred) {
				_uspjeh[i]->GetPredmeti()->AddElement(new Predmet(p), d);
				return true;
			}
		}
		_uspjeh.push_back(new Uspjeh(razred, p, d));
		return true;
	}
	Kandidat(const char * imePrezime, string emailAdresa, string brojTelefona) :_emailAdresa(emailAdresa), _brojTelefona(brojTelefona) {
		_imePrezime = AlocirajNizKaraktera(imePrezime);
	}
	~Kandidat() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _uspjeh.size(); i++) {
			delete _uspjeh[i];
			_uspjeh[i] = nullptr;
		}
	}
	friend ostream& operator<< (ostream &COUT, Kandidat &obj) {
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
		for (size_t i = 0; i < obj._uspjeh.size(); i++)
			COUT << *obj._uspjeh[i] << endl;
		return COUT;
	}
	vector<Uspjeh *> GetUspjeh() { return _uspjeh; }

};
bool Kandidat::viseOd6Predmeta(eRazred razred) {
	int brojac = 0;
	for (size_t i = 0; i < _uspjeh.size(); i++) {
		if (_uspjeh[i]->GetRazred() == razred)
			brojac++;
	}
	if (brojac > 5) {
		thread tr(&Kandidat::posaljiEmail, this, razred, prosjekRazreda(razred));
		tr.join();
		return true;
	}
	return false;
}
void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
	****************************************************************************/

	DatumVrijeme temp,
		datum19062018_1015(19, 6, 2018, 10, 15),
		datum20062018_1115(20, 6, 2018, 11, 15),
		datum30062018_1215(30, 6, 2018, 12, 15),
		datum05072018_1231(5, 7, 2018, 12, 31),
		datum06072018_1231(6, 7, 2018, 12, 31),
		datum07072018_1231(7, 7, 2018, 12, 31),
		datum08072018_1231(8, 7, 2018, 12, 31),
		datum20062018_1115_Copy(datum20062018_1115);
	//funkcija ToCharArray vraca datum i vrijeme kao char *
	cout << datum19062018_1015.ToCharArray() << endl;//treba ispisati: 19.6.2018 10:15
	temp = datum05072018_1231;
	cout << temp.ToCharArray() << endl;//treba ispisati: 5.7.2018 12:31
	cout << datum20062018_1115_Copy.ToCharArray() << endl;//treba ispisati: 20.6.2018 11:15

	const int kolekcijaTestSize = 10;
	Kolekcija<int, int> kolekcija1;
	for (size_t i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i + 1, i * i);
	cout << kolekcija1 << endl;

	Kolekcija<int, int> kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;
	Kolekcija<int, int> kolekcija3;
	kolekcija3 = kolekcija1;
	kolekcija3.SortOpadajuciByT2(); //od najvece prema najmanjoj vrijednosti
	cout << kolekcija3 << crt;

	//napomena se moze dodati i prilikom kreiranja objekta
	Predmet Matematika("Matematika", 5, "Ucesce na takmicenju"),
		Fizika("Fizika", 5),
		Hemija("Hemija", 2),
		Engleski("Engleski", 5),
		SMA("SMA", 3),
		Baze("Baze", 5),
		ASP("ASP", 5);
	Fizika.DodajNapomenu("Pohvala za ostvareni uspjeh"); // dodaje novu napomenu zadrzavajuci ranije dodane
	cout << Matematika << endl;

	Kandidat jasmin("Jasmin Azemovic", "jasmin@gmail.com", "033 281 172");
	Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");

	/*
	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	tom prilikom onemoguciti:
	- dodavanje vise od 6 predmeta za jedan razred
	- dodavanje istoimenih predmeta na nivou jednog razreda,
	- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje minut).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II
	razred, pa onda za I razred i sl.). Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	if (jasmin.AddPredmet(DRUGI, Fizika, datum19062018_1015))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Hemija, datum20062018_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Engleski, datum30062018_1215))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Matematika, datum05072018_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, SMA, datum06072018_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Baze, datum07072018_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, ASP, datum08072018_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Matematiku jer je vec dodana u prvom razredu
	if (jasmin.AddPredmet(PRVI, Matematika, datum05072018_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Fiziku jer nije prosao minut od dodavanja posljednjeg predmeta
	if (jasmin.AddPredmet(PRVI, Fizika, datum05072018_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	/*pretpostavka je da na nivou jednog razreda kandidati imaju 6 predmeta.
	upravo zbog toga, nakon evidentiranja 6 predmeta na nivou jednog razreda kandidatu se salje mail
	sa porukom: "evidentirali ste uspjeh za X razred". ukoliko je prosjek na nivou tog razreda veci
	od minimalan_prosjek
	kandidatu se salje SMS sa porukom: "svaka cast za uspjeh 4.X". slanje poruka i emailova implemenitrati koristeci
	zasebne thread-ove.
	*/

	Uspjeh * u = jasmin(DRUGI);//vraca uspjeh kandidata ostvaren u prvom razredu
	if (u != nullptr)
		cout << *u << endl;


	system("pause>0");
}