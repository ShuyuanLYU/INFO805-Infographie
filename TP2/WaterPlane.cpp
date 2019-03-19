#include "GraphicalObject.h"
#include "Wave.cpp"
#include <math.h>

/// Namespace RayTracer
namespace rt
{
struct WaterPlane : public GraphicalObject
{
    Point3 c;
    Vector3 u, v;
    Material main_m;
    std::vector<Wave> waveList;

    /// Creates a periodic infinite plane passing through \a c and
    /// tangent to \a u and \a v. Then \a w defines the width of the
    /// band around (0,0) and its period to put material \a band_m,
    /// otherwise \a main_m is used.
    WaterPlane(Point3 _c, Vector3 _u, Vector3 _v,
               Material _main_m) : c(_c), u(_u), v(_v), main_m(_main_m) {}

    void coordinates(Point3 p, Real &x, Real &y)
    {
        x = p[0];
        y = p[1];
    }

    void init(Viewer & /* viewer */) {}

    void draw(Viewer & /* viewer */)
    {
        glBegin(GL_TRIANGLES);
        glColor4fv(main_m.ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, main_m.diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, main_m.specular);
        glMaterialf(GL_FRONT, GL_SHININESS, main_m.shinyness);
        glVertex3f(0, 0, 0.0);
        glVertex3f(0, 1000.0, 0.0);
        glVertex3f(1000.0, 0, 0.0);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor4fv(main_m.ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, main_m.diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, main_m.specular);
        glMaterialf(GL_FRONT, GL_SHININESS, main_m.shinyness);
        glVertex3f(0, 1000.0, 0.0);
        glVertex3f(1000.0, 1000.0, 0.0);
        glVertex3f(1000.0, 0, 0.0);

        glEnd();
    }

    Vector3 getNormal(Point3 p)
    {
        waveList.clear();

        addWave(0.1f, 3.2f, 2.4f, 0.0f);
        addWave(0.2f, 2.4f, 0.8f, 0.0f);
        addWave(0.23f, 1.1f, 1.31f, 0.0f);
        addWave(0.03f, 0.54f, 0.52f, 0.0f);
        addWave(0.3f, 1.69f, 1.6f, 0.0f);

        Real x, y, derivativeX, derivativeY, a, r, l, phase;
        Vector3 direction, vy = Vector3(0.0, 0.0, 0.0), vx = Vector3(0.0, 0.0, 0.0);
        coordinates(p, x, y);

        for (std::vector<Wave>::const_iterator it = waveList.begin(); it < waveList.end(); ++it)
        {
            a = (*it).a;
            r = (*it).r;
            l = (*it).l;
            phase = (*it).phase;
            Real t = x * cos(a) + y * sin(a);

            if (t != 0.0f)
                phase = 0.0f;

            Real f = (2 * M_PI * t) / l + phase;

            derivativeX = (r * cos(a) * 2 * M_PI * sin(f)) / l;
            derivativeY = (r * sin(a) * 2 * M_PI * sin(f)) / l;

            vx += Vector3(1, 0, -derivativeX);
            vy += Vector3(0, 1, -derivativeY);
        }

        Vector3 cross = vx.cross(vy);

        Vector3 normal = cross / cross.norm();
        
        return normal;
    }

    Material getMaterial(Point3 /*p*/)
    {
        return main_m;
    }

    Real rayIntersection(const Ray &ray, Point3 &p)
    {
        Real t;
        Vector3 N = this->getNormal(c) / this->getNormal(c).norm();
        Vector3 W = ray.direction / ray.direction.norm();
        Real wn = W.dot(N);
        Vector3 n0w0 = (c - ray.origin);

        if (wn == 0.0f)
        {
            if ((N.dot(n0w0)) == 0)
            {
                t = 0;
                p = ray.origin + W * t;

                return -1.0f;
            }
            else
                return 1.0f;
        }
        else
        {
            t = N.dot(n0w0) / wn;
            p = ray.origin + W * t;
        }

        return t >= 0 ? -1.0f : 1.0f;
    }

    void addWave(Real r, Real a, Real l, Real phase)
    {
        waveList.push_back(Wave(r, a, l, phase));
    }
}; // namespace rt
} // namespace rt
