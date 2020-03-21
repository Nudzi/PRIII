#include<iostream>
#include<string>
#include<vector>
#include<thread>
#include<mutex>
#include<iomanip>
using namespace std;
#pragma warning (disable:4996)
/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
****************************************************************************/

const char* nedozvoljena_operacija = "Nedozvoljena operacija\n";
const char* not_set = "NOT_SET";
const int min_polgavlja = 3;
const int min_karaktera_po_poglavlju = 30;

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
	int _trenutno;
public:
	Kolekcija(const Kolekcija&k) {
		_trenutno = k._trenutno;
		for (size_t i = 0; i < _trenutno; i++){
			*_elementi1[i] = k._elementi1[i];
			*_elementi2[i] = k._elementi2[i];
		}
	}
	Kolekcija() :_trenutno(0), _elementi1(nullptr), _elementi2(nullptr) { }
	void AddElement(const T1& elem1, const T2& elem2) {

		for (int i = 0; i < _trenutno; i++)
			if (_elementi1[i] == elem1 || _elementi2[i] == elem2)
				throw exception(nedozvoljena_operacija);

		T1 * temp1 = new T1[_trenutno + 1];
		T2 * temp2 = new T2[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++) {
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[] _elementi1; 
		delete[] _elementi2; 
		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[_trenutno] = elem1;
		_elementi2[_trenutno] = elem2;
		_trenutno++;
	}

	T1 * getElementi1Pok() { return _elementi1; }
	T2 * getElementi2Pok() { return _elementi2; }
	T1 & getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2 & getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() { return _trenutno; }

	friend ostream & operator<<(ostream & COUT, Kolekcija<T1, T2> & obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
};
class Poglavlje {
	char * _naslov;
	char * _sadrzaj;
	bool _prihvaceno;
	int _ocjena;//da bi se poglavlje smatralo prihvacenim ocjena mora biti u opsegu od 6 - 10
public:
	void Info() {
		cout << "naslov " << _naslov << " ocjena " << _ocjena << endl;
		cout << "sadrzaj: \n" << _sadrzaj << endl;
	}
	Poglavlje(const char * naslov = nullptr, const char * sadrzaj = nullptr)
		:_ocjena(0), _prihvaceno(false) {
		_naslov = AlocirajNizKaraktera(naslov);
		_sadrzaj = AlocirajNizKaraktera(sadrzaj);
	}
	Poglavlje() {
		delete[] _naslov; _naslov = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	friend ostream & operator<<(ostream & COUT, Poglavlje & obj) {
		if (obj._naslov == nullptr || obj._sadrzaj == nullptr)
			return COUT;
		COUT << endl << obj._naslov << endl << obj._sadrzaj << endl;
		if (obj._prihvaceno)
			COUT << "Ocjena: " << obj._ocjena << endl;;
		return COUT;
	}
	char * GetNaslov() { return _naslov; }
	char * GetSadrzaj() { return _sadrzaj; }
	void SetSadrzaj(const char * n) { if(n!=nullptr)delete[] _sadrzaj; _sadrzaj = AlocirajNizKaraktera(n); }
	
	bool GetPrihvaceno() { return _prihvaceno; }
	int GetOcjena() { return _ocjena; }

	void OcijeniPoglavlje(int ocjena) {
		_ocjena = ocjena;
		if (_ocjena > 5 && ocjena <= 10)
			_prihvaceno = true;
	}
};

class ZavrsniRad {
	char* _tema;
	vector<Poglavlje> _poglavljaRada;
	string _datumOdbrane;
	float _konacnaOcjena; //prosjek ocjena svih poglavlja u zavrsnom radu koja se izracunava u momentu zakazivanja odbrane
public:
	friend ostream&operator<<(ostream& cout, ZavrsniRad *rad) {
		cout << "Tema " << rad->_tema << " na datum " << rad->_datumOdbrane << endl;
		for (size_t i = 0; i < rad->_poglavljaRada.size(); i++)
			rad->_poglavljaRada[i].Info();
		cout << "Formirana konacna ocjena: " << rad->_konacnaOcjena << ".\n";
		return cout;
	}
	void SetKonacnuOcjenu() {
		float konacna = 0;
		for (size_t i = 0; i < _poglavljaRada.size(); i++)
			konacna += _poglavljaRada[i].GetOcjena();
		_konacnaOcjena = konacna / _poglavljaRada.size();
	}
	void SetDate(string datum) { _datumOdbrane = datum; }
	bool odobrenaPoglavlja() {
		for (size_t i = 0; i < _poglavljaRada.size(); i++)
			if (!_poglavljaRada[i].GetPrihvaceno())
				return false;
		
		return true;
	}
	bool brojKaraktera() {
		for (size_t i = 0; i < _poglavljaRada.size(); i++)
			if (strlen(_poglavljaRada[i].GetSadrzaj()) < min_karaktera_po_poglavlju)
				return false;
		return true;
	}
	ZavrsniRad & operator=(const ZavrsniRad&rad) {
		if (this != &rad) {
			delete[] _tema;
			_tema = AlocirajNizKaraktera(rad._tema);
			_konacnaOcjena = rad._konacnaOcjena;
			_datumOdbrane = rad._datumOdbrane;
			_poglavljaRada = rad._poglavljaRada;
			
		}return*this;
	}
	bool operator==(const ZavrsniRad& rad) {
		return (strcmp(rad.GetNazivTeme(), _tema) == 0);
	}
	void prosiriSadrzaj(int i, const char * sadrzaj) {
		char *temp = new char[strlen(_poglavljaRada[i].GetSadrzaj()) + strlen(sadrzaj) + 3];
		strcpy(temp, _poglavljaRada[i].GetSadrzaj());
		strcat(temp, " ");
		strcat(temp, sadrzaj);
		_poglavljaRada[i].SetSadrzaj(temp);
	}
	void DodajPoglavlje(const char * nazivPoglavlja, const char* sadrzajPoglavlja) {
		for (size_t i = 0; i < _poglavljaRada.size(); i++)
			if (strcmp(_poglavljaRada[i].GetNaslov(), nazivPoglavlja) == 0) {
				prosiriSadrzaj(i, sadrzajPoglavlja);
				return;
			}
		_poglavljaRada.push_back(Poglavlje(nazivPoglavlja, sadrzajPoglavlja));
	}
	ZavrsniRad(const char * nazivTeme = nullptr) : _konacnaOcjena(0), _datumOdbrane(not_set) {
		_tema = AlocirajNizKaraktera(nazivTeme);
	}

	ZavrsniRad(const ZavrsniRad& org) : _poglavljaRada(org._poglavljaRada), _konacnaOcjena(org._konacnaOcjena), _datumOdbrane(org._datumOdbrane) {
		_tema = AlocirajNizKaraktera(org._tema);
	}
	void SetDatumOdbrane(string datum) { _datumOdbrane = datum; }
	~ZavrsniRad() {
		delete[] _tema; _tema = nullptr;
	}
	char* GetNazivTeme()const { return _tema; }
	vector<Poglavlje>& GetPoglavlja() { return _poglavljaRada; };
	string GetDatumOdbrane()const { return _datumOdbrane; }
	float GetOcjena() { return _konacnaOcjena; }
	friend ostream& operator<<(ostream& COUT, ZavrsniRad& obj) {
		COUT << "Tema rada: " << obj._tema << endl;
		COUT << "Sadrzaj: " << endl;
		for (size_t i = 0; i < obj._poglavljaRada.size(); i++)
			COUT << obj._poglavljaRada[i] << endl;
		COUT << "Datum odbrane rada: " << obj._datumOdbrane << endl << " Ocjena: " << obj._konacnaOcjena << endl;
		return COUT;
	}
};

class Osoba {
protected:
	string _imePrezime;
public:
	Osoba(string imePrezime) : _imePrezime(imePrezime) {}
	string GetImePrezime() { return _imePrezime; }
	virtual void Info() = 0 {};
};

class Nastavnik : public Osoba{
	//Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog nastavnika
	Kolekcija<string, ZavrsniRad> _teme;
public:
	string GetIndeks(int prosjek) {
		string s;
		for (size_t i = 0; i < _teme.getTrenutno(); i++) {
			if (_teme.getElement2(i).GetOcjena() > prosjek)
				s += _teme.getElement1(i) + ", ";
		}
		return s;
	}
	string sadrzaj(int prosjek) {
		///*Funkcija PronadjiNajStudenta ima zadatak da svim studentima koji su uspjesno okoncali svoj zavrsni rad kod nastavnika/mentora 
		//sa najnizom prosjecnom ocjenom
		//i tom prilikom ostvarili ocjenu vecu od proslijedjene, u zasebnom thread.u, 
		//posalje email poruku (mail adresa: brojIndeksa@edu.fit.ba) sa sadrzajem, "Postovani {brojIndeksa}, uzimajuci u obzir cinjenicu 
		//da ste kod mentora {imePrezimeMentora} uspjesno odbranili rad sa ocjenom {ocjena} cast nam je pozvati vas na dodjelu nagrada za 
		//najbolje studente koja ce se odrzatu u na FIT-u 20.09.2018. godine."
		//da su svoj zavrsni rad uspjesno odbranili sa vecom ili manjom ocjenom od prosjecne.
		//funkcija treba da vrati sadrzaj svih poslatih email poruka, a ukoliko niti jedan od nastavnika ne posjeduje evidentirano mentorstvo 
		//na zavrsnom radu, funkcija vraca not_set*/
		string info = "";
		for (size_t i = 0; i < _teme.getTrenutno(); i++) {
			if (_teme.getElement2(i).GetOcjena() > prosjek) {
				info = "mail adresa : " + _teme.getElement1(i) + "@edu.fit.ba\n Postovani " + _teme.getElement1(i) + " uzimajuci u obzir cinjenicu";
				info += " da ste kod mentora " + Osoba::GetImePrezime() + " uspjesno odbranili rad sa ocjenom " + to_string(_teme.getElement2(i).GetOcjena());
				info += " cast nam je pozvati vas na dodjelu nagrada za najbolje studente koja ce se odrzatu u na FIT-u 20.09.2018. godine.";
			}

		}
		return info;
	}
	bool daLiPostojiIndeks(string indeks) {
		for (size_t i = 0; i < _teme.getTrenutno(); i++) {
			if (indeks == _teme.getElement1(i))
				return true;
		}
		return false;
	}
	/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto
	podrazumijeva definisanje datuma odbrane. Odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio.
	Za zakazivanje odbrane rada moraju biti zadovoljeni sljedeci uslovi:
	1. zavrsni rad mora imati broj poglavlja veci od minimalnog
	2. svako poglavlje mora imati broj karaktera veci od minimalnog
	3. svako poglavlje mora biti prihvaceno/odobreno
	ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja),
	postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazan odbrana.
	u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije zadovoljen,
	funkcija vraca nullptr.
	*/
	ZavrsniRad *ZakaziOdbranuRada(string indeks, string datumObrane) {
		int j = -1;
		for (int i = 0; i < _teme.getTrenutno(); i++) {
			if (!daLiPostojiIndeks(indeks) || _teme.getElement2(i).GetPoglavlja().size() < min_polgavlja) {
				if (!_teme.getElement2(i).brojKaraktera() || !_teme.getElement2(i).odobrenaPoglavlja())
					return nullptr;
			}
			else { j = i; }
		}
		if (j == -1)
			return nullptr;
		_teme.getElement2(j).SetKonacnuOcjenu();
		_teme.getElement2(j).SetDatumOdbrane(datumObrane);
		return &_teme.getElement2(j);
	}
	bool DodajZavrsniRad(string indeks, const ZavrsniRad& rad) {
		for (size_t i = 0; i < _teme.getTrenutno(); i++){
			if (strcmp(_teme.getElement2(i).GetNazivTeme(), rad.GetNazivTeme()) == 0 || (_teme.getElement1(i) == indeks))
				return false;
		}
		try
		{
		_teme.AddElement(indeks, rad);
		return true;
		}
		catch (const std::exception&err)
		{
			cout<<err.what();
		}
	}
	Nastavnik(string ime):Osoba(ime) {
	}
	Kolekcija<string, ZavrsniRad>& GetTeme() { return _teme; }
	virtual void Info() { cout << _teme;}
	friend ostream& operator<<(ostream&cout, Nastavnik & n) {
		for (int i = 0; i < n._teme.getTrenutno(); i++){
			cout << n._teme.getElement1(i) << " ::rad:: " << n._teme.getElement2(i) << endl;
		}
		return cout;
	}

};
mutex cuvar;
string PosaljiEmail(Nastavnik **n, int max, int prosjekOcjene, string& indeksi) {
	string s;
	for (size_t i = 0; i < max; i++){
		s = n[i]->sadrzaj(prosjekOcjene);
		if (s != "") {
			cuvar.lock();
			indeksi += n[i]->GetIndeks(prosjekOcjene);
			cout << s;
			cuvar.unlock();
		}
	}
	return indeksi;
}
string  PosaljiPozivZaDodjeluNagrada(Nastavnik ** n, int max, int prosjekOcjene) {
	string studenti = "";
	thread tr(PosaljiEmail, n, max, prosjekOcjene, ref(studenti));
	tr.join();
	return studenti;
}
int main() {
	const int max = 4;
	Nastavnik * nastavnici[max];

	nastavnici[0] = new Nastavnik("Denis Music");
	nastavnici[1] = new Nastavnik("Zanin Vejzovic");
	nastavnici[2] = new Nastavnik("Jasmin Azemovic");
	nastavnici[3] = new Nastavnik("Emina Junuz");


	ZavrsniRad multimedijalni("Multimedijalni informacijski sistem za visoko - obrazovnu ustanovu");
	ZavrsniRad podrsa_operaterima("Sistem za podršku rada kablovskog operatera");
	ZavrsniRad analiza_sigurnosti("Prakticna analiza sigurnosti bežiènih raèunarskih mreža");
	ZavrsniRad kriptografija("Primjena teorije informacija u procesu generisanja kriptografskih kljuèeva");


	/*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj. ukoliko poglavlje vec postoji u zavrsnom radu, 
	funkcija tom poglavlju treba dodati novi sadrzaj i pri tome zadrzi postojeci (izmedju postojeceg i novog sadrzaja se dodaje prazan prostor).
	U slucaju da poglavlje ne postoji, ono se dodaje zajedno sa sadrzaje*/
	//nazivPoglavlja, sadrzajPoglavlja
	multimedijalni.DodajPoglavlje("Uvod", "U ovom poglavlju ce biti rijeci");
	multimedijalni.DodajPoglavlje("Uvod", "o multimedijalnim sistemima koji se danas koriste");
	multimedijalni.DodajPoglavlje("Uvod", "u savremenom poslovanju");
	multimedijalni.DodajPoglavlje("Vrste multimedijalnih sistema", "Danas se moze govoriti o nekoliko vrsta multimedijalnih sistema, a neke od najznacajnijih su ...");
	multimedijalni.DodajPoglavlje("Teorija multimedije", "Sadrzaj koji bi trebao stajati na pocetku treceg poglavlja zavrsnog rada o multimediji studenta IB130011");
	for (size_t i = 0; i < multimedijalni.GetPoglavlja().size(); i++){
		multimedijalni.GetPoglavlja()[i].OcijeniPoglavlje(6 + i);
	}
	/*funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. 
	Zavrsni rad se dodaje studentu sa brojem indeksa proslijedjenim kao prvi parametar.
	sprijeciti dodavanje zavrsnih radova sa istom temom*/
	//brojIndeksa, zavrsniRad
	if (nastavnici[0]->DodajZavrsniRad("IB130011", multimedijalni))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (nastavnici[0]->DodajZavrsniRad("IB120051", podrsa_operaterima))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (!nastavnici[0]->DodajZavrsniRad("IB120056", podrsa_operaterima))//dupliranje rada
		cout << "Zavrsni rad nije dodat!" << endl;
	if (!nastavnici[0]->DodajZavrsniRad("IB120051", kriptografija)) //studentu vec dodijeljen rad
		cout << "Zavrsni rad nije dodat!" << endl;
	if (nastavnici[1]->DodajZavrsniRad("IB140102", analiza_sigurnosti))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (nastavnici[2]->DodajZavrsniRad("IB140002", kriptografija))
		cout << "Zavrsni rad uspjesno dodat!" << endl;

	//brojIndeksa, datumOdbrane
	ZavrsniRad * zr1 = nastavnici[0]->ZakaziOdbranuRada("IB130011", "25.09.2018");
	if (zr1 != nullptr)
		cout << *zr1 << endl;

	zr1 = nastavnici[0]->ZakaziOdbranuRada("IB130111", "25.09.2018");//student sa brojem indeksa IB130111 jos uvijek nije prijavio rad
	if (zr1 != nullptr)
		cout << *zr1 << endl;

	//ispisuje sve podatke o nastavniku i njegovim mentorstvima
	cout << *nastavnici[0] << endl;

	
	cout << "Studenti za dodjelu nagrada: " << PosaljiPozivZaDodjeluNagrada(nastavnici, max, 6.5) << endl;

	///*
	//Koristeci postojeci programski code, demonstrirati i ukratko opisati polimorfizam.
	//*/

	for (int i = 0; i < max; i++)
	{
		delete nastavnici[i];
		nastavnici[i] = nullptr;
	}
	system("pause>0");
	return 0;
}