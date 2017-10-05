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
	
	bmpLoader brickDefuseFile;
    brickDefuseFile.loadFile("Textures/brick.bmp");
    GLuint tex = 0;
    glGenTextures(1,&tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, brickDefuseFile.getWidth(), brickDefuseFile.getHeigth(), 0, GL_BGR,GL_UNSIGNED_BYTE, brickDefuseFile.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    bmpLoader brickNormalFile;
    brickNormalFile.loadFile("Textures/brickNormal.bmp");
    GLuint tex1 = 0;
    glGenTextures(1,&tex1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, brickNormalFile.getWidth(), brickNormalFile.getHeigth(), 0, GL_BGR, GL_UNSIGNED_BYTE, brickNormalFile.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    bmpLoader brickSpecularFile;
    brickSpecularFile.loadFile("Textures/SpecularMap.bmp");
    GLuint tex2 = 0;
    glGenTextures(1,&tex2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, brickSpecularFile.getWidth(), brickSpecularFile.getHeigth(), 0, GL_BGR, GL_UNSIGNED_BYTE, brickSpecularFile.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    bmpLoader brickOcclusionFile;
    brickOcclusionFile.loadFile("Textures/AmbientOcclusionMap.bmp");
    GLuint tex3 = 0;
    glGenTextures(1,&tex3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, tex3);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, brickOcclusionFile.getWidth(), brickOcclusionFile.getHeigth(), 0, GL_BGR, GL_UNSIGNED_BYTE, brickOcclusionFile.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    
    glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 biTangent = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 normalVec = glm::vec3(0.0f, 0.0f, 0.0f);
    
    glm::vec3 tempTangent = glm::normalize(tangent - normalVec * glm::dot(normalVec,tangent));
    
    float det = (glm::dot(glm::cross(normalVec,tangent),biTangent));
    if(det < 0.0f)
    {
		det = -1.0f;
	}
	else
	{
		det = 1.0f;
	}
        
    GLfloat points[] = {0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};
    GLfloat normals[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat texCoords[] = {0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};
    GLfloat tangents[] = {tempTangent.x,tempTangent.y,tempTangent.z,det,tempTangent.x,tempTangent.y,tempTangent.z,det,tempTangent.x,tempTangent.y,tempTangent.z,det};
    
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
    "layout(location = 3) in vec4 vtangent;"
    "out vec3 view_dir_tan;"
    "out vec3 light_dir_tan;"
    "out vec2 texture_coordinates;"
    "uniform mat4 view, proj, model;"
    "uniform vec3 cameraPosition_world;"
    "void main()"
    "{"
   "	gl_Position = proj * view * model * vec4(vertex_position,1.0);"
    "	texture_coordinates = vertex_text;"
    
    "	vec3 lightDirection_world = normalize(vec3(0.0, 0.0, -2.0) - cameraPosition_world);"
    "	vec3 bitangent = cross(vertex_normal, vtangent.xyz) * vtangent.w;"
    
    "	vec3 cameraPosition_local = vec3(inverse(model) * vec4(cameraPosition_world,1.0));"
    "	vec3 lightDirection_local = vec3(inverse(model) * vec4(lightDirection_world,0.0));"
    "	vec3 viewDirection_local = normalize(cameraPosition_local - vertex_position);"
    
    "	view_dir_tan = vec3(dot(vtangent.xyz, viewDirection_local),dot(bitangent, viewDirection_local), dot(vertex_normal, viewDirection_local));"
    "	light_dir_tan = vec3(dot(vtangent.xyz, lightDirection_local),dot(bitangent,lightDirection_local), dot(vertex_normal, lightDirection_local));"
    "}";
    
    const char* fragment_shader = 
    "#version 410\n"
    "uniform sampler2D basic_texture;"
    "uniform sampler2D normal_map;"
    "uniform sampler2D specular_map;"
    "uniform sampler2D occlusion_map;"
    "in vec3 view_dir_tan;"
    "in vec3 light_dir_tan;"
    "in vec2 texture_coordinates;"
    "vec3 light_position_world = vec3(0.0, 0.0, 2.0);"
    "vec3 Ls = vec3(1.0, 1.0, 1.0);"
    "vec3 Ld = vec3(1.0, 1.0, 1.0);"
    "vec3 La = vec3(0.1, 0.1, 0.1);"
    
    "vec4 texel = texture(basic_texture, texture_coordinates);"
    "vec4 specularTexel = texture(specular_map, texture_coordinates);"
    "vec4 occlusionTexel = texture(occlusion_map, texture_coordinates);"
    "vec3 Ks = vec3(specularTexel.rgb);"
    "vec3 Kd = vec3(texel.rgb);"
    "vec3 Ka = vec3(occlusionTexel.rgb);"
    "float specular_exponent = 100.0;"
        
    "out vec4 frag_color;"
    
    "void main()"
    "{"
    " 	vec3 Ia = La * Ka;"
    "	vec3 normal_tan = texture(normal_map, texture_coordinates).rgb;"
    "	normal_tan = normalize(normal_tan * 2.0 - 1.0);"
    "	vec3 direction_to_light_tan = normalize(-light_dir_tan);"
    "	float dot_prod = dot(direction_to_light_tan, normal_tan);"
    "	dot_prod = max(dot_prod, 0.0);"
    "	vec3 Id = Kd * Ld * dot_prod;"
	
	"	vec3 reflection_tan = reflect(normalize(light_dir_tan),normal_tan);"
	"	float dot_prod_specular = dot(reflection_tan, normalize(view_dir_tan));"
	"	dot_prod_specular = max(dot_prod_specular, 0.0);"
	"	float specular_factor = pow(dot_prod_specular, 100.0);"
	"	vec3 Is = Ls * Ks * specular_factor;"
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
    
    int camera_vec_location = glGetUniformLocation(shader_program,"cameraPosition_world");
    glUseProgram(shader_program);
    glUniform3fv(camera_vec_location,1,(const float*)glm::value_ptr(camPos));
    
    int tex_loc = glGetUniformLocation(shader_program,"basic_texture");
    glUseProgram(shader_program);
    glUniform1i(tex_loc,0);
    
    int norm_loc = glGetUniformLocation(shader_program,"normal_map");
    glUseProgram(shader_program);
    glUniform1i(norm_loc,1);
						
	int spec_loc = glGetUniformLocation(shader_program,"specular_map");
    glUseProgram(shader_program);
    glUniform1i(spec_loc,2);
    
    int occl_loc = glGetUniformLocation(shader_program,"occlusion_map");
    glUseProgram(shader_program);
    glUniform1i(occl_loc,3);
    
	
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
		glUniform3fv(camera_vec_location,1,(const float*)glm::value_ptr(camPos));
		
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
