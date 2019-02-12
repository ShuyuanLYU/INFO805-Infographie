#include <qapplication.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Viewer.cpp"
     
using namespace std;

int main(int argc, char** argv)
{
    TriangleSoup triangleSoup;
    Viewer viewer;

    /* ---- Deuxième partie ---- */

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

    QApplication application(argc, argv);

    viewer.ptrSoup = &triangleSoup;
    viewer.setWindowTitle("Viewer triangle soup");
    viewer.show();
    application.exec();


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