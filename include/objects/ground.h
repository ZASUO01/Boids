#pragma once

#include "objects/object.h"

class Ground : public Object {
public:
    Ground(Vec3 pos);

    static constexpr float GROUND_SIZE = 200.0f;


protected:
    virtual void draw() const override;
};