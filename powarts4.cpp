#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <iterator>
//#include <tuple>

using namespace std;

typedef struct nodo{
  vector<pair<int, int> > vic;
  bool visited;
  bool alreadyEnq = false;
} nodo;

typedef struct SPTnodo{
  int distanza;     //distanza minima al momento
  int padre;    //padre nell'albero SPT
  set<int> figli;
  
  SPTnodo(int d) {   //costruttore
      distanza = d;
  }

  SPTnodo(int d, int p) {   //costruttore
      distanza = d;
      padre = p;
  }
} SPTnodo;

vector<nodo> grafo;
vector<SPTnodo> SPTree;
//tuple<int, int, int> tpl;

int contaFigli(int nodo, vector<int>* rispostaFigli) {
    rispostaFigli->push_back(nodo);
    if (SPTree[nodo].figli.size() == 0) return 1;
    int contatore=0;
    for (int figlio : SPTree[nodo].figli) {
        //cout<<figlio<<" ";
        contatore += contaFigli(figlio, rispostaFigli);
    }
    //cout<<endl;
    return contatore+1;
}

int main() {
    ifstream in("input.txt");
    int N, M, POWARTS;
    in >> N >> M >> POWARTS;
    grafo.resize(N);

    //popolazione grafo
    for(int j=0; j<M; j++) {
        int from, to, cost;
        in >> from >> to >> cost;

        grafo[from].vic.push_back(make_pair(to,cost));
        grafo[to].vic.push_back(make_pair(from,cost));
    }
   
    //creazione albero (SPT)
        //creazione coda
        //aggiunta powarts a coda
        //aggiunta figli di powarts e inserimento in albero

    
    priority_queue<pair<int, int> , vector<pair<int, int> >, greater<pair<int, int> > > q; //priority queue con first la distanza e second il nodo
    //queue<int> q;
    q.push(make_pair(0,POWARTS));
    int nodoA,nodoB;

    SPTree.resize(N, SPTnodo(-1,-1));  //inizializza tutti gli SPTnodi a distanza -1 (== null)
    SPTnodo* pow = new SPTnodo(0,-1);  //inizializza il nodo pow con distanza 0
    SPTree[POWARTS] = *pow;            // assegna pow alla sua posizione in SPTree

    
    while(!q.empty()) {
        nodoA = q.top().second;
        if (grafo[nodoA].alreadyEnq)
        {
            q.pop();
            continue;
        }
        
        q.pop();
        grafo[nodoA].alreadyEnq = true;
        
        grafo[nodoA].visited = true;
        for (pair<int, int> adiac : grafo[nodoA].vic)   //per ogni nodo adiacente al nodo in cima alla coda (quello piu vicino a NodoA)
        {
            nodoB = adiac.first;
            int distanzaAB = adiac.second;
            int appoggio = 0;
            int nodoBoriginale = nodoB;

            //grafo[nodoB].vic.erase(make_pair(nodoA, distanzaAB));
            if(grafo[nodoB].visited == false){
                //cout << nodoA << "," << nodoB <<endl;
                if(nodoA != POWARTS)
                {
                    if (SPTree[nodoA].distanza > SPTree[nodoB].distanza && SPTree[nodoB].distanza > -1)
                    {
                        swap(nodoA, nodoB);
                    }

                }
                //ora sono sicuro che A ha distanza minore di B, solo in A possiamo trovare POWARTS e B potrebbe non essere inserito
                if(SPTree[nodoB].padre!=nodoA && SPTree[nodoA].padre!=nodoB)
                {
                    if(SPTree[nodoB].distanza == -1)   //inserimento
                    {
                        SPTree[nodoA].figli.insert(nodoB);                                  //aggiungo il figlio al nodo A
                        SPTree[nodoB].padre = nodoA;                                        //aggiungo il padre al nodo B
                        SPTree[nodoB].distanza = SPTree[nodoA].distanza + distanzaAB;       //aggiorno il valore di distanza in B
                        
                    }
                    else if(SPTree[nodoA].distanza + distanzaAB < SPTree[nodoB].distanza)   //cammino minimo migliore
                    {
                        SPTree[nodoA].figli.insert(nodoB);                                  //aggiungo il figlio al nodo A  
                        int padreAttuale =  SPTree[nodoB].padre;                            //rimuovo A dai figli dell'attuale padre
                        SPTree[padreAttuale].figli.erase(nodoB);                            //
                        SPTree[nodoB].padre = nodoA;                                        //aggiorno il valore di distanza in B
                        SPTree[nodoB].distanza = SPTree[nodoA].distanza + distanzaAB;       //
                    }
                    else if(SPTree[nodoA].distanza + distanzaAB == SPTree[nodoB].distanza )  //percorso alternativo
                    {
                        int padreAttuale =  SPTree[nodoB].padre;                            //rimuovo A dai figli dell'attuale padre
                        SPTree[padreAttuale].figli.erase(nodoB);                            //
                        set<int> padri;
                        int padreAttuale1 = padreAttuale;
                        int padreAttuale2 = nodoA;
                        int padreInComune = POWARTS;
                        while (padreAttuale1 != POWARTS || padreAttuale2 != POWARTS)
                        {
                            if(padreAttuale1 != POWARTS) {
                                if (!padri.insert(padreAttuale1).second)
                                {
                                    padreInComune = padreAttuale1;
                                    break;
                                }
                                padreAttuale1 = SPTree[padreAttuale1].padre;
                            }
                            if (padreAttuale2 != POWARTS) 
                            {
                                if (!padri.insert(padreAttuale2).second)
                                {
                                    padreInComune = padreAttuale2;
                                    break;
                                }
                                padreAttuale2 = SPTree[padreAttuale2].padre;
                            }
                        }                                                                       
                        padreAttuale = padreInComune; 
                        //padreAttuale =   SPTree[nodoA].padre;                               //
                        SPTree[nodoB].padre = padreAttuale;                                 //imposto B come fratello di A
                        SPTree[padreAttuale].figli.insert(nodoB);
                    }
                }
            }
        //if(grafo[nodoB].visited == false) {q.push(make_pair(distanzaAB, nodoB)); grafo[nodoB].visited = true;}        //da controllare 
        
        if(grafo[nodoBoriginale].visited == false) {q.push(make_pair(SPTree[nodoBoriginale].distanza, nodoBoriginale)); }

       }    
    }

    //albero corretto, dobbiamo calcolare il risultato e riportarlo
    ofstream out("output.txt");
    int max = 0;
    int tempFigli;
    int figlioMax;
    vector< pair<int, vector<int> > > maxFigli;
    vector<int> rispostaTemp;
    int tot = 0;

    for (int figlio1 : SPTree[POWARTS].figli)
    {
        maxFigli.push_back(make_pair(contaFigli(figlio1, &rispostaTemp), rispostaTemp));
        //cout<<figlio1<<" "<<rispostaTemp.size()<<endl;
        tot += rispostaTemp.size();
        rispostaTemp.clear();     
    }
    
    //cout<<"TOT: "<<tot<<endl;

    for (pair<int, vector<int> > x : maxFigli) {
        if (max < x.first)
        {
            max = x.first;
            rispostaTemp = x.second;
            
        }
        
    }
    //cout<< max <<endl;
    out << max << endl;
    //out << figlioMax << endl;

    for (int figlio : rispostaTemp)
    {
        out<<figlio<<endl;
        //cout<<figlio<<endl;
        
        
    }

      //stampa di debug - SPTree
    /* for (int i = 0; i < N; i++)
    {

        
        cout << "Nodo: "<< i <<"  distanza: " << SPTree[i].distanza << ", padre : " << SPTree[i].padre << " , " << " figli: ";// << SPTree[i].counterFigli<<endl; //", " << SPTree[i].padre->distanza << SPTree[i].padre->padre << endl;
        for (int f : SPTree[i].figli)
        {
            cout<< f << " , ";
        }
        cout<<endl;
        
    }  */

    return 0;
}