#include <vector>
#include <iostream>
#include <math.h>
#include <list>
#include <algorithm>
typedef float Acceptor;
typedef float Visitor;
typedef float BoundedItem;

using namespace std; 
#define dimensions 91
class NodeSS;
 
struct VisitorClass
{
	int count;
	bool ContinueVisiting;
	std::list<NodeSS> result;

	VisitorClass() : count(0), ContinueVisiting(true) {};

	void operator()(const NodeSS* leaf)
	{
	    result.push_back(leaf->leaf);
	}
};


class SSElem{
public:
	NodeSS *child;
	vector<float> data;
	 SSElem(){child=NULL;}
	 void insertDate(float date[]){
	 for(int i=0;i<dimensions;i++)
	 	{
			data.push_back(date[i]);
		}

	 }

};

class NodeSS{

public:
    int leaf;
	int total_children;
	int height;
	float Radius;
	float  Variance;//raiz cuadrada de distancia entre dos circulos
	SSElem  *centroid;
	list <float> items;
	int  m_level;
	int m_count;
	int MAX;
	vector<SSElem*> Entry;
	NodeSS(){
		Entry.resize(5);
		total_children=0;
	 	height=0;
	 	Radius=0;
	 	m_level=0;
	 	 m_count=0;
	 	centroid=NULL;
	}
	NodeSS(int maxE)
	{
     MAX=maxE;
     Entry.resize(MAX);
     total_children=0;
	 height=0;
	 Radius=0;
	 m_level=0;
	 m_count=0;
	 centroid=NULL;
	}
	void clean(){
	     total_children=0;
		 height=0;
		 Radius=0;
		 m_level=0;
		 m_count=0;
		 centroid=NULL;
		 for(int i=0;i<MAX;i++){
		 	Entry[i]=NULL;
		 }
	}
	bool isLeaf(){ return (m_level == 0); } // leaf=0 es hoja 
};
class SSTree{
public:
	NodeSS * root;
	int dimDate;//dimension de la data;
	int MAXDatos;
	SSTree()
	{
		root=NULL;
	}
    SSTree(int dimensionData,int maxNode){
    	dimDate=dimensionData;
    	MAXDatos=maxNode;
    	root=NULL;

    }
    void insert(float date[dimensions]);
    void searchRange(int date);
    Visitor query(const Acceptor &accept, Visitor visitor);


    private:
     vector<NodeSS*> camino;

    void chooseLeaf(SSElem *elemento,NodeSS*& p);
    SSElem* calcularCentroide(NodeSS*p);
    float distanciaEucleadana(SSElem *elementoU,SSElem *elementoD);
    float calcularRadio(NodeSS*p);
    void split(vector<SSElem*> Entry,SSElem *centro,NodeSS *padre);
};

void SSTree::chooseLeaf(SSElem *elemento,NodeSS*& p){       
		 p=root;
        float auxArea, minArea, tmpArea, tmpEnlargement,enlargement;
        camino.push_back(root);
      
        while ((p)and (not p->isLeaf())){
                //camino.push_back(p);
        	    //break();
                //enlargement=p->m_entry[0]->m_mbr->calEnlargement(*mbr);
                /*for(int i=1;i<p->m_count;i++)
                {
                    tmpEnlargement=p->m_entry[i]->m_mbr->calEnlargement(*mbr);
                    if(tmpEnlargement < enlargement) {
                            enlargement = tmpEnlargement;
                         index = i;
                    }
                    else if(tmpEnlargement == enlargement){
                            enlargement = tmpEnlargement;
                            index = i;
                    }

                 }
                 //cout<<" estoy refornando esto.."<<endl;
                p=p->m_entry[index]->m_child;
                indices.push_back(index);
                camino.push_back(p);	*/

        }
}
SSElem* SSTree::calcularCentroide(NodeSS*p){
	float  centro[dimensions];
	for(int i=0;i<dimensions;i++){centro[i]=0;}
	SSElem *tmp;

	for(int i=0;i<p->m_count;i++)//numero de datos 
	{
        for(int j=0;j<dimensions;j++)
        {
        	centro[j]=(centro[j]+p->Entry[i]->data[j])/2;
        }
	} 
	tmp=new SSElem();
	tmp->insertDate(centro);
    return tmp;
}
float SSTree::distanciaEucleadana(SSElem *elementoU,SSElem *elementoD)
{  float suma=0;
	for(int i=0;i<dimensions;i++)
	{

      suma=suma + (elementoD->data[i]-elementoU->data[i]);
	}
	return sqrt(suma);

}
 float SSTree::calcularRadio(NodeSS*p){
	float radio=0,distancia=0;
       for(int j=0;j<p->m_count;j++)
       {
          distancia=distanciaEucleadana(p->centroid,p->Entry[j]);
          if(distancia>radio){
          	 radio=distancia;
          	}
        }
    return radio;
}
void SSTree::insert(float date[dimensions]){
 		NodeSS *p;
 		SSElem *elemento=new SSElem();
 		elemento->insertDate(date);
 		chooseLeaf(elemento,p);

 		if(!p){//LA PRIMERA VEZ NO HAY NADA
 			root=new NodeSS(MAXDatos);
 			root->Entry[root->m_count]=elemento;
 			root->m_count+=1;
 			root->centroid=elemento;
 			root->Radius=0;
 			root->Variance=0;
 			root->height=1;
 			root->total_children=1;
 		}
 		else if(p->m_count<MAXDatos)//CUANDO ES MENOR hay espacio 
 		{
 			p->Entry[p->m_count]=elemento;
 			p->m_count+=1;
 			p->centroid=calcularCentroide(p);
 			p->Radius=calcularRadio(p);
 		}
 		else//esta lleno el nodo
 		{
           p->m_level+=1;
           p->m_count+=1;

           split(p->Entry,elemento,p);
           //cout<<" p_m_count"<<p->m_level<<endl;
   

 		}
}
void SSTree::split(vector<SSElem*> Entry,SSElem *centro,NodeSS *padre){
      vector<SSElem*> entradas;
      SSElem *tmp1,*tmp2;
      int distancia,distanciaActual,indexX,indexY;

      for(int i=0;i<Entry.size();i++){ entradas.push_back(Entry[i]);}
      entradas.push_back(centro);
      for(int i=0;i<entradas.size();i++)
      {
      	for(int j=i+1;j<entradas.size();j++)
      	{
         distancia=distanciaEucleadana(entradas[i],entradas[j]);
         if(distancia>distanciaActual)
         	distanciaActual=distancia;
            tmp1=entradas[i];
            tmp2=entradas[j];
            indexX=i;
            indexY=j;

      	}
      }
      entradas.erase( entradas.begin()+ indexX); 
      entradas.erase( entradas.begin()+ indexY);
      NodeSS *aux,*aux1;
      aux=new NodeSS(MAXDatos);
      aux->Entry[aux->m_count]=tmp1;
      aux->m_count+=1;
      aux1=new NodeSS(MAXDatos);
      aux->Entry[aux->m_count]=tmp2;
      aux->m_count+=1;

       for(int i=0;i<entradas.size();i++)
      {   distancia=distanciaEucleadana(tmp1,entradas[i]);
	      if(distancia<=(distanciaActual/2)){
	         	aux->Entry[aux->m_count]=entradas[i];
	         	aux->m_count+=1;
	            entradas.erase( entradas.begin()+ i); }

      }
      for(int i=0;i<entradas.size();i++)
      {   distancia=distanciaEucleadana(tmp2,entradas[i]);
	      if(distancia<(distanciaActual/2)){
	         	aux1->Entry[aux1->m_count]=entradas[i];
	         	aux1->m_count+=1; 
	            entradas.erase( entradas.begin()+ i);
	            }
      }
     aux->m_level=aux1->m_level=(padre->m_level)+1;
     aux->height=aux1->height=(padre->height)+1;
     aux->centroid=calcularCentroide(aux);
 	 aux->Radius=calcularRadio(aux);
 	 aux1->centroid=calcularCentroide(aux1);
 	 aux1->Radius=calcularRadio(aux1);

 	 padre->clean();
 	 padre->Entry[padre->m_count]->child=aux;
 	 padre->m_count+=1;
 	 padre->Entry[padre->m_count]->child=aux1;
 	 padre->m_count+=1;
 	}



	Visitor SSTree::query(const Acceptor &accept, Visitor visitor)
	{
		if (root)
		{	
			QueryFunctor query(accept, visitor);
			query(root);
		}
		
		return visitor;
	}

	
	struct QueryFunctor   {
		const Acceptor &accept;
		Visitor &visitor;
		
		explicit QueryFunctor(const Acceptor &a, Visitor &v) : accept(a), visitor(v) {}
	
		void operator()(NodeSS * item)
		{
			NodeSS * node = item;
		
			if (node)
			{
				if (node->leaf)
					for_each(node->items.begin(), node->items.end(), VisitFunctor(accept, visitor));
				else
					for_each(node->items.begin(), node->items.end(), *this);
			}
		}
	};
	struct VisitFunctor {
	
		const Acceptor &accept;
		Visitor &visit;
		
		explicit VisitFunctor(const Acceptor &a, Visitor &v) : accept(a), visit(v) {}
	
		NodeSS* operator()( NodeSS * item ) 
		{
			NodeSS * node = (item);
		
			if (node)
				return node;
			else
				item->leaf = node->leaf;
		}
	};