#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Vecteur
{
    float xyz[3]; // les composantes

  public:
    Vecteur(float x, float y, float z); // constructeur
    Vecteur();

    float operator[](int i) const; // accesseur en lecture
    float &operator[](int i);      // accesseur en ecriture

    /* Retourne le vecteur dont les composantes sont les minima des
       composantes de soi-même et de other. */
    Vecteur inf(const Vecteur &other) const;

    /* Retourne le vecteur dont les composantes sont les maxima des
       composantes de soi-même et de other. */
    Vecteur sup(const Vecteur &other) const;

    Vecteur cross(const Vecteur &v) const;

    void operator+=(const Vecteur v);

    void operator/=(const int divider);
};

class Triangle
{
    Vecteur vX, vY, vZ;

  public:
    Triangle(Vecteur x, Vecteur y, Vecteur z) : vX(x), vY(y), vZ(z) {}

    Vecteur operator[](int i) const;
    Vecteur &operator[](int i);

    Vecteur normal() const;
};

struct TriangleSoup
{
    std::vector<Triangle> triangles; // les triangles

    TriangleSoup() {}

    void read(std::istream &in);

    void boundingBox(Vecteur &_low, Vecteur &_up) const;
};

/// Définit un index sur 3 entiers. Toutes les opérations usuelles
/// sont surchargées (accès, comparaisons, égalité).
struct Index
{
    int idx[3];

    Index() {}

    Index(int i0, int i1, int i2)
    {
        idx[0] = i0;
        idx[1] = i1;
        idx[2] = i2;
    }

    Index(int indices[])
    {
        idx[0] = indices[0];
        idx[1] = indices[1];
        idx[2] = indices[2];
    }

    int operator[](int i) const
    {
        return idx[i];
    }

    int &operator[](int i)
    {
        return idx[i];
    }

    bool operator<(const Index &other) const
    {
        return (idx[0] < other[0]) || ((idx[0] == other[0]) && ((idx[1] < other[1]) || ((idx[1] == other[1]) && (idx[2] < other[2]))));
    }

    bool operator==(const Index &other) const
    {
        return idx[0] == other[0] && idx[1] == other[1] && idx[2] == other[2];
    }
};

// Structure pour calculer le barycentre d'un ensemble de points.
struct CellData {
    std::vector<Vecteur> acc;

    // Crée un accumulateur vide.
    CellData() : acc() {};

    // Ajoute le point v à l'accumulateur.
    void add( const Vecteur& v );
    
    // Retourne le barycentre de tous les points ajoutés.
    Vecteur barycenter() const;
};

struct TriangleSoupZipper
{
    // Définir chaque x - y - z le plus haut et le plus bas indépendemment. Puis diviser par l'index x - y - z;
    TriangleSoup input;
    TriangleSoup* output;
    Index size;
    Vecteur lowerBound;
    Vecteur upperBound;

    // Stocke pour chaque cellule son barycentre.
    // std::map<Index, CellData> index2data;

    // Construit le zipper avec une soupe de triangle en entrée \a
    // anInput, une soupe de triangle en sortie \a anOutput, et un index \a size
    // qui est le nombre de cellules de la boîte découpée selon les 3 directions.
    TriangleSoupZipper(const TriangleSoup &anInput,
                       TriangleSoup &anOutput,
                       Index size);

    /// @return l'index de la cellule dans laquelle tombe \a p.
    Index index(const Vecteur &p) const;

    /// @return le centroïde de la cellule d'index \a idx (son "centre").
    Vecteur centroid(const Index &idx) const;

    /// @return the index of a coord
    int getIndexOfCoord(const Vecteur& v, const int& index) const;

    /// @return the center of a coord
    float getCenterOfCoord(const Index& idx, const int& i) const;

    /// zips the output
    void zip();

    // void smartZip();
};

std::ostream &operator<<(std::ostream &out, Vecteur v)
{
    out << v[0] << " " << v[1] << " " << v[2];

    return out;
}

std::istream &operator>>(std::istream &in, Vecteur &v)
{
    in >> v[0] >> v[1] >> v[2];

    return in;
}

float getDecimalPart(const float &coord)
{
    return coord - floor(coord);
}