#include "entity.hpp"


inline glm::vec2 perp(const glm::vec2& v) { return { v.y, -v.x }; }


bool Entity::check_collision_with_points(const std::vector<glm::vec2> ps,
									     const glm::vec2&             offset) const
{
	assert(!m_triangles.empty());

	for (glm::ivec3 const & t : m_triangles) {

		float distance = INFINITY;
		bool colli = true;

		for (int i = 0; i < 3; ++i) {
			const glm::vec2& p1 = m_transformed_vertices[t[i]];
			const glm::vec2& p2 = m_transformed_vertices[t[(i + 1) % 3]];
			glm::vec2 n = perp(p2 - p1);

			float c_d = 0;

			for (const glm::vec2& p : ps) {
				float d = glm::dot(p - p1 - offset, n);
				if (d > c_d) {
					c_d = d;
				}
			}
			if (c_d == 0) {
				colli = false;
				break;
			}
			float l = glm::length(n);
			c_d /= l;

			if (c_d < distance) {
				distance = c_d;
			}
		}

		if (colli) return true;
	}

	return false;
}


bool Entity::check_collision(const Entity& other, const glm::vec2& offset) const {
	float r = m_radius + other.m_radius;
	if (glm::length2(m_pos - other.m_pos + offset) >= r * r) return false;

	if (m_triangles.empty() && other.m_triangles.empty()) return true;

	if (!m_triangles.empty() && !other.m_triangles.empty()) {

		// test all triangles against all other points and vice versa
		return check_collision_with_points(other.m_transformed_vertices, offset) ||
			   other.check_collision_with_points(m_transformed_vertices, -offset);
	}

	// TODO
	printf("TODO: implement triangles-circle-collision\n");
	return true;
}


bool Entity::check_collision(const Entity& other) const {
	if (m_collision_category & other.m_collision_mask &&
		other.m_collision_category & m_collision_mask)
	{
        int sy = (other.m_pos.y > HEIGHT / 2) - (m_pos.y > HEIGHT / 2);
        int sx = (other.m_pos.x > WIDTH / 2)  - (m_pos.x > WIDTH / 2);
		if (check_collision(other, glm::vec2(sx * WIDTH, sy * HEIGHT))) return true;
	}
	return false;
}

