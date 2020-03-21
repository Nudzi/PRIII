//exception::what()
//throm mojException("Index nije validan", __LINE__);
//moj tip treba da bude prvi a exception treba da bude zadnji(osnovna klasa)
//ako zelimo mijenjati objekat gdje jae bacen exception  pravimo ga izvan try chatch bloka

#include<iostream>
using namespace std;
const char *crt = "\n***********************************************************\n";
class NedozvoljenaOperacija : public exception {
	int linijaKoda;
public:
	NedozvoljenaOperacija(const char * poruka, int lin) :
		exception(poruka), linijaKoda(lin)
	{
		//TO DO
	}
	void ErrorInfo() {
		//TO DO
		cout << "Greska: " << exception::what();
	}
};

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);

	return temp;
}

template<class T>
class Kolekcija
{
	T* _elementi;
	int _trenutno;
public:
	bool Remove(int index) {
		if (index < 0 || index > _trenutno)
			return false;

		T* temp1 = new T[_trenutno - 1];
		int j = 0;
		for (int i = 0; i < _trenutno; i++) {
			if (i != index) {
				temp1[j] = _elementi[i];
				j++;
			}
		}
		delete[] _elementi;

		_elementi = temp1;
		_trenutno--;

		return true;
	}
	//Kolekciju dopuniti u skladu sa potrebama zadatka
	Kolekcija()
	{
		_elementi = nullptr;
		_trenutno = 0;
	}
	~Kolekcija(){
		delete[] _elementi;
		_elementi = nullptr;
	}

	void AddElement(const T& el)
	{
		T* temp = new T[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++){
			temp[i] = _elementi[i];
		}

		delete[] _elementi;
		_elementi = temp;

		_elementi[_trenutno++] = el;
	}

	int GetTrenutno()const { return _trenutno; }

	T& operator[](int index)
	{
		if (index < 0 || index > _trenutno)
			throw NedozvoljenaOperacija("Index nije validan.", __LINE__);
		return _elementi[index];
	}
	T& GetElement(int index)const {
		if (index < 0 || index > _trenutno)
			throw NedozvoljenaOperacija("Index nije validan.", __LINE__);
		return _elementi[index];
	}
	friend ostream& operator<<<>(ostream&, const Kolekcija&);
};

template<class T>
ostream& operator<<(ostream& cout, const Kolekcija<T>& kol)
{
	for (int i = 0; i < kol._trenutno; i++) {
		cout << kol._elementi[i] << endl;
	}
	return cout;
}

//Kataloška prodaja
class Proizvod {
	char _sifra[10];
	char* _naziv;
	double _cijena;
	double _popust; //Izrazen procentualno
					//Dostupno proizvoda
	int _kolicina;
public:
	Proizvod & operator=(Proizvod&p) {
		if (this != &p) {
			strncpy_s(_sifra, p._sifra, _TRUNCATE);
			delete[]  _naziv;
			_naziv = AlocirajNizKaraktera(p._naziv);
			_popust = p._popust;
			_cijena = p._cijena;
			_kolicina = p._kolicina;
		}
		return *this;
	}
	void operator()(const char* sifra = "NOT SET", const char* naziv = "NOT SET", double cijena = 0, int kolicina = 0) {
		//U sluèaju nedozvoljenih vrijednosti za cijenu, popust i kolièinu baciti izuzetak
		strncpy_s(_sifra, sifra, _TRUNCATE);
		delete[]  _naziv;
		_naziv = AlocirajNizKaraktera(naziv);
		_popust = 0;
		if (cijena < 0 || kolicina < 0) {
			_cijena = _kolicina = 0;
			throw NedozvoljenaOperacija("Cijena ili kolicina nisu dobre vrijednosti.", __LINE__);
		}
		_cijena = cijena;
		_kolicina = kolicina;
	}
	double GetCijena()const { return _cijena; }
	double GetPopust()const { return _popust; }
	void SetPopust(double popust) { _popust = popust; }
	int GetKolicinu()const { return _kolicina; }
	void SetKolicinu() { _kolicina = 0; }
	//operator ==
	bool operator==(Proizvod &p) {
		if (strncmp(_sifra, p._sifra, _TRUNCATE) == 0)
			return true;
		return false;
	}
	//povecaj kolicinu
	void PovecajKolicinu(int kolicina) { _kolicina += kolicina; }
	//Konstruktor i destruktor funkcije
	Proizvod(const Proizvod&p) {
		strncpy_s(_sifra, p._sifra, _TRUNCATE);
		_naziv = AlocirajNizKaraktera(p._naziv);
		_popust = p._popust;
		_cijena = p._cijena;
		_kolicina = p._kolicina;
	}
	Proizvod(const char* sifra = "NOT SET", const char* naziv = "NOT SET", double cijena = 0, int kolicina = 0)
	{
		//U sluèaju nedozvoljenih vrijednosti za cijenu, popust i kolièinu baciti izuzetak
		strncpy_s(_sifra, sifra, _TRUNCATE);
		_naziv = AlocirajNizKaraktera(naziv);
		_popust = 0;
		if (cijena < 0 || kolicina < 0) {
			_cijena = _kolicina = 0;
			throw NedozvoljenaOperacija("Cijena ili kolicina nisu dobre vrijednosti.", __LINE__);
		}
		_cijena = cijena;
		_kolicina = kolicina;
	}
	virtual ~Proizvod()
	{
		delete[] _naziv;
		_naziv = nullptr;
	}
	//Implementirati potrebne funkcije za rad sa klasom
	friend ostream& operator<<(ostream&cout, const Proizvod&p) {
		cout << "Sifra artikla: " << p._sifra << endl;
		cout << "Naziv artikla: " << p._naziv << endl;
		cout << "Cijena artikla: " << p._cijena << endl;
		cout << "Kolicina artikla: " << p._kolicina << endl;
		cout << "Popust artikla: " << p._popust << "%" << endl;
		return cout;
	}
	virtual void Info() {
		cout << "Sifra artikla: " << _sifra << endl;
		cout << "Naziv artikla: " << _naziv << endl;
		cout << "Cijena artikla: " << _cijena << endl;
		cout << "Kolicina artikla: " << _kolicina << endl;
		cout << "Popust artikla: " << _popust << "%" << endl;

	}
};

//Implementirati nasljeðivanje
class Odjeca : virtual public Proizvod
{
	Kolekcija<char*> _velicine;
	Kolekcija<char*> _boje;
	char _kategorija[50];
public:
	Odjeca(Odjeca&o) :Proizvod(o) {
		strncpy_s(_kategorija, o._kategorija, _TRUNCATE);
		for (int i = 0; i < _velicine.GetTrenutno(); i++) {
			_velicine.AddElement(o._velicine[i]);
		}
		for (int i = 0; i < _boje.GetTrenutno(); i++) {
			_boje.AddElement(o._boje[i]);
		}
	}
	void DodajVelicinu(const char *velicina) {
		char *temp = AlocirajNizKaraktera(velicina);
		_velicine.AddElement(temp);
	}
	void DodajBoju(const char *boja) {
		char *temp = AlocirajNizKaraktera(boja);
		_boje.AddElement(temp);
	}
	//Potrebne konstruktor i destruktor funkcije
	Odjeca(const char * kategorija, const char* sifra, const char* naziv, double cijena, int kolicina) :
		Proizvod(sifra, naziv, cijena, kolicina)
	{
		strncpy_s(_kategorija, kategorija, _TRUNCATE);
	}
	Odjeca(const char * kategorija, const Proizvod &p) :Proizvod(p)
	{
		strncpy_s(_kategorija, kategorija, _TRUNCATE);
	}
	~Odjeca() {
		for (size_t i = 0; i < _velicine.GetTrenutno(); i++)
			delete[] _velicine[i];

		for (size_t i = 0; i < _velicine.GetTrenutno(); i++)
			delete[] _boje[i];

	}
	//Implementirati potrebne funkcije za rad sa klasom
	void Info() {
		cout << "Proizvod " << endl;
		Proizvod::Info();
		cout <<crt<< "Odjeca: " << endl;
		cout << "Kategorija artikla: " << _kategorija << endl;
		cout << "Boje artikla: ";
		for (size_t i = 0; i < _boje.GetTrenutno(); i++){
			cout << _boje[i] << ", ";
		}
		cout << endl;
		cout << "Velicine artikla: ";
		for (size_t i = 0; i < _velicine.GetTrenutno(); i++) {
			cout << _velicine[i] << ", ";
		}
		cout << crt;
	}
	friend ostream& operator<<(ostream&cout, const Odjeca&p) {
		
		cout << crt;
	return cout;
	}
};

class Katalog
{
	char* _naslov;
	char* _vrijediDo; //Datum u formatu dd.MM.yyyy
	Kolekcija<Proizvod*> _proizvodi;
public:
	void operator()(const char *naslov, const char *vrijediDo) {
		//U sluèaju da datum ne ispunjava zadati format baciti izuzetak
		delete[] _naslov;
		delete[] _vrijediDo;
		_naslov = AlocirajNizKaraktera(naslov);
		int brojac1 = 0, brojac2 = 0;
		for (size_t i = 0; i < strlen(vrijediDo); i++) {
			if (i > 5)
				brojac2++;
			if (vrijediDo[i] == '.' && brojac1 != 2) {
				_vrijediDo = AlocirajNizKaraktera("NO DATE");
				throw NedozvoljenaOperacija("Niste unijeli dva broja za dan ili mjesec", __LINE__);
			}
			brojac1++;
			if (vrijediDo[i] == '.' && brojac1 == 3)
				brojac1 = 0;
		}
		if (brojac2 != 4) {
			_vrijediDo = AlocirajNizKaraktera("NO DATE");
			throw NedozvoljenaOperacija("Niste dobro unijeli godinu.", __LINE__);
		}
		if (brojac2 == 4)
			_vrijediDo = AlocirajNizKaraktera(vrijediDo);

	}
	//Potrebne konstruktor i destruktor funkcije
	Katalog(const char *naslov = "NOT SET", const char *vrijediDo = "01.01.1900") {
		//U sluèaju da datum ne ispunjava zadati format baciti izuzetak
		_naslov = AlocirajNizKaraktera(naslov);
		int brojac1 = 0, brojac2 = 0;
		for (size_t i = 0; i < strlen(vrijediDo); i++) {
			if (i > 5)
				brojac2++;
			if (vrijediDo[i] == '.' && brojac1 != 2) {
				_vrijediDo = AlocirajNizKaraktera("NO DATE");
				throw NedozvoljenaOperacija("Niste unijeli dva broja za dan ili mjesec", __LINE__);
			}
			brojac1++;
			if (vrijediDo[i] == '.' && brojac1 == 3)
				brojac1 = 0;
		}
		if (brojac2 != 4) {
			_vrijediDo = AlocirajNizKaraktera("NO DATE");
			throw NedozvoljenaOperacija("Niste dobro unijeli godinu.", __LINE__);
		}
		if (brojac2 == 4)
			_vrijediDo = AlocirajNizKaraktera(vrijediDo);

	}
	~Katalog()
	{
		delete[] _naslov;
		_naslov = nullptr;

		delete[] _vrijediDo;
		_vrijediDo = nullptr;

		for (size_t i = 0; i < _proizvodi.GetTrenutno(); i++) {
			delete _proizvodi[i];
			_proizvodi[i] = nullptr;
		}
	}
	//Kreirati funkciju za dodavanje proizvoda u katalog i pri tome provjeriti da li se radi
	//o objektu izvedene ili osnovne klase.
	//Onemoguæiti dodavanje identiènih proizvoda u jedan katalog (proizvode razlikovati samo na osnovu šifre).
	//U sluèaju pokušaja dodavanja istih proizvoda baciti izuzetak.
	void DodajProizvod(Proizvod&p) {
		for (size_t i = 0; i < _proizvodi.GetTrenutno(); i++) {
			if (*_proizvodi[i] == p)
				throw NedozvoljenaOperacija("Proizvod je vec dodan", __LINE__);
		}
		Odjeca* pok = dynamic_cast<Odjeca*>(&p);
		if (pok != nullptr)
			_proizvodi.AddElement(new Odjeca(*pok));
		else _proizvodi.AddElement(new Proizvod(p));

	}
	//Funkciju za uklanjanje proizvoda iz kataloga na osnovu šifre.
	//Ukoliko se ne pronaðe traženi proizvod baciti izuzetak.
	void UkloniElement(Proizvod&p) {
		for (size_t i = 0; i < _proizvodi.GetTrenutno(); i++) {
			if (*_proizvodi[i] == p) {
				_proizvodi.Remove(i);
				return;
			}
		}
		throw NedozvoljenaOperacija("Element ne postoji.", __LINE__);
	}
	//Preklopiti operator za ispis kataloške ponude
	friend ostream& operator<<(ostream&cout,  Katalog&k) {
		cout << k._naslov << "\n Vrijedi do: " << k._vrijediDo << endl;
		for (size_t i = 0; i < k._proizvodi.GetTrenutno(); i++){
			cout << *k._proizvodi.GetElement(i);
		}
		return cout;
	}
};

class Narudzba
{
	int _broj;
	char* _datumIsporuke; //Datum u formatu dd.MM.yyyy
	Kolekcija<Proizvod*> _naruceniProizvodi;
	bool _otkazana;
public:
	Narudzba& operator=(const Narudzba&n) {
		if (this != &n) {
			_broj = n._broj;
			_otkazana = n._otkazana;
			delete[] _datumIsporuke;
			_datumIsporuke = AlocirajNizKaraktera(n._datumIsporuke);
			for (int i = 0; i < _naruceniProizvodi.GetTrenutno(); i++){
				delete _naruceniProizvodi[i];
			}
			for (int i = 0; i <  n._naruceniProizvodi.GetTrenutno(); i++){
				_naruceniProizvodi.AddElement(n._naruceniProizvodi.GetElement(i));
			}
		}
		return *this;
	}
	Proizvod * GetProizvod(int i) { return _naruceniProizvodi[i]; }
	int najKolicina() {
		int najKolicina = _naruceniProizvodi[0]->GetKolicinu();
		for (int i = 0; i < _naruceniProizvodi.GetTrenutno(); i++)
			if (najKolicina < _naruceniProizvodi[i]->GetKolicinu())
				najKolicina = _naruceniProizvodi[i]->GetKolicinu();

		return najKolicina;
	}
	//pronalazak najvise narucenog nekog proizvod u narudzbama
	int najIndeks() {
		Proizvod *temp;
		int najKolicina = _naruceniProizvodi[0]->GetKolicinu(), indeks = 0;
		for (int i = 0; i < _naruceniProizvodi.GetTrenutno(); i++) {
			if (najKolicina < _naruceniProizvodi[i]->GetKolicinu()) {
				najKolicina = _naruceniProizvodi[i]->GetKolicinu();
				indeks = i;
			}
		}


		return indeks;
	}
	bool GetOtkazana() { return _otkazana; }
	//Potrebne konstruktor i destruktor funkcije
	//U sluèaju da datum ne ispunjava zadati format baciti izuzetak.
	Narudzba(int broj = 0, const char *vrijediDo = "01.01.1900", bool otkazana = false) :_broj(broj) {
		_otkazana = otkazana;
		int brojac1 = 0, brojac2 = 0;
		for (size_t i = 0; i < strlen(vrijediDo); i++) {
			if (i > 5)
				brojac2++;
			if (vrijediDo[i] == '.' && brojac1 != 2) {
				_datumIsporuke = AlocirajNizKaraktera("NO DATE");
				throw NedozvoljenaOperacija("Niste unijeli dva broja za dan ili mjesec", __LINE__);
			}
			brojac1++;
			if (vrijediDo[i] == '.' && brojac1 == 3)
				brojac1 = 0;
		}
		if (brojac2 != 4) {
			_datumIsporuke = AlocirajNizKaraktera("NO DATE");
			throw NedozvoljenaOperacija("Niste dobro unijeli godinu.", __LINE__);
		}
		if (brojac2 == 4)
			_datumIsporuke = AlocirajNizKaraktera(vrijediDo);

	}
	void operator()(int broj = 0, const char *vrijediDo = "01.01.1900", bool otkazana = false){
		_broj = broj;
		_otkazana = otkazana;
		int brojac1 = 0, brojac2 = 0;
		delete[] _datumIsporuke;
		for (size_t i = 0; i < strlen(vrijediDo); i++) {
			if (i > 5)
				brojac2++;
			if (vrijediDo[i] == '.' && brojac1 != 2) {
				_datumIsporuke = AlocirajNizKaraktera("NO DATE");
				throw NedozvoljenaOperacija("Niste unijeli dva broja za dan ili mjesec", __LINE__);
			}
			brojac1++;
			if (vrijediDo[i] == '.' && brojac1 == 3)
				brojac1 = 0;
		}
		if (brojac2 != 4) {
			_datumIsporuke = AlocirajNizKaraktera("NO DATE");
			throw NedozvoljenaOperacija("Niste dobro unijeli godinu.", __LINE__);
		}
		if (brojac2 == 4)
			_datumIsporuke = AlocirajNizKaraktera(vrijediDo);

	}
	//Kreirati funkciju za dodavanje proizvoda narudžbe sa povratnom vrijednošæu tipa bool.
	//Ukoliko proizvod više nije dostupan baciti izuzetak.
	//Dozvoliti dodavanje istog proizvoda više puta.
	bool DodajProizvod(Proizvod&p) {
		if (p.GetKolicinu() == 0)
			throw NedozvoljenaOperacija("Nema proizvoda na stanju.", __LINE__);

		_naruceniProizvodi.AddElement(&p);
		//p.SetKolicinu();
		return true;
	}
	//Funkciju za uklanjanje odreðenog proizvoda iz narudžbe. Voditi raèuna da narudžba može 
	//imati više puta isti proizvod.
	void UkloniProizvodIzNarudzbe(Proizvod&p) {
		for (size_t i = 0; i < _naruceniProizvodi.GetTrenutno(); i++) {
			if (*_naruceniProizvodi[i] == p) {
				_naruceniProizvodi.Remove(i);
				return;
			}
		}
		throw NedozvoljenaOperacija("Ne postoji taj proizvod u narudzbi.", __LINE__);
	}
	//Dodati funkciju za proraèun ukupne cijene narudžbe.
	//Voditi raèuna o moguænosti dodavanja više istih proizvoda (kolièina).
	//Uraèunati popust u sluèaju da je definisan.
	double UkupnaCijenaNaruzbe() {
		double suma = 0;
		for (size_t i = 0; i < _naruceniProizvodi.GetTrenutno(); i++) {
			suma += _naruceniProizvodi[i]->GetKolicinu() *_naruceniProizvodi[i]->GetCijena();
			if(_naruceniProizvodi[i]->GetPopust() != 0)
				suma*= _naruceniProizvodi[i]->GetPopust() * 0.01;
		}
		return suma;
	}
	//Preklopiti operator za ispis podataka o narudžbi zajedno sa cijenom.
	void Info() {
		cout << "Broj " << _broj << " datuma " << _datumIsporuke << endl;
		if (_otkazana)
			cout << "Narudzba je otkazana." << endl;
		else cout << "Narudzba nije otkazana" << endl;
		for (size_t i = 0; i < _naruceniProizvodi.GetTrenutno(); i++)
		{
			cout << _naruceniProizvodi[i];

		}
		cout << "Ukupna cijena: " << UkupnaCijenaNaruzbe() << endl;
	}
	friend ostream& operator<<(ostream& cout, Narudzba&n) {
		cout << "Broj " << n._broj << " datuma " << n._datumIsporuke << endl;
		if (n._otkazana)
			cout << "Narudzba je otkazana." << endl;
		else cout << "Narudzba nije otkazana" << endl;
		cout << n._naruceniProizvodi;
		cout << "Ukupna cijena: " << n.UkupnaCijenaNaruzbe() << endl;
		return cout;
	}
};

//U posljednjoj klasi samostalno prepoznati izuzetne situacije
class Prodavac
{
	char* _imePrezime;
	Kolekcija<Narudzba>* _narudzbe;
public:
	//Potrebne konstruktor i destruktor funkcije
	Prodavac(const char *imePrezime) {
		_narudzbe = new Kolekcija<Narudzba>();
		_imePrezime = AlocirajNizKaraktera(imePrezime);
	}
	~Prodavac() {
		delete[] _imePrezime;
		delete _narudzbe;
	}
	//Omoguæiti dodavanje narudžbi za prodavaèa
	void DodajNarudzbu(Narudzba&n) {
		_narudzbe->AddElement(n);
	}
	//Omoguæiti otkazivanje narudžbe na osnovu njenog broja.
	void OtkaziNarudzbu(int brojNaruzbe) {
		if (!(_narudzbe->Remove(brojNaruzbe - 1)))
			throw NedozvoljenaOperacija("Narudzba sa tim brojem ne postoji.", __LINE__);
	}
	//Ispis svih podataka o prodavaèu i kratke informacije o
	//svim narudžbama (datum, broj proizvoda i cijena). Posebno izdvojiti otkazane narudžbe.
	void Info() {
		cout << "Prodavac " << _imePrezime << endl;
		for (size_t i = 0; i < _narudzbe->GetTrenutno(); i++)
		{
			_narudzbe->GetElement(i).Info();
		}
	}
	//Implementirati metodu koja u svim aktivnim narudžbama pronalazi proizvod koji se najviše prodavao
	//sa informacijom o tome da li se radi o odjeæi ili osnovnom proizvodu.
	void NajprodavanijiProizvod() {
		int najVecaKolicina = 0;
		Proizvod *temp;
		int najvecaKolicina = _narudzbe->GetElement(0).najKolicina(), indeksNarudzbe = 0,
			indeksNajveceKol = _narudzbe->GetElement(0).najIndeks();
		for (int i = 1; i < _narudzbe->GetTrenutno(); i++) {
			if (!(_narudzbe->GetElement(i).GetOtkazana()))
				if (najvecaKolicina < _narudzbe->GetElement(i).najKolicina()) {
					indeksNarudzbe = i;
					najvecaKolicina = _narudzbe->GetElement(i).najKolicina();
					indeksNajveceKol = _narudzbe->GetElement(i).najIndeks();
				}
		}
		cout << *_narudzbe->GetElement(indeksNarudzbe).GetProizvod(indeksNajveceKol);
	}
};

int main()
{
	//Dodati minimalno 5 proizvoda u katalog.
	//Ispisati ponudu kupcu
	//Omoguæiti kreiranje narudžbe za jednog prodavaèa na osnovu izbora kupca.
	Proizvod p1, p2, p3, p4;
	try
	{
		p1("123", "Top", 5, 20);
		p2("124", "Sorc", 1.5, 5);
		p3("125", "Grudnjak", 2.1, 50);
		p4("126", "Sesir", 3.7, 8);

	}
	catch (NedozvoljenaOperacija&err) {
		err.ErrorInfo();
		cout << "Linija koda " << err.what();
	}
	p1.SetPopust(20);
	cout << p1;
	Odjeca o1("Majice", p1);
	Odjeca o2("Pantalone", p2);
	Odjeca o3("Rublje", p3);
	Odjeca o4("Accessories", p4);
	o1.DodajVelicinu("S");
	o1.DodajVelicinu("XS");
	o2.DodajVelicinu("M");
	o3.DodajVelicinu("B70");
	o4.DodajVelicinu("XXL");
	o4.DodajVelicinu("XS");
	o1.DodajBoju("Plava");
	o1.DodajBoju("Crvena");
	o2.DodajBoju("Zelena");
	o3.DodajBoju("Smedja");
	o4.DodajBoju("Bijela");
	o4.DodajBoju("Crna");

	o1.Info();
	cout << crt;
	o2.Info();
	cout << crt;
	o3.Info();
	cout << crt;
	o4.Info();
	cout << crt;

	Katalog k1;
	Katalog k2;
	try
	{
		k1("Konzum", "13.02.2019");
		k2("Amco", "13.03.2019");
		k1.DodajProizvod(p2);
		k1.DodajProizvod(p3);
		k1.DodajProizvod(o1);

		k2.DodajProizvod(o1);
		k2.DodajProizvod(o3);
		k2.DodajProizvod(p4);

		k1.UkloniElement(p1);
		k2.UkloniElement(o1);

	}
	catch (NedozvoljenaOperacija&err)
	{
		err.ErrorInfo();
		cout << "Linija koda " << err.what();


	}

	cout << k1 << crt << k2 << crt;

	Narudzba n1, n2, n3;
	try
	{
		n1(1,"13.02.2019");
		n2(2,"23.02.2019");
		n3(3,"23.09.2019", true);
		
		n1.DodajProizvod(p1);
		n1.DodajProizvod(o2);

		n2.DodajProizvod(p4);
		n2.DodajProizvod(o3);

		n3.DodajProizvod(p4);
		n3.DodajProizvod(o1);

		n1.UkloniProizvodIzNarudzbe(p2);
		n2.UkloniProizvodIzNarudzbe(p4);
	}
	catch (NedozvoljenaOperacija&err)
	{
		err.ErrorInfo();
		cout << "Linija koda " << err.what();
	}
	n1.Info();
	Prodavac p("Nudzejma Kezo");
	p.DodajNarudzbu(n1);
	p.DodajNarudzbu(n2);
	cout << crt;
	p.DodajNarudzbu(n3);
	p.Info();
	p.NajprodavanijiProizvod();
	system("pause");
	return 0;
}
