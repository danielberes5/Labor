#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <stack>
#include <algorithm>

using namespace std;

class Elem //Deklaralom a fa elemeinek a classat.
{
public:
	char muvelet;
	Elem* jobb;
	Elem* bal;
	Elem* szulo;
	int sorrend; //Szukseg lesz ra a fa felepitesenel.

};
static stack<double> verem;
static void Epit(double szuloIndex, vector<Elem> &muveletek, vector<double> &szamok, stack<double> &verem) //felepiti a binaris fat
{
	//A legkesobbi sorrendel kezdi, majd megkeresi jobbra, majd balra a tole meg kissebb sorrend ertekkel rendelkezot, de a leheto legnagyobb sorrend ertekkel rendelkezot; ez lesz majd a fa kovetkezo aga. 
	int keresoIndex = (szuloIndex + 1);
	int gyerekIndex = 0;
	int max = 0;
	while (keresoIndex < muveletek.size() && (muveletek.at(szuloIndex)).sorrend >(muveletek.at(keresoIndex)).sorrend)
	{
		if (max < muveletek[keresoIndex].sorrend)
		{
			max = muveletek[keresoIndex].sorrend;
			gyerekIndex = keresoIndex;
		}
		keresoIndex++;
	}
	if (max == 0) //Ha az adott oldalon (esetunkben a jobb) nincs tovabbi muvelet a verembe berakja az adott szamerteket kesobbi feldolgozasra.
	{
		verem.push(szamok[szuloIndex + 1]);
	}
	else
	{
		muveletek[szuloIndex].jobb = &(muveletek[gyerekIndex]);
		muveletek[gyerekIndex].szulo = &(muveletek[szuloIndex]);
		Epit(gyerekIndex, muveletek, szamok, verem);
	}
	//Ugyanezt vegigjatszuk a bal oldali faagakkal is.
	keresoIndex = szuloIndex - 1;
	gyerekIndex = 0;
	max = 0;
	while (keresoIndex >= 0 && muveletek[szuloIndex].sorrend > muveletek[keresoIndex].sorrend)
	{
		if (max < muveletek[keresoIndex].sorrend)
		{
			max = muveletek[keresoIndex].sorrend;
			gyerekIndex = keresoIndex;
		}
		keresoIndex--;
	}
	if (max == 0)
	{
		verem.push(szamok[szuloIndex]);
	}
	else
	{
		muveletek[szuloIndex].bal = &(muveletek[gyerekIndex]);
		muveletek[gyerekIndex].szulo = &(muveletek[szuloIndex]);
		Epit(gyerekIndex, muveletek, szamok, verem);
	}

}
static void Bejar(Elem ag, stack<double> &verem) //Amiert a fat felepitettuk jobbrol balra, igy hogy a muveleteket megfeleloen osszehangoljuk a veremben talalhato szamokkal, balrol jobbra fogjok bejarni a fat postorder bejaras segitsgevel.
{
	double szam1;
	double szam2;
	double eredmeny;
	if (ag.bal)
	{
		Bejar(*ag.bal, verem);
	}
	szam1 = verem.top();
	verem.pop();
	if (ag.jobb)
	{
		Bejar(*ag.jobb, verem);

	}
	szam2 = verem.top();
	verem.pop();
	if (ag.muvelet == '+')
	{
		eredmeny = szam1 + szam2;
	}
	if (ag.muvelet == '~')
	{
		eredmeny = szam1 - szam2;
	}
	if (ag.muvelet == '*')
	{
		eredmeny = szam1*szam2;
	}
	if (ag.muvelet == '/')
	{
		eredmeny = szam1 / szam2;
	}
	if (ag.muvelet == '^')
	{
		eredmeny = pow(szam1, szam2);
	}
	if (ag.muvelet == 'v')
	{
		if (szam2>=0 && szam1!=0)
		{
			eredmeny = pow(szam2, 1 / szam1);
		}
		else
		{
			eredmeny = 0;
		}
	}
	verem.push(eredmeny);
}
static vector<string> SplitSzam(string s) //Kivesszuk az operandusokat
{
	vector<string> split;
	int segedIndex = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == '+' || s[i] == '~' || s[i] == '*' || s[i] == '/' || s[i] == '^' || s[i] == 'v')
		{
			if (i - segedIndex != 0)//Ha van olyan string, hogy 5++3 akkor a splitbe csak az 5 es a 3 kerul
			{
				split.push_back(s.substr(segedIndex, i - segedIndex));
			}
			segedIndex = i + 1;
		}
	}
	if ((s.length() - 1) >= segedIndex)//Az elotte levo ciklus nem kezeli le a string vegen levo operandusokat
	{
		split.push_back(s.substr(segedIndex, s.length() - segedIndex));
	}
	return split;
}
static vector<double> Szambeallitas(vector<string> splitszam)
{
	vector<double> szamok;
	const double pi = 3.14159265359;
	const double en = 2.718281828459;
	const double mi = 420;
	for (int i = 0; i < splitszam.size(); i++)
	{
		if (splitszam[i] == "pi")
		{
			szamok.push_back(pi);
		}
		else if (splitszam[i] == "en")
		{
			szamok.push_back(en);
		}
		else if (splitszam[i] == "42!")
		{
			szamok.push_back(mi);
		}
		else
		{

			double valto = stod(splitszam[i]);
			szamok.push_back(valto);
		}
	}
	return szamok;
}
static int SorrendBealitas(vector<Elem> &muveletek) //Beallitja a muveletek sorrendjeit a kesobbi feldolgozasra, illetve visszater a fa gyoker elemenek indexevel.
{
	int sorrendErtek = 1;
	int gyokIndex = 0;
	for (int i = 0; i < muveletek.size(); i++)
	{
		if (muveletek[i].muvelet == '^' || muveletek[i].muvelet == 'v')
		{
			muveletek[i].sorrend = sorrendErtek;
			sorrendErtek++;
			gyokIndex = i;
		}
	}
	for (int i = 0; i < muveletek.size(); i++)
	{
		if (muveletek[i].muvelet == '*' || muveletek[i].muvelet == '/')
		{
			muveletek[i].sorrend = sorrendErtek;
			sorrendErtek++;
			gyokIndex = i;
		}
	}
	for (int i = 0; i < muveletek.size(); i++)
	{
		if (muveletek[i].muvelet == '+' || muveletek[i].muvelet == '~')
		{
			muveletek[i].sorrend = sorrendErtek;
			sorrendErtek++;
			gyokIndex = i;
		}
	}
	return gyokIndex;
}
static vector<Elem> SplitMuveletek(string s) //Ez fogja nekunk kivenni a muveleti jeleket a stringbol.
{
	vector<Elem> muveletek;
	Elem e;
	e.bal = NULL;
	e.jobb = NULL;
	e.szulo = NULL;
	e.sorrend = 0;
	for (int i = 0; i < s.length(); i++)
	{

		if (s[i] == '+' || s[i] == '~' || s[i] == '*' || s[i] == '/' || s[i] == '^' || s[i] == 'v')
		{
			e.muvelet = s[i];
			muveletek.push_back(e);

		}

	}

	return muveletek;
}
static bool MuveletiHiba(vector<Elem> muveletek, vector<double> szamok)
{
	bool hiba = false;
	if (muveletek.size() + 1 != szamok.size())
	{
		hiba = true;
	}
	return hiba;
}
static string Szamol(string s)
{
	vector<Elem> muv = SplitMuveletek(s);
	vector<double> szam = Szambeallitas(SplitSzam(s));
	string ered = "";
	if (szam.empty())
	{
		return ered;
	}
	else
	{
		if (MuveletiHiba(muv, szam))
		{
			cout << "///Muveleti hiba!///" << endl;
			return ered;
		}
		else if (szam.size() == 1 && muv.empty())
		{
			ered = to_string(szam[0]);
			return ered;
		}
		else
		{
			int gyokInd = SorrendBealitas(muv);
			Epit(gyokInd, muv, szam, verem);
			Bejar(muv[gyokInd], verem);
			string ered = to_string(verem.top());
			verem.pop();
			return ered;
		}
	}
}
static string Kezel(string s) //Ez a fuggveny felelos a zarojelek kezeleseert.
{
	int zarojelSzam = count(s.begin(), s.end(), '(');
	if(zarojelSzam != count(s.begin(), s.end(), ')'))
	{
		cout << "///Zarojel hiba!///" << endl;
		return "";
	}
	else
	{
		for (int i = 0; i < zarojelSzam; i++)
		{
			int kezdoIndex = s.length() - 1;
			while (s[kezdoIndex] != '(')
			{
				kezdoIndex--;
			}
			int zaroIndex = kezdoIndex;
			while (s[zaroIndex] != ')')
			{
				zaroIndex++;
			}
			string bal = s.substr(0, kezdoIndex);
			string jobb = s.substr(zaroIndex + 1, s.length() - (zaroIndex + 1));
			string kozep = Szamol(s.substr(kezdoIndex + 1, zaroIndex - (kezdoIndex + 1)));
			s = bal + kozep + jobb;
		}
		return Szamol(s);
	}

}
int main()
{
	string s = "";
	cout << "Adj meg egy kifejezest! pl.:2*(3+2)" << endl;
	while (true)
	{
		cout << "< ";
		cin >> s;
		if (s == "exit")
		{
			break;
		}
		cout << "> " << Kezel(s) << endl;
	}
	return 0;
}
