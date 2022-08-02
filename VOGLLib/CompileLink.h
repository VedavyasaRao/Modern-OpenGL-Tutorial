#pragma once
#define WGL_WGLEXT_PROTOTYPES
#pragma include_alias( "GL/GL.h", "..\..\externals\GLExtensions\GL\GL.h" )
#pragma include_alias( "GL/wglext.h", "..\..\externals\GLExtensions\GL\wglext.h" )
#pragma include_alias( "SOIL2.h", "..\..\externals\SOIL2\src\SOIL2\SOIL2.h" )
#pragma include_alias( "glm/glm.hpp", "..\..\externals\glm\glm\glm.hpp" )
#pragma include_alias( "glm/gtc/matrix_transform.hpp", "..\..\externals\glm\glm\gtc\matrix_transform.hpp" )
#pragma include_alias( "glm/gtc/type_ptr.hpp", "..\..\externals\glm\glm\gtc\type_ptr.hpp" )
#pragma include_alias( "glm/ext/matrix_projection.hpp", "..\..\externals\glm\glm\ext\matrix_projection.hpp" )


#ifdef _DEBUG 
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "..\\..\\externals\\GLExtensions\\lib\\GLExtensions_static_x86_d.lib")
#pragma comment(lib, "..\\..\\externals\\SOIL2\\projects\\VC15\\libs\\SOIL_static_x86_d.lib")
#else
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "..\\..\\externals\\GLExtensions\\lib\\GLExtensions_static_x86.lib")
#pragma comment(lib, "..\\..\\externals\\SOIL2\\projects\\VC15\\libs\\SOIL_static_x86.lib")
#endif

