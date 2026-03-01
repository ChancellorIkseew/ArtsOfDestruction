vec3 a_position  : POSITION; // Входная позиция из Vertex Buffer
vec4 a_color0    : COLOR0;   // Входной цвет из Vertex Buffer
vec2 a_texcoord0 : TEXCOORD0; //
vec3 a_normal    : NORMAL; //

vec4 v_color0    : COLOR0;   // Выходной цвет для фрагментного шейдера
vec2 v_texcoord0 : TEXCOORD0; //
vec3 v_normal    : NORMAL; //

vec4 i_data0 : TEXCOORD4;
vec4 i_data1 : TEXCOORD5;
vec4 i_data2 : TEXCOORD6;
vec4 i_data3 : TEXCOORD7;

