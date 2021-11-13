#include <iostream>
#include <ctime>
using namespace std;

template <typename Tip1, typename Tip2>
struct Cvor {
    Tip1 par1;
    Tip2 par2;
    Cvor<Tip1,Tip2>* rightt=nullptr;
    Cvor<Tip1,Tip2>* leftt=nullptr;
    Cvor<Tip1,Tip2>*parent=nullptr;

};



template <typename Tip1, typename Tip2>
struct par {
    Tip1 par1;
    Tip2 par2;
};



template <typename Tip1, typename Tip2>
class Mapa
{
public:
    Mapa<Tip1,Tip2>() {}
    virtual ~Mapa() {}
    virtual int brojElemenata()const=0;
    virtual void obrisi()=0;
    virtual void obrisi(const Tip1 &kljuc)=0;
    virtual Tip2 operator[] (Tip1 n)const=0;
    virtual Tip2 &operator[] (Tip1 n)=0;

};

template <typename Tip1, typename Tip2>
class NizMapa : public Mapa<Tip1,Tip2>
{
    par<Tip1,Tip2>*mapaa=nullptr;
    int number_of_elements=0;
    int capacity=500;

public:

    NizMapa(): number_of_elements(0)
    {
        mapaa=new par<Tip1,Tip2>[capacity];
    }

    ~NizMapa()
    {
        delete[] mapaa;
    }

    NizMapa(const NizMapa &m);
    NizMapa(NizMapa &&m);
    int brojElemenata()const
    {
        return number_of_elements;
    }

    void obrisi();
    void obrisi(const Tip1 &kljuc);
    Tip2 operator[](Tip1 n)const;
    Tip2 &operator[](Tip1 n);
    NizMapa& operator=(const NizMapa &m);
    NizMapa& operator=(NizMapa &&m);


};

template <typename Tip1, typename Tip2>
NizMapa<Tip1,Tip2>::NizMapa(NizMapa<Tip1,Tip2> &&m)
{
    if(this==&m) return;
    mapaa=m.mapaa;
    number_of_elements=m.brojElemenata();
    capacity=m.capacity;

}

template <typename Tip1, typename Tip2>
NizMapa<Tip1,Tip2>::NizMapa(const NizMapa<Tip1,Tip2>&m)
{
    capacity=m.capacity;
    mapaa=new par<Tip1,Tip2>[capacity];
    for(int i=0; i<m.brojElemenata(); i++) {
        mapaa[i]=m.mapaa[i];
        number_of_elements++;
    }
}


template <typename Tip1, typename Tip2>
void NizMapa<Tip1,Tip2>::obrisi()
{
    delete[] mapaa;
    capacity=100;
    mapaa=new par<Tip1,Tip2>[capacity];
    number_of_elements=0;
}


template <typename Tip1, typename Tip2>
void NizMapa<Tip1,Tip2>::obrisi(const Tip1 &kljuc)
{
    for(int i=0; i<number_of_elements; i++) {
        if(mapaa[i].par1==kljuc) {
            for(int j=i; j<number_of_elements-1; j++) mapaa[j]=mapaa[j+1];
            number_of_elements--;
        }
    }

}


template <typename Tip1, typename Tip2>
Tip2 &NizMapa<Tip1,Tip2>::operator[] (Tip1 n)
{
    for(int i=0; i<number_of_elements; i++) {
        if(mapaa[i].par1==n) {
            return mapaa[i].par2;
        }
    }

    if(number_of_elements==capacity) {
        capacity*=2;
        par<Tip1,Tip2> *novi=new par<Tip1,Tip2>[capacity];
        for(int j=0; j<number_of_elements; j++) {
            novi[j]=mapaa[j];
        }
        delete[] mapaa;
        mapaa=novi;
    }

    number_of_elements++;
    par<Tip1,Tip2> temp;
    temp.par1=n;
    temp.par2=Tip2();
    mapaa[number_of_elements-1]=temp;
    return mapaa[number_of_elements-1].par2;

}

template <typename Tip1, typename Tip2>
Tip2 NizMapa<Tip1,Tip2>::operator[] (Tip1 n) const
{
    for(int i=0; i<number_of_elements; i++) {
        if(mapaa[i].par1==n) {
            return mapaa[i].par2;
        }
    }
    return Tip2();
}



template <typename Tip1, typename Tip2>
NizMapa<Tip1,Tip2> &NizMapa<Tip1,Tip2>::operator=(const NizMapa &m)
{
    if(this==&m) return *this;
    delete[] mapaa;
    capacity=m.capacity;
    mapaa=new par<Tip1,Tip2>[capacity];

    for(int i=0; i<m.brojElemenata(); i++) {
        mapaa[i]=m.mapaa[i];
    }
    number_of_elements=m.number_of_elements;
    return *this;

}

template <typename Tip1, typename Tip2>
NizMapa<Tip1,Tip2> &NizMapa<Tip1,Tip2>::operator=(NizMapa &&m)
{
    if(this==&m) return *this;
    delete[] mapaa;
    mapaa=m.mapaa;
    number_of_elements=m.number_of_elements;
    capacity=m.capacity;
    return *this;
}

template <typename Tip1, typename Tip2>
class BinStabloMapa: public Mapa<Tip1,Tip2>
{
    Cvor<Tip1,Tip2>*root_korijen;
    int number_of_elements;
public:
    BinStabloMapa(): root_korijen(nullptr), number_of_elements(0) {}
    ~BinStabloMapa()
    {
        this->obrisi();
    }
    BinStabloMapa(const BinStabloMapa &b);
    BinStabloMapa(BinStabloMapa &&b);
    int brojElemenata()const
    {
        return number_of_elements;
    }
    void obrisi();
    void obrisi(const  Tip1 &k);
    Tip2 &operator [](Tip1 k);
    Tip2 operator [](Tip1 k) const;
    BinStabloMapa &operator = (const BinStabloMapa &b);
    BinStabloMapa &operator=(BinStabloMapa &&b);
    void kopiraj_mi(Cvor<Tip1,Tip2>* c1, Cvor<Tip1,Tip2>* &c2, Cvor<Tip1,Tip2>* prethodni=nullptr);

};

template <typename Tip1, typename Tip2>
BinStabloMapa<Tip1,Tip2>::BinStabloMapa(const BinStabloMapa &b)
{
    number_of_elements=b.number_of_elements;
    kopiraj_mi(b.root_korijen, root_korijen);
}


template <typename Tip1, typename Tip2>
BinStabloMapa<Tip1,Tip2>::BinStabloMapa(BinStabloMapa &&b)
{
    number_of_elements=b.number_of_elements;
    root_korijen=b.root_korijen;
}

template <typename Tip1, typename Tip2>
void BinStabloMapa<Tip1,Tip2>::obrisi(const Tip1 &k)
{
    Cvor<Tip1,Tip2> *p=root_korijen, *parent=nullptr;
    while(p!=nullptr && k!=p->par1) {
        parent=p;
        if(k<p->par1) p=p->leftt;
        else p=p->rightt;
    }
    if(p==nullptr) return;
    Cvor<Tip1,Tip2>*m;
    if(p->leftt==nullptr) m=p->rightt;
    else {
        if(p->rightt==nullptr) m=p->leftt;
        else {
            Cvor<Tip1,Tip2> *pm=p, *temp;
            m=p->leftt;
            temp=m->rightt;
            while(temp!=nullptr) {
                pm=m;
                m=temp;
                temp=m->rightt;

            }
            if(pm!=p) {
                pm->rightt=m->leftt;
                m->leftt=p->leftt;
            }
            m->rightt=p->rightt;

        }
    }
    if(parent==nullptr) root_korijen=m;
    else {
        if(p==parent->leftt) parent->leftt=m;
        else parent->rightt=m;
    }
    delete p;
    number_of_elements--;


}

template <typename Tip1, typename Tip2>
void BinStabloMapa<Tip1,Tip2>::obrisi()
{
    while(root_korijen!=nullptr) obrisi(root_korijen->par1);
}

template <typename Tip1, typename Tip2>
Tip2 BinStabloMapa<Tip1,Tip2>::operator [](Tip1 k)const
{
    Cvor<Tip1,Tip2> *temp=root_korijen;
    while(temp!=nullptr && k!=temp->par1) {
        if(k<temp->par1) temp=temp->leftt;
        else temp=temp->rightt;
    }
    if(temp==nullptr) return Tip2();
    return temp->par2;

}

template <typename Tip1, typename Tip2>
Tip2 &BinStabloMapa<Tip1,Tip2>::operator [](Tip1 k)
{
    Cvor<Tip1,Tip2> *temp=root_korijen;
    while(temp!=nullptr && k!=temp->par1) {
        if(k<temp->par1) temp=temp->leftt;
        else temp=temp->rightt;
    }

    if(temp==nullptr) {
        temp=new Cvor<Tip1,Tip2>();
        number_of_elements++;
        temp->par1=k;
        temp->par2=Tip2();
        Cvor<Tip1,Tip2>*x=root_korijen, *y=nullptr;
        while(x!=nullptr) {
            y=x;
            if(temp->par1 < x->par1) x=x->leftt;
            else x=x->rightt;
        }

        if(y==nullptr) root_korijen=temp;
        else {
            if(temp->par1 < y->par1) y->leftt=temp;
            else y->rightt=temp;
            temp->parent=y;
        }

    }
    return temp->par2;

}


template <typename Tip1, typename Tip2>
BinStabloMapa<Tip1,Tip2> &BinStabloMapa<Tip1,Tip2>::operator=(BinStabloMapa &&b)
{
    if(this==&b) return *this;
    this->obrisi();
    root_korijen=b.root_korijen;
    number_of_elements=b.number_of_elements;
    return *this;
}


template <typename Tip1, typename Tip2>
void BinStabloMapa<Tip1,Tip2>:: kopiraj_mi(Cvor<Tip1,Tip2>* c1, Cvor<Tip1,Tip2>* &c2, Cvor<Tip1,Tip2>* prethodni)
{
    if(c1!=nullptr) {
        c2=new Cvor<Tip1,Tip2>();
        c2->par1=c1->par1;
        c2->par2=c1->par2;
        c2->parent=prethodni;
        kopiraj_mi(c1->leftt, c2->leftt, c2);
        kopiraj_mi(c1->rightt, c2->rightt, c2);
    }
}


template <typename Tip1, typename Tip2>
BinStabloMapa<Tip1,Tip2> &BinStabloMapa<Tip1,Tip2>::operator=(const BinStabloMapa &b)
{
    if(this==&b) return *this;
    this->obrisi();
    kopiraj_mi(b.root_korijen, root_korijen);
    number_of_elements=b.number_of_elements;
    return *this;
}






int main()
{

    {
        clock_t prvo_vrijeme=clock();
        BinStabloMapa<int, int>mapa;
        int velicina(1000);
        for(int i=0; i<velicina; i++) {
            mapa[rand()%(3*velicina)-velicina]=i;
        }

        for(int i(-velicina); i<velicina; i++) {
            mapa[i]=i;
        }

        for(int i(-velicina); i<velicina; i++) {
            mapa.obrisi(i);
        }
        cout<<"OK";
        clock_t drugo_vrijeme=clock();
        int ukupno_vrijeme=(drugo_vrijeme-prvo_vrijeme)/(CLOCKS_PER_SEC/1000);
        std::cout<<"Vrijeme izvrsenja: "<<ukupno_vrijeme<<" ms."<<std::endl;


    }

    {
        clock_t prvo_vrijeme=clock();
        NizMapa<int, int>mapa1;
        int velicina2(1000);

        for(int i=0; i<velicina2; i++) {
            mapa1[rand()%(3*velicina2)-velicina2]=i;
        }

        for(int i(-velicina2); i<velicina2; i++) {
            mapa1[i]=i;
        }

        for(int i(-velicina2); i<velicina2; i++) {
            mapa1.obrisi(i);
        }
        cout<<"OK";
        clock_t drugo_vrijeme=clock();
        int ukupno_vrijeme=(drugo_vrijeme-prvo_vrijeme)/(CLOCKS_PER_SEC/1000);
        std::cout<<"Vrijeme izvrsenja: "<<ukupno_vrijeme<<" ms."<<std::endl;

    }

    std::cout << "Pripremna Zadaca Za Vjezbu 8, Zadatak 1";
    return 0;
}
