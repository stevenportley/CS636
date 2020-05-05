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

bool BoundingBox::does_intersect(const Ray& ray)
{

    auto swap = [](float& a, float& b){
        float temp = a;
        a = b;
        b = temp;
    };

   /** initalize near and far to x **/
    float tnear = (this->p1.x - ray.origin.x) / ray.direction.x;
    float tfar = (this->p2.x - ray.origin.x) / ray.direction.x;

    if(tnear > tfar) swap(tnear, tfar);

    /** test y **/
    float ty1 = (this->p1.y - ray.origin.y) / ray.direction.y;
    float ty2 = (this->p2.y - ray.origin.y) / ray.direction.y;

    if(ty1 > ty2) swap(ty1, ty2);
    

    if(ty1 > tnear)
        tnear = ty1;

    if(ty2 < tfar)
        tfar = ty2;

    /** test z **/
    float tz1 = (this->p1.z - ray.origin.z) / ray.direction.z;
    float tz2 = (this->p2.z - ray.origin.z) / ray.direction.z;

    if(tz1 > tz2) swap(tz1, tz2);

    if(tz1 > tnear)
        tnear = tz1;

    if(tz2 < tfar)
        tfar = tz2;

    /** box missed **/
    if(tnear > tfar)
        return false;

    /** box is behind **/
    if(tfar < 0)
        return false;

    return true;
}

        