#define GL_GLEXT_PROTOTYPES
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glx.h> 
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/type_ptr.hpp"
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
    
    GLfloat points[] = {0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};
    GLfloat colors[] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    
    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points),points,GL_STATIC_DRAW);    
    
    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors),colors,GL_STATIC_DRAW);
    
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,points_vbo);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
    glBindBuffer(GL_ARRAY_BUFFER,colors_vbo);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);
        
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    
    
    const char* vertex_shader =
    "#version 410\n"
    "layout(location = 0) in vec3 vp;"
    "layout(location = 1) in vec3 vc;"
    "out vec3 color;"
    "uniform mat4 view, proj;"
    "void main()"
    "{"
    "	color = vc;"
    "	gl_Position = proj * view * vec4(vp,1.0);"
    "}";
    
    const char* fragment_shader = 
    "#version 410\n"
    "in vec3 color;"
    "out vec4 frag_color;"
    "void main()"
    "{"
    "	frag_color = vec4(color,1.0f);"
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
    

    
    float cam_speed = 1.0f;
    float cam_yaw_speed = 10.0f;
    float cam_pos[] = {0.0f, 0.0f, 10.0f};
    float cam_yaw = 0.0f;
    
	
    glm::mat4 ident = glm::mat4(1.0f);
	glm::mat4 T = glm::translate(ident,glm::vec3(-cam_pos[0],-cam_pos[1],-cam_pos[2]));
	glm::mat4 R = glm::rotate(ident, cam_yaw, glm::vec3(-1.0f,0.0f,0.0f));
	glm::mat4 view_mat = R * T;
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
						
	
    
    
	
    while (1)
    {
		glm::mat4 T = glm::translate(ident,glm::vec3(-cam_pos[0],-cam_pos[1],-cam_pos[2]));
		glm::mat4 R = glm::rotate(ident, cam_yaw, glm::vec3(0.0f,-1.0f,0.0f));
		glm::mat4 view_mat = R * T;
		glUseProgram(shader_program);
		glUniformMatrix4fv(view_mat_location,1,GL_FALSE,(const float*)glm::value_ptr(view_mat));
		
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
                    XKeyEvent *kevent;
                    char       buffer[1];
                    kevent = (XKeyEvent *) &event;
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_Escape))
                    {
                        exit(0);
                    }
                    if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_w))
                    {
                        cam_pos[2]-=.5;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_s))
                    {
                        cam_pos[2]+=.5;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_a))
                    {
                        cam_pos[0]-=.5;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_d))
                    {
                        cam_pos[0]+=.5;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_q))
                    {
                        cam_yaw+=.05;
                    }
					if((XLookupString((XKeyEvent *)&event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym)XK_e))
                    {
                        cam_yaw-=.05;
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
