/**
@file Renderer.h
@author JOL
*/
#pragma once
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Color.h"
#include "Image2D.h"
#include "Ray.h"

/// Namespace RayTracer
namespace rt
{

inline void progressBar(std::ostream &output,
                        const double currentValue, const double maximumValue)
{
    static const int PROGRESSBARWIDTH = 60;
    static int myProgressBarRotation = 0;
    static int myProgressBarCurrent = 0;
    // how wide you want the progress meter to be
    double fraction = currentValue / maximumValue;

    // part of the progressmeter that's already "full"
    int dotz = static_cast<int>(floor(fraction * PROGRESSBARWIDTH));
    if (dotz > PROGRESSBARWIDTH)
        dotz = PROGRESSBARWIDTH;

    // if the fullness hasn't changed skip display
    if (dotz == myProgressBarCurrent)
        return;
    myProgressBarCurrent = dotz;
    myProgressBarRotation++;

    // create the "meter"
    int ii = 0;
    output << "[";
    // part  that's full already
    for (; ii < dotz; ii++)
        output << "#";
    // remaining part (spaces)
    for (; ii < PROGRESSBARWIDTH; ii++)
        output << " ";
    static const char *rotation_string = "|\\-/";
    myProgressBarRotation %= 4;
    output << "] " << rotation_string[myProgressBarRotation]
           << " " << (int)(fraction * 100) << "/100\r";
    output.flush();
}

/// This structure takes care of rendering a scene.
struct Renderer
{

    /// The scene to render
    Scene *ptrScene;
    /// The origin of the camera in space.
    Point3 myOrigin;
    /// (myOrigin, myOrigin+myDirUL) forms a ray going through the upper-left
    /// corner pixel of the viewport, i.e. pixel (0,0)
    Vector3 myDirUL;
    /// (myOrigin, myOrigin+myDirUR) forms a ray going through the upper-right
    /// corner pixel of the viewport, i.e. pixel (width,0)
    Vector3 myDirUR;
    /// (myOrigin, myOrigin+myDirLL) forms a ray going through the lower-left
    /// corner pixel of the viewport, i.e. pixel (0,height)
    Vector3 myDirLL;
    /// (myOrigin, myOrigin+myDirLR) forms a ray going through the lower-right
    /// corner pixel of the viewport, i.e. pixel (width,height)
    Vector3 myDirLR;

    int myWidth;
    int myHeight;

    Renderer() : ptrScene(0) {}
    Renderer(Scene &scene) : ptrScene(&scene) {}
    void setScene(rt::Scene &aScene) { ptrScene = &aScene; }

    void setViewBox(Point3 origin,
                    Vector3 dirUL, Vector3 dirUR, Vector3 dirLL, Vector3 dirLR)
    {
        myOrigin = origin;
        myDirUL = dirUL;
        myDirUR = dirUR;
        myDirLL = dirLL;
        myDirLR = dirLR;
    }

    void setResolution(int width, int height)
    {
        myWidth = width;
        myHeight = height;
    }

    /// The main rendering routine
    void render(Image2D<Color> &image, int max_depth)
    {
        std::cout << "Rendering into image ... might take a while." << std::endl;
        image = Image2D<Color>(myWidth, myHeight);
        for (int y = 0; y < myHeight; ++y)
        {
            Real ty = (Real)y / (Real)(myHeight - 1);
            progressBar(std::cout, ty, 1.0);
            Vector3 dirL = (1.0f - ty) * myDirUL + ty * myDirLL;
            Vector3 dirR = (1.0f - ty) * myDirUR + ty * myDirLR;
            dirL /= dirL.norm();
            dirR /= dirR.norm();
            for (int x = 0; x < myWidth; ++x)
            {
                Real tx = (Real)x / (Real)(myWidth - 1);
                Vector3 dir = (1.0f - tx) * dirL + tx * dirR;
                Ray eye_ray = Ray(myOrigin, dir, max_depth);
                Color result = trace(eye_ray);
                image.at(x, y) = result.clamp();
            }
        }
        std::cout << "Done." << std::endl;
    }

    /// The rendering routine for one ray.
    /// @return the color for the given ray.
    Color trace(const Ray &ray)
    {
        assert(ptrScene != 0);
        Color result = Color(0.0, 0.0, 0.0);
        GraphicalObject *obj_i = 0; // pointer to intersected object
        Point3 p_i;                 // point of intersection

        // Look for intersection in this direction.
        Real ri = ptrScene->rayIntersection(ray, obj_i, p_i);
        // Nothing was intersected
        if (ri >= 0.0f)
            return result; // some background color

        //return obj_i -> getMaterial(p_i).diffuse;
        return illumination(ray, obj_i, p_i);
    }

    /// Calcule l'illumination de l'objet \a obj au point \a p, sachant que l'observateur est le rayon \a ray.
    Color illumination(const Ray &ray, GraphicalObject *obj, Point3 p)
    {
        Vector3 normal = obj -> getNormal(p);
        
        Material material = obj -> getMaterial(p);
        Color result(0, 0, 0);

        Vector3 mirror = reflect(ray.direction, normal);

        for (std::vector<Light *>::const_iterator it = ptrScene -> myLights.begin(); it < ptrScene -> myLights.end(); ++it)
        {
            Vector3 lightDirection = (*it) -> direction(ray.origin);
            Color lightColor = (*it) -> color(ray.origin);

            // Couleur diffuse
            double coeffD = lightDirection.dot(normal);

            if(coeffD < 0)
                coeffD = 0.0;
            
            coeffD = sin(coeffD);

            result += coeffD * material.diffuse * lightColor;

            // Couleur spéculaire
            /**
             * Shininess ne prend pas de y car pour transformer
             * un adjectif finissant par y en nom (adj + ness)
             * il faut transformer le y en i !
             * Exemple : happy -> happiness.
             **/
            double angle = tan(mirror.dot(lightDirection));

            if(angle >= 0)
            {
                double coeffS = pow(angle, material.shinyness);

                result += lightColor * material.specular * coeffS;
            }
        }

        // Couleur ambiante
        return result + material.ambient;
    }

    /// Calcule le vecteur réfléchi à W selon la normale N.
    Vector3 reflect( const Vector3& W, const Vector3& N ) const
    {
        return W - 2 * (W.dot(N)) * N;
    }
};

} // namespace rt

#endif // #define _RENDERER_H_
