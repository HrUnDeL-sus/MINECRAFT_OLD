#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
const GLfloat texture_coord[]={
0.5f, 0.5f, // top right
0.5f, 0.44f, // bottom right
0.44f, 0.44f, // bottom left
0.44f, 0.5f  // top left
};
int load_texture(char * name);

#endif // TEXTURE_H_INCLUDED
