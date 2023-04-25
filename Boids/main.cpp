#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include "glimac/cone_vertices.hpp"

#include "FreeflyCamera.hpp"
#include "BoidsManager.hpp"

/////////////////////////////////

    // CAMERA PARAMETERS

    float CAM_TRANSLATION_FORCE=0.001;
    float CAM_ROTATION_FORCE=0.1;
    float MOUSE_ROTATION_FORCE=50;
    float FAST_SPEED_FACTOR=10;

    // SIMULATION PARAMETERS

    const int NB_BOIDS=20;
    float BOIDS_SIZE=0.04;
    float TAILS_SIZE=20;

    float BORDERS_DISTANCE=0.2;
    float BORDERS_STRENGTH=0.3; 

    float NEIGHBORS_PARAMETERS[]={  0.4f, 0.005f, // Alignment distance and strength
                                    0.3f, 0.002f, // Cohesion          //
                                    0.1f, 0.5f    // Separation        //
                                };

    /////////////////////////////////

struct BoidProgram 
{
    p6::Shader m_Program;

    GLuint uMVPMatrix;
    GLuint uMVMatrix;
    GLuint uNormalMatrix;

    BoidProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl")}
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
    }
};

int main()
{
    int width=1280;
    int height=720;
    auto ctx = p6::Context{{width, height, "Remi's Boids"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

        // Initialize depth test
        glEnable(GL_DEPTH_TEST);

        // Initialise Programs
        BoidProgram boidProgram=BoidProgram();


        // Initialize camera
        FreeflyCamera camera;

        // Initialize Boids
        std::vector<Boid> boids=createBoids(NB_BOIDS);
        
        // Initialize Render Matrix  
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), (GLfloat)width/(GLfloat)height, 0.1f, 100.f);
        glm::mat4 MVMatrix;
        glm::mat4 NormalMatrix;
        glm::mat4 ViewMatrix;


        GLuint vbo;
        glGenBuffers(1,&vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
            std::vector<glimac::ShapeVertex> cone=glimac::cone_vertices(1,1,32,16);
            glBufferData(GL_ARRAY_BUFFER, cone.size() * sizeof(glimac::ShapeVertex), cone.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

            static constexpr GLuint VERTEX_ATTR_POSITION=0;
            glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

            static constexpr GLuint VERTEX_ATTR_NORMAL=1;
            glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);

            static constexpr GLuint VERTEX_ATTR_TEXTURE=2;
            glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

            glBindBuffer(GL_ARRAY_BUFFER,vbo);
                glVertexAttribPointer(VERTEX_ATTR_POSITION, 
                    3, GL_FLOAT, GL_FALSE, 
                    sizeof(glimac::ShapeVertex), 0);
                glVertexAttribPointer(VERTEX_ATTR_NORMAL,
                    3, GL_FLOAT, GL_FALSE, 
                    sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));
                glVertexAttribPointer(VERTEX_ATTR_TEXTURE,
                    2, GL_FLOAT, GL_FALSE, 
                    sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));
            glBindBuffer(GL_ARRAY_BUFFER,0);

        glBindVertexArray(0);

    /*********************************/

    // Declare your infinite update loop.
    ctx.update = [&]() {

        //Events
        ImGui::Begin("Control");
        ImGui::SliderFloat("Boids size", &BOIDS_SIZE, 0.01f, 1.f);
        ImGui::SliderFloat("Tails size", &TAILS_SIZE, 1.f, 10000.f);
        ImGui::SliderFloat("Alignment distance", &NEIGHBORS_PARAMETERS[0], 0.f, 2.f);
        ImGui::SliderFloat("Alignment strength", &NEIGHBORS_PARAMETERS[1], 0.f, 1.f);
        ImGui::SliderFloat("Cohesion distance", &NEIGHBORS_PARAMETERS[2], 0.f, 2.f);
        ImGui::SliderFloat("Cohesion strength", &NEIGHBORS_PARAMETERS[3], 0.f, 1.f);
        ImGui::SliderFloat("Separation distance", &NEIGHBORS_PARAMETERS[4], 0.f, 2.f);
        ImGui::SliderFloat("Separation strength", &NEIGHBORS_PARAMETERS[5], 0.f, 1.f);
        ImGui::SliderFloat("Borders distance", &BORDERS_DISTANCE, 0.f, 1.f);
        ImGui::SliderFloat("Borders strength", &BORDERS_STRENGTH, 0.f, 1.f);
        ImGui::End();

        //camera move
        float factor=1;
        if (ctx.shift()){factor=FAST_SPEED_FACTOR;};

        if (ctx.key_is_pressed(GLFW_KEY_A))
        {
            camera.moveLeft(factor*CAM_TRANSLATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_D))
        {
            camera.moveLeft(-factor*CAM_TRANSLATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_W))
        {
            camera.moveFront(factor*CAM_TRANSLATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_S))
        {
            camera.moveFront(-factor*CAM_TRANSLATION_FORCE);
        }

        //camera rotation
        if (ctx.key_is_pressed(GLFW_KEY_Q))
        {
            camera.rotateLeft(CAM_ROTATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_E))
        {
            camera.rotateLeft(-CAM_ROTATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_LEFT))
        {
            camera.rotateLeft(CAM_ROTATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_RIGHT))
        {
            camera.rotateLeft(-CAM_ROTATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_UP))
        {
            camera.rotateUp(CAM_ROTATION_FORCE);
        }
        if (ctx.key_is_pressed(GLFW_KEY_DOWN))
        {
            camera.rotateUp(-CAM_ROTATION_FORCE);
        }
        if (ctx.mouse_moved)
        {
            if(ctx.alt() || ctx.mouse_button_is_pressed(p6::Button::Right))
            {
                camera.rotateLeft(-ctx.mouse_delta().x*MOUSE_ROTATION_FORCE);
                camera.rotateUp(ctx.mouse_delta().y*MOUSE_ROTATION_FORCE);
            }
        }

        //Boids simulation
        neighborsManager(boids, NEIGHBORS_PARAMETERS);

        borderManager(boids, BORDERS_DISTANCE, BORDERS_STRENGTH);

        boidsDisplacement(boids);


        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/


            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            boidProgram.m_Program.use();

            ViewMatrix=camera.getViewMatrix();

            MVMatrix = glm::mat4(1);
            MVMatrix = ViewMatrix*MVMatrix;
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            
            glUniformMatrix4fv(boidProgram.uMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(boidProgram.uMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix*MVMatrix));
            glUniformMatrix4fv(boidProgram.uNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
                    
            glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(cone.size()));
            glBindVertexArray(0);


        /*********************************/
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    // Ressources released
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    //glDeleteTextures(1, &textures);
}