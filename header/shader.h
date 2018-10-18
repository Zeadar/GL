#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

//This class name should change. But I don't remember why.
class Shader{
    
public:
    GLuint shader_program;

    Shader(const GLchar *vertex_path, const GLchar *fragment_path){
        GLchar *vertex_code = nullptr;
        GLchar *fragment_code = nullptr;
        std::ifstream vertex_shader_file(vertex_path);
        std::ifstream fragment_shader_file(fragment_path);
    
        if (!vertex_shader_file.is_open())
            std::cout << "Failed to load " << vertex_path << std::endl;
            
        if (!fragment_shader_file.is_open())
            std::cout << "Failed to load " << fragment_path << std::endl;
        
        std::filebuf* vertex_shader_buffer = vertex_shader_file.rdbuf();
        std::filebuf* fragment_shader_buffer = fragment_shader_file.rdbuf();
        
        //I think I'll throw up.
        std::size_t vertex_shader_size = vertex_shader_buffer->pubseekoff(0, vertex_shader_file.end, vertex_shader_file.in);
        vertex_shader_buffer->pubseekpos(0, vertex_shader_file.in);
        std::size_t fragment_shader_size = fragment_shader_buffer->pubseekoff(0, fragment_shader_file.end, fragment_shader_file.in);
        fragment_shader_buffer->pubseekpos(0, fragment_shader_file.in);
        
        //Note: string termination workaround
        vertex_code = new GLchar[vertex_shader_size +1];
        fragment_code = new GLchar[fragment_shader_size +1];
        fragment_code[fragment_shader_size] = '\0';
        vertex_code[vertex_shader_size] = '\0';
        
        vertex_shader_buffer->sgetn(vertex_code, vertex_shader_size);
        fragment_shader_buffer->sgetn(fragment_code, fragment_shader_size);
        
        vertex_shader_file.close();
        fragment_shader_file.close();
        
        // And this ↑ my friends is why you don't load files on such a low level.
        
        //This is ridiculous, but const is what they ask for.
        const GLchar *vertex_shader_source = vertex_code;
        const GLchar *fragment_shader_source = fragment_code;
    
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
        glCompileShader(vertex_shader);
    
        GLint success;
        GLchar *info_log = new GLchar[512];
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
            std::cout << "Vertex shader compilation error in " << vertex_path << "\n" << info_log << std::endl;
            std::cout << "\n---Code---\n" << vertex_shader_source << "\n---Code---\n" << std::endl;

        }
    
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader);
    
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    
        if (!success){
            glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
            std::cout << "Fragment shader compilation error in " << fragment_path << "\n" << info_log << std::endl;
            std::cout << "\n---Code---\n" << fragment_shader_source << "\n---Code---\n" << std::endl;
        }
    
        shader_program = glCreateProgram();
        glAttachShader (shader_program, vertex_shader);
        glAttachShader (shader_program, fragment_shader);
        glLinkProgram(shader_program);
        
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    
        if (!success){
            glGetProgramInfoLog(shader_program, 512, NULL, info_log);
            std::cout << "Shader Program compilation error\n" << info_log << std::endl;
        }
    
        delete[] info_log;
        delete[] vertex_code;
        delete[] fragment_code;
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
    
   void use_program(){ //ugly shorthand method
       glUseProgram(this->shader_program);
   }
};

#endif
