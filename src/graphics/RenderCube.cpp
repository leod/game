#include "graphics/RenderCube.hpp"

#include <GL/gl.h>

namespace game {

RenderCube::RenderCube(PhysicsComponent const* physics, vec3 color)
    : physics(physics), color(color) {
}

void RenderCube::render() {
    auto position = physics->getPosition();
    auto orientation = physics->getOrientation();
	
	auto angle = glm::atan(orientation.x, orientation.z);
 
    auto modelview = mat4();
    modelview = glm::translate(modelview, position);
	modelview = glm::rotate(modelview, glm::degrees(angle), vec3(0, 1, 0));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(modelview));

    glBegin(GL_QUADS);
    glColor3f(color.x, color.y, color.z);

    glVertex3f(  0.5, -0.5, 0.5 );
    glVertex3f(  0.5,  0.5, 0.5 );
    glVertex3f( -0.5,  0.5, 0.5 );
    glVertex3f( -0.5, -0.5, 0.5 );
     
    glVertex3f( 0.5, -0.5, -0.5 );
    glVertex3f( 0.5,  0.5, -0.5 );
    glVertex3f( 0.5,  0.5,  0.5 );
    glVertex3f( 0.5, -0.5,  0.5 );
     
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );

    glVertex3f(  0.5,  0.5,  0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );

    glVertex3f(  0.5, -0.5, -0.5 );
    glVertex3f(  0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();

    glPopMatrix();
}

} // namespace game
