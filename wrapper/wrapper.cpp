#include "wrapper.h"


void check_error() {
    GLenum errorCode = glGetError();
    if (errorCode == GL_NO_ERROR)
        return;
    std::string errorMsg = "";
    switch (errorCode) {
    case GL_INVALID_ENUM:
        errorMsg = "Invalid Enum!";
        break;
    case GL_INVALID_VALUE:
        errorMsg = "Invalid Value";
        break;
    case GL_INVALID_OPERATION:
        errorMsg = "Invalid Operaion";
        break;
    case GL_OUT_OF_MEMORY:
        errorMsg = "Out of Memory";
        break;
    default:
        errorMsg = "Unkown Error";
        break;
    }
    std::cout << "error msg: " << errorMsg << std::endl;

    assert(false);
}