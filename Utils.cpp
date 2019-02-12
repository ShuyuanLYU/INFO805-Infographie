#include "Utils.h"

using namespace std;

Vecteur::Vecteur( float x, float y, float z ) 
{
    xyz[0] = x; 
    xyz[1] = y;
    xyz[2] = z;
}

Vecteur::Vecteur()
{
    xyz[0] = 0.0; 
    xyz[1] = 0.0;
    xyz[2] = 0.0;
}

float Vecteur::operator[]( int i ) const
{
    return xyz[i];
}

float& Vecteur::operator[]( int i )
{
    return xyz[i];
}

Vecteur Vecteur::inf( const Vecteur& other ) const 
{
    return ((xyz[0] + xyz[1] + xyz[2]) < (other[0] + other[1] + other[2])) ? Vecteur(xyz[0], xyz[1], xyz[2]) : other;
}

Vecteur Vecteur::sup( const Vecteur& other ) const
{
    return ((xyz[0] + xyz[1] + xyz[2]) > (other[0] + other[1] + other[2])) ? Vecteur(xyz[0], xyz[1], xyz[2]) : other;
}

Vecteur Vecteur::cross( const Vecteur& v ) const
{
    return Vecteur((xyz[1] * v[2]) - (xyz[2] * v[1]), 
                  (xyz[2] * v[0]) - (xyz[0] * v[2]), 
                  (xyz[0] * v[1]) - (xyz[1] * v[0]));
}

Vecteur Triangle::operator[]( int i ) const
{
    switch (i)
    {
        case 0:
            return vX;
            break;
        case 1:
            return vY;
            break;
        case 2:
            return vZ;
            break;
    }
}

Vecteur& Triangle::operator[]( int i )
{
    switch (i)
    {
        case 0:
            return vX;
            break;
        case 1:
            return vY;
            break;
        case 2:
            return vZ;
            break;
    }
}

Vecteur Triangle::normal() const
{
    Vecteur vA(vY[0] - vX[0], vY[1] - vX[1], vY[2] - vX[2]);
    Vecteur vB(vZ[0] - vX[0], vZ[1] - vX[1], vZ[2] - vX[2]);

    Vecteur normal = vA.cross(vB);
    float somme = abs(normal[0]) + abs(normal[1]) + abs(normal[2]);

    normal[0] = normal[0] / somme;
    normal[1] = normal[1] / somme;
    normal[2] = normal[2] / somme;

    return normal;
}

void TriangleSoup::read(std::istream& in) 
{
    std::string line;

    while(in)
    {
        getline(in, line);

        if(!line.empty() && line[0] != '#')
        {
            istringstream istr( line );

            Vecteur v1, v2, v3;
            istr >> v1 >> v2 >> v3;

            Triangle triangle(v1, v2, v3);
            triangles.push_back(triangle);
        }
    }   
}

void TriangleSoup::boundingBox( Vecteur& low, Vecteur& up) const
{
    for(vector<Triangle>::const_iterator it = triangles.begin(); it != triangles.end(); ++it) 
    {
        for(int i = 0; i < 3; ++i)
        {
            low = (*it)[i].inf(low);
            up = (*it)[i] .sup(up);
        }
    }
}