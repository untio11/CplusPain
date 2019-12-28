#ifndef CPLUSPAIN_PRIMITIVE_H
#define CPLUSPAIN_PRIMITIVE_H

#include <glm/vec3.hpp>
class Prop;

class Primitive {
public:
	Primitive(Prop* _source, int shape, int face, glm::vec3 _center) :
			source(_source), sh(shape), fa(face), center(_center) {};
	// The Mesh that this primitive originated from.
	Prop* source;
	// Index to the shape and face indices of the mesh.
	int sh, fa;
	glm::vec3 center;
	unsigned int morton_code = 0;
};


#endif //CPLUSPAIN_PRIMITIVE_H
