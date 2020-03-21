#include<iostream>
#include<string>
#include<iterator>
#include<regex>
#pragma warning(disable:4996)
using namespace std;
const char* crt = "\n***************************\n";
//Koristiti klasu NedozvoljenaOperacija za bacanje izuzetaka u cijelom programu
class NedozvoljenaOperacija : public exception
{
	int _linijaKoda;
public:
	NedozvoljenaOperacija(const char* poruka, int linijaKoda) : exception(poruka)
	{
		_linijaKoda = linijaKoda;
	}

	void Info(){
		cout << "Greska u liniji koda " << _linijaKoda << ": " << exception::what() << endl;
	}
};

template<class T1, class T2>
class FITKolekcija
{
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	//Konstruktor, destruktor i ostale funkcije potrebne za rad sa klasom
	FITKolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	FITKolekcija(const FITKolekcija& list) {
		_elementi1 = new T1[list._trenutno];
		_elementi2 = new T2[list._trenutno];

		for (int i = 0; i < list._trenutno; i++) {
			_elementi1[i] = list._elementi1[i];
			_elementi2[i] = list._elementi2[i];
		}

		_trenutno = list._trenutno;
	}
	~FITKolekcija() {
		delete[] _elementi1;
		_elementi1 = nullptr;

		delete[] _elementi2;
		_elementi2 = nullptr;
		//_trenutno = 0;
	}
	void Add(const T1&el1, const T2&el2) {
		T1 * elementi1 = new T1[_trenutno + 1];
		T2 * elementi2 = new T2[_trenutno + 1];
		for (size_t i = 0; i < _trenutno; i++){
			elementi1[i] = _elementi1[i];
			elementi2[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = elementi1;
		_elementi2 = elementi2;
		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;
		_trenutno++;
	}
	bool RemovebyIndex(int index) {
		if (index<0 || index>_trenutno)
			return false;

		T1* temp1 = new T1[_trenutno - 1];
		T2* temp2 = new T2[_trenutno - 1];

		int j = 0;
		for (int i = 0; i < _trenutno; i++) {
			if (i != index) {
				temp1[j] = _elementi1[i];
				temp2[j] = _elementi2[i];
				j++;
			}
		}
		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_trenutno--;

		return true;
	}
	T1& GetElement1(int i)const { 
		if (i >= 0 && i < _trenutno)
			return _elementi1[i]; }
	T2& GetElement2(int i)const {
		if (i >= 0 && i < _trenutno)
			return _elementi2[i];
	}
	T1 operator[](int i) {
		if (i >= 0 && i < _trenutno)
			return *_elementi1[i];
	}
	int GetTrenutno()const { return _trenutno; }
};

enum Kategorija { GameChallenge, InnovationChallenge, MobileChallenge, SQLChallenge, CodingChallenge };
const char* kategorijech[] { "GameChallenge", "InnovationChallenge", "MobileChallenge", "SQLChallenge", "CodingChallenge" };

//Klasu proglasiti apstraktnom
//const char * what()const
class Osoba
{
protected:
	char* _ime;
	char* _prezime;
	int _godiste;
public:
	Osoba& operator=(const Osoba&o) {
		if (this != &o) {
			delete[] _ime;
			delete[] _prezime;
			_godiste = o._godiste;
			int vel = strlen(o._ime) + 1;
			_ime = new char[vel];
			strcpy_s(_ime, vel, o._ime);

			int vel1 = strlen(o._prezime) + 1;
			_prezime = new char[vel1];
			strcpy_s(_prezime, vel1, o._prezime);
		}return *this;
	}
	Osoba(const Osoba&o) {
		int vel = strlen(o._ime) + 1;
		_ime = new char[vel];
		strcpy_s(_ime, vel, o._ime);

		int vel1 = strlen(o._prezime) + 1;
		_prezime = new char[vel1];
		strcpy_s(_prezime, vel1, o._prezime);

		_godiste = o._godiste;
	}
	Osoba(const char* ime = "NO", const char* prezime = "NO", int godiste = 1990)
	{
		int vel = strlen(ime) + 1;
		_ime = new char[vel];
		strcpy_s(_ime, vel, ime);

		vel = strlen(prezime) + 1;
		_prezime = new char[vel];
		strcpy_s(_prezime, vel, prezime);

		_godiste = godiste;
	}
	 ~Osoba() {
		 if (_ime != nullptr) {
			 delete[] _ime;
			 _ime = nullptr;
		 }
		 if (_prezime != nullptr) {
			 delete[] _prezime;
			 _prezime = nullptr;
		 }
	}
	 virtual void Info() = 0 {
		cout << "Ime: " << _ime << endl;
		cout << "Prezime: " << _prezime << endl;
		cout << "Godiste: " << _godiste << endl;
	}
};
//Prepoznati nasljedjivanje izmedju odgovarajuæih klasa
//I uèesnici i èlanovi komisije trebaju imati podatke za pristup sistemu (email i password)
class Ucesnik:  public Osoba
{
	char* _email;
	string password; //Najmanje 5 karaktera, kombinacija brojeva i velikih slova (dopuniti sa regex-ima)
	string _obrazovnaInstitucija;
public:
	bool provjeraPassw(string pass) {
		string provjera = "(?=.{5,})(?=.*[A-Z])(?=.+\\d)";
		regex reg(provjera);
		if (regex_search(pass, reg))
			return true;
		return false;
	}
	Ucesnik operator=(const Ucesnik&u) {
		if (this != &u) {
			(Osoba&)(*this) = u;
			delete[] _email;
			int vel = strlen(u._email) + 1;
			_email = new char[vel];
			strcpy_s(_email, vel, u._email);
			password = u.password;
			_obrazovnaInstitucija = u._obrazovnaInstitucija;
		}
		return *this;
	}
	const char* GetIme() const{
		return _ime;
	}
	bool operator==(const Ucesnik&u)const {
		return strcmp(_ime, u._ime) == 0;
	}
	//Potrebne funkcije za rad sa 
	Ucesnik(const char*email, string institucija, const char *ime, const char* prezime, int godiste) :
		Osoba(ime, prezime, godiste) {
		int vel = strlen(email) + 1;
		_email = new char[vel];
		strcpy_s(_email, vel, email);
		string pass;
		do{
			cout << "Unesite password za korisnika " << _ime << endl;
			cin >> pass;
			if (provjeraPassw(pass)) {
				password = pass;
				cout << "oke" << endl;
			}
		} while (!provjeraPassw(pass));
		_obrazovnaInstitucija = institucija;
	}
	Ucesnik(const char*email = "NO", string institucija = "NO") :
		Osoba("ime", "prezime", 2019) {
		int vel = strlen(email) + 1;
		_email = new char[vel];
		strcpy_s(_email, vel, email);
		string pass;
		do {
			cout << "Unesite password za korisnika: " << _ime;
			cin >> pass;
			if (provjeraPassw(pass)) {
				password = pass;
				cout << "Pass je oke" << endl;
			}
		} while (!provjeraPassw(pass));
		_obrazovnaInstitucija = institucija;
	}
	Ucesnik(const Ucesnik&u):Osoba(u) {
		int vel = strlen(u._email) + 1;
		_email = new char[vel];
		strcpy_s(_email, vel, u._email);
		password = u.password;
		_obrazovnaInstitucija = u._obrazovnaInstitucija;
	}
	 ~Ucesnik() {
		delete[] _email;
		_email = nullptr;
	}
	virtual void Info() {
		cout << crt;
		cout << "Email: " << _email << endl;
		cout << "Password: " << password << endl;
		cout << "Institucija: " << _obrazovnaInstitucija << endl;
		Osoba::Info();
		cout << crt;
	}
};

class ClanKomisije:public Ucesnik {
	char* _pozicija; //Radno mjesto u firmi
	string _firma;
public:
	ClanKomisije operator=(const ClanKomisije & clan) {
		if (this != &clan) {
			(Ucesnik&)(*this) = clan;
			delete[] _pozicija;
			_pozicija = new char[strlen(clan._pozicija) + 1];
			strcpy(_pozicija, clan._pozicija);
			_firma = clan._firma;
		}
		return *this;
	}
	//Potrebne funkcije za rad sa klasom
	ClanKomisije(const char* pozicija = "NO", string firma = "NO", const char*email = "NO", string pass = "NO", string obrz = "NO") :
		Ucesnik(email, obrz) {
		int vel = strlen(pozicija) + 1;
		_pozicija = new char[vel];
		strcpy_s(_pozicija, vel, pozicija);
		_firma = firma;
	}
	ClanKomisije(const ClanKomisije&clan) :Ucesnik(clan) {
		int vel = strlen(clan._pozicija) + 1;
		_pozicija = new char[vel];
		strcpy_s(_pozicija, vel, clan._pozicija);
		_firma = clan._firma;
	}
	 ~ClanKomisije() {
		delete[] _pozicija;
		_pozicija = nullptr;
	}
	virtual void Info() {
		cout << "Pozicija: " << _pozicija << endl;
		cout << "Firma: " << _firma << endl;
		Ucesnik::Info();
	}
};

//Prijave koje ne ukljuèuju izradu i predaju projekata (kategorije: SQLChallenge i CodingChallenge)
class PrijavaProjekta  {
	char* _nazivProjekta;
	string _opisProjekta;
	bool _finalizirano; //Atribut oznaèava da li je projekat završen i predan u predviðenom roku
	bool _prezentacija; //Projekti koji se prezentuju i ocjenjuju
public:
	bool dozvola() { return _finalizirano && _prezentacija; }
	//Konstruktor i destruktor
	void SetFinalizirano() {_finalizirano = true;}
	void SetPrezentacija( bool set) {_prezentacija = set;}
	PrijavaProjekta(const char* naziv = "NO", string opis = "NO", bool finalizirano=false, bool prezentovano=false, string datum = "NO", Kategorija k=GameChallenge)
	{
		_opisProjekta = opis;
		_finalizirano = finalizirano;
		_prezentacija = prezentovano;
		int vel = strlen(naziv) + 1;
		_nazivProjekta = new char[vel];
		strcpy_s(_nazivProjekta, vel, naziv);
	}
	~PrijavaProjekta() {
		delete[] _nazivProjekta;
		_nazivProjekta = nullptr;
	}
	//Ostale potrebne funkcije za rad sa klasom
	
};
class Prijava : public PrijavaProjekta{
	string _datum; //Podržani format: dd.MM.yyyy (dopuniti sa regex-ima)
	Kategorija _kategorija;
	FITKolekcija<Ucesnik, bool> _tim; //Tim se sastoji od jednog ili više uèesnika, 
	//drugi parametar oznaèava predstavnika tima
public:

	bool provjeriDatum(string datum) {
		string datump = "([0-9]{1,2})(\.|\/)([0-9]{1,2})(\.|\/)([0-9]{2}|[0-9]{4})";
		if (regex_match(datum, regex(datump)))
			return true;
		return false;
	}
	void Info() {
		cout << _datum << crt << kategorijech[_kategorija] << endl;
		for (size_t i = 0; i < _tim.GetTrenutno(); i++)
		{
			_tim.GetElement1(i).Info();
		}
	}
	string GetDatum() { return _datum; }
	FITKolekcija<Ucesnik, bool> GetUcesnike() { return _tim; }
	Kategorija GetKategorija() { return _kategorija; }
	//Konstruktor
	Prijava( Kategorija k)/*:PrijavaProjekta()*/{
		string datum;
		do {
			cout << "Unesite datum za korisnika: ";
			cin >> datum;
			if (provjeriDatum(datum)) {
				_datum = datum;
				cout << "Datum je oke" << endl;
			}
		} while (!provjeriDatum(datum));
		_kategorija = k; 
	}
	Prijava(const Prijava&p) {
		_datum = p._datum;
		_kategorija = p._kategorija;
		for (int i = 0; i < p._tim.GetTrenutno(); i++) {
			_tim.Add(p._tim.GetElement1(i), p._tim.GetElement2(i));
		}
	}
	//Funkciju za dodavanje èlanova tima uz poštivanje ogranièenja maksimalno dozvoljenog broja èlanova u timu.
	//SQLChallenge i CodingChallenge: 2
	//GameChallenge i MobileChallenge: 3
	//InnovationChallenge: 4
	//Onemoguæiti dodavanje istog uèesnika više puta. Tim može imati najviše jednog predstavnika.
	bool ProvjeraUcesnika(const Ucesnik&u) {
		for (size_t i = 0; i < _tim.GetTrenutno(); i++){
			if (u == _tim.GetElement1(i))
				return true;
		}
		return false;
	}
	bool ProvjeraPredstavnika() {
		for (size_t i = 0; i < _tim.GetTrenutno(); i++){
			if (_tim.GetElement2(i) == true)
				return true;
		}
		return false;
	}
	void Add(const Ucesnik&u, bool predstavnik = false) {
		if (ProvjeraUcesnika(u))
			throw NedozvoljenaOperacija("Ucesnik je vec dodan!", __LINE__);
		if (predstavnik && ProvjeraPredstavnika())
			throw NedozvoljenaOperacija("Predstavnik je vec dodan!", __LINE__);

		if (_kategorija == SQLChallenge || _kategorija==CodingChallenge) {
			if (_tim.GetTrenutno() == 2)
				throw NedozvoljenaOperacija("Mjesto u timu je popunjeno!", __LINE__);
		}
		if(_kategorija == GameChallenge || _kategorija == MobileChallenge) {
			if (_tim.GetTrenutno() == 3)
				throw NedozvoljenaOperacija("Mjesto u timu je popunjeno!", __LINE__);
		}
		if(_kategorija == InnovationChallenge) {
			if (_tim.GetTrenutno() == 4)
				throw NedozvoljenaOperacija("Mjesto u timu je popunjeno!", __LINE__);
		}
		_tim.Add(u, predstavnik);
		
	}
	//Funkciju za promjenu predstavnika tima.
	void PromijeniPredstavnika(const char *ime) {
		for (size_t i = 0; i < _tim.GetTrenutno(); i++)
			_tim.GetElement2(i) = false;
	
		for (size_t i = 0; i < _tim.GetTrenutno(); i++)
			if (strcmp(ime, _tim.GetElement1(i).GetIme()) == 0) {
				_tim.GetElement2(i) = true;
				return;
			}
	}
};

//Prijave koje ukljuèuju izradu projekata (kategorije: GameChallenge, InnovationChallenge i MobileChallenge)
bool sumaDatuma(string d1, string d2) {
	cout << d1 << endl;
	int sumaD1 = atoi(d1.c_str());
	int sumaD2 = atoi(d2.c_str());
	cout << sumaD1;
	if (sumaD1 > sumaD2)
		return true;

	return false;
}
class FITCodingChallenge{
	int _godina;
	string _prijaveDo; //Podržani format unosa: dd.MM.yyyy (dopuniti sa regex-ima)
	vector<Prijava*> _prijave;
	FITKolekcija<ClanKomisije, Kategorija> _komisija;
	FITKolekcija<Prijava*, FITKolekcija<ClanKomisije*, double>> _ocjene;
public:
	//Funkciju za ocjenu projekata (radova) od strane èlanova komisije (kolekcija _ocjene).
	//Kolekcija sadrzi samo pokazivaèe (bez alokacije nove memorije) na postojeæe prijave i vec definisane èlanove komisije.
	//Voditi raèuna da se ne mogu ocijeniti projekti koji nisu finalizirani i nisu odabrani za prezentaciju,
	//niti da ocjenu mogu dati èlanovi komisije koji
	//nisu pridruženi odgovarajuæoj kategoriji prijave. Svaki èlan komisije daje ocjenu za svaki projekat, odnosno rad, u
	//kategoriji gdje je dodijeljen. Ocjene se kreæu u rasponu od 1 do 10 za projekte, a za radove u kategoriji SQL ili CodingChallenge
	//u rasponu osvojenih bodova od 0 do 100. Pretpostaviti da su svi uèesnici prijavljeni u kategorijama SQL i CodingChallenge predali radove.
	//void dodajOcjene(Prijava &p , const FITKolekcija<ClanKomisije*, double>&el) {
	//	for (size_t i = 0; i < _prijave.size(); i++) {
	//		if (_prijave[i]->GetDatum() == p.GetDatum())
	//			if (_prijave[i]->dozvola())
	//				_ocjene.Add(_prijave[i], el);
	//	}
	//}
	void dodajClanaKomsije(const ClanKomisije&k, Kategorija kg) {
		_komisija.Add(k, kg);
	}
	~FITCodingChallenge() {
		for (size_t i = 0; i < _prijave.size(); i++) {
			delete _prijave[i];
			//_prijave.erase(_prijave.begin());//iterator + i)
			//i--;
		}
	}
	//Konstruktor
	FITCodingChallenge(int godina, string datum) {
		_godina = godina;
		_prijaveDo = datum;
	}
	//Operator += za dodavanje novih prijava. Isti uèesnik ne može imati više prijava u sklopu iste kategorije.
	//Poštovati ogranièenje roka za prijave (_prijaveDo).
	//Voditi raèuna o alokaciji memorije za prijave (ne zadržavati adresu proslijeðenog objekta)
	FITCodingChallenge& operator+=( Prijava&p) {
		if(sumaDatuma(_prijaveDo, p.GetDatum()))
			throw NedozvoljenaOperacija("Prosao je datum prijave", __LINE__);

		for (size_t i = 0; i < _prijave.size(); i++)
			for (size_t j = 0; j < _prijave[i]->GetUcesnike().GetTrenutno(); j++)
				if (_prijave[i]->ProvjeraUcesnika(p.GetUcesnike().GetElement1(j)))
					if (_prijave[i]->GetKategorija() == p.GetKategorija())
						throw NedozvoljenaOperacija("Ova prijava je dodana za ovog ucesnika.", __LINE__);

		_prijave.push_back(new Prijava(p));
	}
	//Funkciju koja odreðenu prijavu oznaèava kao finaliziranu i odabranu za prezentaciju ili ne.
	void SetPrijava(PrijavaProjekta&p) {
		p.SetFinalizirano();
		p.SetPrezentacija(true);
	}
	//Operator () za dodavanje èlanova komisije po kategorijama. Sprijeèiti dodavanje istih èlanova u sklopu
	//jedne kategorije.
	void Info() {
		for (size_t i = 0; i < _komisija.GetTrenutno(); i++)
		{
			_komisija.GetElement1(i).Info();
			cout << endl;
		}
	}
	FITCodingChallenge& operator()(const ClanKomisije& clan,  Kategorija k) {
		for (size_t i = 0; i < _komisija.GetTrenutno(); i++){
			if (_komisija.GetElement2(i) == k && strcmp(_komisija.GetElement1(i).GetIme(), clan.GetIme()) == 0)
				throw NedozvoljenaOperacija("Ovaj clan ove kategorije vec postoji.", __LINE__);
		}
		_komisija.Add(clan, k);
	}
	//Funkciju za ispis svih prijava po kategorijama. Funkcija neobavezno prima parametar kategoriju.
	//Ispis treba da ukljuèi i podatke o uèesnicima, odnosno èlanovima tima.
	//Ukoliko kategorija nije proslijeðena, podaci treba da budu grupisani po kategorijama.
	void Info(Kategorija k) {
		for (size_t i = 0; i < _prijave.size(); i++) {
			if (_prijave[i]->GetKategorija() == k) {
				_prijave[i]->Info();
			}
		}
	}
	
	//Funkcija koja na osnovu imena i prezimena èlana komisije ispisuje podatke o njegovim ocjenama ukljuèujuæi
	//prijave na koje se one odnose.

	//Funkcija koja na osnovu imena i prezimena uèesnika ispisuje podatke o njegovim prijavama na takmièenju, zajedno sa 
	//ocjenama, ukoliko su one definisane za prijavu/e datog uèesnika.

	//Funkciju za rangiranje kandidata na osnovu kategorije proslijeðene kao parametar.
	//Funkcija treba da na osnovu ocjena èlanova komisije ispiše rang liste timova zajedno sa prosjeènim ocjenama njihovih projekata (radova).
	//Dakle, konaèna rang lista ukljuèuje nazive timova (u zavisnosti od kategorije i naziv projekta), te prosjeènu ocjenu.

	//Funkciju Info koja ispisuje sve podatke o takmièenju.
};


int main()
{
	//Kreirati objekat klase FITCodingChallenge, te demonstrirati sve implementirane funkcionalnosti.
	FITCodingChallenge fit(2019, "12.03.2019");
	Prijava prijava(GameChallenge);
	Ucesnik u1("u1@hotmail.com", "Hemijski faklutet", "Ana", "Milic", 2000);
	u1.Info();
	Ucesnik u2("u2@hotmail.com", "Ekonomski faultet Mostar", "Emina", "Jusufovic", 1998);
	u2.Info();
	try
	{
	prijava.Add(u2);
	prijava.Add(u1, true);
	prijava.Info();
	fit += prijava;
	}
	catch (NedozvoljenaOperacija &er)
	{
		er.Info();
	}
	ClanKomisije k("Pozicija", "Firma", "Email", "Password", "Obrazovanje");
	fit(k,GameChallenge);
	cout << crt << crt;
	fit.Info(GameChallenge);
	system("pause");

	return 0;
}
