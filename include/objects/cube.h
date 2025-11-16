#pragma once

#include "objects/object.h"

class Cube : public Object {
public:
    Cube(Vec3 pos) : Object(pos) {}

    virtual void update(float delta_time, const std::vector<std::unique_ptr<Object>>& all_objects, Object* player_boid) override;
protected:
    virtual void draw() const override;
};
