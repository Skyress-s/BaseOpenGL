#version 330 core
 
layout(location = 0) in vec4 positionIn;    // 1st attribute buffer = vertex positions
layout(location = 1) in vec4 colorIn;       // 2nd attribute buffer = colors
layout(location = 2) in vec2 uvIn;          // 3rd attribute buffer = UV

out vec4 color;                           //color sent to rest of pipeline
out vec2 texCoords;


uniform mat4 matrix;                //the matrix for the model
uniform mat4 projection;
uniform mat4 view;

void main() {
  texCoords = uvIn;
  color = colorIn;                       //passing on the vertex color
  //gl_Position = matrix * positionIn;      //calculate the position of the model
  //gl_Position =  positionIn;
  
 // gl_Position = positionIn;
  gl_Position = projection * view * matrix * (positionIn);
}
