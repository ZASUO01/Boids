#pragma once

#include "objects/object.h"

class Cube : public Object {
public:
    Cube(Vec3 pos) : Object(pos) {}

protected:
    virtual void draw() const override;
};
