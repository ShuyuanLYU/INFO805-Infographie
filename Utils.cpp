#include "Utils.h"
#include <iostream>

using namespace std;

Vecteur::Vecteur( float x, float y, float z ) 
{
    xyz[0] = x; 
    xyz[1] = y;
    xyz[2] = z;
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