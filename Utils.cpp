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