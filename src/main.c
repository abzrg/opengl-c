#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Be sure to include GLAD before GLFW. The include file for GLAD includes the
// required OpenGL headers behind the scenes (like GL/gl.h) so be sure to
// include GLAD before other header files that require OpenGL.
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "sds/sds.h"
#include "utils/utils.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, GLuint* mode);
void compile_shader(const GLuint shader, const sds shader_source);
GLint shader_compiled(const GLuint shader);
sds get_shader_info_log(const GLuint shader);
GLint program_linked(const GLuint program);
sds get_program_info_log(const GLuint program);

// --- <> --- //

int main(void)
{
    // -----
    // Setup
    // -----

    // GLFW: window and OpenGL context
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int screenWidth = 800;
    const int screenHeight = 600;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Create windown and OpenGL context
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // Call the callback function on every window size

    // Setup GLAD
    // Load OpenGL's function symbols using GLFW's loader
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD.\n");
        return -1;
    }

    // ------------------------------
    // TRANSFER AND DESCRIBE THE DATA
    // ------------------------------

    // Steps:
    //
    // 1. Bind vertex array object (VAO)
    // 2. Copy vertices array to a vertex array buffer for OpenGL use (VBO)
    // 3. Copy index data in an array to an element array buffer for OpenGL use (EBO)
    // 4. Set the vertex attributes pointers
    // 5. Unbind VAO
    // 6. Unbind VBO and EBO
    //
    // Later when drawing:
    //
    // 7. Bind only VAO before draw calls
    // 8. Do the draw calls

    // ---- Rectangle
    const GLfloat vertices_rectangle[] = {
         0.50f,  0.50f, 0.0f, // 0  top right
         0.50f, -0.50f, 0.0f, // 1  bottom right
        -0.50f, -0.50f, 0.0f, // 2  bottom left
        -0.50f,  0.50f, 0.0f, // 3  top left
    };

    GLuint indices_rectangle[] = {
        0, 1, 3, // First sub triangle
        1, 2, 3, // Second sub triangle
    };

    GLuint vertex_array_rectangle = 0;
    glGenVertexArrays(1, &vertex_array_rectangle);
    glBindVertexArray(vertex_array_rectangle);

    GLuint vertex_buffer_rectangle = 0;
    glGenBuffers(1, &vertex_buffer_rectangle);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rectangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_rectangle), vertices_rectangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(*vertices_rectangle), (void*)0);

    GLuint element_buffer_rectangle = 0;
    glGenBuffers(1, &element_buffer_rectangle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_rectangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_rectangle), indices_rectangle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // ---- Left Triangle
    const GLfloat vertices_left_triangle[] = {
        -0.75f, -0.50f, 0.0f, // 0
        -0.50f,  0.00f, 0.0f, // 1
        -0.75f,  0.50f, 0.0f, // 2
    };

    const GLuint indices_left_triangle[] = {
        0, 1, 2,
    };

    GLuint vertex_array_left_triangle = 0;
    glGenVertexArrays(1, &vertex_array_left_triangle);
    glBindVertexArray(vertex_array_left_triangle);

    GLuint vertex_buffer_left_triangle = 0;
    glGenBuffers(1, &vertex_buffer_left_triangle);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_left_triangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_left_triangle), vertices_left_triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(*vertices_left_triangle), (void*)0);

    GLuint element_buffer_left_triangle = 0;
    glGenBuffers(1, &element_buffer_left_triangle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_left_triangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_left_triangle), indices_left_triangle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // ---- Right Triangle
    const GLfloat vertices_right_triangle[] = {
         0.75f, -0.50f, 0.0f, // 0
         0.50f,  0.00f, 0.0f, // 1
         0.75f,  0.50f, 0.0f, // 2
    };

    const GLuint indices_right_triangle[] = {
        0, 2, 1,
    };

    GLuint vertex_array_right_triangle = 0;
    glGenVertexArrays(1, &vertex_array_right_triangle);
    glBindVertexArray(vertex_array_right_triangle);

    GLuint vertex_buffer_right_triangle = 0;
    glGenBuffers(1, &vertex_buffer_right_triangle);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_right_triangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_right_triangle), vertices_right_triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(*vertices_right_triangle), (void*)0);

    GLuint element_buffer_right_triangle = 0;
    glGenBuffers(1, &element_buffer_right_triangle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_right_triangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_right_triangle), indices_right_triangle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // -------------------------------
    // COMPILE AND LINK SHADER PROGRAM
    // -------------------------------


    // Compile vertex Shader
    const sds vertex_shader_source = read_entire_file("./shaders/vertex_shader.glsl");
    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    compile_shader(vertex_shader, vertex_shader_source);
    if (!shader_compiled(vertex_shader)) {
        sds info_log = get_shader_info_log(vertex_shader);
        fprintf(stderr, "Vertex shader compilation failed: %s\n", info_log);

        sdsfree(info_log);
        sdsfree(vertex_shader_source);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Compile fragment Shader
    const sds fragment_shader_source = read_entire_file("./shaders/fragment_shader.glsl");
    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    compile_shader(fragment_shader, fragment_shader_source);
    if (!shader_compiled(fragment_shader)) {
        sds info_log = get_shader_info_log(fragment_shader);
        fprintf(stderr, "Fragment shader compilation failed: %s\n", info_log);

        sdsfree(info_log);
        sdsfree(fragment_shader_source);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Compile (combine or attach the compiled shaders) and link shader program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    if (!program_linked(shader_program)) {
        sds info_log = get_program_info_log(shader_program);
        fprintf(stderr, "Program link failed: %s\n", info_log);

        sdsfree(info_log);
        sdsfree(fragment_shader_source);
        sdsfree(vertex_shader_source);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    // -----------
    // Render Loop
    // -----------


    GLuint mode = GL_FILL; // drawing mode: fill or wireframe
    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window, &mode);

        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);

        // Draw left triangle
        glBindVertexArray(vertex_array_left_triangle);
        glPolygonMode(GL_FRONT_AND_BACK, mode);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Draw rectangle
        glBindVertexArray(vertex_array_rectangle);
        glPolygonMode(GL_FRONT_AND_BACK, mode);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Draw right triangle
        glBindVertexArray(vertex_array_right_triangle);
        glPolygonMode(GL_FRONT_AND_BACK, mode);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Release resources
    sdsfree(vertex_shader_source);
    glDeleteShader(vertex_shader);
    sdsfree(fragment_shader_source);
    glDeleteShader(fragment_shader);
    glDeleteProgram(shader_program);
    glfwTerminate();

    return EXIT_SUCCESS;
}

// --- <> --- //

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, GLuint* mode)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        GLuint tmp_mode = 0;
        if (*mode == GL_LINE) {
            tmp_mode = GL_FILL;
        } else if (*mode == GL_FILL) {
            tmp_mode = GL_LINE;
        }

        *mode = tmp_mode;
    }
}

void compile_shader(const GLuint shader, const sds shader_source)
{
    assert(shader_source);
    glShaderSource(shader, 1, (GLchar const*const*)&shader_source, NULL);
    glCompileShader(shader);
}

GLint shader_compiled(const GLuint shader)
{
    GLint compile_status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    return compile_status;
}

sds get_shader_info_log(const GLuint shader)
{
    GLint info_log_length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

    sds info_log = sdsempty();
    info_log = sdsgrowzero(info_log, info_log_length);
    glGetShaderInfoLog(shader, info_log_length, NULL, info_log);

    return sdstrim(info_log, " \n");
}

GLint program_linked(const GLuint program)
{
    GLint link_status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    return link_status;
}

sds get_program_info_log(const GLuint program)
{
    GLint info_log_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

    sds info_log = sdsempty();
    info_log = sdsgrowzero(info_log, info_log_length);
    glGetProgramInfoLog(program, info_log_length, NULL, info_log);

    return sdstrim(info_log, " \n");
}
