#ifndef CPLUSPAIN_SHADERPROGRAM_H
#define CPLUSPAIN_SHADERPROGRAM_H

#include <string>
#include <vector>
#include "../../lib/glad/include/glad/glad.h"

class ShaderProgram {
private:
    unsigned int ID;
    std::vector<std::pair<std::string, GLenum>> files;
public:
    /**
     * Used to compile the shaders that were passed in addFile().
     * @return A vector of (ID, Shader_type) pairs.
     */
    std::vector<std::pair<unsigned int, GLenum>> compileShaders();

    /**
     * Link the shaders that are given as input.
     * This method is public for those who want more control over the process.
     * @param shaders The shaders that should be linked together.
     * @param program_id The ID of the program.
     * @return The ID of the linked program.
     */
    unsigned int linkShaders(std::vector<std::pair<unsigned int, GLenum>> shaders, unsigned int program_id);

    /**
     * Start the usage of this shader.
     * @pre The files have been added and the program has been created.
     */
    void use();

    /**
     * Add a file containing shader source code to this program to be compiled.
     * @param file_path Path to the file that needs adding.
     * @param type The type of shader that is defined in the file.
     */
    void addFile(std::string file_path, GLenum type);

    /**
     * Actually compile and link the program based on the source files that were added.
     * Automatically compiles and links the the shaders that were added.
     * @return The ID of the program, or 0 if something failed.
     */
    unsigned int createProgram();

    /**
     * Fetch the ID of the program.
     * @return The ID of this program, or 0 if no program is available.
     */
    unsigned int getID() { return ID; }

    ~ShaderProgram();
};


#endif //CPLUSPAIN_SHADERPROGRAM_H
