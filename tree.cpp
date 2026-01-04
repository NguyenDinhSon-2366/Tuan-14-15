#include <iostream>
#include <algorithm>
using namespace std;

enum ThuTuDuyet { TRUOC = -1, GIUA, SAU };

class CayTimKiemNhiPhan {
public: 
    struct Nut { 
        int giaTri; 
        Nut *trai, *phai; 
        Nut(int v) : giaTri(v), trai(0), phai(0) {} 
    };
    
private:
    Nut* goc;
    
    void Them(int v, Nut*& n) {
        if (!n) n = new Nut(v);
        else if (v < n->giaTri) Them(v, n->trai);
        else if (v > n->giaTri) Them(v, n->phai);
    }
    
    void Duyet(Nut* n, ThuTuDuyet tt, void(*ham)(int)) {
        if (!n) return;
        if (tt == TRUOC) ham(n->giaTri);
        Duyet(n->trai, tt, ham);
        if (tt == GIUA) ham(n->giaTri);
        Duyet(n->phai, tt, ham);
        if (tt == SAU) ham(n->giaTri);
    }
    
public:
    CayTimKiemNhiPhan() : goc(0) {}
    void ThemNut(int v) { Them(v, goc); }
    void In(ThuTuDuyet tt, void(*ham)(int)) { Duyet(goc, tt, ham); }
};

template<class T>
class CayNhiPhan {
public: 
    struct Nut { 
        T giaTri; 
        Nut *trai, *phai; 
        Nut(T v) : giaTri(v), trai(0), phai(0) {} 
    };
    
private:
    Nut* goc;
    
    Nut* TaoNut(T* mang, int n, int i) {
        if (i >= n) return 0;
        Nut* p = new Nut(mang[i]);
        p->trai = TaoNut(mang, n, 2*i+1);
        p->phai = TaoNut(mang, n, 2*i+2);
        return p;
    }
    
    void Duyet(Nut* n, ThuTuDuyet tt, void(*ham)(T)) {
        if (!n) return;
        if (tt == TRUOC) ham(n->giaTri);
        Duyet(n->trai, tt, ham);
        if (tt == GIUA) ham(n->giaTri);
        Duyet(n->phai, tt, ham);
        if (tt == SAU) ham(n->giaTri);
    }
    
public:
    CayNhiPhan() : goc(0) {}
    CayNhiPhan(T* mang, int n) { goc = TaoNut(mang, n, 0); }
    void In(ThuTuDuyet tt, void(*ham)(T)) { Duyet(goc, tt, ham); }
    Nut* LayGoc() { return goc; }
};

void InSo(int v) { cout << v << " "; }

void HoanDoi(int& a, int& b) { int t = a; a = b; b = t; }

void VunDong(int* mang, int n, int i) {
    int lonNhat = i, trai = 2*i+1, phai = 2*i+2;
    if (trai < n && mang[trai] > mang[lonNhat]) lonNhat = trai;
    if (phai < n && mang[phai] > mang[lonNhat]) lonNhat = phai;
    if (lonNhat != i) { 
        HoanDoi(mang[i], mang[lonNhat]); 
        VunDong(mang, n, lonNhat); 
    }
}

void SapXepVunDong(int* mang, int n) {
    for (int i = n/2-1; i >= 0; i--) VunDong(mang, n, i);
    for (int i = n-1; i >= 0; i--) { 
        HoanDoi(mang[0], mang[i]); 
        VunDong(mang, i, 0); 
    }
}
template<class T>
bool CayGiongNhau(typename CayNhiPhan<T>::Nut* p, typename CayNhiPhan<T>::Nut* q) {
    if (!p && !q) return true;
    if (!p || !q) return false;
    return (p->giaTri == q->giaTri) && 
           CayGiongNhau<T>(p->trai, q->trai) && 
           CayGiongNhau<T>(p->phai, q->phai);
}

int main() {
    int mang[] = {4,1,3,2,16,9,10,14,8,7};
    int n = sizeof(mang)/sizeof(mang[0]);
    
    CayNhiPhan<int> cay1(mang, n);
    cout << "Cay 1 - Duyet truoc: "; 
    cay1.In(TRUOC, InSo);
    cout << "\nCay 1 - Duyet giua: "; 
    cay1.In(GIUA, InSo);
    cout << "\nCay 1 - Duyet sau: "; 
    cay1.In(SAU, InSo);
    
    SapXepVunDong(mang, n);
    CayNhiPhan<int> cay2(mang, n);
    cout << "\n\nSau sap xep vun dong - Duyet truoc: "; 
    cay2.In(TRUOC, InSo);
    
    int mang2[] = {4,3,1,2,16,9,10,14,8,7};
    CayNhiPhan<int> cay3(mang2, n);
    cout << "\n\nCay 3 - Duyet truoc: "; 
    cay3.In(TRUOC, InSo);
    
    cout << "\n\nSo sanh cay 1 va 3: ";
    CayGiongNhau<int>(cay1.LayGoc(), cay3.LayGoc()) ? 
        cout << "Giong nhau" : cout << "Khac nhau";
    
    cout << "\n\n=== Cay Tim Kiem Nhi Phan ===" << endl;
    CayTimKiemNhiPhan cayTimKiem;
    for (int giaTri : {50,30,20,40,70,60,80}) 
        cayTimKiem.ThemNut(giaTri);
    cout << "Cay BST - Duyet giua (da sap xep): "; 
    cayTimKiem.In(GIUA, InSo);
    cout << "\nCay BST - Duyet truoc: "; 
    cayTimKiem.In(TRUOC, InSo);
    
    return 0;
}