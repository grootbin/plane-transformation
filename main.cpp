#include <iostream>
#include <opencv2/opencv.hpp>
#include "rasterizer.hpp"
#include "Triangle.hpp"

constexpr double MY_PI = 3.1415926;

Eigen::Matrix3f get_scale_matrix(float xScale, float yScale)
{
    Eigen::Matrix3f scale;
    scale << xScale, 0, 0,
            0, yScale, 0,
            0, 0, 1;

    return scale;
}

Eigen::Matrix3f get_rotation_matrix(float angle)
{
    double arc_angle = angle/180.0*MY_PI;
    Eigen::Matrix3f rotation;
    rotation << std::cos(arc_angle), -std::sin(arc_angle), 0,
            std::sin(arc_angle), std::cos(arc_angle), 0,
            0, 0, 1;

    return rotation;
}

Eigen::Matrix3f get_translation_matrix(float x, float y)
{
    Eigen::Matrix3f scale;
    scale << 1, 0, x,
            0, 1, y,
            0, 0, 1;

    return scale;
}

int main(int argc, const char** argv)
{
    rst::rasterizer r(700, 700);

    std::vector<Triangle> tris;
    Triangle t1;
    t1.setVertex(0, {0, 0});
    t1.setVertex(1, {173.2050807, 100});
    t1.setVertex(2, {-173.2050807, 100});
    t1.setColor(0, 80, 81, 109);
    t1.setColor(1, 121, 124, 131);
    t1.setColor(2, 170, 174, 149);
    tris.emplace_back(t1);

    Triangle t2;
    t2.setVertex(0, {0, 0});
    t2.setVertex(1, {-173.2050807, 100});
    t2.setVertex(2, {0, -200});
    t2.setColor(0, 121, 124, 131);
    t2.setColor(1, 170, 174, 149);
    t2.setColor(2, 213, 207, 159);
    tris.emplace_back(t2);

    Triangle t3;
    t3.setVertex(0, {0, 0});
    t3.setVertex(1, {0, -200});
    t3.setVertex(2, {173.2050807, 100});
    t3.setColor(0, 170, 174, 149);
    t3.setColor(1, 213, 207, 159);
    t3.setColor(2, 250, 249, 184);
    tris.emplace_back(t3);

    int key = 0;
    int frame_count = 0;

    float xScale = 1.0;
    float yScale = 1.0;
    float angle = 0;
    float x = 0.0;
    float y = 0.0;
    while(key != 27)
    {
        r.clear();

        r.set_scale(get_scale_matrix(xScale, yScale));
        r.set_rotation(get_rotation_matrix(angle));
        r.set_translation(get_translation_matrix(x, y));
        r.draw(tris);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        switch(key)
        {
        case 'a':
            xScale -= 0.1;
            break;
        case 'd':
            xScale += 0.1;
            break;
        case 'w':
            yScale += 0.1;
            break;
        case 's':
            yScale -= 0.1;
            break;
        case 'f':
            angle += 10;
            break;
        case 'h':
            angle -= 10;
            break;
        case 'j':
            x -= 10;
            break;
        case 'l':
            x += 10;
            break;
        case 'k':
            y -= 10;
            break;
        case 'i':
            y += 10;
            break;
        default:
            break;
        }
    }

    return 0;
}
