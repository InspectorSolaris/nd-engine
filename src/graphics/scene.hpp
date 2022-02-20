#pragma once

#include "pch.hpp"
#include "tools.hpp"

namespace nd::src::graphics
{
    using Index = u16;

    struct Vertex final
    {
        glm::vec3 position;
        glm::vec3 color;
    };

    struct Transform final
    {
        glm::vec3 rotation;
        glm::vec3 scalation;
        glm::vec3 translation;
    };

    struct Camera final
    {
        glm::vec3 location;
        glm::vec3 center;
        glm::vec3 up;

        f32 fovx;
        f32 ratio;
        f32 near;
        f32 far;
    };

    struct Mesh final
    {
        vec<Index>  indices;
        vec<Vertex> vertices;
    };

    struct Instance final
    {
        Transform transform;

        u64 meshIndex;
    };

    struct Scene final
    {
        Camera camera;

        vec<Mesh>     meshes;
        vec<Instance> instances;
    };
} // namespace nd::src::graphics
