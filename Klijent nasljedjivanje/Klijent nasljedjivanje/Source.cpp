//incijalnio kada postavimo

#include<iostream>
using namespace std;
#pragma warning (disable:4996)
template<class T1, class T2>
class List {
	T1* _elementi1;
	T2* _elementi2;
	const int _max;
	int _trenutno;
public:
	List(int max = 0) : _max(max) {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}

	List(const List& list) : _max(list._max) {
		_elementi1 = new T1[list._trenutno];
		_elementi2 = new T2[list._trenutno];

		for (int i = 0; i < list._trenutno; i++) {
			_elementi1[i] = list._elementi1[i];
			_elementi2[i] = list._elementi2[i];
		}

		_trenutno = list._trenutno;
	}
	~List() {
		delete[] _elementi1;
		_elementi1 = nullptr;

		delete[] _elementi2;
		_elementi2 = nullptr;
	}
	bool Add(const T1& el1, const T2& el2) {
		if (_trenutno == _max)
			return false;

		for (int i = 0; i < _trenutno; i++)
		{
			if (_elementi1[i] == el1 && _elementi2[i] == el2)
				return false;
		}

		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];


		for (int i = 0; i < _trenutno; i++) {
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;
		_trenutno++;
	}
	bool Remove(int index) {
		if (index < 0 || index > _trenutno)
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
	T1& GetElement1(int index) {
		if (index >= 0 && index < _trenutno)
			return _elementi1[index];
	}
	T2& GetElement2(int index) {
		if (index >= 0 && index < _trenutno)
			return _elementi2[index];
	}
	int GetTrenutno()const { return _trenutno; }
	friend ostream& operator<<(ostream&, const List&);
};
template<class T1, class T2>
ostream& operator<<(ostream& cout, const List<T1, T2>& list) {
	for (int i = 0; i < list._trenutno; i++) {
		cout << list._elementi1[i] << "\t" << list._elementi2[i] << endl;
	}

	return cout;
}
char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

class Pitanje
{
protected:
	int _id;
	char* _oblast;
	char* _tekst;
	double _bodovi;
public:
	double GetBodovi()const { return _bodovi; }
	Pitanje(const Pitanje&p) {
		_id = p._id;
		_bodovi = p._bodovi;
		_tekst = AlocirajNizKaraktera(p._tekst);
		_oblast = AlocirajNizKaraktera(p._oblast);
	}
	Pitanje() {
		_id = 0;
		_oblast = nullptr;
		_tekst = nullptr;
		_bodovi = 0;
	}
	Pitanje(int id, const char* tekst, const char* oblast, double bodovi) :
		_id(id), _bodovi(bodovi)
	{
		_tekst = AlocirajNizKaraktera(tekst);
		_oblast = AlocirajNizKaraktera(oblast);
	}
	virtual ~Pitanje() = 0 {
		delete[] _tekst;
		_tekst = nullptr;

		delete[] _oblast;
		_oblast = nullptr;
	}
	virtual void Info() {
		cout << "Pitanje: " << _tekst << endl;
	}
	virtual void OdgPitanje() = 0 { cout << _tekst << endl; }
};

//Pitanje na kojem studenti upisuju odgovore.
//Pohranjuje se taèan odgovor od strane edukatora.
class EsejskoPitanje : public Pitanje {
	char* _tacanOdgovor;
	char* _odgovorStudenta;
public:
	void setOdgStudenta(const char *temp) { _odgovorStudenta = AlocirajNizKaraktera(temp);}

	virtual void OdgPitanje() {
		Pitanje::OdgPitanje();
		char temp[280];
		cin.getline(temp, 280);
		setOdgStudenta(temp);
	}
	//Konstruktor i destruktor
	EsejskoPitanje(const char *tacanodgovor, const char* odgovorStudenta) :Pitanje() {
		_tacanOdgovor = AlocirajNizKaraktera(tacanodgovor);
		_odgovorStudenta = nullptr;
	}
	EsejskoPitanje(const char *tacanodgovor, const char* odgovorStudenta, int id,
		double bodovi, const char*tekst, const char *oblast) :Pitanje(id, tekst, oblast, bodovi) {
		_tacanOdgovor = AlocirajNizKaraktera(tacanodgovor);
		_odgovorStudenta = nullptr;
	}
	~EsejskoPitanje() {
		delete[] _tacanOdgovor;
		_tacanOdgovor = nullptr;
		delete[] _odgovorStudenta;
		_odgovorStudenta = nullptr;
	}
	EsejskoPitanje(const EsejskoPitanje&p) :Pitanje(p) {
		_tacanOdgovor = AlocirajNizKaraktera(p._tacanOdgovor);
		_odgovorStudenta = AlocirajNizKaraktera(p._odgovorStudenta);
	}
	void Info() {
		cout << _odgovorStudenta << endl;
	}
	//Funkciju za izmjenu svih atributa klase
	void IzmjeniAtributeKlase(const char* tacanOdgovor, const char* odgovorStudenta) {
		delete[] _tacanOdgovor;
		_tacanOdgovor = AlocirajNizKaraktera(tacanOdgovor);
		delete[] _odgovorStudenta;
		_odgovorStudenta = AlocirajNizKaraktera(odgovorStudenta);
	}
};

//Pitanje koje može imati više taènih odgovora. Student bira ponudjene opcije. 
class MCMAPitanje : virtual public Pitanje
{
protected:
	List<char*, bool> _odgovori; //ponudjeni odgovori sa oznakama tacnih odgovora
	int* _oznaceniOdgovoriStudenta; //cuvaju se redni brojevi (npr. 1 - 3 - 5) ponudjenih odgovora 
									//koje je odabrao student
	int bodovi = 0;
	bool ProsiriOdgovoreStudenta(const int& odgovor){
		for (int i = 0; i < bodovi; i++)
			if (odgovor == _oznaceniOdgovoriStudenta[i])
				return false;
		int * t = new int[bodovi + 1];
		for (int i = 0; i < bodovi; i++)
			t[i] = _oznaceniOdgovoriStudenta[i];
		delete[] _oznaceniOdgovoriStudenta;
		t[int(bodovi++)] = odgovor;
		_oznaceniOdgovoriStudenta = t;
		return true;
	}
public:
	virtual void OdgPitanje() {
		Pitanje::OdgPitanje();
		int brojac = 0;
		do {
			int mojOdg;
			cin >> mojOdg;
			if (!ProsiriOdgovoreStudenta(mojOdg))
				cout << "Odgovor je vec dodan!" << endl;
		} while (_bodovi == brojac);
	}
	//Konstruktor i destruktor
	//Broj bodova inicijalno postaviti na 0 i uvecavati svakim oznacenim tacnim odgovorom.
	MCMAPitanje() :Pitanje() {
		_oznaceniOdgovoriStudenta = nullptr;
	}
	~MCMAPitanje() {
		for (size_t i = 0; i < _odgovori.GetTrenutno(); i++) {
			delete _odgovori.GetElement1(i);
		}
		delete _oznaceniOdgovoriStudenta;
	}
	MCMAPitanje(MCMAPitanje& p) :Pitanje(p) {
		for (size_t i = 0; i < _odgovori.GetTrenutno(); i++) {
			_odgovori.Add(p._odgovori.GetElement1(i), _odgovori.GetElement2(i));

		}
		_oznaceniOdgovoriStudenta = p._oznaceniOdgovoriStudenta;
	}
	//Funkciju za izmjenu podataka o pitanju
	void IzmjenaPitanja(const char* tekst) {//dopuniti do kraja kuci
		delete[] _tekst;
		_tekst = AlocirajNizKaraktera(tekst);
	}
	//Funkciju za dodavanje odgovora u listu koja treba da osigura da su najmanje 2 (od max) odgovora oznacena kao tacna.
	// Dok se ne dodaju svi predvidjeni odgovori pitanje ne treba biti upotrebljivo ukoliko prethodni uslov nije ispunjen
	//(ne prikazivati pitanja na ispisu).
	//Napomena: Podatke o odgovorima cuvati u zasebnoj memoriji i istu dealocirati unutar same klase
	void DodajOdgovor(const char* odgovor, bool istinitost) {
		char *temp = AlocirajNizKaraktera(odgovor);
		_odgovori.Add(temp, istinitost);//jer se salje onako smao jedan karakter, pa napravimo pomocnu varijablu
										//koja ima cijelu vrijednost objekta
		if (istinitost == true)
			_bodovi += 1;
	}
	//Funkciju za uklanjanje odgovora na osnovu rednog broja u listi. Ukoliko se uklanja odgovor koji je oznacen kao
	//tacan i pri tome u listi ostaje samo još jedan odgovor oznacen kao tacan, pitanje postaje neupotrebljivo dok 
	//se ne doda jos jedan tacan odgovor.
	void UkloniOdgovor(int redniBroj) {
		if (_odgovori.GetElement2(redniBroj - 1) == true)
			_bodovi -= 1;
		_odgovori.Remove(redniBroj - 1);
	}
	void Info() {
		if (_bodovi < 2)
			return;
		if (_bodovi > 1) {
			cout << _odgovori;
		}
	}
	//Funkciju za promjenu odgovora na odredjenom rednom broju
	void SetOdgovor(const char* odgovor, int redniBroj) {
		char * temp = new char[strlen(odgovor) + 1];
		delete _odgovori.GetElement1(redniBroj - 1);
		_odgovori.GetElement1(redniBroj - 1) = AlocirajNizKaraktera(temp);
	}
};

//Pitanje koje može imati jedan taèan odgovor. Student bira jednu od ponudjenih opcija.
class MCSAPitanje : virtual public MCMAPitanje
{
public:

	//Konstruktor
	MCSAPitanje(int brojBodova) :MCMAPitanje() { _bodovi = brojBodova; }
	//Broj bodova se postavlja na vrijednost proslijedjenog parametra.
	MCSAPitanje(MCSAPitanje &p):MCMAPitanje(p) {
	}
	//Preklopljene verzije funkcija za dodavanje i uklanjanje odgovora koje dozvoljavaju da samo jedan odgovor bude
	//oznacen kao tacan.
	//Svi prethodno navedeni uslovi trebaju biti ispunjeni, ali u ovom sluèaju sa iskljucivo jednim tacnim odgovorom.
	void DodajOdgovor(const char* odgovor, bool istinitost) {
		char *temp = AlocirajNizKaraktera(odgovor);
		_odgovori.Add(temp, istinitost);//jer se salje onako smao jedan karakter, pa napravimo pomocnu varijablu
										//koja ima cijelu vrijednost objekta
		if (_bodovi == 1) {
			cout << "Vec imamo tacan odgovor." << endl;
			return;
		}
		if (istinitost == true)
			_bodovi += 1;
	}
	void UkloniOdgovor(int redniBroj) {
		if (_odgovori.GetElement2(redniBroj - 1) == true)
			_bodovi -= 1;
		_odgovori.Remove(redniBroj - 1);
	}
	void Info() {
		if (_bodovi != 2)
			return;
		if (_bodovi == 1) {
			cout << _odgovori;
		}
	}
};

//Pojam je tip pitanja kod kojeg se definiše pojam zadat tekstom pitanja. Definicija/Opis pojma se tretira kao
//esejski dio pitanja, dok se karakteristike tog pojma postavljaju kroz MCMA tip pitanja.
class Pojam : virtual public EsejskoPitanje, virtual public MCMAPitanje
{
	bool _slozen; //pojam moze biti prost ili slozen
public:
	//Konstruktor
	//Jednostavno pitanje tipa Pojam nosi minimalno 3 boda, a ukoliko je rijec o slozenom pojmu minimalno 5 bodova.
	Pojam(bool slozen, const char *tekst, const char* odgStudenta, int id, const char*tekstPitanja, const char* oblast, double bodovi)
		:EsejskoPitanje(tekst, odgStudenta), MCMAPitanje(), Pitanje(id, tekstPitanja, oblast, bodovi) {
		_slozen = slozen;
	}
	//Dodati ostale potrebne funkcije za rad sa klasom
	Pojam(Pojam &p) :EsejskoPitanje(p), MCMAPitanje(p) {
		_slozen = p._slozen;
	}
};

class Test {
	char _brojIndeksa[10]; //broj indeksa studenta za kojeg je formiran test
	Pitanje* _pitanja[30]; // test studenta sadrzi najvise 30 pitanja razlicitog tipa
	double _maksimalnoBodova;
	double _osvojenoBodova;
	double _rezultat; //izrazen u procentima
	int _ocjena;
	int _brojac;
public:
	void Info() {
		cout << _brojIndeksa << endl;
		cout << "Osvojeno bodova: " << _osvojenoBodova << " od " << _maksimalnoBodova << endl;
		cout << "Rezultat: " << _osvojenoBodova / _maksimalnoBodova * 100 << "%." << endl;
		cout << "Ocjena: " << _ocjena << endl;
	}
	Test(const char* indeks) {
		for (int i = 0; i < 30; i++) {
			_pitanja[i] = nullptr;
		}
		strncpy(_brojIndeksa, indeks, _TRUNCATE);
		_rezultat = 0;
		_osvojenoBodova = 0;
		_maksimalnoBodova = 0;
		_ocjena = 0;
		_brojac = 0;
	}
	//Omoguciti formiranje testa sa razlicitim tipom pitanja, te dodati funkcije za unos, provjeru tacnih odgovora 
	//i ocjenu studenta.
	//Po potrebi dopuniti i prethodne klase novim atributima/funkcijama.
	void DodajPitanje(Pitanje &p) {
		if (_brojac == 30)
			return;
		Pojam* pok = dynamic_cast<Pojam*>(&p);
		if (pok != nullptr)
			_pitanja[_brojac++] = new Pojam(*pok);
		MCSAPitanje* pok = dynamic_cast<MCSAPitanje*>(&p);
		if (pok != nullptr)
			_pitanja[_brojac++] = new MCSAPitanje(*pok);
		MCMAPitanje* pok = dynamic_cast<MCMAPitanje*>(&p);
		if (pok != nullptr)
			_pitanja[_brojac++] = new MCMAPitanje(*pok);
		EsejskoPitanje* pok = dynamic_cast<EsejskoPitanje*>(&p);
		if (pok != nullptr)
			_pitanja[_brojac++] = new EsejskoPitanje(*pok);

	}
	void PokreniTest() {
		for (size_t i = 0; i < _brojac; i++) {
			_pitanja[i]->TekstPitanja();
		}
	}
};

int main()
{
	//Testirati sve implementirane funkcionalnosti programa. 
	//Kreirati najmanje jedan test sa 10 pitanja.

	system("pause");
	return 0;
}