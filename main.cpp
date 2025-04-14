#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include "Cout_color.h"
#include "version.h"
#include "tenseurs.h"
 // #include "tensors.h"


using namespace std;
/// --------------------------------------------------------------------------------
/// EXEMPLE DE CLASS AVEC METHODE MINIMALE POUR FONCTIONNER AVEC Tenseurs.h
/// --------------------------------------------------------------------------------
class Symbole {
private:
    string nom;

    template<typename T>
    std::string to_string_generic(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
    }

public:
    Symbole() : nom("0") {}  // Constructeur par défaut

    Symbole(string n) : nom(std::move(n)) {}

    // Constructeur par liste d'initialisation  necessaire...
    Symbole(std::initializer_list<char> b) : nom(b) {}

    // Addition : concaténation
    Symbole operator+(const Symbole& other) const {
        return Symbole("(" + nom + "+" + other.nom + ")");
    }

    template<typename T, typename = typename std::enable_if<!std::is_same<typename std::decay<T>::type, Symbole>::value>::type>
    Symbole(T val) : nom(to_string_generic(val)) {}
    // Multiplication entre deux symboles
    Symbole operator*(const Symbole& other) const {
        return Symbole(nom + "*" + other.nom);
    }

    // Affichage
    friend ostream& operator<<(ostream& os, const Symbole& s) {
        os << s.nom;
        return os;
    }

    // Multiplication par un scalaire à gauche : 3 * c
    friend Symbole operator*(int scalar, const Symbole& s) {
        return Symbole(std::to_string(scalar) + "*" + s.nom);
    }

    // Copie
    Symbole(const Symbole&) = default;
    Symbole& operator=(const Symbole&) = default;
};
/// ---------------------------------------------------------------------------------

int main()
{
      // En-tête colorée
    cout << blue;
    std::cout << "-----------------------------------------------------------------------\n";
    cout << white;
    cout << blue << "Tensor Class C++ \n";
    cout << green << "Coded by JP CHAMPEAUX                 \n";
    cout << white;
    cout << red << AutoVersion::FULLVERSION_STRING << endl;
    cout << AutoVersion::DATE << "/" << AutoVersion::MONTH << "/" << AutoVersion::YEAR << endl;
    cout << AutoVersion::STATUS << " version" << endl;
    cout << blue;
    cout << "Dépendances :         none " << endl;
    std::cout << "-----------------------------------------------------------------------\n";
    cout << white;

    clock_t t1, t2;
    t1 = clock();

    /// --------------------------
    /// Création et initialisation
    /// --------------------------
    cout <<yellow<<"Creation tenseur 2D (3x3) avec initialisation à 0 :" <<white<<endl;
    Tensor<double> T2D_3x3({3,3});
    cout<<T2D_3x3<<endl ; //.print(true);

    // Modification des valeurs
    T2D_3x3(0, 0) = 1.0;
    T2D_3x3(0, 1) = 2.0;
    T2D_3x3(0, 2) = 3.0;
    T2D_3x3(1, 0) = 4.0;
    T2D_3x3(1, 1) = 5.0;
    T2D_3x3(1, 2) = 6.0;
    T2D_3x3(2, 0) = 7.0;
    T2D_3x3(2, 1) = 8.0;
    T2D_3x3(2, 2) = 9.0;

    cout<<yellow<< "Tenseur apres modification Nom(id,id)=val :" <<white<< endl;
    T2D_3x3.print(true);

    double sum = T2D_3x3.sum();
    cout<<yellow<< "Somme des valeurs Nom.sum() : "<<white << sum << endl;


    // Création d'un autre tenseur 2D de même forme et d'initialisation différente
    cout <<yellow<< "\nCréation d'un autre tenseur 2D (3x3) avec initialisation à 1." <<white<< endl;
    Tensor<double> t2D_2({3, 3}, 1.0);
    t2D_2.print(true);

    // Addition de deux tenseurs
    cout <<yellow<< "\nAddition de deux tenseurs (T2D_3x3 + t2D_2) :" << white<< endl;
    Tensor<double> sum_tensors = T2D_3x3 + t2D_2;
    sum_tensors.print(true);

    // Multiplication élément par élément (Hadamard product)
    cout <<yellow<< "\nMultiplication élément par élément de deux tenseurs (T2D_3x3 * t2D_2) :" <<white<< endl;
    Tensor<double> prod_tensors = T2D_3x3 * t2D_2;
    prod_tensors.print(true);

    // Multiplication par un scalaire
    cout <<yellow<< "\nMultiplication de t2D par un scalaire (2.5) :" <<white<< endl;
    Tensor<double> scalar_prod = T2D_3x3 * 2.5;
    scalar_prod.print(true);

    // Norme du tenseur (sans métrique)
    cout <<yellow<< "\nNorme du tenseur 2D sans métrique :" <<white<< endl;
    double norm = T2D_3x3.pseudo_norm();
    cout <<yellow<< "Norme : " << norm <<white<< endl;


    // Reshape du tenseur
    cout <<yellow<< "\nReshape du tenseur t2D en une forme (1, 9) :" <<white<< endl;
    T2D_3x3.reshape({1, 9});
    T2D_3x3.print(true);

    // Slicing (découpage) : Création d'un sous-tenseur en coupant une tranche
    cout <<yellow<< "\nDécoupage du tenseur t2D avec slicing (0, 0, 3) -> (0, 0, 6) :" <<white<< endl;
    Tensor<double> sliced_tensor = T2D_3x3.slice({{1, 3, 6}});
    sliced_tensor.print(true);

    // Affichage du tenseur après découpage
    cout <<yellow<< "\nAffichage du sous-tenseur obtenu après découpage :" <<white<< endl;
    sliced_tensor.print(true);


      // Création d'un tenseur 2D de forme (3, 1)
    cout <<yellow<< "Création d'un tenseur 2D (3x1) avec initialisation à 0." << white<< endl;
    Tensor<double> t2Dp({3, 1});
    t2Dp.print(true);

    // Modification des valeurs
    t2Dp(0, 0) = 1.0;
    t2Dp(1, 0) = 2.0;
    t2Dp(2, 0) = 3.0;

    cout <<yellow<< "\nAffichage du tenseur après modification des valeurs :" << white<< endl;
    t2Dp.print(true);

    // Création d'un tenseur métrique 3x3 compatible avec le tenseur t2D
    cout <<yellow<< "\nCréation d'un tenseur métrique (3x3) :" <<white<< endl;
    Tensor<double> metric({3, 3});
    metric(0, 0) = 1.0;
    metric(0, 1) = 0.0;
    metric(0, 2) = 0.0;
    metric(1, 0) = 0.0;
    metric(1, 1) = -1.0;
    metric(1, 2) = 0.0;
    metric(2, 0) = 0.0;
    metric(2, 1) = 0.0;
    metric(2, 2) = -1.0;  // Matrice identité (3x3)
    metric.print(true);

    // Assigner la métrique au tenseur t2D
    t2Dp.set_metric(metric);

    // Calcul de la norme du tenseur t2D avec la métrique
    cout <<yellow<< "\nPseudo Norme du tenseur 2D avec la métrique (identité 3x3) :" <<white<< endl;
    double norm_with_metric = t2Dp.pseudo_norm();

    cout <<yellow<< "\nNorme avec métrique : \n" << norm_with_metric <<white<< endl;

    // Création d'un tenseur 3x3x3
    Tensor<double> t({3, 3, 3});
    t(0, 0, 0) = 1.0;
    t(0, 0, 1) = 2.0;
    t(0, 0, 2) = 3.0;
    t(0, 1, 0) = 4.0;
    t(0, 1, 1) = 5.0;
    t(0, 1, 2) = 6.0;
    t(0, 2, 0) = 7.0;
    t(0, 2, 1) = 8.0;
    t(0, 2, 2) = 9.0;
    t(1, 0, 0) = 10.0;
    t(1, 0, 1) = 11.0;
    t(1, 0, 2) = 12.0;
    t(1, 1, 0) = 13.0;
    t(1, 1, 1) = 14.0;
    t(1, 1, 2) = 15.0;
    t(1, 2, 0) = 16.0;
    t(1, 2, 1) = 17.0;
    t(1, 2, 2) = 18.0;
    t(2, 0, 0) = 19.0;
    t(2, 0, 1) = 20.0;
    t(2, 0, 2) = 21.0;
    t(2, 1, 0) = 22.0;
    t(2, 1, 1) = 23.0;
    t(2, 1, 2) = 24.0;
    t(2, 2, 0) = 25.0;
    t(2, 2, 1) = 26.0;
    t(2, 2, 2) = 27.0;

    // Affichage du tenseur 3x3x3 sous forme matricielle
    std::cout <<yellow<<"\nAffichage du tenseur 3x3x3 en représentation matricielle 2D:\n"<<white;
    t.print(true);


     // Création des tenseurs A et B (matrices 2x2)

    // Tenseur A 2x2
    Tensor<int> A({2, 2});
    A(0, 0) = 1;
    A(0, 1) = 2;
    A(1, 0) = 3;
    A(1, 1) = 4;

    // Tenseur B 2x3
    Tensor<int> B({2, 3});
    B(0, 0) = 5;
    B(0, 1) = 6;
    B(0, 2) = 3;
    B(1, 0) = 7;
    B(1, 1) = 8;
    B(1, 2) = 9;


    A.print(true);
    B.print(true);

    // Produit tensoriel de A et B
    Tensor<int> C = A.tensor_product(B);

    // Affichage du résultat
    C.print(true);

    cout<<yellow<<"Tenseur de Class :"<<white<<"\nT1 ="<<endl;

    Tensor<Symbole> Aa({2, 2});
    Aa({0, 0}) = Symbole("a");
    Aa({0, 1}) = Symbole("b");
    Aa({1, 0}) = Symbole("c");
    Aa({1, 1}) = Symbole("d");
    Aa.print();
    cout<<"\nT2 ="<<endl;

    Tensor<Symbole> Bb({2, 2});
    Bb({0, 0}) = Symbole("x");
    Bb({0, 1}) = Symbole("y");
    Bb({1, 0}) = Symbole("z");
    Bb({1, 1}) = Symbole("w");
    Bb.print();

    cout<<"\nT1 (x) T2 ="<<endl;

    Tensor<Symbole> Cc = Aa.tensor_product(Bb);
    Cc.print(true);
    cout<<endl;

    Tensor<Symbole> D ;
    string pi = "3.14";
    D = Bb * 3.14 + 2*Aa;
    D.print();



    /// PERMUTATIOLN
    vector<size_t> Shape = {2,2,2};
    vector<double> Datas = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0};
    Tensor<double> Tens(Shape, Datas);
    cout<<"inti Teseur ="<<endl;
    Tens.print();
    Tensor<double> Permutation;
    Permutation  = Tens.permute({0,2,1});
    cout<<"Après permutation 0,1,2 -> 0,2,1"<<endl;
    Permutation.print();

    /// Contraction
    cout<<"inti Teseur ="<<endl;
    Tens.print();

    Tensor<double> Contr;
    Contr = Tens.contract(0, 1);  // contraction sur les deux premières dimensions

    cout << "\nTenseur contracte (sur axes 0 et 1):" << endl;
    Contr.print();

    vector<size_t> shape_Aaa = {2, 3};
    vector<double> data_Aaa = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Tensor<double> Aaa(shape_Aaa, data_Aaa);
    cout << "Tenseur A (2x3) avant contraction:" << endl;
    Aaa.print(true);

    // Création du deuxième tenseur B de forme (3, 4)
    vector<size_t> shape_Bbb = {3, 2};
    vector<double> data_Bbb = {7.0, 8.0, 9.0, 10.0, 11.0, 12.0};
    Tensor<double> Bbb(shape_Bbb, data_Bbb);
    cout << "Tenseur B (3x4) avant contraction:" << endl;
    Bbb.print(true);

    // Contraction de A et B sur l'axe 1 de A et l'axe 0 de B (axes correspondants)
    auto contracted = Aaa.contract_with(Bbb, 1, 0);  // Contracter A[1] et B[0]

    // Affichage du résultat de la contraction
    cout << "\nTenseur contracté (axes 1 de A et 0 de B):" << endl;
    contracted.print(true);

    /// Contraction scalaire
    /// la métrique doit être portée par v1
    cout << "\nTenseur contraction vecteurs" << endl;
    vector<size_t> shape_v1 = {3};
    vector<double> data_v1 = {1.0, 2.0, 3.0};
    Tensor<double> Tv1(shape_v1, data_v1);
    Tv1.print();

    vector<size_t> shape_v2 = {3};
    vector<double> data_v2 = {1.0, 2.0, 3.0};
    Tensor<double> Tv2(shape_v2, data_v2);
    Tv2.print();
    contracted = Tv1.contract_with(Tv2, 0, 0);
    contracted.print(true);


    t2 = clock();
    float temp = (float)(t2 - t1) / CLOCKS_PER_SEC;

    cout << green << "\nTout est fini en " << temp << " sec" << endl;
    cout << white << "Have Fun World is a Playground\n\n";
    cout << " (°)~(°) \n";
    cout << " (=0.0=) \n";
    cout << "   (w)  \n";
    cout << " (:) (:)___°\n";
    return 0;

}

