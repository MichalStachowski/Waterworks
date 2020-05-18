//Stachowski_Michal

//Zadanie projektowe polega na wygenerowaniu mapy miast z losowymi sciezkami, sprawdzenie spojnosci powstalej sieci.
//Jeżeli siec jest spojna to należy wygenerować sieć wodociagowa (wodociagi nie moga tworzyc pętli)
// Wykorzystano algorytm DFS (przeszukiwanie w glab).
#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;    

const int N = 100;
const int M = 100;
    
struct siec
{
	int w;
	int k;
	int a;
	int pkt_start;
	int wymiar;
	int los;
	int x;
	int szukany;
	char tab[N][M];
	int macierz[N][M];
	bool odwiedzono[N];
};

void Wymiary(siec *wsk);
void Tworzenie_mapy(siec *wsk);
void Wypis_mapa(siec *wsk);
void Gen_macierz_sasiedztw(siec *wsk);
void Losowanie_sciezek_uzupelnianie_macierzy_sasiedztw(siec *wsk);
void DFS(siec *wsk, int punkt);
void Sprawdzenie_spojnosci(siec *wsk);
void Rysuj_wodociag(siec *wsk, int a);
void Nowa_trasa(siec *wsk, int punkt);


int main()
{
	srand(time(NULL));
	siec *wsk;
	wsk = new siec[1];
	
	Wymiary(wsk);
	Tworzenie_mapy(wsk);	
	Gen_macierz_sasiedztw(wsk);
	Losowanie_sciezek_uzupelnianie_macierzy_sasiedztw(wsk);
	Wypis_mapa(wsk);

	int punkt=wsk->pkt_start;
	for (int i=0; i<4; i++) DFS(wsk, punkt);
	Sprawdzenie_spojnosci(wsk);
	
	for (int i=0; i<wsk->wymiar; i++) wsk->odwiedzono[i] = false;	
	for (int i=0; i<4; i++) Nowa_trasa(wsk, punkt);
	
	Wypis_mapa(wsk);		
	delete wsk;

	return 0;	
}


void Wymiary(siec *wsk)
{
	wsk->a=65;
	wsk->pkt_start=0;
	
	while (true)
	{
		cout << "Podaj ilosc miast w wierszu: ";
	    cin >> wsk->w;
	    cout << "Podaj ilosc miast w kolumnie: ";
	    cin >> wsk->k;
		cout << endl;
		
		if(wsk->w>5 or wsk->k>5 or (!cin))
		{
			while(wsk->w>5 or wsk->k>5 or (!cin))
			{
				cout << "Wprowadzona zostalo nieprawidlowa wartosc. (nalezy wprowadzic wartosc od 1 do 5)\n\n";
				cin.clear();
				cin.sync();	
				break;
			}
		}
		else break;
	}	
}


void Tworzenie_mapy(siec *wsk)
{
	for (int i=0; i<wsk->w*2; i++)
    {
        for (int j=0; j<wsk->k*5; j++)
        {
            if(i%2==0 and j%5==0)
            {
                wsk->tab[i][j] = wsk->a;
                wsk->a++;
            }
            else wsk->tab[i][j] = 32;
        }
    }	
}


void Wypis_mapa(siec *wsk)
{
	for (int i=0; i<wsk->w*2; i++)
    {
        for (int j=0; j<wsk->k*5; j++)
        {
            cout << wsk->tab[i][j];
        }
    cout << endl;
    }	
}


void Gen_macierz_sasiedztw(siec *wsk)
{
	wsk->wymiar=wsk->w*wsk->k;
    for(int i=0; i<wsk->wymiar; i++)
    {
        for (int j=0; j<wsk->wymiar; j++)    wsk->macierz[i][j]=0;
    }
    
   	for (int i=0; i<wsk->wymiar; i++) wsk->odwiedzono[i] = false;
}


void Losowanie_sciezek_uzupelnianie_macierzy_sasiedztw(siec *wsk)
{
	for (int i=0; i<wsk->w*2-1; i++)
    {    
        for (int j=0; j<wsk->k*5-4; j++)
        {
            wsk->los=rand()%4;
            if(i%2==0)
            {      
                if(wsk->tab[i][j]==32)
                {                  
                    wsk->x=((i/2)*wsk->k)+(j/5);
                    if (j%5==1 and wsk->los!=1)
                    {
                        wsk->tab[i][j] = 45;
                        wsk->macierz[wsk->x][wsk->x+1]=1;
                        wsk->macierz[wsk->x+1][wsk->x]=1;
                    }                  
                    if (wsk->tab[i][j-1]==45 and wsk->tab[i][j+1]==32) wsk->tab[i][j]=45;
                    if (wsk->tab[i][j-1]==45 and wsk->tab[i][j+1]>65) wsk->tab[i][j]=45;              
                }
            }
            else
            {
                if(wsk->tab[i][j]==32)
                {
                    if (j%5==0 and wsk->los!=1)
                    {
                        wsk->tab[i][j]=124;
                        wsk->x=((i/2)*wsk->k)+(j/5);
                        wsk->macierz[wsk->x][wsk->x+wsk->k]=1;
                        wsk->macierz[wsk->x+wsk->k][wsk->x]=1;
                    }
                }
            }
        }
    }	
}


void DFS(siec *wsk, int punkt)
{
	wsk->odwiedzono[punkt] = true;
	for(int i=0; i<wsk->wymiar; i++)
	{
		
		if(wsk->macierz[punkt][i]==1 and wsk->odwiedzono[i]==false)	
		{
			punkt=i;
			DFS(wsk, punkt);		
		}
	}
}


void Sprawdzenie_spojnosci(siec *wsk)
{
	int spojnosc=0;
   	for(int i=0; i<wsk->wymiar; i++)
   	{  		
		if(wsk->odwiedzono[i]==false) 
	   	{
		   cout << "\nPowyzsza siec nie jest spojna.\n";
		   system("pause");
		   exit(0);
		}
		else spojnosc++;		
		if(spojnosc==wsk->wymiar) cout << "Siec jest spojna.\n\n";
	}
}


void Rysuj_wodociag(siec *wsk, int a)
{
	for (int i=0; i<wsk->w*2-1; i++)
    {    
        for (int j=0; j<wsk->k*5-4; j++)
        {
    		if(a==1 and wsk->tab[i][j]==wsk->szukany) 
			{
				wsk->tab[i][j+1]=61;
				wsk->tab[i][j+2]=61;
				wsk->tab[i][j+3]=61;
				wsk->tab[i][j+4]=61;
			}
			else if(a==2 and wsk->tab[i][j]==wsk->szukany) 
			{
				wsk->tab[i][j-1]=61;
				wsk->tab[i][j-2]=61;
				wsk->tab[i][j-3]=61;
				wsk->tab[i][j-4]=61;
			}
			else if(a==3 and wsk->tab[i][j]==wsk->szukany) wsk->tab[i+1][j+1]=124;
			else if(a==4 and wsk->tab[i][j]==wsk->szukany)	wsk->tab[i-1][j+1]=124;			   	
    	}
	}
}


void Nowa_trasa(siec *wsk, int punkt)
{
	wsk->odwiedzono[punkt] = true;
	for(int i=0; i<wsk->wymiar; i++)
	{		
		
		if(wsk->macierz[punkt][i]==1 and wsk->odwiedzono[i]==false)	
		{
			if(i-punkt==1)
			{
				wsk->szukany = 65+punkt;
				Rysuj_wodociag(wsk, 1);
			}
			else if (punkt-i==1)
			{
				wsk->szukany = 65+punkt;
				Rysuj_wodociag(wsk, 2);
			}
			else if (i-punkt==wsk->k)
			{
				wsk->szukany = 65+punkt;
				Rysuj_wodociag(wsk, 3);
			}
			else if (punkt-i==wsk->k)
			{
				wsk->szukany = 65+punkt;
				Rysuj_wodociag(wsk, 4);
			} 
			punkt=i;
			Nowa_trasa(wsk, punkt);		
		}
	}
}
