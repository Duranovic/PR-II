// Zadatak 002 - Izostanci (Integralno).cpp : Defines the entry point for the console application.
// AKO BUDE GREŠAKA, PREDLOŽI IZMJENU. 

#include "stdafx.h"
#include <fstream>
#include <iostream>

using namespace std;

int BROJ_INDEKSA = 150001;

const char * crt = "\n----------------------------------------------------\n";
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}
};
struct Izostanak {
	char * _razlogIzostanka;
	Datum _datumIzostanka;
	int _brojSati;
	bool _opravdano;//pravdanje zahtijeva da se evidentira i razlog izostanka	
	void Unos(Datum datumIzostanka, int brojSati) {
		_datumIzostanka.Unos(*datumIzostanka._dan, *datumIzostanka._mjesec, *datumIzostanka._godina);
		_brojSati = brojSati;
		_opravdano = false;
		_razlogIzostanka = new char;
		_razlogIzostanka[0] = '\0';
	}
	void Dealociraj() { delete[]_razlogIzostanka; _razlogIzostanka = nullptr; }
	void Ispis() { _datumIzostanka.Ispis(); cout << _razlogIzostanka << " " << _brojSati; }
	void Opravdaj(const char * razlogIzostanka) {
		if (_razlogIzostanka != nullptr)
			delete _razlogIzostanka;
		int vel = strlen(razlogIzostanka) + 1;
		_razlogIzostanka = new char[vel];
		strcpy_s(_razlogIzostanka, vel, razlogIzostanka);
		_opravdano = true;
	}
};
struct Student {
	//format broja indeksa: IB150051
	//osigurati automatsko generisanje broja indeksa pocevsi od IB150001
	char _brojIndeksa[9];
	char * _imePrezime;
	Izostanak * _izostanci;
	int _brojIzostanaka;
	//Potpisi funkcija trebaju odgovarati onima u main-u
	//1. Unos
	void Unos(char * indeks, const char * imePrezime)
	{
		strcpy_s(_brojIndeksa, indeks);
		_imePrezime = new char[strlen(imePrezime) + 1];
		strcpy_s(_imePrezime, strlen(imePrezime) + 1, imePrezime);
		_izostanci = nullptr;
		_brojIzostanaka = 0;
	}
	//2. Dealociraj
	void Dealociraj()
	{
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			_izostanci[i].Dealociraj();
		}
		delete[] _izostanci; _izostanci = nullptr;
	}
	/*3. DodajIzostanak - dodaje novi izostanak u listu izostanaka.	Onemoguciti ponavljanje izostanaka na nacin da se uvecava samo broj sati ukoliko
	korisnik (profesor) vise puta pokusa dodati izostanak na isti dan.*/
	void DodajIzostanak(Izostanak izostanak)
	{
		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			if (*izostanak._datumIzostanka._dan == *_izostanci[i]._datumIzostanka._dan)
				if (*izostanak._datumIzostanka._mjesec == *_izostanci[i]._datumIzostanka._mjesec)
					if (*izostanak._datumIzostanka._godina == *_izostanci[i]._datumIzostanka._godina)
					{
						_izostanci[i]._brojSati += izostanak._brojSati;
						return;
					}
		}

		Izostanak * temp = new Izostanak[_brojIzostanaka + 1];
		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			temp[i].Unos(_izostanci[i]._datumIzostanka, _izostanci[i]._brojSati);
			if (_izostanci[i]._opravdano == true)
			{
				temp[i].Opravdaj(_izostanci[i]._razlogIzostanka);
			}
		}
		delete[] _izostanci;
		_izostanci = temp;
		_izostanci[_brojIzostanaka].Unos(izostanak._datumIzostanka, izostanak._brojSati);
		_brojIzostanaka++;

	}
	//4. Ispis - ispisuje izostanke u formatu prikazanom na slici (nije obavezno da ispis bude identican, ali je svakako pozeljno). Prilikom svakog ispisa, izostanke je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj indeksa.
	void Ispis()
	{
		cout << "Broj Indeksa: " << _brojIndeksa << endl;
		cout << "Ime i prezime: " << _imePrezime << endl;
		cout << crt;

		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			_izostanci[i].Ispis();
			cout << crt;
		}
		cout << crt;

		int vel = strlen(_brojIndeksa) + strlen(".txt") + 1;
		char * nazivFajla = new char[vel];
		strcpy_s(nazivFajla, vel, _brojIndeksa);
		strcat_s(nazivFajla, vel, ".txt\0");

		ofstream unos;
		unos.open(nazivFajla);
		unos << "Broj Indeksa: " << _brojIndeksa << endl;
		unos << "Ime i prezime: " << _imePrezime << endl;
		unos << crt;

		for (size_t i = 0; i < _brojIzostanaka; i++)
		{
			unos << "Datum izostanka: "<<*_izostanci[i]._datumIzostanka._dan << "/" << *_izostanci[i]._datumIzostanka._mjesec << "/" << *_izostanci[i]._datumIzostanka._godina<<endl;
			unos << _izostanci[i]._razlogIzostanka << "  " << _izostanci[i]._brojSati;
			unos << crt;
		}
		unos << crt;
		unos.close();
	}
	//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu datuma, pronalazi i vraca pokazivac na izostanak. Ukoliko trazeni izostanak ne postoji funkcija vraca nullptr.
	void Sortiraj()
	{
		int lijevi;
		int desni;

		for (size_t i = 1; i < _brojIzostanaka; i++)
		{
			int j = i;

			while (j != 0)
			{
				lijevi = *_izostanci[j - 1]._datumIzostanka._dan * 30 + *_izostanci[j - 1]._datumIzostanka._mjesec * 12 + *_izostanci[j - 1]._datumIzostanka._godina;
				desni = *_izostanci[j]._datumIzostanka._dan * 30 + *_izostanci[j]._datumIzostanka._mjesec * 12 + *_izostanci[j]._datumIzostanka._godina;
			
				if (desni < lijevi)
				{
					Izostanak temp;

						temp.Unos(_izostanci[j]._datumIzostanka, _izostanci[j]._brojSati);
						if (_izostanci[j]._opravdano == true)
						{
							temp._opravdano = _izostanci[j]._opravdano;
							temp._razlogIzostanka = new char[strlen(_izostanci[j]._razlogIzostanka) + 1];
							strcpy_s(temp._razlogIzostanka, strlen(_izostanci[j]._razlogIzostanka) + 1, _izostanci[j]._razlogIzostanka);
						}
					
					_izostanci[j] = _izostanci[j - 1];
					_izostanci[j - 1] = temp;
				}
				j--;
			}
		}
	}
	
	Izostanak * BinarnaPretraga(Datum d)
	{
		this->Sortiraj();
		int lijevo = 0;
		int desno = _brojIzostanaka-1;
		int sredina = (lijevo + desno) / 2;

		while (lijevo <= desno)
		{
			if ((*d._dan * 30 + *d._mjesec * 12 + *d._godina) == (*_izostanci[sredina]._datumIzostanka._dan * 30 + *_izostanci[sredina]._datumIzostanka._mjesec * 12 + *_izostanci[sredina]._datumIzostanka._godina))
				return &_izostanci[sredina];

			else if ((*d._dan * 30 + *d._mjesec * 12 + *d._godina) < (*_izostanci[sredina]._datumIzostanka._dan + *_izostanci[sredina]._datumIzostanka._mjesec * 12 + *_izostanci[sredina]._datumIzostanka._godina))
			{
				desno = sredina - 1;
			}
			else
			{
				lijevo = sredina + 1;
			}
			sredina = (lijevo + desno) / 2;
		}
		return nullptr;
	}


};
//1. PronadjiNajveciNeopravdaniIzostanak - rekurzivna funkcija koja vraca pokazivac na neopravdani izostanak koji ima najveci broj sati
Izostanak * PronadjiNajveciNeopravdaniIzostanak(Student student, int brojIzostanaka)
{
	Izostanak * pok = nullptr;
	if (brojIzostanaka < 0)
	{
		pok = nullptr;
		return pok;
	}
	pok = PronadjiNajveciNeopravdaniIzostanak(student, brojIzostanaka - 1);

	if (student._izostanci[brojIzostanaka]._opravdano == true)
	{
		return pok;
	}
	else if (pok != nullptr)
	{
		if (pok->_brojSati >= student._izostanci[brojIzostanaka]._brojSati)
		{
			return pok;
		}
		else
		{
			return &student._izostanci[brojIzostanaka];
		}
	}
	else
	{
		return &student._izostanci[brojIzostanaka];
	}
}

//2. GenerisiSljedeciBrojIndeksa - generise i vraca naredni broj indeksa

char * GenerisiSljedeciBrojIndeksa()
{
	int max = strlen("IB150000") + 1;
	char * povratni = new char[max];
	strcpy_s(povratni, max, "IB150000");
	int temp = BROJ_INDEKSA;
	int brojCifara = 0;

	while (temp > 0)
	{
		temp /= 10;
		brojCifara++;
	}

	_itoa_s(BROJ_INDEKSA, &povratni[max - 1 - brojCifara],max,10);
	BROJ_INDEKSA++;
	return povratni;
}

int main() {
	Datum jucer, prije5Dana, prije10Dana;
	jucer.Unos(12, 6, 2016);
	prije5Dana.Unos(8, 6, 2016);
	prije10Dana.Unos(3, 6, 2016);

	Student denis;
	denis.Unos(GenerisiSljedeciBrojIndeksa(), "Denis Music");

	Izostanak izostanakJucer, izostanakPrije5Dana, izostanakPrije10Dana;

	izostanakJucer.Unos(jucer, 5);
	denis.DodajIzostanak(izostanakJucer);

	izostanakPrije5Dana.Unos(prije5Dana, 3);
	izostanakPrije5Dana.Opravdaj("Odsutan zbog bolesti - gripa");
	denis.DodajIzostanak(izostanakPrije5Dana);
	izostanakPrije5Dana.Unos(prije5Dana, 2);
	denis.DodajIzostanak(izostanakPrije5Dana);

	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);
	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);

	denis.Ispis();
	Izostanak * p = denis.BinarnaPretraga(jucer);
	cout << "Binarnom pretragom pronadjen izostanak -> ";
	p->Ispis();
	cout << endl;
	denis.Ispis();
	Izostanak * p2 = PronadjiNajveciNeopravdaniIzostanak(denis, denis._brojIzostanaka-1);
	cout << "Rekurzivno pronadjen najveci neopravdani izostanak -> ";
	p2->Ispis();


	cout << endl;
	//dealocirati zauzetu memoriju
	denis.Dealociraj();
	p = nullptr;
	p2 = nullptr;

	system("pause>0");
	return 0;
}
