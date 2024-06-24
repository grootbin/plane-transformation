#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector2f* v)
{
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    return {c1,c2,c3};
}

static bool insideTriangle(float x, float y, const Vector2f* _v)
{   
    Vector3f v01 = {_v[1].x() - _v[0].x(), _v[1].y() - _v[0].y(), 0};
    Vector3f v0p = {x - _v[0].x(), y - _v[0].y(), 0};
    Vector3f c1 = v01.cross(v0p);

    Vector3f v12 = {_v[2].x() - _v[1].x(), _v[2].y() - _v[1].y(), 0};
    Vector3f v1p = {x - _v[1].x(), y - _v[1].y(), 0};
    Vector3f c2 = v12.cross(v1p);

    Vector3f v20 = {_v[0].x() - _v[2].x(), _v[0].y() - _v[2].y(), 0};
    Vector3f v2p = {x - _v[2].x(), y - _v[2].y(), 0};
    Vector3f c3 = v20.cross(v2p);

    if ((c1.z() < 0 && c2.z() < 0 && c3.z() < 0) || (c1.z() >= 0 && c2.z() >= 0 && c3.z() >= 0))
    {
        return true;
    }

    return false;
}

auto to_vec3(const Eigen::Vector2f& v2, float z = 1.0f)
{
    return Vector3f(v2.x(), v2.y(), z);
}

void rst::rasterizer::draw(const std::vector<Triangle> &tris)
{
    Eigen::Matrix3f transform = translation * rotation * scale;
    for (auto &t : tris)
    {
        Triangle newTri;

        Eigen::Vector3f v[] = {
            transform * to_vec3(t.v[0], 1.0f),
            transform * to_vec3(t.v[1], 1.0f),
            transform * to_vec3(t.v[2], 1.0f)
        };

        for (auto& vec : v) {
            vec /= vec.z();
        }

        for (int i = 0; i < 3; ++i)
        {
            newTri.setVertex(i, v[i].head<2>());
            newTri.color[i] = t.color[i];
        }

        rasterize_triangle(newTri);
    }
}

//Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle &t) {
    int xMin = std::min(t.v[0].x(), std::min(t.v[1].x(), t.v[2].x()));
    int xMax = std::max(t.v[0].x(), std::max(t.v[1].x(), t.v[2].x()));
    int yMin = std::min(t.v[0].y(), std::min(t.v[1].y(), t.v[2].y()));
    int yMax = std::max(t.v[0].y(), std::max(t.v[1].y(), t.v[2].y()));

    float x = 0;
    float y = 0;
    for (int i = xMin; i <= xMax; i++)
    {
        for (int j = yMin; j <= yMax; j++)
        {
            x = i + 0.5;
            y = j + 0.5;

            if (insideTriangle(x, y, t.v))
            {
                // If so, use the following code to get the interpolated z value.
                auto[alpha, beta, gamma] = computeBarycentric2D(x, y, t.v);
                Vector3f color = alpha * t.color[0] + beta * t.color[1] + gamma * t.color[2];

                set_pixel({i, j}, color*255);
            }
        }
    }
}

void rst::rasterizer::set_scale(const Eigen::Matrix3f& s)
{
    scale = s;
}

void rst::rasterizer::set_rotation(const Eigen::Matrix3f& r)
{
    rotation = r;
}

void rst::rasterizer::set_translation(const Eigen::Matrix3f& t)
{
    translation = t;
}

void rst::rasterizer::clear()
{
    std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
}

void rst::rasterizer::set_pixel(const Eigen::Vector2f& point, const Eigen::Vector3f& color)
{
    auto ind = (height/2-1-point.y())*width + point.x() + width/2;
    frame_buf[ind] = color;
}
