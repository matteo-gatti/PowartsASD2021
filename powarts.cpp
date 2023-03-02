#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <iterator>

using namespace std;

typedef struct nodo{
  vector<pair<int, int> > vic;
  bool visited;
} nodo;

typedef struct SPTnodo{
  int distanza;     //distanza minima al momento
  SPTnodo* padre;    //padre nell'albero SPT
  set<int> figli;
  
  SPTnodo(int d) {   //costruttore
      distanza = d;
  }

  SPTnodo(int d, SPTnodo* p) {   //costruttore
      distanza = d;
      padre = p;
  }
} SPTnodo;

vector<nodo> grafo;
vector<SPTnodo> SPTree;

int main() {
    ifstream in("input19.txt");
    int N, M, POWARTS;
    in >> N >> M >> POWARTS;
    grafo.resize(N);

    //popolazione grafo
    for(int j=0; j<M; j++) {
        int from, to, cost;
        in >> from >> to >> cost;
        grafo[from].vic.push_back(make_pair(to,cost));
        grafo[to].vic.push_back(make_pair(from,cost)); //aggiunta link anche al contrario
    }

    /* stampa per debug del vettore di adiacenza
    for(int j=0; j<N; j++) {
        for(pair<int, int> x : grafo[j].vic) {
            int v = x.first;
            int w = x.second;
            cout << "Nodo: "<< j <<"  " << v << " " << w << endl;
        }
    }*/

    
    //creazione albero (SPT)
        //creazione coda
        //aggiunta powarts a coda
        //aggiunta figli di powarts e inserimento in albero

    
    queue<int> q;
    q.push(POWARTS);
    int nodoAttuale;

    SPTree.resize(N, SPTnodo(-1));  //inizializza tutti gli SPTnodi a distanza -1 (== null)
    SPTnodo* pow = new SPTnodo(0);   
    SPTree[POWARTS] = *pow;
    pow = &SPTree[POWARTS];
    set<int> tempFigli;
    
    while(!q.empty()) {
        nodoAttuale = q.front();
        q.pop();
        
        grafo[nodoAttuale].visited = true;
        for (pair<int, int> adiac : grafo[nodoAttuale].vic)   //per ogni nodo adiacente al nodo in cima alla coda
        {
            int nodoAdiacente = adiac.first;
            int DistanzaAdiacAttuale = adiac.second;
            
            if(!(SPTree[nodoAttuale].padre == &SPTree[nodoAdiacente] && SPTree[nodoAttuale].distanza != -1 )){

            // percorso nodoAttuale - nodoAdiacente
            if(SPTree[nodoAdiacente].distanza == -1) //se è NULL e quindi non sono ancora nell'albero
            {
                //inserimento
                SPTree[nodoAdiacente].distanza = DistanzaAdiacAttuale + SPTree[nodoAttuale].distanza; //= new SPTnodo(DistanzaAdiacAttuale + SPTree[nodoAttuale].distanza, &SPTree[nodoAttuale]);
                SPTree[nodoAdiacente].padre = &SPTree[nodoAttuale]; 
                SPTree[nodoAdiacente].padre->figli.insert(nodoAdiacente);
                
               /* SPTnodo* father = SPTree[nodoAdiacente].padre;          //aggiunta dei figli ai diversi padri, ricorsivamente
                while(father != pow->padre)
                {                    
                    father->figli.insert(nodoAdiacente);
                    father = father->padre;
                }*/
                
            }
            else
            {
                //confrontiamo
                if(SPTree[nodoAdiacente].distanza  >  DistanzaAdiacAttuale + SPTree[nodoAttuale].distanza)  //caso in cui il nuovo valore di distanza sia minore
                {
                    SPTree[nodoAdiacente].distanza = DistanzaAdiacAttuale + SPTree[nodoAttuale].distanza;
                    //sottrazione figli

                    //vettore figli + adiacente
                    tempFigli = SPTree[nodoAdiacente].figli;
                    tempFigli.insert(nodoAdiacente);

                    //sottrai a padre vecchio
                    if (SPTree[nodoAdiacente].padre->figli.find(nodoAdiacente)==SPTree[nodoAdiacente].padre->figli.end())
                    {
                        auto it = tempFigli.begin();
                        while (it != tempFigli.end())
                        {
                            SPTree[nodoAdiacente].padre->figli.erase(*it);
                            it++;
                        }
                    }                    

                    //cambio padre
                    SPTree[nodoAdiacente].padre = &SPTree[nodoAttuale];

                    //agginugi a padre nuovo
                    SPTree[nodoAdiacente].padre->figli.insert(tempFigli.begin(), tempFigli.end());

                  /*  SPTnodo* father = SPTree[nodoAdiacente].padre;          //aggiunta dei figli ai diversi padri, ricorsivamente
                    while(father != pow->padre)
                    {                    
                        father->figli.insert(tempFigli.begin(), tempFigli.end());
                        father = father->padre;
                    }*/

                   
                }
                else if(SPTree[nodoAdiacente].distanza  ==  DistanzaAdiacAttuale + SPTree[nodoAttuale].distanza && SPTree[nodoAdiacente].padre != &SPTree[nodoAttuale]) // caso in cui il nuovo valore di distanza sia uguale, dobbiamo portare su il nodo adiacente come figlio del padre di suo padre
                {
                    //sottrazione figli

                    //vettore figli + adiacente
                    tempFigli = SPTree[nodoAdiacente].figli;
                    tempFigli.insert(nodoAdiacente);

                    //sottrai a padre vecchio
                    auto it = tempFigli.begin();
                    while (it != tempFigli.end())
                    {
                        SPTree[nodoAdiacente].padre->figli.erase(*it);
                        it++;
                    }

                    //cambio padre
                    SPTree[nodoAdiacente].padre = SPTree[nodoAttuale].padre;

                    //agginugi a padre nuovo
                    SPTree[nodoAdiacente].padre->figli.insert(tempFigli.begin(), tempFigli.end());

                }            
                else if(nodoAdiacente != POWARTS && SPTree[nodoAdiacente].padre != &SPTree[nodoAttuale])
                {//percorso nodoAdiacente - nodoAttuale   
                        if(SPTree[nodoAttuale].distanza  >  DistanzaAdiacAttuale + SPTree[nodoAdiacente].distanza)  //caso in cui il nuovo valore di distanza sia minore
                        {
                            SPTree[nodoAttuale].distanza = DistanzaAdiacAttuale + SPTree[nodoAdiacente].distanza;

                            //vettore figli + adiacente
                            tempFigli = SPTree[nodoAttuale].figli;
                            tempFigli.insert(nodoAttuale);
                            //sottrai a padre vecchio
                            
                            if (SPTree[nodoAttuale].padre->figli.find(nodoAttuale)==SPTree[nodoAttuale].padre->figli.end())
                            {
                                auto it = tempFigli.begin();
                                while (it != tempFigli.end())
                                {
                                    SPTree[nodoAttuale].padre->figli.erase(*it);
                                    it++;
                                }
                            }

                            //cambio padre
                            SPTree[nodoAttuale].padre = &SPTree[nodoAdiacente];

                            //agginugi a padre nuovo
                            SPTree[nodoAttuale].padre->figli.insert(tempFigli.begin(), tempFigli.end());
                            
                           /* SPTnodo* father = SPTree[nodoAttuale].padre;          //aggiunta dei figli ai diversi padri, ricorsivamente
                            while(father != pow->padre)
                            {                    
                                father->figli.insert(tempFigli.begin(), tempFigli.end());
                                father = father->padre;
                            }*/

                        }
                        else if(SPTree[nodoAttuale].distanza  ==  DistanzaAdiacAttuale + SPTree[nodoAdiacente].distanza && SPTree[nodoAttuale].padre != &SPTree[nodoAdiacente]) // caso in cui il nuovo valore di distanza sia uguale, dobbiamo portare su il nodo adiacente come figlio del padre di suo padre
                        {
                            //sottrazione figli

                            //vettore figli + adiacente
                            tempFigli = SPTree[nodoAttuale].figli;
                            tempFigli.insert(nodoAttuale);

                            //sottrai a padre vecchio
                            auto it = tempFigli.begin();
                            while (it != tempFigli.end())
                            {
                                SPTree[nodoAttuale].padre->figli.erase(*it);
                                it++;
                            }

                            //cambio padre
                            SPTree[nodoAttuale].padre = SPTree[nodoAdiacente].padre;

                            //agginugi a padre nuovo
                            SPTree[nodoAttuale].padre->figli.insert(tempFigli.begin(), tempFigli.end());

                        }
                }

            }
            if(grafo[nodoAdiacente].visited == false) {q.push(nodoAdiacente); grafo[nodoAdiacente].visited = true;}
            }
        }
        
    }

    //albero corretto, dobbiamo calcolare il risultato e riportarlo
    ofstream out("output.txt");
    int max = 0;
    set<int>* ptrFigli;
    int figlioMax;
    for (int figlio : SPTree[POWARTS].figli)
    {
        if (SPTree[figlio].padre == pow)
        {
            if (SPTree[figlio].figli.size() > max)
            {
                max = SPTree[figlio].figli.size();
                ptrFigli = &(SPTree[figlio].figli);
                figlioMax = figlio;
            }
            
        }
        
    }
    out << max+1 << endl;
    out << figlioMax << endl;

    for (int figlio : SPTree[figlioMax].figli)
    {
        out<<figlio<<endl;
    }


    //stampa di debug - SPTree
    for (int i = 0; i < N; i++)
    {
        cout << "Nodo  "<< i <<":  " << SPTree[i].distanza << ", " << SPTree[i].padre << "  , puntatore= " << &SPTree[i] <<" figli: ";// << SPTree[i].counterFigli<<endl; //", " << SPTree[i].padre->distanza << SPTree[i].padre->padre << endl;
        for (int f : SPTree[i].figli)
        {
            cout<< f << " - ";
        }
        cout<<endl;
        
    }
    return 0;
}