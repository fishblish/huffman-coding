#include <stdlib.h>
#include <stdio.h>

#define NMAX 256

char fn1[] = "src1.txt";
char fn2[] = "zapis.txt";

typedef struct wierzcholek{
    char co;
    int ile;
    int flaga;
    int num;
    struct wierzcholek *lewy, *prawy;
    }wierzcholek, *pwierzcholek;

typedef struct {
    int first;
    int tab[NMAX];
    void *data;
    }Prioq,*pPrioq;

pwierzcholek root;

wierzcholek wszystkie[2*NMAX-1];
int a=0;
int wysokosc;

void inicjuj_drzewo()
{
    root=NULL;
}

void inicjuj_kolejka(Prioq *q, void *data)
{
    q->first=0;
    q->data = wszystkie;
}


void wstaw_kolejka(Prioq *q,wierzcholek ver, int *a )
{
  wierzcholek *wszystkie;

  wszystkie = (wierzcholek*)q->data;
    if(q->first<NMAX && *a < 2*NMAX-1){
        q->tab[q->first++] = *a;
        wszystkie[(*a)++] = ver;

    }
    else
        printf("Kolejka pelna");
}    

int wiekszyPrio ( void *data, int x, int y )
{
    wierzcholek *wszystkie;

    wszystkie = (wierzcholek*)data;
    if((&wszystkie[x])->ile < (&wszystkie[y])->ile)
        return 1;
    if(((&wszystkie[x])->co==-1)&&((&wszystkie[y])->co!=-1)&& ((&wszystkie[x])->ile == (&wszystkie[y])->ile)) /*jako co=-1 oznaczam wierzcholki*/
        return 1;
    return 0;
     
}

int wyjmij_min(Prioq *q)
{
    int i,j,z;
    if(q->first > 0)
    {
        for(j=0,i=1; i < q->first; i++)
            if(wiekszyPrio(q->data, q->tab[i],q->tab[j]))
                j=i;
        
        z=q->tab[j];
        
        q->tab[j]=q->tab[--q->first];
       
        return z;
        
    }
    else
        printf("Kolejka jest pusta");
        
  return -1;
}



void liczenie_liter(int znak[],char *nazwa)
{
    int i;
    char litera;
    FILE *plik;

    for(i=0;i<NMAX;i++)
        znak[i]=0;
    plik=fopen(nazwa,"r");
    if(plik==NULL)
        printf("Blad otwarcia\n");
    else
    {
        
        while(fscanf(plik,"%c",&litera)!=EOF){
            znak[(int)litera]++;
        }
        fclose(plik);
        
        for(i=0;i<NMAX;i++)
            if(znak[i]!=0)
                printf("%c %d\n",i,znak[i]);

    }
}
int wierzcholki_z_liter(int znaki[],pPrioq q){
    int i,j;
    wierzcholek liscie[NMAX];
    j=0;
    for(i=0;i<NMAX;i++)
    {
               
        if(znaki[i]!=0)
        {
            (&liscie[j])->co=i;
            (&liscie[j])->ile=znaki[i];
            (&liscie[j])->flaga=0;
            (&liscie[j])->lewy=NULL;
            (&liscie[j])->prawy=NULL;
            liscie[j].num = j;
            wstaw_kolejka(q,liscie[j], &a);
            j++;
        }
    }
    return j;
}

void polacz(Prioq *q)
{
    int min1,min2;
    wierzcholek nowy;
    
    min1=wyjmij_min(q);
    min2=wyjmij_min(q);
    (&nowy)->co=-1;
    (&nowy)->ile=(&wszystkie[min1])->ile+(&wszystkie[min2])->ile;
    (&nowy)->flaga=0;
    (&nowy)->lewy=&wszystkie[min1];
    (&nowy)->prawy=&wszystkie[min2];
    wstaw_kolejka ( q, nowy, &a );
}

int znajdz_wysokosc ( wierzcholek *w )
{
  int w1, w2;

  if ( !w )
    return 0;
  w1 = znajdz_wysokosc ( w->lewy );
  w2 = znajdz_wysokosc ( w->prawy );
  return (w1 > w2 ? w1 : w2) + 1;
} /*znajdz_wysokosc*/

#define IND(i,j) ((i)*(wysokosc+2)+(j))

void WypiszKod ( int tab[], int i )
{
  int j;

  printf ( "%3d (%c): ", tab[IND(i,0)],
           tab[IND(i,0)] > 31 ? tab[IND(i,0)] : ' ' );
  for ( j = 1; tab[IND(i,j)] != -1; j++ )
    printf ( "%1d", tab[IND(i,j)] );
  printf ( "\n" );
} /*WypiszKod*/

void znajdz_kod(int n,int tab[])
{
    int i,j;
    pwierzcholek krok;

    for(i=0;i<n;i++)
        for(j=0; j <= wysokosc+1; j++)
            tab[IND(i,j)] = -1;
    for(i=0;i<n;){
        krok=root;
        j=1;
        while(krok->lewy!=NULL){
            if(krok->lewy->flaga==0){
                krok=krok->lewy;
                tab[IND(i,j++)]=0;
            }
            else{
                if(krok->prawy->flaga==0){
                    krok=krok->prawy;
                    tab[IND(i,j++)]=1;
                }
                else{
                    krok->flaga=1;
                    break;
                    
                    }
            }
        }
        
        if(krok->lewy==NULL){
            krok->flaga=1;
            tab[IND(i,0)]=krok->co;
          WypiszKod ( tab, i );
            i++;
            
        }
    }        
}

void r_znajdz_kod2 ( wierzcholek *w, int poziom, int tt[], int tab[] )
{
  int i, j;

  if ( w->lewy && w->prawy ) {
    tt[poziom] = 0;
    r_znajdz_kod2 ( w->lewy, poziom+1, tt, tab );
    tt[poziom] = 1;
    r_znajdz_kod2 ( w->prawy, poziom+1, tt, tab );
  }
  else {
    i = w->num;
    tab[IND(i,0)] = w->co;
    for ( j = 0; j < poziom; j++ )
      tab[IND(i,j+1)] = tt[j];
    WypiszKod ( tab, i );
  }
} /*r_znajdz_kod2*/

void znajdz_kod2 ( int n, int tab[] )
{
  int i, j;
  int *tt;

  for ( i = 0; i < n; i++ )
    for ( j=0; j <= wysokosc+1; j++ )
      tab[IND(i,j)] = -1;

  tt = malloc ( wysokosc*sizeof(int) );
  r_znajdz_kod2 ( root, 0, tt, tab );
  free ( tt );
} /*znajdz_kod2*/

void odkoduj()
{
    pwierzcholek krok=root;
    char droga;
    
    FILE *plik;
    plik=fopen(fn2,"r");
    if(plik==NULL)
        printf("Blad otwarcia\n");
    else{
        
        while(fscanf(plik,"%c",&droga)!=EOF){
            
            if(krok->lewy!=NULL){
                if(droga=='0')
                    krok=krok->lewy;
                else
                    krok=krok->prawy;
            
                if(krok->lewy==NULL){
                    printf("%c",krok->co);
                    krok=root;
                }
            }
        }
        fclose(plik);
    }
}
           
void zakoduj(int n,int kod[])
{
    int i,j;
    char litera;
    FILE *plik1;
    FILE *plik2;
    plik1=fopen(fn2,"w");
    plik2=fopen(fn1,"r");
    if(plik1==NULL || plik2==NULL)
        printf("Blad otwarcia\n");
    else
    {
        while(fscanf(plik2,"%c",&litera)!=EOF){
            for(i=0;i<n;i++){
                if(litera==kod[IND(i,0)]){
                    printf("%c ",kod[IND(i,0)]);
            
                    for(j=1;kod[IND(i,j)]!=-1;j++){
                        printf("%d",kod[IND(i,j)]);
                        fprintf(plik1,"%d",kod[IND(i,j)]);
                    }
                    printf("\n");
                }
            }
/*            fprintf ( plik1, "\n" );*/

        }
        fclose(plik1);
        fclose(plik2);
    }
}
int main()
{
    int n;
    int znaki[NMAX];
    char *nazwa_pliku=fn1;
    Prioq kolejka;
    int *kod;
 
    inicjuj_kolejka(&kolejka, wszystkie);
    inicjuj_drzewo();
    liczenie_liter(znaki,nazwa_pliku);
    
    n=wierzcholki_z_liter(znaki,&kolejka);
    kod = malloc ( n*(n+1)*sizeof(int) );
    while((&kolejka)->first>1){
      polacz(&kolejka);
    }
    
    root=&wszystkie[(&kolejka)->tab[0]];
    wysokosc = znajdz_wysokosc ( root );
    printf ( "wysokosc = %d\n", wysokosc );    
    
    znajdz_kod2(n,kod);
    zakoduj(n,kod);
    odkoduj();

    return 0;
}

