#include <Fl/gl.h>

class Tree {
    private:
        GLubyte display_list;   // The display list that does all the work.
        GLuint  texture_obj;    // The object for the tree texture.
        bool    initialized;    // Whether or not we have been initialised.

    public:
        // Constructor. Can't do initialization here because we are
        // created before the OpenGL context is set up.
        Tree(void) { display_list = 0; initialized = false; };

        // Destructor. Frees the display lists and texture object.
        ~Tree(void);

        // Initializer. Creates the display list.
        bool    Initialize(void);

        // Draw Trees function
        bool DrawTree(float trunk_radius, float leaves_radius, float x_offset, float y_offset, float height);

        // Does the drawing.
        void    Draw(void);
};