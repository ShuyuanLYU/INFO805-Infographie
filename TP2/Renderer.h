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
struct Background
{
    virtual Color backgroundColor(const Ray &ray) = 0;
};

struct MyBackground : public Background
{
    Color backgroundColor(const Ray &ray)
    {
        if (ray.direction[2] < 0.0)
        {
            Color result(0.0f, 0.0f, 0.0f);

            Real x = -0.5f * ray.direction[0] / ray.direction[2];
            Real y = -0.5f * ray.direction[1] / ray.direction[2];
            Real d = sqrt(x * x + y * y);
            Real t = std::min(d, 30.0f) / 30.0f;
            x -= floor(x);
            y -= floor(y);
            if (((x >= 0.5f) && (y >= 0.5f)) || ((x < 0.5f) && (y < 0.5f)))
                result += (1.0f - t) * Color(0.2f, 0.2f, 0.2f) + t * Color(1.0f, 1.0f, 1.0f);
            else
                result += (1.0f - t) * Color(0.4f, 0.4f, 0.4f) + t * Color(1.0f, 1.0f, 1.0f);

            return result;
        }
        else
            return ray.direction[2] * Color(0.0, 0.0, 1.0) + (1 - ray.direction[2]) * Color(1.0, 1.0, 1.0);
    }
};

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

    Background *ptrBackground;

    Renderer() : ptrScene(0)
    {
        ptrBackground = new MyBackground();
    }
    Renderer(Scene &scene) : ptrScene(&scene)
    {
        ptrBackground = new MyBackground();
    }
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

        GraphicalObject *obj_i = 0; // pointer to intersected object
        Point3 p_i;                 // point of intersection

        // Look for intersection in this direction.
        Real ri = ptrScene->rayIntersection(ray, obj_i, p_i);
        // Nothing was intersected
        if (ri >= 0.0f)
            return background(ray); // some background color

        return illumination(ray, obj_i, p_i);
    }

    /// Calcule l'illumination de l'objet \a obj au point \a p, sachant que l'observateur est le rayon \a ray.
    Color illumination(const Ray &ray, GraphicalObject *obj, Point3 p)
    {
        Vector3 normal = obj->getNormal(p);

        Material material = obj->getMaterial(p);
        Color result(0, 0, 0);

        Vector3 mirror = reflect(ray.direction, normal);

        for (std::vector<Light *>::const_iterator it = ptrScene->myLights.begin(); it < ptrScene->myLights.end(); ++it)
        {
            Vector3 lightDirection = (*it)->direction(p);
            Color lightColor = (*it)->color(p);

            // Couleur de la lumière avec ombre
            Ray pointToLightRay(p, lightDirection);
            lightColor = this->shadow(pointToLightRay, lightColor);

            // Couleur diffuse
            double coeffD = lightDirection.dot(normal) / (lightDirection.norm() * normal.norm());

            if (coeffD < 0)
                coeffD = 0.0;

            result += coeffD * material.diffuse * lightColor;

            // Couleur spéculaire
            /**
             * Shininess ne prend pas de 'y' car pour transformer
             * un adjectif finissant par 'y' en nom (adj + ness)
             * il faut transformer le 'y' en 'i' !
             * Exemple : happy -> happiness.
             **/
            double angle = mirror.dot(lightDirection) / (lightDirection.norm() * mirror.norm());

            if (angle >= 0)
            {
                double coeffS = pow(angle, material.shinyness);

                result += coeffS * material.specular * lightColor;
            }
        }

        // Couleur ambiante
        return result + material.ambient;
    }

    /// Calcule le vecteur réfléchi à W selon la normale N.
    Vector3 reflect(const Vector3 &W, const Vector3 &N) const
    {
        return W - 2 * (W.dot(N)) * N;
    }

    // Affiche les sources de lumières avant d'appeler la fonction qui
    // donne la couleur de fond.
    Color background(const Ray &ray)
    {
        Color result = Color(0.0, 0.0, 0.0);
        for (Light *light : ptrScene->myLights)
        {
            Real cos_a = light->direction(ray.origin).dot(ray.direction);
            if (cos_a > 0.99f)
            {
                Real a = acos(cos_a) * 360.0 / M_PI / 8.0;
                a = std::max(1.0f - a, 0.0f);
                result += light->color(ray.origin) * a * a;
            }
        }

        if (ptrBackground != 0)
            result += ptrBackground->backgroundColor(ray);
        return result;
    }

    /// Calcule la couleur de la lumière (donnée par lightColor) dans la
    /// direction donnée par le rayon. Si aucun objet n'est traversé,
    /// retourne lightColor, sinon si un des objets traversés est opaque,
    /// retourne du noir, et enfin si les objets traversés sont
    /// transparents, attenue la couleur.
    Color shadow(const Ray &ray, Color lightColor)
    {
        Point3 rayOrigin = ray.origin - ray.direction * 0.001f;

        GraphicalObject *originObject = 0;
        Point3 pointOfIntersection;

        ptrScene->rayIntersection(Ray(rayOrigin, ray.direction, ray.depth), originObject, pointOfIntersection);

        rayOrigin += ray.direction * 0.001f;

        while (lightColor.max() > 0.003f)
        {
            rayOrigin += ray.direction * 0.001f;

            GraphicalObject *intersectedObj = 0;

            if (ptrScene->rayIntersection(Ray(rayOrigin, ray.direction, ray.depth), intersectedObj, pointOfIntersection) >= 0.0f 
                || intersectedObj == originObject)
                break;
            else
            {
                Material material = intersectedObj->getMaterial(pointOfIntersection);

                lightColor = lightColor * material.diffuse * material.coef_refraction;

                rayOrigin = pointOfIntersection;
                originObject = intersectedObj;
            }
        }

        return lightColor;
    }
};

} // namespace rt

#endif // #define _RENDERER_H_
