#include <qapplication.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Viewer.cpp"
     
using namespace std;

int main(int argc, char** argv)
{
    /* ---- Quatrième partie ---- */


    TriangleSoup triangleSoup;

    ifstream input( argv[1] );

    try 
    {
        triangleSoup.read( input );
    }
    catch ( char const * msg ) 
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

    //tsz.smartZip();

    QApplication application(argc, argv);

    Viewer viewer(&outputAfterCompression); // &outputAfterCompression = tsz.output;

    string title = "Nombre de triangles en entrée : " + to_string(triangleSoup.triangles.size());
 
    viewer.setWindowTitle(QString::fromStdString(title));
    viewer.show();
    application.exec();


    /* ---- Deuxième & troisième parties ---- */

    // TriangleSoup triangleSoup;

    // ifstream input( argv[1] );

    // try 
    // {
    //     triangleSoup.read( input );
    // }
    // catch ( char const * msg ) 
    // {
    //     std::cerr << "Exception: " << msg << std::endl;
    // }
    // catch (...) 
    // {
    //     std::cerr << "Exception." << std::endl;
    // }
    // input.close();

    // QApplication application(argc, argv);

    // Viewer viewer(&triangleSoup);
 
    // viewer.setWindowTitle("Viewer triangle soup");
    // viewer.show();
    // application.exec();


    /* ---- Premiere partie ---- */

    // // Read command lines arguments.
    // QApplication application(argc, argv);

    // // Instantiate the viewer.
    // Viewer viewer;

    // // Give a name
    // viewer.setWindowTitle("Viewer triangle soup");

    // // Make the viewer window visible on screen.
    // viewer.show();

    // // Run main loop.
    // application.exec();
    
    return 0;
}