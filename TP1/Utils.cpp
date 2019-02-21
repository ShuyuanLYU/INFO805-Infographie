#include "Utils.h"
#include <algorithm>

using namespace std;

Vecteur::Vecteur(float x, float y, float z)
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

float Vecteur::operator[](int i) const
{
    return xyz[i];
}

float &Vecteur::operator[](int i)
{
    return xyz[i];
}

Vecteur Vecteur::inf(const Vecteur &other) const
{
    return Vecteur(min(xyz[0], other[0]), min(xyz[1], other[1]), min(xyz[2], other[2]));
}

Vecteur Vecteur::sup(const Vecteur &other) const
{
    return Vecteur(max(xyz[0], other[0]), max(xyz[1], other[1]), max(xyz[2], other[2]));
}

Vecteur Vecteur::cross(const Vecteur &v) const
{
    return Vecteur((xyz[1] * v[2]) - (xyz[2] * v[1]),
                   (xyz[2] * v[0]) - (xyz[0] * v[2]),
                   (xyz[0] * v[1]) - (xyz[1] * v[0]));
}

Vecteur Triangle::operator[](int i) const
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

Vecteur &Triangle::operator[](int i)
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

    normal[0] /= somme;
    normal[1] /= somme;
    normal[2] /= somme;

    return normal;
}

void TriangleSoup::read(std::istream &in)
{
    std::string line;

    while (in)
    {
        getline(in, line);

        if (!line.empty() && line[0] != '#')
        {
            istringstream istr(line);

            Vecteur v1, v2, v3;
            istr >> v1 >> v2 >> v3;

            Triangle triangle(v1, v2, v3);
            triangles.push_back(triangle);
        }
    }
}

void TriangleSoup::boundingBox(Vecteur &low, Vecteur &up) const
{
    for (vector<Triangle>::const_iterator it = triangles.begin(); it != triangles.end(); ++it)
    {
        for (int i = 0; i < 3; ++i)
        {
            low = (*it)[i].inf(low);
            up = (*it)[i].sup(up);
        }
    }
}

TriangleSoupZipper::TriangleSoupZipper(const TriangleSoup &anInput, TriangleSoup &anOutput, Index size)
{
    input = anInput;
    output = &anOutput;
    this->size = size;

    lowerBound = anInput.triangles[0][0];
    upperBound = anInput.triangles[0][0];

    anInput.boundingBox(lowerBound, upperBound);
}

/// @return l'index de la cellule dans laquelle tombe \a p.
Index TriangleSoupZipper::index(const Vecteur &p) const
{
    Index index(0, 0, 0);

    for (int i = 0; i < 3; ++i)
        index[i] = getIndexOfCoord(p, i);

    return index;
}

/// @return le centroïde de la cellule d'index \a idx (son "centre").
Vecteur TriangleSoupZipper::centroid(const Index &idx) const
{
    return Vecteur(getCenterOfCoord(idx, 0), getCenterOfCoord(idx, 1), getCenterOfCoord(idx, 2));
}

int TriangleSoupZipper::getIndexOfCoord(const Vecteur &v, const int &i) const
{
    return floor(size[i] * (v[i] - lowerBound[i]) / (upperBound[i] - lowerBound[i]));
    // int a = floor(size[i] * (v[i] - lowerBound[i]) / (upperBound[i] - lowerBound[i]));

    // if(a > size[i])
    //     return size[i];
    // else if (a < 0)
    //     return 0;
    // else
    //     return a;  
}

float TriangleSoupZipper::getCenterOfCoord(const Index &idx, const int &i) const
{
    return lowerBound[i] + ((upperBound[i] - lowerBound[i]) / size[i]) * (idx[i] + 0.5);
}

void TriangleSoupZipper::zip()
{
    for (vector<Triangle>::const_iterator it = input.triangles.begin(); it != input.triangles.end(); ++it)
    {

        Index indexX = index((*it)[0]);
        Index indexY = index((*it)[1]);
        Index indexZ = index((*it)[2]);

        if (!(indexX == indexY || indexX == indexZ || indexY == indexZ))
        {
            // Méthode pas jolie :
            output->triangles.push_back(Triangle(centroid(indexX), centroid(indexY), centroid(indexZ)));

            // Méthode jolie :
            // output->triangles.push_back(Triangle(index2data.find(indexX) -> second.barycenter(),
            //                                      index2data.find(indexY) -> second.barycenter(),
            //                                      index2data.find(indexZ) -> second.barycenter()));
        }
    }
}

// Ajoute le point v à l'accumulateur.
// void CellData::add(const Vecteur &v)
// {
//     acc.push_back(v);
// }

// void Vecteur::operator+=(const Vecteur v)
// {
//     xyz[0] += v[0];
//     xyz[1] += v[1];
//     xyz[2] += v[2];
// }

// void Vecteur::operator/=(const int divider)
// {
//     xyz[0] /= divider;
//     xyz[1] /= divider;
//     xyz[2] /= divider;
// }

// Retourne le barycentre de tous les points ajoutés.
// Vecteur CellData::barycenter() const
// {
//     Vecteur v(0, 0, 0);

//     for (vector<Vecteur>::const_iterator it = acc.begin(); it != acc.end(); ++it)
//         v += *it;

//     v /= acc.size();

//     return v;
// }

// void TriangleSoupZipper::smartZip()
// {
    // for (int i = 0; i < size[0]; ++i)
    // {
    //     for (int j = 0; j < size[1]; ++j)
    //     {
    //         for (int k = 0; k < size[2]; ++k)
    //             index2data.insert(pair<Index, CellData>(Index(i, j, k), CellData()));
    //     }
    // }

    //cout << index2data.size();

    // for (vector<Triangle>::const_iterator it = input.triangles.begin(); it != input.triangles.end(); ++it)
    // {
    //     index2data.find(index((*it)[0])) -> second.add((*it)[0]);
    //     index2data.find(index((*it)[1])) -> second.add((*it)[1]);
    //     index2data.find(index((*it)[2])) -> second.add((*it)[2]);
    // }

    //zip();
// }