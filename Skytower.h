/*
Skytower class for a funhouse that I will create and add spooky castle texture to
*/

#include <Fl/gl.h>

class Skytower {
private:
    GLubyte display_list;   // The display list that does all the work.
    GLuint  texture_obj;    // The object for the texture.
    bool    initialized;    // Whether or not we have been initialised.

public:
    // Constructor. Can't do initialization here because we are
    // created before the OpenGL context is set up.
    Skytower(void) { display_list = 0; initialized = false; };

    // Destructor. Frees the display lists and texture object.
    ~Skytower(void);

    // Initializer. Creates the display list.
    bool    Initialize(void);

    // Does the drawing.
    void    Draw(void);
}; 
