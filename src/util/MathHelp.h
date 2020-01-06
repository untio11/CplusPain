#ifndef CPLUSPAIN_MATHHELP_H
#define CPLUSPAIN_MATHHELP_H

#include <glm/vec3.hpp>
#include <algorithm>

namespace MathHelp {
	static glm::vec3 getMin(glm::vec3 a, glm::vec3 b) {
		auto min = glm::vec3(0);

		min.x = std::min(a.x, b.x);
		min.y = std::min(a.y, b.y);
		min.z = std::min(a.z, b.z);

		return min;
	}

	static glm::vec3 getMax(glm::vec3 a, glm::vec3 b) {
		auto max = glm::vec3(0);

		max.x = std::max(a.x, b.x);
		max.y = std::max(a.y, b.y);
		max.z = std::max(a.z, b.z);

		return max;
	}
}

#endif //CPLUSPAIN_MATHHELP_H
