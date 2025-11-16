#pragma once

#include "objects/object.h"

class Tower : public Object {
public:
    Tower(Vec3 pos);

    static constexpr float TOWER_HEIGHT = 30.0f;

protected:
    virtual void draw() const override;
};