#pragma once

#include "objects/object.h"

class Tower : public Object {
public:
    Tower(Vec3 pos);

protected:
    virtual void draw() const override;
};