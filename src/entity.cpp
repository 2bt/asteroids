// vim: ts=4 sw=4 sts=4 et
#include "entity.hpp"


inline glm::vec2 perp(const glm::vec2& v) { return { v.y, -v.x }; }


static bool triangle_collision(const glm::vec2* a, const glm::vec2* b) {
    for (int side = 0; side < 2; ++side) {
        float dist = INFINITY;
        for (int i = 0; i < 3; ++i) {
            const glm::vec2& p1 = a[i];
            const glm::vec2& p2 = a[(i + 1) % 3];
            glm::vec2 n = perp(p2 - p1);
            float c_d = 0;
            for (int j = 0; j < 3; ++j) {
                float d = glm::dot(b[j] - p1, n);
                if (d > c_d) c_d = d;
            }
            if (c_d == 0) return false;
            float l = glm::length(n);
            c_d /= l;
            if (c_d < dist) {
                dist = c_d;
            }
        }
        std::swap(a, b);
    }
    return true;
}


bool Entity::check_collision(const Entity& other, const glm::vec2& offset) const {
    float r = m_radius + other.m_radius;
    if (glm::length2(m_pos - other.m_pos + offset) >= r * r) return false;
    if (m_triangles.empty() && other.m_triangles.empty()) return true;
    if (!m_triangles.empty() && !other.m_triangles.empty()) {

        // test all triangles against all other triangles
        for (glm::ivec3 const & t1 : m_triangles) {
            std::array<glm::vec2, 3> a = {
                m_transformed_vertices[t1[0]] + offset,
                m_transformed_vertices[t1[1]] + offset,
                m_transformed_vertices[t1[2]] + offset,
            };
            for (glm::ivec3 const & t2 : other.m_triangles) {
                std::array<glm::vec2, 3> b = {
                    other.m_transformed_vertices[t2[0]],
                    other.m_transformed_vertices[t2[1]],
                    other.m_transformed_vertices[t2[2]],
                };
                if (triangle_collision(a.data(), b.data())) return true;
            }
        }
        return false;
    }

    // TODO
    printf("TODO: implement triangles-circle-collision\n");
    return true;
}


bool Entity::check_collision(const Entity& other) const {
    if (m_collision_category & other.m_collision_mask &&
        other.m_collision_category & m_collision_mask)
    {
        glm::vec2 dif = other.m_pos - m_pos;
        glm::vec2 offset = {};
        if (dif.x >  WIDTH / 2)  offset.x =  WIDTH;
        if (dif.x < -WIDTH / 2)  offset.x = -WIDTH;
        if (dif.y >  HEIGHT / 2) offset.y =  HEIGHT;
        if (dif.y < -HEIGHT / 2) offset.y = -HEIGHT;

        return check_collision(other, offset);
    }
    return false;
}

