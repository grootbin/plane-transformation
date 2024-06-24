#ifndef RASTERIZER_TRIANGLE_H
#define RASTERIZER_TRIANGLE_H

#include <eigen3/Eigen/Eigen>

using namespace Eigen;
class Triangle{

public:
    Vector2f v[3]; /*the original coordinates of the triangle, v0, v1, v2 in counter clockwise order*/
    /*Per vertex values*/
    Vector3f color[3]; //color at each vertex;

    //Texture *tex;
    Triangle();

    void setVertex(int ind, Vector2f ver); /*set i-th vertex coordinates */
    void setColor(int ind, float r, float g, float b); /*set i-th vertex color*/
    Vector3f getColor(int ind) const { return color[ind]*255; } // Only one color per triangle.
};

#endif //RASTERIZER_TRIANGLE_H
