#include <qapplication.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Viewer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Material.h"
#include "PointLight.h"
#include "WaterPlane.cpp"

using namespace std;
using namespace rt;

void addBubble(Scene &scene, Point3 c, Real r, Material transp_m)
{
  Material revert_m = transp_m;
  std::swap(revert_m.in_refractive_index, revert_m.out_refractive_index);
  Sphere *sphere_out = new Sphere(c, r, transp_m);
  Sphere *sphere_in = new Sphere(c, r - 0.02f, revert_m);
  scene.addObject(sphere_out);
  scene.addObject(sphere_in);
}

int main(int argc, char **argv)
{
  // Read command lines arguments.
  QApplication application(argc, argv);

  // Creates a 3D scene
  Scene scene;

  // Light at infinity
  Light *sun = new PointLight(GL_LIGHT1, Point4(-50, 4, 2, 1),
                              Color(0.98, 0.84, 0.12));
  scene.addLight(sun);

  // Objects

  // dans main

  /* Artistic output */
  /*//Whales
  addBubble(scene, Point3(0, 0, -19), 20.0, Material::chrome());
  addBubble(scene, Point3(-10, 15, -5), 6.0, Material::chrome());
  addBubble(scene, Point3(-15, 30, -2), 3.0, Material::chrome());

  //Whale 1 Eye
  addBubble(scene, Point3(-20, -1, 20), 1.5, Material::pupil());
  addBubble(scene, Point3(-20, -1, 20), 3, Material::glass());

  */

  /* Technical output */
  addBubble(scene, Point3(14, 0, 5), 2.0, Material::emerald());
  addBubble(scene, Point3(3, 3, -2), 7.0, Material::bronze());
  addBubble(scene, Point3(12, 0, 10), 1.0, Material::glass());
  addBubble(scene, Point3(10, -2, 5), 1.0, Material::whitePlastic());
  addBubble(scene, Point3(8, 0, 8), 2.0, Material::redPlastic());

  addBubble(scene, Point3(-5, -5, 5), 1.0, Material::glass());
  addBubble(scene, Point3(-6, -5, 7), 1.0, Material::myRed());
  addBubble(scene, Point3(-4.5, -5.5, 9), 1.0, Material::myJaune());
  addBubble(scene, Point3(-4, -6, 11), 1.0, Material::myRuby());
  addBubble(scene, Point3(-4.5, -4.5, 13), 1.0, Material::Myglass());
  addBubble(scene, Point3(-5, -4, 15), 1.0, Material::myJade());
  addBubble(scene, Point3(-4.5, -4.5, 17), 1.0, Material::myPearl());

  addBubble(scene, Point3(15, 1, 10), 1.0, Material::glass());
  addBubble(scene, Point3(18, 3, 14), 1.0, Material::glass());
  addBubble(scene, Point3(13, 4, 17), 1.0, Material::glass());

  WaterPlane *waterPlane = new WaterPlane(Point3(0, 0, 0), Vector3(5, 0, 0), Vector3(0, 5, 0),
                                          Material::blueWater());
  scene.addObject(waterPlane);

  // Instantiate the viewer.
  Viewer viewer;
  // Give a name
  viewer.setWindowTitle("Ray-tracer preview");

  // Sets the scene
  viewer.setScene(scene);

  // Make the viewer window visible on screen.
  viewer.show();

  // Run main loop.
  application.exec();

  return 0;
}
