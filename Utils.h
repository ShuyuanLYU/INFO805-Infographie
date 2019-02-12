#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Vecteur 
{
    float xyz[ 3 ]; // les composantes

    public:
    Vecteur( float x, float y, float z ); // constructeur
    Vecteur();

    float  operator[]( int i ) const;     // accesseur en lecture
    float& operator[]( int i );           // accesseur en ecriture

    /* Retourne le vecteur dont les composantes sont les minima des
       composantes de soi-même et de other. */
    Vecteur inf( const Vecteur& other ) const;

    /* Retourne le vecteur dont les composantes sont les maxima des
       composantes de soi-même et de other. */
    Vecteur sup( const Vecteur& other ) const;

    Vecteur cross( const Vecteur& v ) const;
};

std::ostream& operator<<( std::ostream& out, Vecteur v )
{ 
    out << v[ 0 ] << " " << v[ 1 ] << " " << v[ 2 ]; 

    return out;
}

std::istream& operator>>( std::istream& in, Vecteur& v )
{ 
    in >> v[ 0 ] >> v[ 1 ] >> v[ 2 ];

    return in;
}

class Triangle 
{
    Vecteur vX, vY,vZ;

    public:
    Triangle(Vecteur x, Vecteur y, Vecteur z) : vX(x), vY(y), vZ(z) {}

    Vecteur  operator[]( int i ) const;
    Vecteur& operator[]( int i );

    Vecteur normal() const;
};

struct TriangleSoup 
{
    std::vector<Triangle> triangles; // les triangles

    TriangleSoup() {}

    void read( std::istream& in );

    void boundingBox( Vecteur& _low, Vecteur& _up) const;
};