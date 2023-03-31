#version 330 core
 
layout(location = 0) in vec4 positionIn;    // 1st attribute buffer = vertex positions
layout(location = 1) in vec4 colorIn;       // 2nd attribute buffer = colors
layout(location = 2) in vec2 uvIn;          // 3rd attribute buffer = UV

out vec4 color;                           //color sent to rest of pipeline
out vec2 texCoords;
out vec3 Normal;
out vec3 FragPos;


uniform mat4 matrix;                //the matrix for the model
uniform mat4 projection;
uniform mat4 view;

void main() {
  texCoords = uvIn;
  color = colorIn;                       //passing on the vertex color
  FragPos = vec3(matrix * positionIn);
  Normal = mat3(transpose(inverse(matrix))) * vec3(colorIn.xyz);  
  
  
  gl_Position = projection * view * matrix * (positionIn);
}
