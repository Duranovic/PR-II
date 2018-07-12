// Sortiranje stringova(nizova char).cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

const char *crt = "\n--------------------------------------------------\n";


void Unos(char *&niz, const char * naziv)
{
	niz = new char[strlen(naziv) + 1];
	strcpy_s(niz, strlen(naziv) + 1, naziv);
}
void Ispis(char * niz [], int vel)
{
	for (size_t i = 0; i < vel; i++)
	{
		cout << i+1<<". "<<niz[i] << endl;
	}
	cout << crt;
}
void Sortiraj(char *niz[], int max)
{
	for (size_t i = 0; i < max; i++)
	{
		int j = i;
		while (j != 0)
		{
			bool prolaz = false;
		
				if (strlen(niz[j]) < strlen(niz[j - 1]))
				{
					prolaz = true;
				}
				else if (strlen(niz[j]) == strlen(niz[j - 1]))
				{
					for (size_t brojac = 0; brojac < 100; brojac++)
					{
						if(strlen(niz[j]) > brojac)
						{
							if (niz[j][brojac] == niz[j-1][brojac])
							{
								prolaz = true;
							}
							if (niz[j][brojac] < niz[j - 1][brojac])
							{
								prolaz = true;
								break;
							}
							else
							{
								prolaz = false;
								break;
							}
						}
					}
				}
				if (prolaz)
				{
					char * temp;
					Unos(temp, niz[j]);
					delete[] niz[j];
					Unos(niz[j], niz[j - 1]);
					delete[] niz[j - 1];
					Unos(niz[j - 1], temp);
				}
				j--;
		}
	}
}


int main()
{
	const int max = 6;
	char * nizKaraktera[max];
	Unos(nizKaraktera[0], "Cokolada");
	Unos(nizKaraktera[1], "Sok");
	Unos(nizKaraktera[2], "Miris");
	Unos(nizKaraktera[5], "Cokolade");
	Unos(nizKaraktera[3], "Milka");
	Unos(nizKaraktera[4], "Mirisna");


	Ispis(nizKaraktera, max);
	Sortiraj(nizKaraktera, max);
	Ispis(nizKaraktera, max);


	system("pause>0");
    return 0;
}

