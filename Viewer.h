#include <vector>
#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer
{
    public:
    Viewer() : QGLViewer() {}
  
    protected :
    /// Called at each draw of the window
    virtual void draw();
    
    /// Called before the first draw
    virtual void init();
    
    /// Called when pressing help.
    virtual QString helpString() const;
};