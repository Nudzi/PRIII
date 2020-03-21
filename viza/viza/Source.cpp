#include<iostream>
#include<regex>
#include<vector>
#include<mutex>
#include<exception>
#include<chrono>
using namespace std;

const char *crt = "\n-----------------------------------------------\n";

enum Status { ZAHTJEV_ZAPRIMLJEN, ODUSTAO, ODBIJENA, IZDATA, ISTEKLA, PONISTENA };
enum Sortiranje { RASTUCI, OPADAJUCI };
enum Drzava { SAD, AUSTRALIJA, ENGLESKA };

class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	int sumaDana()const { return *_dan + *_mjesec * 30 + *_godina * 365; }
	//post  inkrement void opetator++(int) { Vrijeme tmp(this); <- konst kopije
	///		 (*sati)++; return tmp; <- kons kopije //vraca se onaj prije promijene
	//pre  inkrement void opetator++() { (*sati)++; return *this; Vrijeme& operator++ 
	//jer vraca vrijednost jer ga uvecava prije ++v
	Datum(const Datum&d) {
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
	}
	friend bool operator==(const Datum&d, const Datum&dd) {
		return *d._dan == *dd._dan && *d._mjesec == *dd._mjesec && *d._godina == *dd._godina;
	}
	Datum &operator=(const Datum&d) {
		if (this != &d) {
			_dan = new int(*d._dan);
			_mjesec = new int(*d._mjesec);
			_godina = new int(*d._godina);
		}
		return *this;
	}
	Datum operator++() {
		(*_dan) = *_dan + 1;
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
	friend ostream& operator<< (ostream &COUT, Datum &obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
	friend ostream& operator<< (ostream &COUT, Datum *obj) {
		COUT << *obj->_dan << "." << *obj->_mjesec << "." << *obj->_godina;
		return COUT;
	}
};
template<class T1, class T2, int max>
class FITKolekcija {
	T1 * _elementi1[max];
	T2 * _elementi2[max];
	int _trenutno;
public:
	void Sortiraj(Sortiranje nacin) {
		for (size_t i = 1; i < _trenutno; i++) {
			int j = i;
			if (nacin == 0) {
				while (j > 0 && *_elementi1[j] > *_elementi1[j - 1]) {
					T1 temp = *_elementi1[j];
					*_elementi1[j] = *_elementi1[j - 1];
					*_elementi1[j - 1] = temp;
					T2 temp2 = *_elementi2[j];
					*_elementi2[j] = *_elementi2[j - 1];
					*_elementi2[j - 1] = temp2;
					j--;
				}
			}
			if (nacin == 1) {
				while (j > 0 && *_elementi1[j] < *_elementi1[j - 1]) {
					T1 temp = *_elementi1[j];
					*_elementi1[j] = *_elementi1[j - 1];
					*_elementi1[j - 1] = temp;
					T2 temp2 = *_elementi2[j];
					*_elementi2[j] = *_elementi2[j - 1];
					*_elementi2[j - 1] = temp2;
					j--;
				}
			}
		}
	}
	void Dodaj(const T1&el1, const T2&el2) {
		if (_trenutno == max)
			throw exception("Prekoracenje kapaciteta.");
		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno++] = new T2(el2);
	}
	FITKolekcija() {
		for (size_t i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}
	~FITKolekcija() {
		for (size_t i = 0; i < max; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	T1 * GetT1(int i) { return _elementi1[i]; }
	T2 * GetT2(int i) { return _elementi2[i]; }
	int GetTrenutno() { return _trenutno; }

	friend ostream& operator<< (ostream &COUT, FITKolekcija &obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << *obj._elementi1[i] << " " << *obj._elementi2[i] << endl;
		return COUT;
	}
};

class Viza {
protected:
	Drzava _drzava;
	Datum _vazenjeOD;
	Datum _vazenjeDO;
	char * _napomena;
	// string se odnosi na razlog promjene statusa, a moguæe je samo 10 izmjena statusa
	FITKolekcija<Status, string, 10> _statusi;
public:
	Viza(const Viza&v) {
		_drzava = v._drzava;
		_vazenjeDO = v._vazenjeDO;
		_vazenjeOD = v._vazenjeOD;
		_napomena = new char[strlen(v._napomena) + 1];
		strcpy_s(_napomena, strlen(v._napomena) + 1, v._napomena);
	}
	friend bool operator==( Viza&v,  Viza& p) {
		return v.GetDrzava() == p.GetDrzava() && v.GetVazenjeDO() == p.GetVazenjeDO() && v.GetVazenjeOD() == p.GetVazenjeOD();
	}
	bool vratiStatus(Status s) {
		for (size_t i = 0; i < _statusi.GetTrenutno(); i++){
			if (*_statusi.GetT1(i) == s)
				return true;
		}
		return false;
	}
	int vratiPosljednjiStatus() {
		if (_statusi.GetTrenutno() == 0) return -1;
		return *_statusi.GetT1(_statusi.GetTrenutno() - 1);}
	bool isPonistena() {
		for (size_t i = 0; i < _statusi.GetTrenutno(); i++)
			if (*_statusi.GetT1(i) == PONISTENA)
				return true;
		
			return false;
	}
	Viza(Drzava drzava, Datum _OD, Datum _DO, const char * napomena = nullptr) :_vazenjeDO(_DO), _vazenjeOD(_OD)
	{
		if (napomena != nullptr) {
			_napomena = new char[strlen(napomena) + 1];
			strcpy_s(_napomena, strlen(napomena) + 1, napomena);
		}
		else
			_napomena = nullptr;
		_drzava = drzava;
	}
	~Viza() { delete[] _napomena; _napomena = nullptr; }
	Datum GetVazenjeOD() { return _vazenjeOD; }
	Drzava GetDrzava() { return _drzava; }
	Datum GetVazenjeDO() { return _vazenjeDO; }
	char * GetNapomena() { return _napomena; }
	void SetNapomena(const char * n) { delete[] _napomena;
	_napomena = new char[strlen(n) + 1];
	strcpy_s(_napomena, strlen(n) + 1, n);
	}
	FITKolekcija<Status, string, 10> & GetStatuse() { return _statusi; }
	virtual void Info() = 0 {
		
		cout << _drzava << " " << _vazenjeDO << " od " << _vazenjeOD << endl;
		cout << _napomena << endl;
		cout << _statusi;

	}
};

class PoslovnaViza :public Viza {
	string _kompanija; //naziv kompanije koja potvrðuje da posluje sa aplikantom
public:
	PoslovnaViza(PoslovnaViza &v):Viza(v) {
		_kompanija = v._kompanija;
	}
	PoslovnaViza(Drzava drzava, Datum _OD, Datum _DO, string kompanija, const char * napomena = "Nema opisa") : Viza(drzava, _OD, _DO, napomena) {
		_kompanija = kompanija;
	}
	virtual void Info() {
		cout << _drzava << " " << _vazenjeOD << " " << _vazenjeDO << " " << _kompanija << endl;
		cout << ((_napomena != nullptr) ? _napomena : "") << endl;
		cout << "STATUSI -> " << _statusi << endl;
	}
	friend ostream & operator << (ostream & COUT, PoslovnaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._kompanija << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;

	}

};
class TuristickaViza : public Viza {
	string _adresa; //adresa hotele ili drugog smještaja na kojoj æe aplikant boraviti
public:
	virtual void Info() {

	}
	TuristickaViza(const TuristickaViza &v) :Viza(v) {
		_adresa = v._adresa;
	}
	TuristickaViza(Drzava drzava, Datum _OD, Datum _DO, string adresa, const char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		_adresa = adresa;
	}
	friend ostream & operator << (ostream & COUT, TuristickaViza & obj) {
		COUT << obj._drzava << " " << obj._vazenjeOD << " " << obj._vazenjeDO << " " << obj._adresa << endl;
		COUT << ((obj._napomena != nullptr) ? obj._napomena : "") << endl;
		COUT << "STATUSI -> " << obj._statusi << endl;
		return COUT;

	}
};

class Aplikant {
	char * _imePrezime;
	Datum * _datumRodjenja;
	char * _brojPasosa;
	vector<Viza *> _vize;
public:
	//UNUTAR NAPOMENE MIJENJA SADRŽAJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
	void ZamijeniRijec(string prije, string poslije) {
		regex r("(\\b" + prije + "\\b)|(\\B" + prije + "\\B)");
		for (size_t i = 0; i < _vize.size(); i++)
		{
			if (_vize[i]->GetNapomena() != nullptr) {
				string zamijeni(_vize[i]->GetNapomena());
				string zamijenjenaRijec = regex_replace(zamijeni, r, poslije);
				_vize[i]->SetNapomena(zamijenjenaRijec.c_str());
			}
		}
	}
	vector<Viza *> GetVizeByDrzava(Drzava d, Status s) {
		vector<Viza *> viza;
		for (size_t i = 0; i < _vize.size(); i++){
			if (_vize[i]->GetDrzava() == d && _vize[i]->vratiStatus(s)) {
				viza.push_back(_vize[i]);
			}
		}
		return viza;
	}
	void PrintajVizu(Viza *v) {
		thread tr(&Viza::Info, v);
		mutex l;
		l.lock();
		cout << v->GetDrzava() << " ";
		cout << &v->GetVazenjeOD();
		cout <<  " od " << &v->GetVazenjeDO() << endl;
		cout << v->GetNapomena() << endl;
		cout << &v->GetStatuse();
		this_thread::sleep_for(chrono::seconds(5));
		l.unlock();
		tr.join();
	}
	/*
	STATUSI SE MOGU DODAVATI SAMO U REDOSLIJEDU U KOJEM SE NALAZE U ENUMERACIJI NPR.
	- NIJE MOGUÆE ODBITI VIZU AKO PRETHODNO NIJE IMALA STATUS DA JE ZAHTJEV_ZAPRIMLJEN;
	- NIJE MOGUÆE PONIŠTITI VIZU KOJA PRETHODNO NIJE IZDATA
	*/
	bool DodajStatus(Viza *p, Status status, const char *naziv = "NO NAME\n") {
		for (size_t i = 0; i < _vize.size(); i++) {
			if (_vize[i] == p) {
				if (_vize[i]->vratiPosljednjiStatus() + 1 != status)
					return false;
				_vize[i]->GetStatuse().Dodaj(status, naziv);
				return true;
			}
		}
		cout << "Ne postoji ova viza" << endl;
		return false;
	}
	/*
	NOVA VIZA SE NE MOŽE DODATI UKOLIKO:
	- JE APLIKANT MLAÐI OD 10 GODINA
	- U TOM PERIODU (ODNOSI SE NA PERIOD VIZE OD - DO) NE POSTOJI VAŽEÆA 
	(POJAM VAŽEÆA PODRAZUMIJEVA DA JE POSLJEDNJI STATUS VIZE BIO IZDATA TJ.
	DA VIZA NIJE PONIŠTENA ILI ISTEKLA) VIZA ZA TU DRŽAVU, BEZ OBZIRA NA NJENU VRSTU.
	- JE NEKA OD RANIJIH VIZA PONIŠTENA (OVO SE ODNOSI SAMO NA VIZE ZA DRŽAVU: SAD)
	*/
	bool DodajVizu(Viza *v) {
		Datum danas(16,02,2107);
		if (_datumRodjenja->sumaDana() - 3650 > (danas.sumaDana()))
			return false;
		if(danas.sumaDana() < v->GetVazenjeDO().sumaDana())
			return false;
		if (v->vratiPosljednjiStatus() == PONISTENA || v->vratiPosljednjiStatus() == ISTEKLA)
			return false;
		if (v->GetDrzava() == SAD && v->isPonistena())
			return false;

		if (dynamic_cast<PoslovnaViza*>(v) != nullptr) {
			_vize.push_back(new PoslovnaViza(*dynamic_cast<PoslovnaViza*>(v)));

		}
		else if (dynamic_cast<TuristickaViza*>(v) != nullptr)
			_vize.push_back(new TuristickaViza(*dynamic_cast<TuristickaViza*>(v)));
		return true;

	}
	Aplikant(const char * imePrezime, Datum datumRodjenja, const char * brPasosa) {
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		vel = strlen(brPasosa) + 1;
		_brojPasosa = new char[vel];
		strcpy_s(_brojPasosa, vel, brPasosa);
		_datumRodjenja = new Datum(datumRodjenja);
	}
	~Aplikant() {
		delete[]_imePrezime; _imePrezime = nullptr;
		delete[]_brojPasosa; _brojPasosa = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
		for (size_t i = 0; i < _vize.size(); i++)
		{
			delete _vize[i]; _vize[i] = nullptr;
		}
	}
	friend ostream & operator << (ostream & COUT, Aplikant & obj) {
		COUT << obj._imePrezime << " " << *obj._datumRodjenja << " " << obj._brojPasosa << endl;
		COUT << "VIZE -> " << endl;
		for (size_t i = 0; i < obj._vize.size(); i++)
			obj._vize[i]->Info();
		return COUT;
	}

	vector<Viza * > GetVize() { return _vize; }
};

void main() {

	Datum danas(15, 2, 2017);
	Datum sutra(++danas); // inkrementiranjem se uvecava dan za 1
	Datum prekosutra;
	Datum dat15022017(15, 2, 2017), dat18082017(18, 8, 2017), dat11052018(11, 5, 2018), dat11122017(11, 12, 2017);

	prekosutra = danas;
	if (prekosutra == danas)
		cout << "Datumi su jednaki" << endl;
	cout << danas << endl << sutra << endl << prekosutra << crt;

	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int, 7> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	//NA KOJI NAÈIN ÆE SE VRŠITI SORTIRANJE PRVENSTVENO ZAVISI OD VRIJEDNOSTI PARAMETRA, 
	//A VRŠI SE NA OSNOVU VRIJEDNOSTI TIPA T1
	brojevi.Sortiraj(RASTUCI);
	cout << brojevi << crt;
	brojevi.Sortiraj(OPADAJUCI);
	cout << brojevi << crt;

	try
	{
		//U SLUÈAJU PREKORAÈENJA DEFINISANE VELIÈINE KOLEKCIJE, POTREBNO JE BACITI IZUZETAK
		brojevi.Dodaj(v13, v196);
	}
	catch (exception & err)
	{
		cout << "Error -> " << err.what() << endl;
	}

	Aplikant jasmin("Jasmin Azemovic", Datum(15, 5, 1979), "BS6985645");
	PoslovnaViza poslovna(SAD, dat15022017, dat18082017, "Microsoft");

	/*
	NOVA VIZA SE NE MOŽE DODATI UKOLIKO:
	- JE APLIKANT MLAÐI OD 10 GODINA
	- U TOM PERIODU (ODNOSI SE NA PERIOD VIZE OD - DO) NE POSTOJI VAŽEÆA (POJAM VAŽEÆA PODRAZUMIJEVA DA JE POSLJEDNJI STATUS VIZE BIO IZDATA TJ. DA VIZA NIJE PONIŠTENA ILI ISTEKLA) VIZA ZA TU DRŽAVU, BEZ OBZIRA NA NJENU VRSTU.
	- JE NEKA OD RANIJIH VIZA PONIŠTENA (OVO SE ODNOSI SAMO NA VIZE ZA DRŽAVU: SAD)
	*/
	if (jasmin.DodajVizu(&poslovna))
		cout << "Podaci o vizi uspješno zaprimljeni" << endl;
	/*
	STATUSI SE MOGU DODAVATI SAMO U REDOSLIJEDU U KOJEM SE NALAZE U ENUMERACIJI NPR.
	- NIJE MOGUÆE ODBITI VIZU AKO PRETHODNO NIJE IMALA STATUS DA JE ZAHTJEV_ZAPRIMLJEN;
	- NIJE MOGUÆE PONIŠTITI VIZU KOJA PRETHODNO NIJE IZDATA
	*/
	if (jasmin.DodajStatus(&poslovna, ZAHTJEV_ZAPRIMLJEN, "provjeriti navode sa predstavnicima kompanije Microsoft"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;


	if (jasmin.DodajStatus(&poslovna, PONISTENA, "predstavnicima kompanije Microsoft nemaju informaciju o posjeti navedenog aplikanta"))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	TuristickaViza turisticka(ENGLESKA, dat18082017, dat11122017, "Whitehall Pl, London SW1A 2BD, UK");

	if (jasmin.DodajStatus(&turisticka, ZAHTJEV_ZAPRIMLJEN))
		cout << "Status vize uspijesno promijenjen" << endl;
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

	if (jasmin.DodajStatus(&turisticka, IZDATA)) {
		cout << "Status vize uspijesno promijenjen" << endl;
		//U ZASEBNOJ NITI URADITI PRINTANJE VIZE...KORISTITI METODU SLEEP_FOR
	}
	else
		cout << "Aplikant " << jasmin << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;
	jasmin.PrintajVizu(&poslovna);
	//VRAÆA PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODREÐENU DRŽAVU, A KOJE SU IMALE ODREÐENI STATUS
	vector<Viza *> jasminVize = jasmin.GetVizeByDrzava(SAD, PONISTENA);

	//UNUTAR NAPOMENE MIJENJA SADRŽAJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
	jasmin.ZamijeniRijec("nemaju informaciju", "su upoznti sa");
	system("pause>0");
}