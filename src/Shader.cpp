#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath){
    int succes;
    char infoLog[512];

    GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragShader);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &succes);
    if(!succes){
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "Linking error:" << std::endl << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

void Shader::activate(){
    glUseProgram(id);
}

std::string Shader::loadShaderSrc(const char* filename){
    std::ifstream file;
    std::stringstream buf;

    std::string ret = "";

    file.open(filename);

    if(file.is_open()){
        buf << file.rdbuf();
        ret = buf.str();
    }else{
        std::cout << "Could not open " << filename << std::endl;   
    }

    file.close();

    return ret;
}

GLuint Shader::compileShader(const char* filepath, GLenum type){
    int succes;
    char infoLog[512];

    GLuint ret = glCreateShader(type);
    std::string shaderSrc = loadShaderSrc(filepath);
    const GLchar* shader = shaderSrc.c_str();
    glShaderSource(ret, 1, &shader, NULL);
    glCompileShader(ret);

    glGetShaderiv(ret, GL_COMPILE_STATUS, &succes);
    if(!succes){
        glGetShaderInfoLog(ret, 512, NULL, infoLog);
        std::cout << "Error with frag shader comp.: " << std::endl << infoLog << std::endl;
    }

    return ret;
}

void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set4Float(const std::string& name, float v1, float v2, float v3, float v4) {
	glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
}

void Shader::setMat4(const std::string& name, glm::mat4 val){
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}