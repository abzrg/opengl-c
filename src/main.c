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

    // Describe the input vertices for OpenGL via Vertex array (VAO) and vertex buffer (VBO) objects
    const GLfloat vertices[] = {
        // Middle rectangle
         0.50f,  0.50f, 0.0f, // 0  top right
         0.50f, -0.50f, 0.0f, // 1  bottom right
        -0.50f, -0.50f, 0.0f, // 2  bottom left
        -0.50f,  0.50f, 0.0f, // 3  top left

        // Left triangle
        -0.75f, -0.50f, 0.0f, // 4
        -0.50f,  0.00f, 0.0f, // 5
        -0.75f,  0.50f, 0.0f, // 6

        // Right tirangle
         0.75f, -0.50f, 0.0f, // 7
         0.50f,  0.00f, 0.0f, // 8
         0.75f,  0.50f, 0.0f, // 9
    };

    // Note: We start from index 0!
    GLuint indices[] = {
        0, 1, 3, // First triangle
        1, 2, 3, // Second triangle
        4, 5, 6, // Third triangle
        7, 8, 9, // Fourth triangle
    };

    // Note: The core profile *requires* vertex array (attribute) object (VAO). However, the
    //       compatibility defines, by default, a VAO, with index 0.
    // Note: After the call to `glBindVertexArray`, the corresponding VBO(s) and attribute
    //       pointer(s) should be bound/configured and then the VAO should be unbound for later use.
    // Note: If we only define one VAO for objects of various layout, we bind it once and will never
    //       unbind it, and as a result, we need to bind its buffer and then configure the attribute
    //       pointer, which can be tedious. The better alternative is to define VAO, and VBO, set
    //       the data and attributes once per (unique) geometry, and bind the corresponding VAO when
    //       the geometry is needed to be drawn.
    GLuint vertex_array = 0; // VAO
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    GLuint vertex_buffer = 0; // VBO
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Note: Index `0` (1st argument) of the currently bound vertex array is will be bound to the
    //       currently bound vertex buffer. This function makes the link between a VBO and a VAO.
    //       Having made this link, we later on don't need to bind the vertex buffer or re-specity
    //       the attributes via a call to this (`glVertexAttribPointer`) function. We only bind the
    //       corresponding VAO.
    // Note: Make sure the vertex shader has an input attribute for position (vec3) at the index
    //       location 0 of the vertex array object:
    //
    //          layout (location = 0) in vec3 aPos;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(*vertices), (void*)0);

    // Note: The last element buffer object that gets bound while a VAO is bound, is stored as the
    //       VAOs element buffer object. Binding to a VAO then also automatically binds that EBO.
    // Note: A VAO stores the `glBindBuffer` calls when the target is `GL_ELEMENT_ARRAY_BUFFER`.
    //       This also means it stores its unbind calls so make sure you don't unbind the element
    //       array buffer before unbinding your VAO, otherwise it doesn't have an EBO configured.
    GLuint element_buffer = 0; // EBO
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Note: Vertex attributes are not enabled by default.
    glEnableVertexAttribArray(0); // Enable position attribute.

    // Note: Later, before draw calls it should be bound again.
    glBindVertexArray(0); // Unbind vertex array.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // must unbound after unbounding VAO


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

    // Render loop
    GLuint mode = GL_FILL; // drawing mode: fill or wireframe
    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window, &mode);

        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        // Note: VBO is already baked into VAO, so no need to bind to the VBO corresponding to the
        //       triangle.
        glBindVertexArray(vertex_array);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

        glPolygonMode(GL_FRONT_AND_BACK, mode);
        // Note: The glDrawElements function takes its indices from the EBO currently bound to the
        //       `GL_ELEMENT_ARRAY_BUFFER` target.
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0); // We want 6 indicies to draw

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
