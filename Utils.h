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
};

struct TriangleSoup 
{
    std::vector<Triangle> triangles; // les triangles

    TriangleSoup() {}

    void read( std::istream& in );
};