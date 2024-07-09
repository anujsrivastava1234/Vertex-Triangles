#include <iostream>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <vector>

// Screen Dimensions
int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window *gGrpahicsApplicationWindow = nullptr;
SDL_GLContext gOpenGlContext = nullptr;

//vertex array object
GLuint gVertexArrayObject {0};

//vertex buffer object
GLuint gVertexBufferObject {0};

bool gQuit = false;

//Program object (for our shaders)
GLuint gGraphicsPipelineShaderProgram {0};

//Vertex shader -> final position of the vertex buffer
 const std::string gVertexShader = 
    "#version 410 core\n"
    "in vec4 position;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
    "}\n";

//Fragment shader -> part determinses final color that will be send to the screen
const std::string gFragmentShader = 
    "#version 410 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    " color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
    "}\n";

//Compile shader program
GLuint CompileShader(GLuint type, const std::string& source)
{
    GLuint shaderObject ;
    if(type == GL_VERTEX_SHADER)
    {       
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }else if(type == GL_FRAGMENT_SHADER)
    {   
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }
    const char* src = source.c_str();
    glShaderSource(shaderObject,1, &src, NULL);
    glCompileShader(shaderObject);

    //Retrive the result of our compilation
    int result;

    //glGetShaderiv is to retrive the compilation status
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE)
    {
        int lenght;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &lenght);
        char* log = new char[lenght];
        glGetShaderInfoLog(shaderObject, lenght, &lenght, log);

        if(type == GL_VERTEX_SHADER ){
            std::cout << "ERROR: GL_VERTEX_SHADEr compilation failed!\n" << log << std::endl;
        }else if(type == GL_FRAGMENT_SHADER)
        {
            std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n" << log << std::endl;
        }

        //Reclain our memory
        delete[] log;

        //Delete the broker shader
        glDeleteShader(shaderObject);
    }


    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexShaderSource,
                         const std::string& fragmentShaderSource)
{
    GLuint programObject = glCreateProgram();

    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    //Valid our program
    glValidateProgram(programObject);
    //glDetachShader, glDeleteShader
    // after the final program object delte and detach individaul shaders
    glDetachShader(programObject, myVertexShader);
    glDetachShader(programObject, myFragmentShader);

    //Delete the individual shader 
    glDeleteShader(myFragmentShader);
    glDeleteShader(myVertexShader);
    
    return programObject;
}

// Create the graphics pipeline
void CreateGraphicsPipeline()
{
    gGraphicsPipelineShaderProgram = CreateShaderProgram(gVertexShader, gFragmentShader);
}

void GetOpenGlVersion()
{
    std::cout << "Vendor  : " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Shading Language : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

//function for vertex specification -> on the CPU side , use GLFLOAT instead of float
void vertexSpecification()
{
    //define the x, y ,z positions for the 3 vertex positions of a triangle
    const std::vector<GLfloat> vertexPosition{
        -0.8f, -0.8f, 0.0f, // Left Position
        0.8f, -0.8f, 0.0f , // Right Position
        0.0f, 0.8f, 0.0f    // Top Position
    };

    // 1) Create a vertex array object gVertexArrayObject {0}
    /*
        bind the vertex array with the vertex array object
        2) Create a vertex array buffer object -> gVertexbufferObject {0}
            bind the buffer with Target and VBO
        3) Fill the VBO 
            glBufferData -> create a buffer object data store
    */
    //search in the docs.gl
    //create a VAO  ,
    // on the GPU
    glGenVertexArrays(1,&gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    //create a VBO
    glGenBuffers(1,&gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 
                vertexPosition.size() * sizeof(GLfloat),
                vertexPosition.data(),
                GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            0,
                            (void*)0);
    
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);

}

void Intialize()
{
    //Intialize the the sdl
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL" << SDL_GetError() << std::endl;
        exit(1);
    }else{
        std::cout << "Intialzed SDL" << std::endl;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24  );

    // Create the Window structure
    gGrpahicsApplicationWindow = SDL_CreateWindow(
        "OpenGL Window",
        0,0,
        gScreenWidth, gScreenHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(gGrpahicsApplicationWindow == nullptr) return;

    // Create an OpenGl Context
    gOpenGlContext = SDL_GL_CreateContext(gGrpahicsApplicationWindow);

    if(gOpenGlContext == nullptr) return;

    //initialize OpenGL context
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        exit(1);
    }

    GetOpenGlVersion();
}

void Input()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            std::cout << "Quit" << std::endl;
            gQuit = true;
        }
    } 
}

void PreDraw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.0f, 1.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);
}

void Draw()
{
    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

    glDrawArrays(GL_TRIANGLES,0, 3);

    glUseProgram(0);
    
}

void MainLoop()
{
    while(!gQuit)
    {
        //handle input 
        Input();

        //Place befor edraw calls
        PreDraw();
        
        //Draw calls in opengl
        Draw();

        //Update the screen
        SDL_GL_SwapWindow(gGrpahicsApplicationWindow);
    }
}

void CleanUp()
{
    SDL_DestroyWindow(gGrpahicsApplicationWindow);

    SDL_Quit();
}

int main(int argc, char const *argv[])
{
    //1) Setup the graphics program
    Intialize();

    //2) Intialize the Vertex specification
    vertexSpecification();

    //3) Create a graphics pipeline
    CreateGraphicsPipeline();

    //4) Call the main application loop
    MainLoop();

    //5) Call the cleanup function when our program
    CleanUp();
    return 0;
}
