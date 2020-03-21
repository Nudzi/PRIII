//statik pripada samo toj klasi dok global svima
//on je samo najavljen ali nije incijaliziran kada se otvore zagrade
//nego ih incijalizierati na globalnom nivou, samo sada nekome pripada
//int Uposlenik::maxsifra=0;
//cout << Uposlenik::maxsifra << endl; kao public
//static int getsifra()const {return maxsifra;}  (samo moze vratiti, ne mijenjati static i slati statik)	kao private
//static const char *nazivKompanije vrijedi samo za jednu kompaniju --> const char *Uposlenik::nazivKompanije
//ako saljemo po vrijednosti poziva se kons. kopije tako da se drugi ne moze pozvati

#include<iostream>
using namespace std;

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

class Datum
{
	int _dan;
	int _mjesec;
	int _godina;
public:
	Datum() {
		_dan = 1; _mjesec = 1; _godina = 1900;
	}
	Datum(int d, int m, int g) : _dan(d), _mjesec(m), _godina(g) {}
	void Info() { cout << _dan << "." << _mjesec << "." << _godina << endl; }
	//suma datuma
	int SumauDanima()const { return _dan + _mjesec * 30 + _godina * 365; }
	void KopirajDatum(const Datum& d) {
		_dan = d._dan;
		_mjesec = d._mjesec;
		_godina = d._godina;
	}
	bool ManjiPocetakOd(const Datum& d) {
		if (d.SumauDanima() < SumauDanima())
			return true;
		return false;
	}
	bool VeciKrajOd(const Datum& d) {
		if (d.SumauDanima() > SumauDanima())
			return true;
		return false;
	}
	bool UOdredjenomPeriodu(const Datum&poc, const Datum & kraj) {
		if (ManjiPocetakOd(poc) && VeciKrajOd(kraj))
			return true;
		return false;
	}
};

enum TipNaplate { PoOsobi, Jednokratno, PoNocenju };
class Usluga
{
	char* _naziv;
	double _cijena;
	TipNaplate _tipNaplate;
public:
	//funkcija promijenja cijena za PDV
	void PromijeniCijenuSaPDVom() { _cijena -= _cijena * 0.17; }
	//funkcija kopiraj
	void KopirajUslugu(const Usluga& usluga) {
		_naziv = AlocirajNizKaraktera(usluga._naziv);
		_cijena = usluga._cijena;
		_tipNaplate = usluga._tipNaplate;
	}
	//konst. kopije
	/*Usluga(const Usluga& usluga) {
		_naziv = AlocirajNizKaraktera(usluga._naziv);
		_cijena = usluga._cijena;
		_tipNaplate = usluga._tipNaplate;
	}*/
	Usluga() : _naziv(nullptr), _cijena(0), _tipNaplate(TipNaplate(0)) {}
	Usluga(const char* naziv, int cijena, TipNaplate tipNaplate) :
		_cijena(cijena), _tipNaplate(tipNaplate) {
		_naziv = AlocirajNizKaraktera(naziv);
	}

	~Usluga() {
		delete[] _naziv;
		_naziv = nullptr;
	}

	void Info() {
		cout << "Naziv usluge: " << _naziv << endl;
		cout << "Cijena: " << _cijena << endl;
	}
};

enum TipSmjestaja { StudioApartman, SingleRoom, DoubleRoom, SuperiorApartman };
const char* TipSmjestajaString[] = { "StudioApartman", "SingleRoom", "DoubleRoom", "SuperiorApartman" };

class Smjestaj
{
	int _broj; //Broj sobe, apartmana
	TipSmjestaja* _tipSmjestaja;
	int _maxBrojOsoba;
	double _cijenaPoOsobi;
public:
	TipSmjestaja  GetTipSmjestaja()const { return *_tipSmjestaja; }
	//Get Max broj osoba 
	int GetMAxBrojOsoba()const { return _maxBrojOsoba; }
	//getCijenu
	double GetCijenuPoOsobi()const { return _cijenaPoOsobi; }
	//get broj
	int GetBroj()const { return _broj; }
	//deafaultni konstruktor
	Smjestaj() {
		_broj = 1;
		_tipSmjestaja = nullptr;
		_maxBrojOsoba = 0;
		_cijenaPoOsobi = 0;
	}
	//konstruktor kopije
	Smjestaj(const Smjestaj& smjestaj) {
		_broj = smjestaj._broj;
		_maxBrojOsoba = smjestaj._maxBrojOsoba;
		_cijenaPoOsobi = smjestaj._cijenaPoOsobi;
		_tipSmjestaja = new TipSmjestaja(*smjestaj._tipSmjestaja);
	}
	Smjestaj(int broj, TipSmjestaja tip, int maxBrojOsoba, double cijenaPoOsobi) :
		_broj(broj), _tipSmjestaja(new TipSmjestaja(tip)), _maxBrojOsoba(maxBrojOsoba), _cijenaPoOsobi(cijenaPoOsobi)
	{
	}
	//kopiraj funkcija
	void KopirajSveParametre(const Smjestaj& smjestaj) {
		_broj = smjestaj._broj;
		_maxBrojOsoba = smjestaj._maxBrojOsoba;
		_cijenaPoOsobi = smjestaj._cijenaPoOsobi;
		delete _tipSmjestaja;
		_tipSmjestaja = new TipSmjestaja(*smjestaj._tipSmjestaja);
	}

	~Smjestaj() {
		delete _tipSmjestaja;
		_tipSmjestaja = nullptr;
	}

	void Info() const {
		cout << "Broj sobe: " << _broj << endl;
		cout << "Tip smjestaja: " << TipSmjestajaString[*_tipSmjestaja] << endl;
		cout << "Kapacitet smjestaja: " << _maxBrojOsoba << endl;
		cout << "Cijena po osobi: " << _cijenaPoOsobi << endl;

	}

};

class Gost
{
	char* _imePrezime;
	char* _kontakTelefon;
public:
	const char *GetIme() { return _imePrezime; }
	void KopirajGosta(const Gost&gost) {
		delete[] _imePrezime;
		_imePrezime = AlocirajNizKaraktera(gost._imePrezime);
		delete[] _kontakTelefon;
		_kontakTelefon = AlocirajNizKaraktera(gost._kontakTelefon);
	}
	Gost() {
		_imePrezime = nullptr;
		_kontakTelefon = nullptr;
	}
	Gost(const Gost&gost) {
		_imePrezime = AlocirajNizKaraktera(gost._imePrezime);
		_kontakTelefon = AlocirajNizKaraktera(gost._kontakTelefon);
	}
	Gost(const char* imePrezime, const char* kontaktTelefon) {
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_kontakTelefon = AlocirajNizKaraktera(kontaktTelefon);
	}

	void Info() {
		cout << "Ime i prezime: " << _imePrezime << endl;
		cout << "Kontakt telefon: " << _kontakTelefon << endl;
	}
	~Gost() {
		delete[] _imePrezime; _imePrezime = nullptr;
		delete[] _kontakTelefon; _kontakTelefon = nullptr;
	}
};

enum StatusRezervacije { Aktivna, Potvrdjena, BoravakUToku, Otkazana };
class Rezervacija
{
	Datum _pocetakBoravka;
	Datum _krajBoravka;
	Gost* _gost; //Gost koji je rezervisao smještaj
	Smjestaj _smjestaj;
	int _brojOsoba; //Ukupan broj osoba koji boravi u smjestaju
	Usluga* _usluge; //Dodatne usluge koje je gost koristio tokom boravka
	int _brojUsluga;
	double _ukupanIznos;
	StatusRezervacije _status;
public:
	Gost GetGosta()const { return *_gost; }
	double GetUkupanIznos()const { return _ukupanIznos; }
	//ukupan iznos sa PDV-om
	void SetIznos() { _ukupanIznos -= _ukupanIznos*0.17; }
	Usluga * GetUsluga()const { return _usluge; }
	int GetBrojUsluga()const { return _brojUsluga; }
	//promijeni status
	void PromijeniStatus(StatusRezervacije status) { _status = status; }
	
	//kopirajRezervaciju
	void KopirajRezervaciju(const Rezervacija&rezervacija) {
		_pocetakBoravka.KopirajDatum(rezervacija.GetPocetak());
		_krajBoravka.KopirajDatum(rezervacija.GetKraj());
		//delete _gost;
		_gost = new Gost(*rezervacija._gost);
		_smjestaj.KopirajSveParametre(rezervacija._smjestaj);
		_brojOsoba = rezervacija._brojOsoba;
		_brojUsluga = rezervacija._brojUsluga;
		_usluge = new Usluga[_brojUsluga];
		for (size_t i = 0; i < _brojUsluga; i++)
			_usluge[i].KopirajUslugu(rezervacija._usluge[i]);

		_ukupanIznos = rezervacija._ukupanIznos;
		_status = rezervacija._status;
	}
	//deafaultni kons
	Rezervacija():_pocetakBoravka(), _krajBoravka(), _gost(new Gost()), _smjestaj(), _brojOsoba(0), _brojUsluga(0), _status(Aktivna){
		_ukupanIznos = _smjestaj.GetCijenuPoOsobi()*_brojOsoba;
	}
	//GetBrojOSoba
	int GetBrojOsoba()const { return _brojOsoba; }
	void Info() {
		_pocetakBoravka.Info(); cout << " do ";
		_krajBoravka.Info();
		_smjestaj.Info();
		cout << "Broj osoba " << _brojOsoba << " sa iznosom " << _ukupanIznos << endl;
		cout << _status << endl;

	}
	Datum GetPocetak() const { return _pocetakBoravka; }
	Datum GetKraj() const { return _krajBoravka; }

	StatusRezervacije GetStatus() const { return _status; }

	Rezervacija(const Datum& pocetak, const Datum& kraj, const Gost& gost, const Smjestaj& smjestaj, int brojOsoba)
		:_pocetakBoravka(pocetak), _krajBoravka(kraj), _gost(new Gost(gost)), _smjestaj(smjestaj),
		_brojOsoba(brojOsoba), _status(Aktivna){
		_brojUsluga = 0;
		_ukupanIznos = smjestaj.GetCijenuPoOsobi()*_brojOsoba;
	}
	/*Rezervacija(int brojOsoba, double ukupanIznos, int brojUsluga, Datum pocetak, Datum kraj,
	Gost gost, Smjestaj smjestaj, Usluga usluga, StatusRezervacije status = Aktivna)
	:_pocetakBoravka(pocetak), _krajBoravka(kraj), _gost(new Gost(gost)), _smjestaj(smjestaj),
	_usluge(new Usluga(usluga)), _status(status)
	{
	}*/
	Smjestaj GetSmjestaj() const { return _smjestaj; }
	void RezervisiUslugu(const Usluga& u) {
		Usluga* temp = new Usluga[_brojUsluga + 1];
		for (size_t i = 0; i < _brojUsluga; i++) {
			temp[i].KopirajUslugu(_usluge[i]);
		}

		delete[] _usluge;
		_usluge = temp;

		_usluge[_brojUsluga++].KopirajUslugu(u);
	}
	
};

class Hotel
{
	char* _naziv;
	//Ponuda smještaja
	Smjestaj* _smjestaji;
	int _brojSmjestaja;
	//Rezervacije hotela (protekle, aktivne i nadolazeæe)
	Rezervacija* _rezervacije;
	int _brojRezervacija;
	static int _stopaPDV;
public:
	
	Gost  PronadjiVIPGosta() {
		double temp = 0;
		int indeks = -1;
		for (size_t i = 0; i < _brojRezervacija; i++){
			if (_rezervacije[i].GetUkupanIznos() > temp) {
				temp = _rezervacije[i].GetUkupanIznos();
				indeks = i;
			}
		}
		return _rezervacije[indeks].GetGosta();
	}
	/*Vrši ispis raèuna sa svim stavkama rezervacije (posebno stavka za smještaj, a posebno za sve usluge sa
	pripadajuæim cijenama, te ukupnim iznosom raèuna na kraju).
	Parametar tipa bool govori da li je potrebno obraèunati
	PDV po utvrðenoj stopi na ukupan iznos raèuna te na sve stavke pojedinaèno.*/
	void CheckOut(const Rezervacija&rezervacija, bool daLiObracunatiPDV) {
		if (daLiObracunatiPDV) {
			cout << "-----------------Racun sa PDV-om-----------------" << endl;
			for (size_t i = 0; i < _brojRezervacija; i++) {
				for (size_t j = 0; j < _rezervacije[i].GetBrojUsluga(); j++) {
					cout << "USLUGA BROJ " << j + 1 << endl;
					_rezervacije[i].GetUsluga()[j].PromijeniCijenuSaPDVom();
					_rezervacije[i].GetUsluga()[j].Info();
				}
				_rezervacije[i].SetIznos();
				cout << "Ukupan iznos sa PDV-om " << _rezervacije[i].GetUkupanIznos() << endl;
			}
		}
		else {
			cout << "-----------------Racun bez PDV-om-----------------" << endl;
			for (size_t i = 0; i < _brojRezervacija; i++) {
				for (size_t j = 0; j < _rezervacije[i].GetBrojUsluga(); j++) {
					cout << "USLUGA BROJ " << j + 1 << endl;
					_rezervacije[i].GetUsluga()[j].Info();
				}
				cout << "Ukupan iznos sa PDV-om " << _rezervacije[i].GetUkupanIznos() << endl;
			}
		}
	}
	void Info() {
		for (size_t i = 0; i < _brojRezervacija; i++)
			for (size_t j = 0; j < _rezervacije[i].GetBrojUsluga(); j++)
				_rezervacije[i].GetUsluga()[j].Info();
	}

	Hotel(const char* naziv) : _smjestaji(nullptr), _brojSmjestaja(0),
		_rezervacije(nullptr), _brojRezervacija(0) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	void CheckIn(const Rezervacija&rezervacija) {
		int indeks = daLiPostojiRezervacija(rezervacija, Potvrdjena);
		if (indeks == -1)
			return;
		_rezervacije[indeks].PromijeniStatus(BoravakUToku);
	}
	//provejra da li postoji neka rezervacija
	int daLiPostojiRezervacija(const Rezervacija&rezervacija, StatusRezervacije potvrdjena = Aktivna) {
		int indeks = -1;
		if (potvrdjena != Potvrdjena) {
			for (size_t i = 0; i < _brojRezervacija; i++)
				if (_rezervacije[i].GetSmjestaj().GetBroj() == rezervacija.GetSmjestaj().GetBroj()) {
					indeks = i;
					return indeks;
				}
		}
		else {
			for (size_t i = 0; i < _brojRezervacija; i++)
				if (_rezervacije[i].GetSmjestaj().GetBroj() == rezervacija.GetSmjestaj().GetBroj())
					if (_rezervacije[i].GetStatus() == Potvrdjena) {
						indeks = i;
						return indeks;
					}
		}
	}

	//prije nego se prijave rezervacija mora da bude potvrdena
	void PotvrdiRezervaciju(const Rezervacija&rezervacija) {
		int indeks = daLiPostojiRezervacija(rezervacija);
		_rezervacije[indeks].PromijeniStatus(Potvrdjena);
	}
	//provjera perioda dodavanja reyervacije
	bool ProvjeraPeriodaRezervacije(const Rezervacija&rezervacija) {
		int provjera = -1;
		for (size_t i = 0; i < _brojRezervacija; i++) {
			if (_rezervacije[i].GetPocetak().ManjiPocetakOd(rezervacija.GetPocetak()) &&
				_rezervacije[i].GetKraj().VeciKrajOd(rezervacija.GetKraj()))
				provjera = i;
		}
		if (provjera == -1)
			return false;
		return true;
	}
	bool ProvjeraKapacitetaRezervacije(const Rezervacija&rezervacija) {
		for (size_t i = 0; i < _brojRezervacija; i++) {
			if (_rezervacije[i].GetSmjestaj().GetMAxBrojOsoba() < rezervacija.GetBrojOsoba())
				return true;
			return false;
		}
	}
	//Pronalazi i vraæa podatke o najtraženijem smještaju hotela (onaj koji je najviše puta rezervisan u
	//datom periodu)
	//brojac SMejstaja
	int BrojacSmjestaja(int indeks) {
		int brojac = 0;
		for (size_t i = 0; i < _brojSmjestaja; i++)
			if (indeks == _smjestaji[i].GetTipSmjestaja())
				brojac++;
		return brojac;
	}
	Smjestaj GetNajTrazenijiSmjestaj(const Datum& pocetak, const Datum& kraj ) {
		int indeks = 0, niz[4]{ 0 };
		while (indeks!=4){
			niz[indeks] = BrojacSmjestaja(indeks);
			indeks++;
		}
		int max = 0;
		for (size_t i = 0; i < _brojSmjestaja; i++){
			if (max < niz[i])
				max = i;
		}
		return _smjestaji[max];
	}

	//Funkcija kojom se dodaje nova rezervacija pod uslovom da smjestaj nije zauzet u trazenom periodu 
	//i da kapacitet smještaja nije manji od broja osoba. Funkcija treba da ispiše sve uslove koje rezervacija
	//ne ispunjava.
	bool DodajRezervaciju(const Rezervacija& rezervacija) {
		if (_brojRezervacija != 0) {
			if (!(ProvjeraPeriodaRezervacije(rezervacija))) {
				cout << "Ne postoji slobodan smjestaj u ovom periodu..." << endl;
				for (size_t i = 0; i < _brojRezervacija; i++) {
					cout << "Pocetak -> ";
					_rezervacije[i].GetPocetak().Info();
					cout << " kraj -> ";
					_rezervacije[i].GetKraj().Info();
				}
				return false;
			}

			if (!(ProvjeraKapacitetaRezervacije(rezervacija))) {
				for (size_t i = 0; i < _brojRezervacija; i++)
					cout << _rezervacije[i].GetSmjestaj().GetMAxBrojOsoba() << " <- (MAX BROJ) || " << rezervacija.GetBrojOsoba() << " <- trazeni broj." << endl;

				return false;
			}

		}
		Rezervacija *temp = new Rezervacija[_brojRezervacija+1];
		for (size_t i = 0; i < _brojRezervacija; i++){
			temp[i].KopirajRezervaciju(_rezervacije[i]);
		}
		rezervacija.GetUsluga()->Info();
		temp[_brojRezervacija].KopirajRezervaciju(rezervacija);
		delete[] _rezervacije;
		_rezervacije = temp;
		_brojRezervacija++;
		return true;
	}

	//funkcija dodaje smjestaj
	bool DodajSmjestaj(const Smjestaj& smjestaj) {
		for (size_t i = 0; i < _brojSmjestaja; i++)
			if (_smjestaji[i].GetBroj() == smjestaj.GetBroj())
				return false;

		Smjestaj *temp = new Smjestaj[_brojSmjestaja + 1];
		for (size_t i = 0; i < _brojSmjestaja; i++) {
			temp[i].KopirajSveParametre(_smjestaji[i]);
		}
		temp[_brojSmjestaja].KopirajSveParametre(smjestaj);
		delete[] _smjestaji;
		_smjestaji = temp;
		_brojSmjestaja++;
		return true;
	}
	//funkcija trazi smjestaj u odredjenom datumu
	void IspisiSlobodneSmjestaje(Datum d1, Datum d2) {
		if (_brojRezervacija == 0) {
			for (size_t i = 0; i < _brojSmjestaja; i++)
				_smjestaji[i].Info();
		}
		return;
		for (size_t j = 0; j < _brojSmjestaja; j++)
			for (size_t i = 0; i < _brojRezervacija; i++)
				if (_rezervacije[i].GetSmjestaj().GetBroj() == _smjestaji[j].GetBroj())
					//if (_rezervacije[i].GetStatus() == Aktivna)
					if (_rezervacije[i].GetPocetak().ManjiPocetakOd(d1) &&
						(_rezervacije[i].GetKraj().VeciKrajOd(d2)))
						_rezervacije[i].Info();
	}

};
int Hotel::_stopaPDV = 0.17;
int main()
{

#pragma region Hotel

	const int brojUsluga(5);
	Hotel hotel1("Hotel Mostar");

	Usluga* usluge[brojUsluga];
	usluge[0] = new Usluga("Parking", 5, TipNaplate(1));
	usluge[1] = new Usluga("Dorucak", 10, TipNaplate(0));
	usluge[2] = new Usluga("Rucak", 15, TipNaplate(0));
	usluge[3] = new Usluga("Wellness & SPA", 40, TipNaplate(2));
	usluge[4] = new Usluga("Veseraj", 10, TipNaplate(2));

	Smjestaj apartman(1, TipSmjestaja(0), 5, 30);
	Smjestaj soba1(2, TipSmjestaja(1), 2, 20);
	Smjestaj soba2(3, TipSmjestaja(2), 4, 20);
	Smjestaj soba3(4, TipSmjestaja(0), 2, 15);

	//Funkciju za dodavanje smjestajnih kapaciteta hotela. 
	//Hotel ne moze imati više smještaja pod istim brojem.
	hotel1.DodajSmjestaj(apartman);
	hotel1.DodajSmjestaj(soba1);
	hotel1.DodajSmjestaj(soba2);
	hotel1.DodajSmjestaj(soba3); //Neæe se dodati radi zauzetosti broja smještaja

								 //Funkcija koja vrši ispis podataka o svim slobodnim smještajima hotela 
								 //u zadatom periodu.
	hotel1.IspisiSlobodneSmjestaje(Datum(24, 10, 2018), Datum(1, 11, 2018));

	//Ispis dodatnih usluga
	for (size_t i = 0; i < brojUsluga; i++)
		usluge[i]->Info();


#pragma endregion

#pragma region RezervacijaGost1

	Gost gost1("Marija Periæ", "033/222-555");
	Rezervacija rezervacija1(Datum(28, 10, 2018), Datum(2, 11, 2018), gost1, apartman, 3);
	rezervacija1.RezervisiUslugu(*usluge[0]); //Parking
	rezervacija1.RezervisiUslugu(*usluge[1]); //Doruèak za 3 osobe

	 //Funkcija kojom se dodaje nova rezervacija pod uslovom da smjestaj nije zauzet u trazenom periodu 
	 //i da kapacitet smještaja nije manji od broja osoba. Funkcija treba da ispiše sve uslove koje rezervacija
	 //ne ispunjava.
	 if (hotel1.DodajRezervaciju(rezervacija1))
	 	cout << "Uspjesno ste rezervisali smjestaj." << endl;
	 else
	 	cout << "Rezervacija smjestaja nije uspjela." << endl;
	 
	 hotel1.Info();
	 cout << endl;
	hotel1.PotvrdiRezervaciju(rezervacija1); //Prije nego se gosti prijave u hotel rezervacija mora biti potvrðena.
	hotel1.CheckIn(rezervacija1); //Mijenja status rezervacije na BoravakUToku, 
								  //		pod uslovom da je ista prethodno potvrðena.
	 
	
	 
	 	/*Vrši ispis raèuna sa svim stavkama rezervacije (posebno stavka za smještaj, a posebno za sve usluge sa
	 		pripadajuæim cijenama, te ukupnim iznosom raèuna na kraju). 
			Parametar tipa bool govori da li je potrebno obraèunati
	 		PDV po utvrðenoj stopi na ukupan iznos raèuna te na sve stavke pojedinaèno.*/
	 	hotel1.CheckOut(rezervacija1, true);
	 
	 #pragma endregion
	 
	 #pragma region RezervacijaGost2
	 	Gost gost2("Adijan Mariæ", "066/111-333");
	 	Rezervacija rezervacija2(Datum(29, 10, 2018), Datum(5, 11, 2018), gost2, apartman, 6);
	 
	 	if (hotel1.DodajRezervaciju(rezervacija2))
	 		cout << "Uspjesno ste rezervisali smjestaj." << endl;
	 	else
	 		cout << "Rezervacija smjestaja nije uspjela." << endl;
	 
	 #pragma endregion
	 
	    //Napomena: Za potrebe testiranje funkcija u nastavku dopuniti testni program.
	 
	 	//Pronalazi i vraæa podatke o najtraženijem smještaju hotela (onaj koji je najviše puta rezervisan u
		//datom periodu)
	 	hotel1.GetNajTrazenijiSmjestaj(Datum(1, 1, 2018), Datum(24, 1, 2018)).Info();
	 
	 	//Pronalazi i ispisuje podate o VIP gostu 
		//(onaj koji je u sumi utrošio najviše novca u hotelu u trenutnoj godini - 2018).
	 	hotel1.PronadjiVIPGosta().Info();
	 
	 	for (size_t i = 0; i < brojUsluga; i++)
	 	{
	 		delete usluge[i];
	 	}

	system("pause");

	return 0;
}