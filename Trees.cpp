

/*
 * Funhouse.cpp: A class for drawing the funhouse.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "trees.h"
#include "libtarga.h"
#include <stdio.h>
#include <math.h>
#include <GL/glu.h>

#define RADCON 3.14159265/180.0

 // Destructor

Tree::~Tree(void)
{
    if (initialized)
    {
        glDeleteLists(display_list, 1);
        glDeleteTextures(1, &texture_obj);
    }
}

bool Tree::DrawTree(float trunk_radius, float leaves_radius, float x_offset, float y_offset, float height)
{
    float colors[3][3] = {
        {50./255., 168./255., 82./255.},
        {168./255., 105./255., 50./255.},
        {189./255., 77./255., 34./255.}
    };
    /* sides */
    int color_idx = rand() % 3;
    float deg_offset = 10;
    glBegin(GL_TRIANGLES);
    glColor3f(colors[color_idx][0], colors[color_idx][1], colors[color_idx][2]);
    for (int k = 0; k <= 360; k += 1) {
        float pos = k * RADCON;
        float next_pos = (k + deg_offset) * RADCON;
        glVertex3f(x_offset, y_offset, height);
        glVertex3f(leaves_radius * cos(pos) + x_offset, leaves_radius * sin(pos) + y_offset, 0.5 * height);
        glVertex3f(leaves_radius * cos(next_pos) + x_offset, leaves_radius * sin(next_pos) + y_offset, 0.5 * height);
    }
    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.414, 0.305, 0.258);
    for (float k = 0; k <= 360; k += deg_offset) {
        float pos = k * RADCON;
        float next_pos = (k + deg_offset) * RADCON;
        glVertex3f(trunk_radius * cos(pos) + x_offset, trunk_radius * sin(pos) + y_offset, 0.75 * height);
        glVertex3f(trunk_radius * cos(pos) + x_offset, trunk_radius * sin(pos) + y_offset, 0);
        glVertex3f(trunk_radius * cos(next_pos) + x_offset, trunk_radius * sin(next_pos) + y_offset, 0);
        glVertex3f(trunk_radius * cos(next_pos) + x_offset, trunk_radius * sin(next_pos) + y_offset, 0.75 * height);

    }
    glEnd();

    return true;
}

// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool Tree::Initialize(void)
{
    ubyte* image_data;
    int	    image_height, image_width;

    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
    if (!(image_data = (ubyte*)tga_load("BrickTexture.tga", &image_width,
        &image_height, TGA_TRUECOLOR_24)))
    {
        fprintf(stderr, "Ground::Initialize: Couldn't load BrickTexture.tga\n");
        return false;
    }

    // This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, &texture_obj);
    glBindTexture(GL_TEXTURE_2D, texture_obj);

    // This sets a parameter for how the texture is loaded and interpreted.
    // basically, it says that the data is packed tightly in the image array.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // This sets up the texture with high quality filtering. First it builds
    // mipmaps from the image data, then it sets the filtering parameters
    // and the wrapping parameters. We want the texture to be repeated over the
    // cube.
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width, image_height,
        GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_NEAREST_MIPMAP_LINEAR);

    // This says what to do with the texture. Modulate will multiply the
    // texture by the underlying color.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Now do the geometry. Create the display list.
    display_list = glGenLists(1);
    glNewList(display_list, GL_COMPILE);
    // Use white, because the texture supplies the color.
    glColor3f(1.0, 1.0, 1.0);

    // The surface normal is up for the ground.
    glNormal3f(0.0, 0.0, 1.0);

    // Turn on texturing and bind the texture.
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_obj);

    float tree_params[7][5] = {
        {2, 5, 35, -30, 20},
        {3, 7, 10, -30, 24},
        {3, 6, 20, -40, 22},
        {2, 6, -40, 20, 18},
        {1.5, 5.5, -30, 45, 19},
        {2, 6, -25, 35, 17},
        {2, 6, -35, 25, 16}
    };

    for (int i = 0; i < 7; i++) {
        DrawTree(tree_params[i][0],
                 tree_params[i][1],
                 tree_params[i][2],
                 tree_params[i][3],
                 tree_params[i][4]
                );
    }

    glEndList();

    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}


// Draw just calls the display list we set up earlier.
void
Tree::Draw(void)
{
    glPushMatrix();
    glCallList(display_list);
    glPopMatrix();
}
