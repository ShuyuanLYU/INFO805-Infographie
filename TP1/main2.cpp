#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include "Utils.cpp"

using namespace std;

int main(int argc, char **argv)
{
    TriangleSoup triangleSoup;

    ifstream input(argv[1]);

    if (argc != 6)
        cout << "5 arguments sont attendus" << endl;
    else
    {
        try
        {
            triangleSoup.read(input);
        }
        catch (char const *msg)
        {
            std::cerr << "Exception: " << msg << std::endl;
        }
        catch (...)
        {
            std::cerr << "Exception." << std::endl;
        }
        input.close();

        TriangleSoup outputAfterCompression;

        TriangleSoupZipper tsz(triangleSoup, outputAfterCompression, Index(50, 50, 50));

        tsz.zip();

        float tauxCompression = outputAfterCompression.triangles.size() / triangleSoup.triangles.size() * 100.0;

        cout << "Nombre de triangles en entrée : " + to_string(triangleSoup.triangles.size()) << endl;
        cout << "Nombre de triangles en sortie : " + to_string(outputAfterCompression.triangles.size()) << endl;
        cout << "Taux de compression : marche pas/20 : " + to_string(tauxCompression) << endl;
    }

    return 0;
}