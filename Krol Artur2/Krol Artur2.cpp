// Krol Artur.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;
const int NIESKONCZONOSC = 99999999;
struct miasto;

struct droga
{
	int dlugosc;
	int nrcelu;
};

struct miasto
{
	int rodzaj;
	int kara;
	int koszt;
	int kosztostateczny;
	int nrwkopcu;
	droga *wychodzacedrogi;
	droga *wpadajacedrogi;
	int liczbawychodzacychdrog;
	int licznikwpadajacychdrog;
};
struct kopiec
{
	int maksymalnyrozmiar;
	int* tablica;
	miasto *m;
	kopiec(int rozmiar, miasto *m2)  // kopiec minimum, zape³nianie kopca
	{
		maksymalnyrozmiar = rozmiar + 1;
		tablica = new int[maksymalnyrozmiar];
		m = m2;
		for (int i = 1; i < maksymalnyrozmiar; i++)
		{
			tablica[i] = i;
			m[i].nrwkopcu = i;
			m[i].koszt = NIESKONCZONOSC;
			m[i].kosztostateczny = NIESKONCZONOSC;
		}
	}
	void naprawgora(int numersyna) // nr el który nale¿y naprawic
	{
		if (numersyna == 1) // spr czy nowy nr to korzen
		{
			return;
		}

		int nrojca = numersyna / 2;

		if (m[tablica[nrojca]].koszt > m[tablica[numersyna]].koszt) // syn < ojciec
		{
			int pom = tablica[numersyna];
			tablica[numersyna] = tablica[nrojca];
			tablica[nrojca] = pom;

			m[tablica[numersyna]].nrwkopcu = numersyna; // polozenie w kopcu
			m[tablica[nrojca]].nrwkopcu = nrojca;

			naprawgora(nrojca);
		}

	}
	void naprawwdol(int numer)
	{
		int nrsyna1 = 2 * numer; //syn lewy
		int nrsyna2 = 2 * numer + 1; // syn prawy

		if (nrsyna1 >= maksymalnyrozmiar) // numer syna2 tez przekracza rozmiar
		{
			return;
		}


		int tanszysyn;
		if (nrsyna2 == maksymalnyrozmiar)// prawy syn nie istnieje, przekracza maksymalny rozmiar tablicy
		{
			tanszysyn = nrsyna1;
		}
		else if (m[tablica[nrsyna1]].koszt > m[tablica[nrsyna2]].koszt) // dwóch synów
		{
			tanszysyn = nrsyna2;
		}
		else
		{
			tanszysyn = nrsyna1;
		}
		if (m[tablica[numer]].koszt > m[tablica[tanszysyn]].koszt)
		{
			int pom = tablica[numer];
			tablica[numer] = tablica[tanszysyn];
			tablica[tanszysyn] = pom;

			m[tablica[numer]].nrwkopcu = numer;
			m[tablica[tanszysyn]].nrwkopcu = tanszysyn;

			naprawwdol(tanszysyn);
		}
	}

	bool czypusty()
	{
		if (m[tablica[1]].koszt == NIESKONCZONOSC)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int top()
	{
		return tablica[1];
	}


	void usun(int numer)
	{
		m[tablica[numer]].koszt = NIESKONCZONOSC;
		naprawwdol(numer);
	}

	~kopiec()
	{
		delete[] tablica;
	}

};

void dijkstra(miasto *m, int liczbamiast, int nrpoczatku)
{
	kopiec k(liczbamiast, m);
	m[nrpoczatku].koszt = 0;
	k.naprawgora(m[nrpoczatku].nrwkopcu);
	while (k.czypusty() == false)
	{
		int nrmiastaobecnego = k.top();
		m[nrmiastaobecnego].kosztostateczny = m[nrmiastaobecnego].koszt;
		k.usun(1);
		// k.usun(m[nrmiastaobecnego].nrwkopcu);  
		for (int i = 0; i < m[nrmiastaobecnego].liczbawychodzacychdrog; i++)
		{
			int nrsasiada = m[nrmiastaobecnego].wychodzacedrogi[i].nrcelu; // i to nr drogi
			if (m[nrsasiada].kosztostateczny == NIESKONCZONOSC) // interesuj¹ nas nieodwiedzone ostatecznie miasta
			{
				int nowykoszt = m[nrmiastaobecnego].kosztostateczny + m[nrmiastaobecnego].wychodzacedrogi[i].dlugosc + m[nrmiastaobecnego].kara; // suma dróg

				if (nowykoszt < m[nrsasiada].koszt)
				{
					m[nrsasiada].koszt = nowykoszt;
					k.naprawgora(m[nrsasiada].nrwkopcu);
				}
			}
		}

	}


}

void przejsciewstecz(int poczatek, int koniec, miasto* m, int *trasa, int &dlugosctrasy) // zageszczenie drog do tych najwazniejszych
{
	int aktualnywierzcholek = koniec;
	int licznikwierzcholkow = 0;
	while (aktualnywierzcholek != poczatek)
	{
		trasa[licznikwierzcholkow] = aktualnywierzcholek;
		licznikwierzcholkow++;
		for (int i = 0; i < m[aktualnywierzcholek].licznikwpadajacychdrog; i++) //spr kazdego sasiada
		{
			int nrsasiada = m[aktualnywierzcholek].wpadajacedrogi[i].nrcelu;
			int kosztdrogi = m[nrsasiada].kosztostateczny + m[aktualnywierzcholek].wpadajacedrogi[i].dlugosc + m[nrsasiada].kara; // suma dróg
			if (m[aktualnywierzcholek].kosztostateczny == kosztdrogi) // dobry sasiad, jesli if jest prawdziwy to przeszlismy od tego sasiad
			{
				aktualnywierzcholek = nrsasiada; // cofamy sie do tego sasiada, jesli od niego przyszlismy
				break;
			}
		}
	}
	trasa[licznikwierzcholkow] = poczatek;
	licznikwierzcholkow++;
	dlugosctrasy = licznikwierzcholkow;
}

int main()
{

	int n, k;

	cin >> n;
	cin >> k;

	miasto *m;
	int nrpoczatku;
	int nrkonca;
	int nrsprzedawcy = -1;
	int nrrycerzy = -1;

	m = new miasto[n + 1];

	int dlugosctrasy = 0;
	int *trasa;
	trasa = new int[3 * n];
	int dlugoscobjazdu = 0;
	int *objazd;
	objazd = new int[3 * n];

	for (int i = 1; i <= n; i++)
	{
		m[i].licznikwpadajacychdrog = 0;
		m[i].wpadajacedrogi = new droga[n + 1]; // nie powtarzaja sie
	}

	for (int i = 1; i <= n; i++)
	{
		cin >> m[i].rodzaj;
		cin >> m[i].liczbawychodzacychdrog;


		if (m[i].liczbawychodzacychdrog == 0)
		{
			m[i].wychodzacedrogi = NULL;


		}
		else {
			m[i].wychodzacedrogi = new droga[m[i].liczbawychodzacychdrog];


		}
		m[i].kara = 0; // kara domyœlnie dla ka¿dego miasta




		for (int j = 0; j < m[i].liczbawychodzacychdrog; j++)
		{

			cin >> m[i].wychodzacedrogi[j].nrcelu;
			m[i].wychodzacedrogi[j].nrcelu++;
			cin >> m[i].wychodzacedrogi[j].dlugosc;

			int cel = m[i].wychodzacedrogi[j].nrcelu;
			int it = i;
			int jt = j;
			int licznikt = m[cel].licznikwpadajacychdrog;
			int liczbat = m[i].liczbawychodzacychdrog;

			m[cel].wpadajacedrogi[m[cel].licznikwpadajacychdrog].nrcelu = i;
			m[cel].wpadajacedrogi[m[cel].licznikwpadajacychdrog].dlugosc = m[i].wychodzacedrogi[j].dlugosc;
			m[cel].licznikwpadajacychdrog++;
		}
		if (m[i].rodzaj == 1)
		{
			nrsprzedawcy = i;
		}
		if (m[i].rodzaj == 3)
		{
			nrrycerzy = i;
		}
		if (m[i].rodzaj == 2)
		{
			m[i].kara = k;
		}
		if (m[i].rodzaj == 5)
		{
			nrkonca = i;
		}
		if (m[i].rodzaj == 4)
		{
			nrpoczatku = i;
		}

	}
	if (nrrycerzy != -1)
	{
		if (nrsprzedawcy == -1)
		{
			m[nrrycerzy].kara = NIESKONCZONOSC;
		}
		else {
			dijkstra(m, n, nrrycerzy);

			przejsciewstecz(nrrycerzy, nrsprzedawcy, m, objazd, dlugoscobjazdu);
			m[nrrycerzy].kara = 2 * m[nrsprzedawcy].kosztostateczny;
		}
	}
	dijkstra(m, n, nrpoczatku);
	przejsciewstecz(nrpoczatku, nrkonca, m, trasa, dlugosctrasy);

	for (int i = dlugosctrasy - 1; i >= 0; i--)
	{
		if (trasa[i] == nrrycerzy)
		{
			for (int j = dlugoscobjazdu - 1; j >= 0; j--)
			{
				cout << objazd[j] - 1 << ' ';
			}
			for (int j = 1; j < dlugoscobjazdu; j++) // j = 1 bo sprzedawca krzaków odwiedzany jest tlyko raz
			{
				cout << objazd[j] - 1 << ' ';
			}
		}
		else
		{
			cout << trasa[i] - 1 << ' ';
		}

	}


	for (int i = 1; i <= n; i++)
	{
		if (m[i].liczbawychodzacychdrog > 0)
		{
			delete[] m[i].wychodzacedrogi;
		}
		delete[] m[i].wpadajacedrogi;
	}
	delete[] m;
	return 0;

}