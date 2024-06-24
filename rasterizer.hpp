#pragma once

#include <eigen3/Eigen/Eigen>
#include <algorithm>
#include "Triangle.hpp"
using namespace Eigen;

namespace rst
{
    class rasterizer
    {
    public:
        rasterizer(int w, int h);

        void set_scale(const Eigen::Matrix3f& s);
        void set_rotation(const Eigen::Matrix3f& r);
        void set_translation(const Eigen::Matrix3f& t);

        void set_pixel(const Eigen::Vector2f& point, const Eigen::Vector3f& color);

        void clear();

        std::vector<Eigen::Vector3f>& frame_buffer() { return frame_buf; }

        void draw(const std::vector<Triangle> &tris);

        void rasterize_triangle(const Triangle &t);

    private:
        Eigen::Matrix3f scale;
        Eigen::Matrix3f rotation;
        Eigen::Matrix3f translation;

        std::vector<Eigen::Vector3f> frame_buf;

        int width, height;
    };
}