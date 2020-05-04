#include <optional>
#include <vector>
#include <iostream>
#include <vector3.h>
#include "model.h"
#include "boundingbox.h"

BoundingBox::BoundingBox(Vector3 p1, Vector3 p2)
{
    this->p1 = p1;
    this->p2 = p2;
}

BoundingBox::BoundingBox(const std::vector<BoundingBox>& boxes)
{
    if(boxes.size() == 0)
    {
        std::cout << "Boxes of size 0 in constructor" << std::endl;
        exit(1);
    }

    float smallest_x = boxes[0].p1.x < boxes[0].p2.x ? boxes[0].p1.x : boxes[0].p2.x;
    float smallest_y = boxes[0].p1.y < boxes[0].p2.y ? boxes[0].p1.y : boxes[0].p2.y;
    float smallest_z = boxes[0].p1.z < boxes[0].p2.z ? boxes[0].p1.z : boxes[0].p2.z;

    float greatest_x = boxes[0].p1.x > boxes[0].p2.x ? boxes[0].p1.x : boxes[0].p2.x;
    float greatest_y = boxes[0].p1.y > boxes[0].p2.y ? boxes[0].p1.y : boxes[0].p2.y;
    float greatest_z = boxes[0].p1.z > boxes[0].p2.z ? boxes[0].p1.z : boxes[0].p2.z;



    for(auto& box : boxes)
    {

        if(box.p1.x < smallest_x)
            smallest_x = box.p1.x;
        if(box.p1.y < smallest_y)
            smallest_y = box.p1.y;
        if(box.p1.z < smallest_z)
            smallest_z = box.p1.z;

        if(box.p1.x > greatest_x)
            greatest_x = box.p1.x;
        if(box.p1.y > greatest_y)
            greatest_y = box.p1.y;
        if(box.p1.z > greatest_z)
            greatest_z = box.p1.z;


        if(box.p2.x < smallest_x)
            smallest_x = box.p2.x;
        if(box.p2.y < smallest_y)
            smallest_y = box.p2.y;
        if(box.p2.z < smallest_z)
            smallest_z = box.p2.z;

        if(box.p2.x > greatest_x)
            greatest_x = box.p2.x;
        if(box.p2.y > greatest_y)
            greatest_y = box.p2.y;
        if(box.p2.z > greatest_z)
            greatest_z = box.p2.z;

    }

    this->p1 = { smallest_x, smallest_y, smallest_z};
    this->p2 = { greatest_x, greatest_y, greatest_z};

}

bool does_intersect(const Ray& ray)
{
    return false;
}

        