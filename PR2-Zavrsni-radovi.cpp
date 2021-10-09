#include<iostream>
#include<string>
#include<vector>
#include<thread>
#include<exception>
#include<regex>
#include<mutex>
using namespace std;

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
****************************************************************************/
const char* crt = "\n-------------------------------------------\n";
const char* nedozvoljena_operacija = "Nedozvoljena operacija";
const char* not_set = "NOT_SET";
const int min_polgavlja = 3;
const int min_karaktera_po_poglavlju = 30;

char* AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	Kolekcija() :_trenutno(0), _elementi1(nullptr), _elementi2(nullptr) { }
	~Kolekcija() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi1 = nullptr;
	}
	Kolekcija(const Kolekcija& original) {
		_trenutno = original._trenutno;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = original._elementi1[i];
			_elementi2[i] = original._elementi2[i];
		}
	}
	Kolekcija& operator=(const Kolekcija& original) {
		if (this != &original)
		{
			delete[] _elementi1;
			delete[] _elementi2;
			_trenutno = original._trenutno;
			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];
			for (size_t i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = original._elementi1[i];
				_elementi2[i] = original._elementi2[i];
			}
		}
		return *this;
	}
	T1* getElementi1Pok() { return _elementi1; }
	T2* getElementi2Pok() { return _elementi2; }
	T1& getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() { return _trenutno; }

	friend ostream& operator<<(ostream& COUT, Kolekcija<T1, T2>& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	void AddElement(const T1& t1, const T2& t2) {
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		for (size_t i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[]_elementi1;
		delete[]_elementi2;

		temp1[_trenutno] = t1;
		temp2[_trenutno] = t2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_trenutno++;
	}
};
class Poglavlje {
	char* _naslov;
	char* _sadrzaj;
	bool _prihvaceno;
	int _ocjena;//da bi se poglavlje smatralo prihvacenim ocjena mora biti u opsegu od 6 - 10
public:
	Poglavlje(const char* naslov = nullptr, const char* sadrzaj = nullptr)
		:_ocjena(0), _prihvaceno(false) {
		_naslov = AlocirajNizKaraktera(naslov);
		_sadrzaj = AlocirajNizKaraktera(sadrzaj);
	}
	Poglavlje(const Poglavlje& original) {
		_naslov = AlocirajNizKaraktera(original._naslov);
		_sadrzaj = AlocirajNizKaraktera(original._sadrzaj);
		_prihvaceno = original._prihvaceno;
		_ocjena = original._ocjena;
	}
	Poglavlje& operator=(const Poglavlje& original) {
		if (this != &original)
		{
			delete[] _naslov;
			delete[] _sadrzaj;
			_naslov = AlocirajNizKaraktera(original._naslov);
			_sadrzaj = AlocirajNizKaraktera(original._sadrzaj);
			_prihvaceno = original._prihvaceno;
			_ocjena = original._ocjena;
		}
		return *this;
	}
	Poglavlje() {
		delete[] _naslov; _naslov = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	void DeleteSadrzaj() { delete[] _sadrzaj; _sadrzaj = nullptr; }
	void SetSadrzaj(string sadrzaj) { _sadrzaj = AlocirajNizKaraktera(sadrzaj.c_str()); }
	friend ostream& operator<<(ostream& COUT, Poglavlje& obj) {
		if (obj._naslov == nullptr || obj._sadrzaj == nullptr)
			return COUT;
		COUT << endl << obj._naslov << endl << obj._sadrzaj << endl;
		if (obj._prihvaceno)
			COUT << "Ocjena: " << obj._ocjena << endl;;
		return COUT;
	}
	char* GetNaslov() { return _naslov; }
	char* GetSadrzaj() { return _sadrzaj; }
	bool GetPrihvaceno() { return _prihvaceno; }
	int GetOcjena() { return _ocjena; }
	void SetOcjena(int ocjena) { _ocjena = ocjena; }
	void SetPrihvaceno(bool prihvaceno) { _prihvaceno = prihvaceno; }

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
	ZavrsniRad(const char* nazivTeme = nullptr) : _konacnaOcjena(0), _datumOdbrane(not_set) {
		_tema = AlocirajNizKaraktera(nazivTeme);
	}

	ZavrsniRad(const ZavrsniRad& org) : _poglavljaRada(org._poglavljaRada), _konacnaOcjena(org._konacnaOcjena), _datumOdbrane(org._datumOdbrane) {
		_tema = AlocirajNizKaraktera(org._tema);
	}

	~ZavrsniRad() {
		delete[] _tema; _tema = nullptr;
	}
	char* GetNazivTeme()const { return _tema; }
	vector<Poglavlje>& GetPoglavlja() { return _poglavljaRada; };
	string GetDatumOdbrane()const { return _datumOdbrane; }
	float GetOcjena() { return _konacnaOcjena; }
	void SetDatumOdbrane(string datumOdbrane) { _datumOdbrane = datumOdbrane; }

	ZavrsniRad& operator=(const ZavrsniRad& org) {
		if (this != &org) {
			delete[] _tema;
			_tema = AlocirajNizKaraktera(org._tema);
			_datumOdbrane = org._datumOdbrane;
			_poglavljaRada = org._poglavljaRada;
			_konacnaOcjena = org._konacnaOcjena;
		}
		return *this;
	}

	friend ostream& operator<<(ostream& COUT, ZavrsniRad& obj) {
		COUT << "Tema rada: " << obj._tema << endl;
		COUT << "Sadrzaj: " << endl;
		for (size_t i = 0; i < obj._poglavljaRada.size(); i++)
			COUT << obj._poglavljaRada[i] << endl;
		COUT << "Datum odbrane rada: " << obj._datumOdbrane << endl << " Ocjena: " << obj._konacnaOcjena << endl;
		return COUT;
	}
	void DodajPoglavlje(string naslov, string sadrzaj) {
		for (size_t i = 0; i < _poglavljaRada.size(); i++)
		{
			if (strcmp(_poglavljaRada[i].GetNaslov(), naslov.c_str()) == 0) {
				string temp = _poglavljaRada[i].GetSadrzaj();
				temp += " " + sadrzaj;
				_poglavljaRada[i].DeleteSadrzaj();
				_poglavljaRada[i].SetSadrzaj(temp.c_str());
			}
		}
		_poglavljaRada.push_back(Poglavlje(naslov.c_str(), sadrzaj.c_str()));
	}
	void OcijeniPoglavlje(string naslov, int ocjena) {
		for (size_t i = 0; i < _poglavljaRada.size(); i++)
		{
			if (strcmp(_poglavljaRada[i].GetNaslov(), naslov.c_str()) == 0)
				if (ocjena > 5 && ocjena < 11) {
					_poglavljaRada[i].SetOcjena(ocjena);
					_poglavljaRada[i].SetPrihvaceno(true);
					break;
				}
				else
					throw exception("Ocjena nije validna!");
		}
		throw exception("Trazeno poglavlje ne postoji!\n");
	}
};

class Mentor {
	string _imePrezime;
	//Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog Mentora
	Kolekcija<string, ZavrsniRad> _teme;
public:
	Mentor(string imePrezime) :_imePrezime(imePrezime) {}
	Kolekcija<string, ZavrsniRad>& GetTeme() { return _teme; };
	string GetImePrezime() { return _imePrezime; }
	void Info() {
		cout << _imePrezime << endl << _teme << endl;
	}
	Mentor(const Mentor& original) {
		_imePrezime = original._imePrezime;
		_teme = original._teme;
	}
	Mentor& operator=(const Mentor& original) {
		if (this != &original)
		{
			_imePrezime = original._imePrezime;
			_teme = original._teme;
		}
		return *this;
	}
	/*funkcija DodajZavrsniRad ima zadatak da odredjenom Mentoru dodijeli mentorstvo na zavrsnom radu. zavrsni rad se dodaje studentu sa brojem indeksa proslijedjenim kao prvi parametar.
sprijeciti dodavanje zavrsnih radova sa istom temom*/
//parametri:brojIndeksa, zavrsniRad
	bool DodajZavrsniRad(string brojIndeksa, ZavrsniRad& zavrsniRad) {
		for (size_t i = 0; i < _teme.getTrenutno(); i++)
		{
			if (strcmp(brojIndeksa.c_str(), _teme.getElement1(i).c_str()) == 0)
				return false;
			if (strcmp(zavrsniRad.GetNazivTeme(), _teme.getElement2(i).GetNazivTeme()) == 0)
				return false;
		}
		_teme.AddElement(brojIndeksa, zavrsniRad);
	}
	//	/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva definisanje
//	datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio i pri tom su zadovoljeni sljedeci uslovi:
//		1. zavrsni rad ima broj poglavlja veci od minimalnog
//		2. svako poglavlje ima broj karaktera veci od minimalnog
//		3. svako poglavlje je prihvaceno/odobreno
//	ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja), postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazan odbrana.
//	u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije zadovoljen, funkcija vraca nullptr.
//*/
	ZavrsniRad* ZakaziOdbranuRada(string brojIndeksa, string datumOdbrane) {
		float prosjek = 0;
		int brojac = 0;
		for (size_t i = 0; i < _teme.getTrenutno(); i++)
		{
			if (brojIndeksa == _teme.getElement1(i)) {
				for (size_t j = 0; j < _teme.getElement2(i).GetPoglavlja().size(); j++)
				{
					if (strlen(_teme.getElement2(i).GetPoglavlja()[j].GetSadrzaj()) > min_karaktera_po_poglavlju && _teme.getElement2(i).GetPoglavlja()[j].GetPrihvaceno()) {
						prosjek += _teme.getElement2(i).GetPoglavlja()[j].GetOcjena();
						brojac++;
						_teme.getElement2(i).SetDatumOdbrane(datumOdbrane);
					}
				}
				cout << "Prosjek iznosi: " << prosjek / brojac << endl;
			}
			else {
				return nullptr;
			}
			return &_teme.getElement2(i);
		}
	}
};

int main() {
	//	cout << crt << "UPLOAD RADA OBAVEZNO IZVRSITI U ODGOVARAJUCI FOLDER NA FTP SERVERU" << endl;
	//	cout << "U slucaju da je Upload folder prazan pritisnite tipku F5" << crt;

	const int max = 4;
	Mentor* mentori[max];

	mentori[0] = new Mentor("Armina Hubana");
	mentori[1] = new Mentor("Zanin Vejzovic");
	mentori[2] = new Mentor("Jasmin Azemovic");
	mentori[3] = new Mentor("Emina Junuz");
	//parametri: naziv zavrsnog rada
	ZavrsniRad multimedijalni("Multimedijalni informacijski sistem za visoko - obrazovnu ustanovu");
	ZavrsniRad podrsa_operaterima("Sistem za podršku rada kablovskog operatera");
	ZavrsniRad analiza_sigurnosti("Prakticna analiza sigurnosti bežiènih raèunarskih mreža");
	ZavrsniRad kriptografija("Primjena teorije informacija u procesu generisanja kriptografskih kljuèeva");

	/*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj. ukoliko poglavlje vec postoji u zavrsnom radu, funkcija tom poglavlju treba dodati novi sadrzaj i pri tome zadrzi postojeci (izmedju postojeceg i novog sadrzaja se dodaje prazan prostor). u slucaju da poglavlje ne postoji, ono se dodaje zajedno sa sadrzaje*/
	//parametri: nazivPoglavlja, sadrzajPoglavlja
	multimedijalni.DodajPoglavlje("Uvod", "U ovom poglavlju ce biti rijeci");
	multimedijalni.DodajPoglavlje("Uvod", "o multimedijalnim sistemima koji se danas koriste");
	multimedijalni.DodajPoglavlje("Uvod", "u savremenom poslovanju");
	multimedijalni.DodajPoglavlje("Vrste multimedijalnih sistema", "Danas se moze govoriti o nekoliko vrsta multimedijalnih sistema, a neke od najznacajnijih su ...");
	multimedijalni.DodajPoglavlje("Teorija multimedije", "Sadrzaj koji bi trebao stajati na pocetku treceg poglavlja zavrsnog rada o multimediji studenta IB130011");
	multimedijalni.DodajPoglavlje("Zakljucak", "U ovom radu su predstavljeni osnovni koncepti i prakticna primjena...");

	try {
		/*funkcija OcijeniPoglavlje, na osnovu naziva poglavlja, dodjeljuje ocjenu poglavlju te ukoliko je ocjena pozitivna (6 - 10) onda poglavlje oznacava prihvacenim. U slucaju da ocjena nije validna ili poglavlje ne postoji, funkcija baca izuzetak sa odgovarajucom porukom*/
		//parametri:nazivPoglavlja, ocjena

		multimedijalni.OcijeniPoglavlje("Uvod", 8);
		multimedijalni.OcijeniPoglavlje("Vrste multimedijalnih sistema", 8);
		multimedijalni.OcijeniPoglavlje("Teorija multimedije", 9);
		multimedijalni.OcijeniPoglavlje("Zakljucak", 7);
		multimedijalni.OcijeniPoglavlje("Naziv poglavlja ne postoji", 8);
	}
	catch (exception& err) {
		cout << "Greska -> " << err.what() << endl;
	}

	/*funkcija DodajZavrsniRad ima zadatak da odredjenom Mentoru dodijeli mentorstvo na zavrsnom radu. zavrsni rad se dodaje studentu sa brojem indeksa proslijedjenim kao prvi parametar.
	sprijeciti dodavanje zavrsnih radova sa istom temom*/
	//parametri:brojIndeksa, zavrsniRad
	if (mentori[0]->DodajZavrsniRad("IB130011", multimedijalni))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (mentori[0]->DodajZavrsniRad("IB120051", podrsa_operaterima))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (!mentori[0]->DodajZavrsniRad("IB120056", podrsa_operaterima))//dupliranje rada
		cout << "Zavrsni rad nije dodat!" << endl;
	if (!mentori[0]->DodajZavrsniRad("IB120051", kriptografija)) //studentu vec dodijeljen rad
		cout << "Zavrsni rad nije dodat!" << endl;
	if (mentori[1]->DodajZavrsniRad("IB140102", analiza_sigurnosti))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (mentori[2]->DodajZavrsniRad("IB140002", kriptografija))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	//
	//	/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva definisanje
	//	datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio i pri tom su zadovoljeni sljedeci uslovi:
	//		1. zavrsni rad ima broj poglavlja veci od minimalnog
	//		2. svako poglavlje ima broj karaktera veci od minimalnog
	//		3. svako poglavlje je prihvaceno/odobreno
	//	ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja), postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazan odbrana.
	//	u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije zadovoljen, funkcija vraca nullptr.
	//*/
	//
	//parametri:brojIndeksa, datumOdbrane
	ZavrsniRad* zr1 = mentori[0]->ZakaziOdbranuRada("IB130011", "25.09.2018");
	if (zr1 != nullptr)
		cout << *zr1 << endl;

	zr1 = mentori[0]->ZakaziOdbranuRada("IB130111", "25.09.2018");//student sa brojem indeksa IB130111 jos uvijek nije prijavio rad
	if (zr1 != nullptr)
		cout << *zr1 << endl;

	//ispisuje sve podatke o Mentoru i njegovim mentorstvima
	mentori[0]->Info();

	for (int i = 0; i < max; i++) {
		delete mentori[i];
		mentori[i] = nullptr;
	}
	system("pause>0");
	return 0;
}