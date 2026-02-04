# OpenGL (Using C)

Build and run:

```console
$ make
$ ./build/main
```

Third-party Dependencies:

- [GLFW](https://)
- [GLAD]()
- [sds]() (Simple Dynamic String) library

Bugs I encountered:
- used `GLfloat` instead of `GLuint` when defining indices that were to be transferred to the EBO.
