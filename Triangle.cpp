#define GL_GLEXT_PROTOTYPES
#define GLM_ENABLE_EXPERIMENTAL
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glx.h> 
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "BmpLoader.h"
#include <math.h>

#include <stdio.h>
#include <iostream>


#include <X11/X.h> 
#include <X11/keysym.h>

static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display   *dpy;
Window     win;

int main(int argc, char **argv)
{
    XVisualInfo         *vi;
    Colormap             cmap;
    XSetWindowAttributes swa;
    GLXContext           cx;
    XEvent               event;

    dpy = XOpenDisplay(NULL);
    vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
    cx = glXCreateContext(dpy, vi, None, GL_TRUE);
	cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.border_pixel = 0;
    swa.event_mask = KeyPressMask | ExposureMask | ButtonPressMask | StructureNotifyMask;
    win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0, 1024, 768, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
    XSetStandardProperties(dpy, win, "OpenGl Test", "OpenGl Test", None, argv, argc, NULL);
    glXMakeCurrent(dpy, win, cx);
    XMapWindow(dpy, win);
    
    
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	bmpLoader textureFile;
    textureFile.loadFile("Brick.bmp");
    GLuint tex = 0;
    glGenTextures(1,&tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureFile.getWidth(), textureFile.getHeigth(), 0, GL_BGR,GL_UNSIGNED_BYTE, textureFile.getData());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    
    GLfloat points[] = {0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};
    GLfloat normals[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat texCoords[] = {0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    
    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points),points,GL_STATIC_DRAW);    
    
    GLuint normals_vbo = 0;
    glGenBuffers(1, &normals_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals),normals,GL_STATIC_DRAW);
    
    GLuint texCoords_vbo;
    glGenBuffers(1, &texCoords_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, texCoords_vbo);
    glBufferData(GL_ARRAY_BUFFER,sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, texCoords_vbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    
    
    const char* vertex_shader =
    "#version 410\n"
    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_normal;"
    "layout(location = 2) in vec2 vertex_text;"
    "out vec3 position_eye, normal_eye;"
    "out vec2 texture_coordinates;"
    "uniform mat4 view, proj, model;"
    "void main()"
    "{"
    "	texture_coordinates = vertex_text;"
    "	position_eye = vec3(view * model * vec4(vertex_position, 1.0));"
    "	normal_eye = vec3(view * model * vec4(vertex_normal, 0.0));"
    "	gl_Position = proj * vec4(position_eye, 1.0);"
    "}";
    
    const char* fragment_shader = 
    "#version 410\n"
    "uniform sampler2D basic_texture;"
    "uniform mat4 view;"
    "in vec3 position_eye, normal_eye;"
    "in vec2 texture_coordinates;"
    "vec3 light_position_world = vec3(0.0, 0.0, 2.0);"
    "vec3 Ls = vec3(1.0, 1.0, 1.0);"
    "vec3 Ld = vec3(0.7, 0.7, 0.7);"
    "vec3 La = vec3(0.2, 0.2, 0.2);"
    
    "vec4 texel = texture(basic_texture, texture_coordinates);"
    "vec3 Ks = vec3(0.1, 0.1, 0.1);"
    "vec3 Kd = vec3(texel.xyz);"
    "vec3 Ka = vec3(texel.xyz);"
    "float specular_exponent = 100.0;"
        
    "out vec4 frag_color;"
    
    "void main()"
    "{"
    " 	vec3 Ia = La * Ka;"
    
    
	"	vec3 light_position_eye = vec3(view * vec4(light_position_world, 1.0));"
	"	vec3 distance_to_light_eye = light_position_eye - position_eye;"
	"	vec3 direction_to_light_eye = normalize(distance_to_light_eye);"
	"	float dot_prod = dot(direction_to_light_eye, normal_eye);"
	"	dot_prod = max(dot_prod, 0.0);"
	"	vec3 Id = Ld * Kd * dot_prod;"
	
	"	vec3 reflection_eye = reflect(-direction_to_light_eye, normal_eye);"
	"	vec3 surface_to_viewer_eye = normalize(-position_eye);"
	"	float dot_prod_specular = dot(reflection_eye, surface_to_viewer_eye);"
	"	dot_prod_specular = max(dot_prod_specular, 0.0);"
	"	float specular_factor = pow(dot_prod_specular, specular_exponent);"
	"	vec3 Is = Ls* Ks * specular_factor;"
	
    "	frag_color = vec4(Is + Id + Ia, 1.0);"
    "}";
    
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program,fs);
    glAttachShader(shader_program,vs);
    glLinkProgram(shader_program);
    
    
    glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 10.0f);
    glm::quat camRot = glm::angleAxis(0.0f,glm::vec3(0.0f, 0.0f, -1.0f));
    
    glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat modelRot = glm::angleAxis(0.0f,glm::vec3(0.0f, 0.0f, -1.0f));
    
    
       
	
    glm::mat4 ident = glm::mat4(1.0f);
	glm::mat4 T = glm::translate(ident,-camPos);
	glm::mat4 R = glm::toMat4(camRot);
	glm::mat4 view_mat = R * T;
	
	glm::mat4 modelTranslate = glm::translate(ident,modelPos);
	glm::mat4 modelRotation = glm::toMat4(modelRot);
	glm::mat4 modelMatrix = modelTranslate * modelRotation;
	
	
	
	float near = 0.1f;
	float far = 100.0f;
	float fov = (67.0f * 2 * M_PI) / 360;
	float aspect = (float)1024/(float)768;
	float range = tan(fov * 0.5f) * near;
	float Sx = (2.0f * near) / (range * aspect + range * aspect);
	float Sy = near / range;
	float Sz = -(far + near) / (far - near);
	float Pz = -(2.0f * far * near) / (far - near);
	float proj_mat[] = {Sx, 0.0f, 0.0f, 0.0f,
						0.0f, Sy, 0.0f, 0.0f,
						0.0f, 0.0f, Sz, -1.0f,
						0.0f, 0.0f, Pz, 0.0f};
						

	int view_mat_location = glGetUniformLocation(shader_program,"view");
    glUseProgram(shader_program);
    glUniformMatrix4fv(view_mat_location,1,GL_FALSE,(const float*)glm::value_ptr(view_mat));
    
    int proj_mat_location = glGetUniformLocation(shader_program,"proj");
    glUseProgram(shader_program);
    glUniformMatrix4fv(proj_mat_location,1,GL_FALSE,proj_mat);
    
    int model_mat_location = glGetUniformLocation(shader_program,"model");
    glUseProgram(shader_program);
    glUniformMatrix4fv(model_mat_location,1,GL_FALSE,(const float*)glm::value_ptr(modelMatrix));
    
    int tex_loc = glGetUniformLocation(shader_program,"basic_texture");
    glUseProgram(shader_program);
    glUniform1i(tex_loc,0);
						
	
    
    
	
    while (1)
    {
		
		T = glm::translate(ident,-camPos);
		R = glm::toMat4(camRot);
		view_mat = R * T;
		
		modelTranslate = glm::translate(ident,modelPos);
		modelRotation = glm::toMat4(modelRot);
		modelMatrix = modelTranslate * modelRotation;
		
		
		
		glUseProgram(shader_program);
		glUniformMatrix4fv(view_mat_location,1,GL_FALSE,(const float*)glm::value_ptr(view_mat));
		glUniformMatrix4fv(model_mat_location,1,GL_FALSE,(const float*)glm::value_ptr(modelMatrix));
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glXSwapBuffers(dpy, win);
		
		
        while(XPending(dpy))
        {
            XNextEvent(dpy, &event);
            switch (event.type)
            {
                case KeyPress:
                {
                    KeySym     keysym;
                    char       buffer[1];
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_Escape))
                    {
                        exit(0);
                    }
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_w))
                    {
                        glm::mat3 camRotMat = glm::toMat3(camRot);
                        glm::vec3 movement = glm::vec3(0.0f, 0.0f, -0.3f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        camPos += finalMovement;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_s))
                    {
                        glm::mat3 camRotMat = glm::toMat3(camRot);
                        glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.3f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        camPos += finalMovement;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_a))
                    {
                        glm::mat3 camRotMat = glm::toMat3(camRot);
                        glm::vec3 movement = glm::vec3(-0.3f, 0.0f, 0.0f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        camPos += finalMovement;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_d))
                    {
                        glm::mat3 camRotMat = glm::toMat3(camRot);
                        glm::vec3 movement = glm::vec3(0.3f, 0.0f, 0.0f);
                        glm::vec3 finalMovement = movement * camRotMat;
                        camPos += finalMovement;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_q))
                    {
                        glm::quat rotateLeft = glm::angleAxis(glm::radians(-2.0f),glm::vec3(0.0f, 1.0f, 0.0f));
                        camRot *= rotateLeft;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_e))
                    {
                        glm::quat rotateRight = glm::angleAxis(glm::radians(2.0f),glm::vec3(0.0f, 1.0f, 0.0f));
                        camRot *= rotateRight;
                    }
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_j))
                    {
                        glm::quat rotateLeft = glm::angleAxis(glm::radians(2.0f),glm::vec3(0.0f, 1.0f, 0.0f));
                        modelRot *= rotateLeft;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_l))
                    {
                        glm::quat rotateRight = glm::angleAxis(glm::radians(-2.0f),glm::vec3(0.0f, 1.0f, 0.0f));
                        modelRot *= rotateRight;
                    }
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_i))
                    {
                        glm::mat3 modelRotMat = glm::toMat3(modelRot);
                        glm::vec3 movement = glm::vec3(0.0f, 0.0f, -0.3f);
                        glm::vec3 finalMovement = modelRotMat * movement;
                        modelPos += finalMovement;
                    }
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_k))
                    {
                        glm::mat3 modelRotMat = glm::toMat3(modelRot);
                        glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.3f);
                        glm::vec3 finalMovement = modelRotMat * movement;
                        modelPos += finalMovement;
                    }

                    break;
                }
                case ButtonPress:
                    break;
                case ConfigureNotify:
                    glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
                case Expose:
                    break;
            }
        } 


    }

  return 0;
}
