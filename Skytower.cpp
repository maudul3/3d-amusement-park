

/*
 * Funhouse.cpp: A class for drawing the funhouse.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Skytower.h"
#include "libtarga.h"
#include <stdio.h>
#include <math.h>
#include <GL/glu.h>
#define RADCON 3.14159265/180.0


 // Destructor

Skytower::~Skytower(void)
{
    if (initialized)
    {
        glDeleteLists(display_list, 1);
        glDeleteTextures(1, &texture_obj);
    }
}


// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool Skytower::Initialize(void)
{
    ubyte* image_data;
    int	    image_height, image_width;

    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
    if (!(image_data = (ubyte*)tga_load("windows-texture.tga", &image_width,
        &image_height, TGA_TRUECOLOR_24)))
    {
        fprintf(stderr, "Skytower::Initialize: Couldn't load windows-texture.tga\n");
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

    float x_offset = -35, y_offset = -35, z_offset = 20, window_height = 10, antenna_height = 4;
    float pole_radius = 3, bottom_radius = 12, top_radius = 6, antenna_radius = 1;

    /* sides */
    int deg_offset = 40;
    glBegin(GL_QUADS);
    glColor3f(0.5, 0.5, 0.5);
    for (float k = 0; k <= 360; k += deg_offset) {
        float pos = k * RADCON;
        float next_pos = (k + deg_offset) * RADCON;
        glVertex3f(pole_radius * cos(pos) + x_offset, pole_radius * sin(pos) + y_offset, z_offset);
        glVertex3f(pole_radius * cos(pos) + x_offset, pole_radius * sin(pos) + y_offset, 0);
        glVertex3f(pole_radius * cos(next_pos) + x_offset, pole_radius * sin(next_pos) + y_offset, 0);
        glVertex3f(pole_radius * cos(next_pos) + x_offset, pole_radius * sin(next_pos) + y_offset, z_offset);

    }
    glEnd();

    deg_offset = 20;

    glBegin(GL_TRIANGLES);
    for (float k = 0; k <= 360; k += deg_offset) {
        float pos = k * RADCON;
        float next_pos = (k + deg_offset) * RADCON;
        glVertex3f(x_offset, y_offset, z_offset + window_height);
        glVertex3f(top_radius * cos(pos) + x_offset, top_radius * sin(pos) + y_offset, z_offset + window_height);
        glVertex3f(top_radius * cos(next_pos) + x_offset, top_radius * sin(next_pos) + y_offset, z_offset + window_height);

    }
    glEnd();

    glBegin(GL_TRIANGLES);
    for (float k = 0; k <= 360; k += deg_offset) {
        float pos = k * RADCON;
        float next_pos = (k + deg_offset) * RADCON;
        glVertex3f(x_offset, y_offset, z_offset);
        glVertex3f(bottom_radius * cos(next_pos) + x_offset, bottom_radius * sin(next_pos) + y_offset, z_offset);
        glVertex3f(bottom_radius * cos(pos) + x_offset, bottom_radius * sin(pos) + y_offset, z_offset);


    }
    glEnd();

    glBegin(GL_TRIANGLES);
    for (float k = 0; k <= 360; k += deg_offset) {
        float pos = k * RADCON;
        float next_pos = (k + deg_offset) * RADCON;
        glVertex3f( x_offset, y_offset, z_offset + window_height + antenna_height);
        glVertex3f(antenna_radius * cos(pos) + x_offset, antenna_radius * sin(pos) + y_offset, z_offset + window_height);
        glVertex3f(antenna_radius * cos(next_pos) + x_offset, antenna_radius * sin(next_pos) + y_offset, z_offset + window_height);
    }
    glEnd();

    // Turn on texturing and bind the texture.
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_obj);

    glColor3f(1, 1, 1);
    deg_offset = 9;
    glBegin(GL_QUADS);
    for (float k = 0; k <= 360; k += deg_offset) {
        float pos = k * RADCON;
        float next_pos = (k + deg_offset) * RADCON;
        glTexCoord2f(1, 0);
        glVertex3f(top_radius * cos(pos) + x_offset, top_radius * sin(pos) + y_offset, z_offset + window_height);
        glTexCoord2f(1, 1);
        glVertex3f(bottom_radius * cos(pos) + x_offset, bottom_radius * sin(pos) + y_offset, z_offset);
        glTexCoord2f(0, 1);
        glVertex3f(bottom_radius * cos(next_pos) + x_offset, bottom_radius * sin(next_pos) + y_offset, z_offset);
        glTexCoord2f(0, 0);
        glVertex3f(top_radius * cos(next_pos) + x_offset, top_radius * sin(next_pos) + y_offset, z_offset + window_height);

    }
    glEnd();
    
    glDisable(GL_TEXTURE_2D);

    glEndList();

    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}


// Draw just calls the display list we set up earlier.
void
Skytower::Draw(void)
{
    glPushMatrix();
    glCallList(display_list);
    glPopMatrix();
}
