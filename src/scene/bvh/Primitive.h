#ifndef CPLUSPAIN_PRIMITIVE_H
#define CPLUSPAIN_PRIMITIVE_H

#include <glm/vec3.hpp>

class Prop;

namespace Morton {
	/**
	 * Expands a 10-bit integer into 30 bits by inserting 2 zeros after each bit.
	 * @return 30 bit representation of 10 bit int.
	 */
	static unsigned int expandBits(unsigned int v) {
		v = (v * 0x00010001u) & 0xFF0000FFu;
		v = (v * 0x00000101u) & 0x0F00F00Fu;
		v = (v * 0x00000011u) & 0xC30C30C3u;
		v = (v * 0x00000005u) & 0x49249249u;
		return v;
	}

	/**
	 * Calculates a 30-bit Morton code for the given 3D point located within the unit cube [0,1].
	 * @return 30 bit morton code.
	 */
	static unsigned int morton3D(float x, float y, float z) {
		x = std::min(std::max(x * 1024.0f, 0.0f), 1023.0f);
		y = std::min(std::max(y * 1024.0f, 0.0f), 1023.0f);
		z = std::min(std::max(z * 1024.0f, 0.0f), 1023.0f);
		unsigned int xx = expandBits((unsigned int)x);
		unsigned int yy = expandBits((unsigned int)y);
		unsigned int zz = expandBits((unsigned int)z);
		return xx * 4 + yy * 2 + zz;
	}
}

class Primitive {
public:
	Primitive(Prop* _source, int shape, int face, glm::vec3 _center) :
			source(_source), sh(shape), fa(face), center(_center), morton_code(0), index(0) {};
	// The Mesh that this primitive originated from.
	Prop* source;
	// Index into source->primitives
	int index;
	// Index to the shape and face indices of the mesh.
	int sh, fa;
	// Might not need the actual center coordinate.
	glm::vec3 center;
	unsigned int morton_code;

	inline bool operator < (Primitive const &prim) const {
		return (morton_code < prim.morton_code);
	};

//	inline bool equals(Primitive const &node) const {
//		return (node.fa == fa && node.sh == sh);
//	};
};


#endif //CPLUSPAIN_PRIMITIVE_H
