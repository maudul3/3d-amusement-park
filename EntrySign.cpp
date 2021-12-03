/*
 * Funhouse.cpp: A class for drawing the funhouse.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "EntrySign.h"
#include "libtarga.h"
#include <stdio.h>
#include <math.h>
#include <GL/glu.h>

 // Destructor
EntrySign::~EntrySign(void)
{
    if (initialized)
    {
        glDeleteLists(display_list, 1);
        glDeleteTextures(1, &texture_obj);
    }
}


// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool
EntrySign::Initialize(void)
{
    ubyte* image_data;
    int	    image_height, image_width;

    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
    if (!(image_data = (ubyte*)tga_load("carnival.tga", &image_width,
        &image_height, TGA_TRUECOLOR_24)))
    {
        fprintf(stderr, "Ground::Initialize: Couldn't load carnival.tga\n");
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

    glBegin(GL_QUADS);
    float x_offset = -47.0;
    float y_offset = -17;

    // Front face (facing towards the rollercoaster)
    glTexCoord2f(0, 0);
    glVertex3f(0.01 + x_offset, -3 + y_offset, 12.0);
    glTexCoord2f(1, 0);
    glVertex3f(0.01 + x_offset, 30 + y_offset, 12.0);
    glTexCoord2f(1, 1);
    glVertex3f(0.01 + x_offset, 30 + y_offset, 20.0);
    glTexCoord2f(0, 1);
    glVertex3f(0.01 + x_offset, -3 + y_offset, 20.0);

    // Back face (facing towards the rollercoaster)
    glVertex3f(-3.01 + x_offset, -3 + y_offset, 20.0);
    glVertex3f(-3.01 + x_offset, 30 + y_offset, 20.0);
    glVertex3f(-3.01 + x_offset, 30 + y_offset, 12.0);
    glVertex3f(-3.01 + x_offset, -3 + y_offset, 12.0);


    glEnd();

    // Create a prism that will serve as the left support
    glBegin(GL_QUADS);
    // Floor
    glColor3f(1, 0.76, 0.59);
    glVertex3f(0 + x_offset, -3 + y_offset, 0.0);
    glVertex3f(0 + x_offset, 0 + y_offset, 0.0);
    glVertex3f(-3 + x_offset, 0  + y_offset, 0.0);
    glVertex3f(-3 + x_offset, -3 + y_offset, 0.0);

    // Top
    glVertex3f(0 + x_offset, -3 + y_offset, 20.0);
    glVertex3f(0 + x_offset, 0 + y_offset, 20.0);
    glVertex3f(-3 + x_offset, 0 + y_offset, 20.0);
    glVertex3f(-3 + x_offset, -3 + y_offset, 20.0);

    // Front face (facing towards the rollercoaster)
    glVertex3f(0 + x_offset, -3 + y_offset, 0.0);
    glVertex3f(0 + x_offset, 0 + y_offset, 0.0);
    glVertex3f(0 + x_offset, 0 + y_offset, 20.0);
    glVertex3f(0 + x_offset, -3 + y_offset, 20.0);

    // Back face (facing towards the emptiness)
    glVertex3f(-3 + x_offset, -3 + y_offset, 20.0);
    glVertex3f(-3 + x_offset, 0 + y_offset, 20.0);
    glVertex3f(-3 + x_offset, 0 + y_offset, 0.0);
    glVertex3f(-3 + x_offset, -3 + y_offset, 0.0);

    // Side face (facing towards the emptiness)
    glVertex3f(-3 + x_offset, -3 + y_offset, 0.0);
    glVertex3f(0 + x_offset, -3 + y_offset, 0.0);
    glVertex3f(0 + x_offset, -3 + y_offset, 20.0);
    glVertex3f(-3 + x_offset, -3 + y_offset, 20.0);

    // Other Side face (facing towards the emptiness)
    glVertex3f(-3 + x_offset, y_offset, 20.0);
    glVertex3f(0 + x_offset, y_offset, 20.0);
    glVertex3f(0 + x_offset, y_offset, 0.0);
    glVertex3f(-3 + x_offset, y_offset, 0.0);

    glEnd();


    y_offset = 13;
    // Create a prism that will serve as the right support
    glBegin(GL_QUADS);

    // Floor
    glVertex3f(0 + x_offset, -3 + y_offset, 0.0);
    glVertex3f(0 + x_offset, 0 + y_offset, 0.0);
    glVertex3f(-3 + x_offset, 0 + y_offset, 0.0);
    glVertex3f(-3 + x_offset, -3 + y_offset, 0.0);

    // Top
    glVertex3f(0 + x_offset, -3 + y_offset, 20.0);
    glVertex3f(0 + x_offset, 0 + y_offset, 20.0);
    glVertex3f(-3 + x_offset, 0 + y_offset, 20.0);
    glVertex3f(-3 + x_offset, -3 + y_offset, 20.0);

    // Front face (facing towards the rollercoaster)
    glVertex3f(0 + x_offset, -3 + y_offset, 0.0);
    glVertex3f(0 + x_offset, 0 + y_offset, 0.0);
    glVertex3f(0 + x_offset, 0 + y_offset, 20.0);
    glVertex3f(0 + x_offset, -3 + y_offset, 20.0);

    // Back face (facing towards the emptiness)
    glVertex3f(-3 + x_offset, -3 + y_offset, 20.0);
    glVertex3f(-3 + x_offset, 0 + y_offset, 20.0);
    glVertex3f(-3 + x_offset, 0 + y_offset, 0.0);
    glVertex3f(-3 + x_offset, -3 + y_offset, 0.0);

    // Side face (facing towards the emptiness)
    glVertex3f(-3 + x_offset, -3 + y_offset, 0.0);
    glVertex3f(0 + x_offset, -3 + y_offset, 0.0);
    glVertex3f(0 + x_offset, -3 + y_offset, 20.0);
    glVertex3f(-3 + x_offset, -3 + y_offset, 20.0);

    // Other Side face (facing towards the emptiness)
    glVertex3f(-3 + x_offset, y_offset, 20.0);
    glVertex3f(0 + x_offset, y_offset, 20.0);
    glVertex3f(0 + x_offset, y_offset, 0.0);
    glVertex3f(-3 + x_offset, y_offset, 0.0);

    glEnd();

    // Turn texturing off again, because we don't want everything else to
    // be textured.
    glDisable(GL_TEXTURE_2D);
    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}


// Draw just calls the display list we set up earlier.
void
EntrySign::Draw(void)
{
    glPushMatrix();
    glCallList(display_list);
    glPopMatrix();
}
