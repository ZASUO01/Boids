#pragma once

#include "objects/object.h"

class Ground : public Object {
public:
    Ground(Vec3 pos);

protected:
    virtual void draw() const override;
};