#include<iostream>
#include<regex>
#include<mutex>
#include<thread>
#include<exception>
#include<string>
#include<vector>

using namespace std;
class Datum {
	int *_dan, *_mjesec, *_godina;
public:
	int sumaDana() { return *_dan + *_mjesec * 30 + *_godina * 365; }
	friend bool operator==(const Datum&k, const Datum&k2) {
		return (*k._dan == *k2._dan && *k._mjesec == *k2._mjesec && *k._godina == *k2._godina);
	}
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum&d) {
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
	}
	Datum & operator=(const Datum&d) {
		if (this != &d) {
			_dan = new int(*d._dan);
			_mjesec = new int(*d._mjesec);
			_godina = new int(*d._godina);
		}
		return *this;
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
};

class Izuzetak :public exception {
	Datum _datum;//KORISTITE DANASNJI DATUM
	string _funkcija;//FUNKCIJA U KOJOJ JE NASTAO IZUZETAK
	int _linija;//LINIJA CODE-A U KOJOJ JE NASTAO IZUZETAK
public:
	Izuzetak(const char * poruka, Datum date, string funkcija, int linija):exception(poruka) {
		_datum = date;
		_funkcija = funkcija;
		_linija = linija;
	}
	friend ostream& operator<<(ostream& cout, Izuzetak & err) {
		cout << err.what() << ". U liniji " << err._linija << " u funkciji " << err._funkcija << " (" << err._datum << ")" << endl;
		return cout;
	}
};

template<class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int * _trenutno;
public:
	Kolekcija(const Kolekcija & k) {
		_trenutno = new int(*k._trenutno);
		for (size_t i = 0; i < *_trenutno; i++) {
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}
	Kolekcija &operator=( Kolekcija & k) {
		if (this != &k) {
			*_trenutno = *k._trenutno;
			for (int i = 0; i < *_trenutno; i++) {
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];
			}
		}
		return *this;
	}
	void AddElement(const T1 &el1, const T2 & el2) {
		if (*_trenutno == max)
			throw Izuzetak("Max broj elemenata dosegnut",Datum(1,1,1900), __FUNCTION__, __LINE__);

		_elementi1[*_trenutno] = el1;
		_elementi2[*_trenutno] = el2;
		(*_trenutno)++;
	}
	Kolekcija() {
		_trenutno = new int (0);
	}
	int GetMax() { return max; }
	T1 GetElement1(int i)const {
		if (i< 0 || i> *_trenutno)
			throw Izuzetak("Indeks nemoguc", Datum(1, 1, 1900), __FUNCTION__, __LINE__);
		return _elementi1[i];
	}
	T1 &operator[](int i) {
		if (i< 0 || i> *_trenutno)
			throw Izuzetak("Indeks nemoguc", Datum(1, 1, 1900), __FUNCTION__, __LINE__);
		return _elementi1[i];
	}
	T2 GetElement2(int i)const {
		if (i < 0 || i > *_trenutno)
			throw Izuzetak("Indeks nemoguc", Datum(1, 1, 1900),__FUNCTION__, __LINE__);
		return _elementi2[i];
	}
	int GetTrenutno()const { return *_trenutno; }
	int size()const { return *_trenutno; }
	friend ostream & operator<<(ostream& cout, const Kolekcija&k) {
		for (int i = 0; i < k.GetTrenutno(); i++){
			cout << k.GetElement1(i) << " - " << k.GetElement2(i) << endl;
		}
		return cout;
	}

};
enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
const char* enumKursevichar[]= { "HtmlCSSJavaScript", "SoftwareEngeneeringFundamentals", "MasteringSQL", "WindowsSecurity" };
class Kurs {
	enumKursevi _kurs;
	Datum _pocetak;
	Datum _kraj;
	char * _imePredavaca;
public:
	const char* imePredavaca() { return _imePredavaca; }
	enumKursevi GetKurs() { return _kurs; }
	Datum GetKraj()const { return _kraj; }
	Datum GetPocetak()const { return _pocetak; }
	friend bool operator==(const Kurs&k, const Kurs&k1) {
		if (k._pocetak == k1._pocetak && k._kraj == k1._kraj && k._kurs == k1._kurs)
			if (strcmp(k._imePredavaca, k1._imePredavaca) == 0)
				return true;
		return false;
	}
	Kurs() {
	}
	Kurs(const Kurs&k) {
		_kurs = k._kurs; _pocetak = k._pocetak; _kraj = k._kraj;
		_imePredavaca = new char[strlen(k._imePredavaca) + 1];
		strcpy_s(_imePredavaca, strlen(k._imePredavaca) + 1, k._imePredavaca);
	}
	Kurs & operator=(const Kurs&k) {
		if (this != &k) {
			_kurs = k._kurs; _pocetak = k._pocetak; _kraj = k._kraj;
			delete[] _imePredavaca;
			_imePredavaca = new char[strlen(k._imePredavaca) + 1];
			strcpy_s(_imePredavaca, strlen(k._imePredavaca) + 1, k._imePredavaca);
		}
		return *this;
	}
	Kurs(enumKursevi kurs, const char*ime, const Datum& poc, const Datum& kraj) :_kurs(kurs), _pocetak(poc), _kraj(kraj) {
		_imePredavaca = new char[strlen(ime) + 1];
		strcpy_s(_imePredavaca, strlen(ime) + 1, ime);
	}
	friend ostream& operator<<(ostream& cout, const Kurs & k) {
		cout << "Pocetak kursa " << k._pocetak << " kraj " << k._kraj << endl;
		cout << "Ime predavaca " << k._imePredavaca << endl;
		cout << "Kurs " << enumKursevichar[k._kurs] << endl;
		return cout;
	}
};
bool ProvjeriFormatTelefona(string broj) {
	string pravilo = "(\\()(0)(6)(\\d{1})(\\))(\\d{3})(\\-)(\\d{3})";
	if (regex_match(broj, regex(pravilo)))
		return true;
	return false;
}
class Polaznik {
	static int ID;
	const int _polaznikID;
	//SVAKOM NOVOM POLAZNIKU AUTOMATSKI DODIJELITI NOVI ID (AUTOINCREMENT) POCEVSI OD BROJA 1
	char * _imePrezime;
	string _kontaktTelefon; //BROJ TELEFONA TREBA BITI U FORMATU (06X)XXX-XXX 
	Kolekcija<Kurs *, int, 10> _uspjesnoOkoncaniKursevi;
	//INT PREDSTAVLJA OSTVARENI PROCENAT NA ISPITU, A JEDAN POLAZNIK MOZE POLOZITI NAJVISE 10 KURSEVA
public:
	Kolekcija<Kurs *, int, 10> getOkoncanjeKUrseve() { return _uspjesnoOkoncaniKursevi; }

	void AddKurs(const Kurs&k, int postotak) {
		_uspjesnoOkoncaniKursevi.AddElement(new Kurs(k), postotak);
	}
	int GetID() { return _polaznikID; }
	friend bool operator==(const Polaznik&p, const Polaznik & o) {
		return (p._kontaktTelefon == o._kontaktTelefon);
	}
	Polaznik(const char * imePrezime, string telefon) :_polaznikID(ID++) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_kontaktTelefon = telefon;
	}
	Polaznik(const Polaznik&p): _polaznikID(ID){
		int size = strlen(p._imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, p._imePrezime);
		_kontaktTelefon = p._kontaktTelefon;
		for (size_t i = 0; i < p._uspjesnoOkoncaniKursevi.GetTrenutno(); i++){
			_uspjesnoOkoncaniKursevi.AddElement(new Kurs(*p._uspjesnoOkoncaniKursevi.GetElement1(i)), p._uspjesnoOkoncaniKursevi.GetElement2(i));
		}
	}
	Polaznik& operator=(const Polaznik&p){
		if (this != &p) {
			delete[]  _imePrezime;
			int size = strlen(p._imePrezime) + 1;
			_imePrezime = new char[size];
			strcpy_s(_imePrezime, size, p._imePrezime);
			_kontaktTelefon = p._kontaktTelefon;
			for (size_t i = 0; i < p._uspjesnoOkoncaniKursevi.GetTrenutno(); i++) {
				_uspjesnoOkoncaniKursevi.AddElement(new Kurs(*p._uspjesnoOkoncaniKursevi.GetElement1(i)), p._uspjesnoOkoncaniKursevi.GetElement2(i));
			}
		}
		return *this;
	}
	~Polaznik() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (int i = 0; i < _uspjesnoOkoncaniKursevi.size(); i++)
		{
			delete _uspjesnoOkoncaniKursevi[i];
			_uspjesnoOkoncaniKursevi[i] = nullptr;
		}
	}
	friend ostream& operator<<(ostream&cout, const Polaznik&p) {
		cout << "Polaznik ID " << p._polaznikID << "  ---- IME PREZIME " << p._imePrezime << endl;
		cout << "Kontakt telefon " << p._kontaktTelefon << endl;
		cout << p._uspjesnoOkoncaniKursevi;
		return cout;
	}
}; 
int Polaznik::ID = 1;
class SkillsCentar {
	string _nazivCentra;
	vector<Kurs> _kursevi;//KURSEVI KOJE NUDI ODREDJENI CENTAR
	Kolekcija<Kurs, Polaznik *, 150> _aplikanti;
public:
	SkillsCentar &operator=(const SkillsCentar&s) {
		if (this != &s) {
			_nazivCentra = s._nazivCentra;
			_kursevi = s._kursevi;
			//_aplikanti = s._aplikanti;
		}return *this;
	}
	friend ostream& operator<<(ostream&cout, const SkillsCentar&p) {
		cout << "Naziv " << p._nazivCentra << endl;
		cout << "Kursevi ";
		for (size_t i = 0; i < p._kursevi.size(); i++){
			cout << p._kursevi[i];
		}
		cout << "Aplikanti ";
		cout << p._aplikanti;
		return cout;
	}
	bool RemoveKurs(Kurs&k) {
		for (size_t i = 0; i < _aplikanti.size(); i++) {
			if (_aplikanti.GetElement1(i).GetKurs() == k.GetKurs())
				return false;
		}

		for (size_t i = 0; i < _kursevi.size(); i++) {
			if (_kursevi[i].GetKurs() == k.GetKurs()) {
				_kursevi.erase(_kursevi.begin());
				i--;
			}
		}
		return true;
	}
	//VRACA INFORMACIJE O POLAZNICIMA KOJI SU KOD ODREDJENOG PREDAVACA USPJESNO OKONCALI ODREDJENI KURS
	vector<Polaznik> GetPolazniciByPredavac(const char* predavac, enumKursevi k) {
		vector<Polaznik> polaznici;
		for (size_t i = 0; i < _aplikanti.size(); i++) {
			for (size_t j = 0; j < _aplikanti.GetElement2(i)->getOkoncanjeKUrseve().GetTrenutno(); j++) {
				if (strcmp(_aplikanti.GetElement2(i)->getOkoncanjeKUrseve().GetElement1(j)->imePredavaca(), predavac) == 0)
					if (_aplikanti.GetElement2(i)->getOkoncanjeKUrseve().GetElement1(j)->GetKurs() == k)
						polaznici.push_back(*_aplikanti.GetElement2(i));
			}
		}
		return polaznici;
	}
	
	

	void DodajUspjesnoOkoncanKurs(int IDpolaznika, Kurs&k, int postotak) {
		//FUNKCIJA JE ZADUZENA DA POLAZNIKU DODA INFORMACIJU O USPJESNO POLOZENOM KURSU 
		//KOJI JE POSLAN KAO PARAMETAR. PREDUSLOV ZA DODAVANJE JE DA JE POLAZNIK PRETHODNO
		//APLICIRAO ZA TAJ KURS, TE DA JE NA ISPITU OSTVARIO VISE OD 55%

		for (size_t i = 0; i < _aplikanti.GetTrenutno(); i++) {
			if (_aplikanti.GetElement2(i)->GetID() == IDpolaznika) {
				if (postotak > 55) {
					if (_aplikanti.GetElement1(i).GetKurs() == k.GetKurs())
						_aplikanti.GetElement2(i)->AddKurs(k, postotak);
				}
			}
		}
	}
	void AddAplikaciju(const Kurs& k, const Polaznik& p) {
		bool imalikursa = false;
		for (size_t i = 0; i < _kursevi.size(); i++)
			if (_kursevi[i] == k)
				imalikursa = true;
		
		if(!imalikursa)
			throw Izuzetak("Kurs ne postoji.", Datum(1, 1, 1900), __FUNCTION__, __LINE__);

		for (size_t i = 0; i < _aplikanti.size(); i++){
			if (*_aplikanti.GetElement2(i) == p && _aplikanti.GetElement1(i) == k)
				throw Izuzetak("Kurs vec dodan za ovog polaznika.", Datum(1, 1, 1900), __FUNCTION__, __LINE__);
		}

		_aplikanti.AddElement(k, new Polaznik(p));
	}
	bool ProvjeriKoliziju(const Kurs& k) {
		for (size_t i = 0; i < _kursevi.size(); i++){
			if (_kursevi[i].GetKraj().sumaDana() - k.GetPocetak().sumaDana() < 0)
				return false;
		}
		return true;
	}
		//JEDAN KURS NE SMIJE POCETI DOK DRUGI TRAJE TJ. VRIJEME ODRZAVANJA KURSA SE NE SMIJE POKLAPATI
	void AddKurs(const Kurs& k) {
		for (size_t i = 0; i < _kursevi.size(); i++){
			if (_kursevi[i] == k)
				throw Izuzetak("Kurs vec dodan.", Datum(1, 1, 1900),  __FUNCTION__, __LINE__);
		}
		_kursevi.push_back(k);
	}
	SkillsCentar(string naziv) { _nazivCentra = naziv; }
	SkillsCentar(const SkillsCentar & obj) :_kursevi(obj._kursevi), _aplikanti(obj._aplikanti) {
		_nazivCentra = obj._nazivCentra;
	}
};
const char *crt = "\n---------------------------------------\n";
void main()
{
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
	****************************************************************************/

#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region TestiranjeKolekcije
	Kolekcija<int, int, 10> kolekcija1;
	try
	{
		for (size_t i = 0; i < kolekcija1.GetMax(); i++)
			kolekcija1.AddElement(i, i + 2);
		//FUNKCIJA TREBA BACITI IZUZETAK U SLUCAJ DA NEMA VISE PROSTORA ZA DODAVANJE NOVIH ELEMENATA
		cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
		//cout << kolekcija1.GetElement1(20) << " " << kolekcija1.GetElement2(20) << endl;
		//FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO ELEMENT NA ZAHTIJEVANOJ LOKACIJI NE POSTOJI
	}
	catch (Izuzetak & err)
	{
		cout << "Greska -> " << err << endl;
	}

	cout << kolekcija1 << endl;
	Kolekcija<int, int, 10> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;
	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;
	Kolekcija<int, int, 10> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
#pragma endregion
#pragma region TestiranjeKursa
	Kurs sef(SoftwareEngeneeringFundamentals, "Jasmin Azemovic", Datum(28, 1, 2016), Datum(15, 2, 2016));
	cout << sef << endl;
	Kurs msql(MasteringSQL, "Adel Handzic", Datum(28, 2, 2016), Datum(15, 3, 2016));
	//sef = msql;
	//if (sef == msql)//KURSEVI SU ISTI AKO POSJEDUJU IDENTICNE VRIJEDNOSTI SVIH ATRIBUTA
	//	cout << "ISTI SU KURSEVI" << endl;
	Kurs msql2(msql);
	//cout << msql2 << endl;
#pragma endregion
#pragma region TestiranjePolaznika
	string telefon1 = "(061)111-111";
	string telefon2 = "(061)111222";
	string telefon3 = "(061)111-333";

	if (!ProvjeriFormatTelefona(telefon1))
		cout << "Broj " << telefon1 << " nije validan" << endl;
	if (!ProvjeriFormatTelefona(telefon2))
		cout << "Broj " << telefon2 << " nije validan" << endl;
	if (!ProvjeriFormatTelefona(telefon3))
		cout << "Broj " << telefon3 << " nije validan" << endl;


	Polaznik denis("Denis Music", telefon1);
	Polaznik denis2("Denis2 Music2", telefon3);

#pragma endregion

#pragma region TestiranjeCentra
	try {
		SkillsCentar mostar("Skills Center Mostar");
		if (mostar.ProvjeriKoliziju(sef))
			//JEDAN KURS NE SMIJE POCETI DOK DRUGI TRAJE TJ. VRIJEME ODRZAVANJA KURSA SE NE SMIJE POKLAPATI
			mostar.AddKurs(sef);
		if (mostar.ProvjeriKoliziju(msql))
			//mostar.AddKurs(msql);
		if (mostar.ProvjeriKoliziju(msql2))
			//mostar.AddKurs(msql2);
//		//ONEMOGUCITI DODAVANJE IDENTICNIH KURSEVA. 
		//FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO SE POKUSA DODATI IDENTICAN KURS
		mostar.AddAplikaciju(sef, denis);
		//ONEMOGUCITI APLICIRANJE ZA KURSEVE KOJI NISU REGISTROVANI U CENTRU
		//mostar.AddAplikaciju(msql, denis);
		//mostar.AddAplikaciju(msql, denis);
		//ONEMOGUCITI APLICIRANJE ZA ISTI KURS. FUNKCIJA BAZA IZUZETAK
		cout << denis << endl;
		denis2 = denis;
		if (denis == denis2)
			//POLAZNICI SU ISTI AKO POSJEDUJU ISTO IME I AKO SU POHADJALI ISTE KURSEVE
			cout << "ISTI SU" << endl;

		mostar.DodajUspjesnoOkoncanKurs(1, sef, 60);//BROJ 1 OZNACAVA ID POLAZNIKA.
		//FUNKCIJA JE ZADUZENA DA POLAZNIKU DODA INFORMACIJU O USPJESNO POLOZENOM KURSU 
		//KOJI JE POSLAN KAO PARAMETAR. PREDUSLOV ZA DODAVANJE JE DA JE POLAZNIK PRETHODNO
		//APLICIRAO ZA TAJ KURS, TE DA JE NA ISPITU OSTVARIO VISE OD 55%
		mostar.DodajUspjesnoOkoncanKurs(1, msql, 83);
		vector<Polaznik> listaPolaznika = mostar.GetPolazniciByPredavac("Jasmin Azemovic", SoftwareEngeneeringFundamentals);//VRACA INFORMACIJE O POLAZNICIMA KOJI SU KOD ODREDJENOG PREDAVACA USPJESNO OKONCALI ODREDJENI KURS
		for (size_t i = 0; i < listaPolaznika.size(); i++)
		{
			cout << listaPolaznika[i];
		}
		if (mostar.RemoveKurs(sef))//ONEMOGUCITI UKLANJANJE KURSA KOJI SU VEC PRIJAVILI NEKI OD POLAZNIKA
			cout << "Kurs " << sef << " uspjesno uklonjen iz centra " << mostar << endl;

		SkillsCentar sarajevo = mostar;
		cout << sarajevo << endl;
	}
	catch (Izuzetak & err) {
		cout << err << endl;
	}
#pragma endregion
	system("pause");
}