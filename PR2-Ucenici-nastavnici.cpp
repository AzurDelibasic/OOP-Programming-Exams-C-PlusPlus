#include <iostream>
#include <string>
#include <regex>
#include <mutex>
#include <thread>
#include <exception>
#include <vector>

using namespace std;

enum Predmeti { Matematika, Historija, Geografia, Hemija, Biologija, Engleski, Muzicko, Fizicko };
enum Razredi { I1, I2, I3, II1, II2, II3, III1, III2, III3, IV1, IV2, IV3 };
const char* ispisPredmeti[] = { "Matematika", "Historija", "Geografia", "Hemija", "Biologija", "Engleski", "Muzicko", "Fizicko" };
const char* ispisRazredi[] = { "I1", "I2", "I3", "II1", "II2", "II3", "III1", "III2", "III3", "IV1", "IV2", "IV3" };
const char* crt = "\n-----------------------------\n";

char* Alociraj(const char* sadrzaj) {
	if (sadrzaj == nullptr) return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
template<class T1, class T2>
class Kolekcija
{
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	Kolekcija()
	{
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
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
		if (this != &original) {
			delete[]_elementi1;
			delete[]_elementi2;

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
	T1& GetElement1(int lokacija) const { return _elementi1[lokacija]; }
	T2& GetElement2(int lokacija) const { return _elementi2[lokacija]; }
	int GetTrenutno() const { return _trenutno; }
	void AddElement(const T1& t1, const T2& t2) {
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		for (size_t i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[_trenutno] = t1;
		temp2[_trenutno] = t2;
		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_trenutno++;
	}
	~Kolekcija()
	{
		delete[]_elementi1;
		delete[]_elementi2;
	}
	void RemoveElement(int lokacija) {
		T1* temp1 = new T1[_trenutno - 1];
		T2* temp2 = new T2[_trenutno - 1];
		int brojac = 0;
		for (size_t i = 0; i < _trenutno; i++)
		{
			if (i != lokacija) {
				temp1[brojac] = _elementi1[i];
				temp2[brojac] = _elementi2[i];
				brojac++;
			}
		}
		delete[]_elementi1;
		delete[]_elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_trenutno--;
	}
	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj)
	{
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
		{
			COUT << obj.GetElement1(i) << "   " << obj.GetElement2(i) << endl;
		}
		return COUT;
	}
};
class Datum
{
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000)
	{
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}

	Datum(const Datum& obj)
	{
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}

	Datum& operator=(const Datum& obj) {
		if (this != &obj) {
			delete _dan;
			delete _mjesec;
			delete _godina;

			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
		}
		return *this;
	}
	~Datum()
	{
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}


	friend ostream& operator<< (ostream& COUT, const Datum& obj)
	{
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}
};
class Osoba
{
protected:
	char* _imePrezime;
	Datum* _datumRodjenja;
	string _brojTelefona;
public:
	Osoba(const char* imePrezime, Datum datum)
	{
		_imePrezime = Alociraj(imePrezime);
		_datumRodjenja = new Datum(datum);
		_brojTelefona = "";
	}
	Osoba(const Osoba& obj) :_brojTelefona(obj._brojTelefona)
	{
		_imePrezime = new char[strlen(obj._imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(obj._imePrezime) + 1, obj._imePrezime);
		_datumRodjenja = new Datum(*obj._datumRodjenja);
	}
	Osoba& operator=(const Osoba& obj) {
		if (this != &obj) {
			delete[] _imePrezime;

			_imePrezime = Alociraj(obj._imePrezime);
			_datumRodjenja = new Datum(*obj._datumRodjenja);
			_brojTelefona = obj._brojTelefona;
		}
		return *this;
	}
	char* GetImePrezime() { return _imePrezime; }
	~Osoba() { delete[]_imePrezime; delete _datumRodjenja; }
	friend bool operator==(const Osoba& o1, const Osoba& o2) {
		if (strcmp(o1._imePrezime, o2._imePrezime) == 0)
			return true;
		return false;
	}
	//bool setBrojTelefona(string brojTelefona)
	//{
	//    if (ProvjeriValidnostBrojaTelefona(brojTelefona))
	//    {
	//        _brojTelefona = brojTelefona;
	//        return true;
	//    }
	//    return false;
	//}

};

class Nastavnik :public Osoba
{
	Kolekcija<Predmeti, Razredi>* _predmetiRazredi;
public:
	Nastavnik(const char* imePrezime, Datum datum) : Osoba(imePrezime, datum) {
		_predmetiRazredi = new Kolekcija<Predmeti, Razredi>;
	}
	Nastavnik(const Nastavnik& original) : Osoba(original) {
		_predmetiRazredi = new Kolekcija<Predmeti, Razredi>(*original._predmetiRazredi);
	}
	Nastavnik& operator=(const Nastavnik& original) {
		if (this != &original) {
			delete _predmetiRazredi;

			Osoba::operator=(original);
			_predmetiRazredi = new Kolekcija<Predmeti, Razredi>(*original._predmetiRazredi);

		}
		return *this;
	}
	~Nastavnik() {
		delete _predmetiRazredi; _predmetiRazredi = nullptr;
	}
	Kolekcija<Predmeti, Razredi>& getPredmetiRazredi() { return *_predmetiRazredi; }

	/*NASTAVNIK MOZE PREDAVATI ISTI PREDMET, ALI TO MORA BITI U RAZLICITIM ODJELJENJIMA I NAJVISE NA DVIJE RAZLICITE GODINE*/
	bool AddPredmetRazred(Predmeti predmet, Razredi razred) {
		int brojac = 0;
		for (size_t i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
		{
			if (_predmetiRazredi->GetElement1(i) == predmet && _predmetiRazredi->GetElement2(i) == razred)
				return false;
			if (_predmetiRazredi->GetElement2(i) == razred) {
				brojac++;
				if (brojac == 2 && _predmetiRazredi->GetElement1(i) == predmet)
					return false;
			}
		}
		_predmetiRazredi->AddElement(predmet, razred);
		return true;
	}
	bool RemovePredmetRazred(Predmeti predmet) {
		for (size_t i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
		{
			if (_predmetiRazredi->GetElement1(i) == predmet)
			{
				_predmetiRazredi->RemoveElement(i);
				return true;
			}
		}
		return false;
	}
};
//bool ProvjeriValidnostBrojaTelefona(string telefon)//globalna funkcija
//{
//    //broj telefona treba biti u formatu: +387(61)-222-333
//}
class Ucenik : public Osoba
{
	int _brojUDnevniku;
	Razredi _razred;
	vector<Nastavnik*> _nastavnici;
	Kolekcija<Predmeti, int> _predmetiOcjene;
public:
	Ucenik(const char* imePrezime, Datum datum, int brojUDnevniku, Razredi razred) :Osoba(imePrezime, datum) {
		_brojUDnevniku = brojUDnevniku;
		_razred = razred;
	}
	Ucenik(const Ucenik& original) : Osoba(original) {
		_brojUDnevniku = original._brojUDnevniku;
		_razred = original._razred;
		_nastavnici = original._nastavnici;
		_predmetiOcjene = original._predmetiOcjene;
	}
	Ucenik& operator=(const Ucenik& original) {
		if (this != &original) {
			Osoba::operator=(original);
			_brojUDnevniku = original._brojUDnevniku;
			_razred = original._razred;
			_nastavnici = original._nastavnici;
			_predmetiOcjene = original._predmetiOcjene;
		}
		return *this;
	}
	vector < Nastavnik*>& getNastavnici() { return _nastavnici; }
	bool imaIsti(Nastavnik& nastavnik)
	{
		for (int i = 0; i < _nastavnici.size(); i++)
		{
			if (*_nastavnici[i] == nastavnik)
				return true;
		}
		return false;
	}
	friend ostream& operator<< (ostream& COUT, const Ucenik& obj)
	{
		COUT << "Broj u dnevniku: " << obj._brojUDnevniku << endl;
		COUT << "Ime i prezime:  " << obj._imePrezime << endl;
		return COUT;
	}
	Kolekcija<Predmeti, int>& getPredmetiOcjene() { return _predmetiOcjene; }
	//    /*VODITI RACUNA DA SE DODAJU REFERENCE NA NASTAVNIKE KAKO BI SVI UCENICI DIJELILI ISTE INFORMACIJE O
	//    NASTAVNICIMA U SLUCAJU DA SE NASTAVNIKU DODA NEKI PREDMET*/
	//    /*ZA USPJESNO DODAVANJE, NASTAVNIK MORA POSJEDOVATI (PREDAVATI) NAJMANJE JEDAN PREDMET U TOM RAZREDU */
	bool AddNastavnik(Nastavnik& nastavnik)
	{
		_nastavnici.push_back(&nastavnik);
		return true;
	}

	bool AddPredmetOcjenu(Predmeti predmet, int ocjena)
	{
		for (int i = 0; i < _nastavnici.size(); i++)
			for (int j = 0; j < _nastavnici[i]->getPredmetiRazredi().GetTrenutno(); j++)
				if (_nastavnici[i]->getPredmetiRazredi().GetElement1(j) == predmet)
				{
					_predmetiOcjene.AddElement(predmet, ocjena);
					return true;
				}
		return false;
	}
	float Prosjek() {
		float prosjek = 0;
		int brojac = 0;
		for (size_t i = 0; i < _predmetiOcjene.GetTrenutno(); i++)
		{
			prosjek += _predmetiOcjene.GetElement2(i);
			brojac++;
		}
		return prosjek / brojac;
	}
	float GetProsjekByNastavnik(const char* imePrezime) {
		float prosjek = 0;
		for (size_t i = 0; i < _nastavnici.size(); i++)
		{
			if (strcmp(_nastavnici[i]->GetImePrezime(), imePrezime) == 0)
				prosjek += Prosjek();
		}
		return prosjek;
	}
};

void main()
{
	/****************************************************************************
	// 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	// 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR
	// CE BITI OZNACENO KAO "RE"
	// 3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	// 4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_.TXT
	// 5. NAZIV I LISTA PARAMETARA FUNKCIJA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.
	// 6. NIJE POTREBNO BACATI IZUZETKE. SVE FUNKCIJE VRACAJU TRUE ILI FALSE
	****************************************************************************/

#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion

#pragma region TestiranjeKolekcije
	Kolekcija<int, int> kolekcija1;
	int brojElemenata = 15;
	for (size_t i = 0; i < brojElemenata; i++)
		kolekcija1.AddElement(i, i);

	kolekcija1.RemoveElement(0);//uklanja element na osnovu lokacije
	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
	cout << kolekcija1 << endl;

	Kolekcija<int, int> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;
	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;
	Kolekcija<int, int> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
#pragma endregion
	//
	//#pragma region TestiranjeNastavnika
	Nastavnik jasmin("Jasmin Azemovic", Datum(15, 2, 1980));
	Nastavnik adel("Adel Handzic", Datum(15, 10, 1981));
	/*NASTAVNIK MOZE PREDAVATI ISTI PREDMET, ALI TO MORA BITI U RAZLICITIM ODJELJENJIMA I NAJVISE U DVA
	RAZREDA (NA DVIJE RAZLICITE GODINE)*/
	if (jasmin.AddPredmetRazred(Matematika, I1))
		cout << "Predmeti dodan" << endl;
	if (jasmin.AddPredmetRazred(Historija, I1))
		cout << "Predmeti dodan" << endl;

	if (jasmin.AddPredmetRazred(Geografia, I1))
		cout << "Predmeti dodan" << endl;

	if (jasmin.AddPredmetRazred(Geografia, I2))
		cout << "Predmeti dodan" << endl;

	if (adel.AddPredmetRazred(Historija, III1))
		cout << "Predmeti dodan" << endl;

	if (adel.RemovePredmetRazred(Historija))
		cout << "Predmet uklonjen!" << endl;

	Nastavnik goran(adel);
	if (goran == adel)
		cout << "Nastavnici identicni" << endl;
	goran = jasmin;
	if (goran == jasmin)
		cout << "Nastavnici identicni" << endl;

	Kolekcija<Predmeti, Razredi>& predmetiRazredi = jasmin.getPredmetiRazredi();
	//
	//    if (!jasmin.setBrojTelefona("+38761111222"))
	//        cout << "Broj telefona nije dodan!" << endl;
	//    if (!jasmin.setBrojTelefona("+387-61-111-222"))
	//        cout << "Broj telefona nije dodan!" << endl;
	//    if (jasmin.setBrojTelefona("+387(61)-111-222"))
	//        cout << "Broj telefona uspjesno dodan!" << endl;
	//#pragma endregion
	//
	//#pragma region TestiranjeUcenika
	Ucenik denis("Denis Music", Datum(8, 10, 1990), 3, III1);
	Ucenik elmin("Elmin Sudic", Datum(15, 6, 1993), 1, I1);
	Ucenik adil("Adil Joldic", Datum(13, 8, 1992), 2, II1);
	//    /*VODITI RACUNA DA SE DODAJU REFERENCE NA NASTAVNIKE KAKO BI SVI UCENICI DIJELILI ISTE INFORMACIJE O
	//    NASTAVNICIMA U SLUCAJU DA SE NASTAVNIKU DODA NEKI PREDMET*/
	//    /*ZA USPJESNO DODAVANJE, NASTAVNIK MORA POSJEDOVATI (PREDAVATI) NAJMANJE JEDAN PREDMET U TOM RAZREDU */
	if (denis.AddNastavnik(jasmin))
		cout << "Nastavnik uspjesno dodan" << endl;
	/*ONEMOGUCITI DODAVANJE ISTIH NASTAVNIKA; POJAM ISTI PODRAZUMIJEVA IDENTICNE VRIJEDNOSTI SVIH ATRIBUTA*/
	if (denis.AddNastavnik(adel))
		cout << "Nastavnik uspjesno dodan" << endl;
	/*DA BI SE DODALA OCJENA ZA ODREDJENI PREDMET UCENIK MORA POSJEDOVATI NASTAVNIKA KOJI PREDAJE TAJ PREDMET*/
	if (denis.AddPredmetOcjenu(Matematika, 3))
	    cout << "Ocjena uspjesno dodana" << endl;
	if (denis.AddPredmetOcjenu(Historija, 4))
	    cout << "Ocjena uspjesno dodana" << endl;
//
    vector<Nastavnik*>& nastavnici = denis.getNastavnici();
    cout << "Ucenik Denis ima dodijeljena " << nastavnici.size() << " nastavnika" << endl;
//
    Kolekcija<Predmeti, int>& uspjeh = denis.getPredmetiOcjene();
    cout << "Denis ima " << uspjeh.GetTrenutno() << " polozenih predmeta" << endl;
    cout << "Prosjecna ocjena kod nastavnika Jasmin Azemovic je " << denis.GetProsjekByNastavnik("Jasmin Azemovic") << endl;
//
    Ucenik denis2(denis);
    cout << denis2 << endl;

#pragma endregion
	system("pause");
}