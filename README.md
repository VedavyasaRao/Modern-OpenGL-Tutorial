# Modern OpenGL Tutorial and Application for Windows OS #

*Learn Modern OpenGL from scratch and its use in areas such as clinical imaging.*

#### Introduction ####
[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEi-EWXqUURC12cdMkrCRBhGNBPnKoZT-LhAhT6obaPyKc5CwoqapxKAdfG9QrfaT-fs31EWRL6gEhvaKEeY8TcaWl04npK5sivELAb75aR1y6bFMvls4wl7w_A1QkfaZadAb98BAMohLKM5EXELYBH9WhwjRj4OyzMyH0cQBMPbuBAxW_p-t2aTkF5Xmw=w200-h87)](https://modernopengl-vedavyasarao.blogspot.com/2022/06/introduction.html)

OpenGL is a graphics library that can be used to render interactive 2D and 3D graphics applications.OpenGL has wide range of applications.This tutorials attempts to teach basic to advanced concepts one at a time. The goal is to create an advanced , multifunctional library from scratch. The tutorials will be a set of C++ header (.h) files.This tutorial is primarily targeted for windows OS. Visual Studio development environment is used for writing, compiling and debugging the code.

#### Implementation - Initializing OpenGL Context  ####
[![Implementation - Initializing OpenGL Context](https://blogger.googleusercontent.com/img/a/AVvXsEhZq7gKgInHdijAzTSZ3XLCFJs3hL8133ndoQqjnbE02IwypbScbbmikChtP8pwIERQalTyewoVsehGPfU40zA1Te8Ho1fT9kmmbyni4girSEtVMhas2DQVUgTP-xWRL0mUdQkGjka7LKj8B2Z1Mwg9h_7BLEHho0QckgNJVAGBkbP2CmavYnH9MJZYiHW6=w200-h83)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/modern-opengl-library-implementation-1.html)

OpenGL is a drawing library that requires a context to draw upon. 
Creating OpenGL context in windows OS is not a trivial task. It's mainly because opengl32.lib supplied by windows OS supports OpenGL specification 1.1. 
The display card providers such as NVidia or Intel or AMD actually implement the latest and greatest OpenGL specification, including additional functionality and supply it as OpenGL Installable Client Driver or ICD.

#### Lesson01 : Initializing OpenGL Context ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEiZcxcJddbg0bF-3h-YaroI9VE7TatcTu16KuO3YaWLc81NY1AnLOrEll9jqLw7Ql84q8LdLcxhTjNbcp0jviVlsCAyTF0I33p9eodOe8Wp4P_n4xo6uDbpePKyJ0EozwrFoICcrtXFeNFLTxTKwzwL6LBMEzwl0BUY4fqGzTKBuhtlRV4mP6zIGlHF_22m=w200-h118)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/example01-creating-opengl-context.html)

This lesson discusses  implementing the basic operations of creating a hosting window, Initializing it with OpenGL context, rendering it and handle mouse/keyboard inputs.


#### Primer: Vector ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEivfC41hcgMyUsHlR2XsGoZAbi_Qfp1b3veJL3a-mtFva8aMRMFuPe-sKG2tLgMnPBfWIyFiLnJNKTZwAcpdFmIcs3YpM6u0eYICo_LiXnNNRaSyyGHOxYykWTO4K3rBnXM3ef_CVuJMrZ0C_YJTRIGXMG133wy8F_uFS8zy_6ol3Uv78mXNsMt96mmLBtl=w200-h51)](https://modernopengl-vedavyasarao.blogspot.com/2022/06/essential-3d-math.html)

Vectors are widely used  in 3D calculations. A 3D vector is represented as [Vx, Vy,  Vz] where  Vx, Vy and Vz represent numbers in 3D cartesian space.

#### Primer: Matrices and Affine transformations ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEjAkx6cCx0urZ3-mMWWSJKxKdYn6x5UFHFyG8m8gYbIhQ7v4YeYSZU7RTVIuSQXTBHj8YJXm35kPxJldhIYULaPgDkDPcfQxeZmA_gSuILfdLvMK--ipCPkMQVMsEOFs4GpJ8yXlqQ-i1WEu6Ru61P6IydKMbja8j1s9O4QnR7LeecdM9e4_MG9irMiPfj2=w200-h141)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/matrices.html)

OpenGL uses matrix operations for a lot of purposes. For example, translations, scaling, rotations. Also computing of projection matrices, view matrices etc. The following discusses it in detail.

#### Implementation - BaseCamera and Camera Data ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEi2kMpOI3ZA4Bm9c9-kSgLJ9Sg3_zv4GgUNy2dnlLdAU73grv0H7O2SbxL7Fc19uVL-eYmau11sabjyK8dWcSC-mTZRV8nPpq4WUVNU4Wvak9y_eyrdC_ka3lObkZ6q71mdfQngQe87xVO1duL_zI8lso40Qq-JffK-rhhxy7-GDE3wm08oaWesOpHujc7Q=s16000)](https://modernopengl-vedavyasarao.blogspot.com/2026/04/opengl-library-implementation-camera.html)

Camera is used for projections and animations. It holds camera data which is used for storing mouse and keyboard input information. Further, it stores computed transformation information such as pitch, yaw and roll angles, translation, scaleby as well as Model, View and Projection matrix information of the 3D object. 

#### Primer: Graphics Pipeline ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEj_ZqEvKj7YFGDYulb0VXpe3RRL3JPo675kaNFvT8yc5aV5mZt-fYnB3CDBtk3vABUwxXM8eJA3oVnaMO6jDAvI4vxPE-Nu85EgtGINDpKhefRAmTMYUk-DUKtjWCm-waf5iJcb-VRAZfwi8CdNK3Dlw1Fxru4OeU7X9gwNopGwYs6yvom5QXVKiAGf1k4N=w90-h200)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/lesson02-graphics-pipeline-vao-vbo.html)

A scene consists of  a set of  3D objects. Transformations such as translation, scaling and rotation as a result of Camera movement, mouse and keyboard input brings them into life. 
For example  the following diagram shows a multi color cube with  50 degrees pitch and 20 degrees yaw

#### Implementation:Vertex Buffer ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEjoOB8ZnQIS6XcwJEYrKcXhBAX_78FJWy1ahiOOsLsiYlsjUoi3sjxX5b3bmDnkpywWNall0nbLvDq_BGpQjaCBFZLqA8eM2A-j7cxrCuTNNKUc7KOJcXvmOC95DHkjDRF5epy1ObdXOtvb5nAtGF300YRvZGmi-F4RqYAtqxDPBCS0Mpk6pGWAzM04cP1r=w200-h60)](https://modernopengl-vedavyasarao.blogspot.com/2026/04/implementationvertex-buffer.html)

In this post we shall deep dive and understand mechanics behind sending vertex data to the GPU. Vertex data is obtained from mesh objects such as cubemesh which will be discussed later. The vertex data consists of Position, Color, Normal, and Texture Coordinate of each vertex. They are basically stored into VBO, EBO buffers. Later they are packaged into VAO objects.

#### Implementation: 3D Mesh ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEg0KDwYWWLGmLYxnek6VYYH-LSCK8KVS5OV_pQj63LxAYLhDsNlDfWHgXwbheKSLPn1L58t-i2eV1d-udGXtX3KOAs9hA1EC4vRhyYNd6VBFe5GI_Z-u1ILVFeZdRqsGun348vILnbuUrJLcGprae0TgimYae0lIom0FO2JUuyDwxM5ozh2Z3fVlwmziL1G=w200-h123)](https://modernopengl-vedavyasarao.blogspot.com/2026/04/implementation-igeometrymeshindexed-and.html)

In this post we shall deep dive and understand implementing a mesh.

#### Implementation:Shader programs ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEg6SQnYBFMuTqx_9sFFkavV6Ig7OstfrP_Nr6lHYu15EIWmuZHzw1d-1e1ciTmBiKJYUUGGXheOusgDDmcblNfu6j4BmDHQqFlzxfqWC-nhDv02xazDY7vJz0DU19futqwpBk5Z1Oz8YfdiiW7sfxSHg6auX5NemZyA73ag2t2QcdneSJG1amsdM2ucAPhH=w200-h83)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/primershader-programs.html)

The following provide an overview of shader programs and their implementation in this tutorial.

#### Implementation: Pipeline ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEhUOOb5KSWAeh6U-droik0MJReRXpHtP3Z7Q1X60bCw4X9XmGpLG3Bo5VWX_D4y2_2603JHHkje0ZiAntxATCqsymGRZeVYDP7cDFflrI412E_bbfjswTiV-YGrB6EuuSOp7BlAz3Tuobqn0dNLmYA0XJlrGM_SnUL6pw_MPaedF3RBXxphEL_OZhyBLFHd=w200-h185)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/modern-opengl-library-implementation.html)

In this post we shall deep dive and understand mechanics behind rendering 3D objects on the screen.
So far we discussed  FrameBuffers,Vertex Array Object containing VBO and EBO buffers and  Program Object containing Vertex Shader and Fragment Shaders.

#### Implementation: CubeMesh and SingleColoredCube ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEj4ph7JVdOQTwkBIf5c6sA7w4FdvgjGkf2M9IaqwbaD8Eoo8NZMmn1pSerWRNirg60_brzntDIqSaUEh6gvRgwoVEJsoDzcIEaRD16ZCRv2BVsY_yNOcW9FNHRPYR1GGBJoAsaVvoF5CCIXvKnB9-v_3KF3X9OMdF7ZZslSa7hIm1G2Qt8q7h200ft58PRp=w200-h99)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/implementationcubemesh-and.html)

In this post we shall deep dive and understand implementing a mesh and geometric object for cube shape. We will try to draw a single colored cube as shown above and rotate it along the three axes. The cube looks elongated because aspect ratio is not applied.

#### Lesson02: Single Colored cube ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEgY7m1O4AJtXLOSJ1GbPAbI6XY3uCOxK5LEseWspIHcPdM9E_QOlM7gtIr1aI7pv0kP9-cugTmKXT3wgUO4bLV5aNf1pumS0BaS3YrUMmgpOOrMa4U7kXatALuGd1jaLYK69WPEJOyWj6qZITZSS1bKXQ-S990tJtmw9qk4ejS_3RXox6L2RZ-4ZaSnMyjI=w200-h118)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/lesson02-single-colored-cube.html)

In the previous posts the graphics pipeline and vertex processing were explained. Also, the implementation of the CubeMesh and SingleColoredCube were covered. 
In this post we will try to draw a single colored cube as shown above. The cube looks elongated because aspect ratio is not applied.

#### Implementation: Multi Colored Cube ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEg0Yd0UKnoInRZeXlGyX84aezLo0lsElA1g3E_CoVyCEOYEZOtYg8kWdfUSCY6qYifmoa2w9TW8H3pwoJ6fwfwIh-JrwrQHRHqvaLxOY_t5fEaGWbMOum1LqWQmIr2iwkeHITo6G3lyJUJM6Q7tNAJHT_WZYj7owm-QoHZ3l0CiXV498p9WcJbfoOAFG72H=w200-h96)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/implementationmulti-colored-cube.html)

In SingleColoredCube we saw that VBOs were used to draw the single colored cube using Position data. In this post we  will discuss how to draw a Multi Colored cube using VBOs with Position and Color data having unique color on each face. It looks as shown at the bottom. It looks elongated because aspect ratio is not applied.

#### Lesson03: Multi-Colored Cube ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEipJv53Qe1tGABlw0mDR_2OU3pgMtaaSgGJKwMaRuMdLLys600UYrgHoq76NDofLrlZa7mtqsXUb7vij-2TcdKnyOoMneXmM0NJoIM09biQlMAMwlJH5JUyd_QLaGU5CvBU_zMAdd9UPwhvV_swig0tIxjtuZvGnwbUhvlhH8MHeUAOrv8P7iWI1KZ8ga-j=w200-h118)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/lesson05colored-cube.html)

In the previous posts the graphics pipeline and vertex processing were explained. Also, the implementation of the CubeMesh and SingleColoredCube were covered. 
In this post we will try to draw a single colored cube as shown above. The cube looks elongated because aspect ratio is not applied.

#### Implementation:Indexed Cube with interpolated colors ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEiVOGZBppfpb-t1S9rQHO9nBFpdIcMRGfRn2_Pdjkat8Fvpc9TcMwanrdtxU2IP3ZSwe_f79Uyi_hoSeXhy7jkyb8XyO4aWFMmjNCGVN0d3_8WCrCwRKr5W76_OWZZ53qPkAKGa6ZthiBtKHD_TTRDZApnLDt3uHhfh_nzlXUoyPGCZ_jYRMo5qAy6J8eWO=w200-h96)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/blog-post.html)

In the previous posts the graphics pipeline and vertex processing were explained. Also, the implementation of the CubeMesh and IndexedCube were covered. 
In this post we will try to draw a cube with interpolated colors as shown above. The cube looks elongated because aspect ratio is not applied.

#### Implementation: Textured Cube ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEh602DbVp5eBgQjPXwXqB3_gQTE4Jmit32V61fV2DUMEWgX31UmDLoyZvR4lqrvrrhH1Zvp5ei-es4IGUH1AvDdT3k4JQd4b6UJVTtti-5rkvk1Y5vKwtRjxev3EIbGEiaRpcqqJbf1tw47sRqxcmcuUJ8tB3c0m-g0Oeg-r2aOwSNUz1S5pV-Ag7cR9rbS=w200-h148)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/implementationtextured-cube.html)

In the previous discussions, we covered sending vertex data - Position and color. In this post we will try to send texture vertex data. It's implemented in TexturedCube class.

#### Lesson05: Textured Cube ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEjUhcIU64BOTBSi5QjGkgivyt92vm0f29RkaMPifL2JSb1ZdXq6QMChKYRa4V8vpSBYJysa2XeWKwL518Id2EExLbCnZgBC3a02N5DrpD1GvbpQAPXqppprCHT9Z_fMOPFASbf_vF-ZV7dTcqC5fiGnLJ0Tph77ytCwAEuDafmtIAWNvvprMaRsGjg6_mR9=w200-h118)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/lesson04textured-cube.html)

In the previous posts the graphics pipeline and vertex processing were explained. Also, the implementation of the TextureUtil and TexturedCube were covered. 
In this post we will try to draw a cube with brick texture as shown above. The cube looks elongated because aspect ratio is not applied.

#### Primer:Lighting ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEhtLOCMjXYoxGfIbEp3uDNCyMC6A8krr04ja_CQMJYQ7-EyyKK9BuMUB-00NtIOZwbF19CuqG8HCzCi4hRK7Om7XI0P49hHoup02Id4DG4_PavFJQV5yyDt3h6zgdNA8cP2v6uO3fm4djhR9zY0R3sBwKlffqfFU_8U-WpOgGFoioQWKW26fvprFzY3c5AU=w200-h56)](https://modernopengl-vedavyasarao.blogspot.com/2026/05/primerlighting.html)

Lighting is an important factor to consider while rendering realistic 3D shapes.  The following discusses adapting lighting conditions while rendering 3D images.

#### Implementation: LightedTexCube ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEijT-PYv47g-jAuhS2UbbJjZC-eU-Jdwe90Qy2Qv0ey6hl2G_ULf5BdLUSUboAAZ7NIeRmpFrNgtRLqG4-5xuixiwS5HyHccVyGEpw3s2HvWbim3HFCpceR3xj1fQ0g8PUhO-XQky3f98iMZ-1VpSB1KBOc5G3dAbEhms0gTEYhq_w8gmqjucZQ1Q4mihpd=w200-h147)](https://modernopengl-vedavyasarao.blogspot.com/2026/05/implementation-phong-lighted-cube.html)

In previous examples, we saw that Position, Color, Texture data were sent to draw cubes. In this post we  will discuss how to implement  Phong lighting models and other lights such as Directional, Point and Spot Lights. For this surface normals needs to be sent as VBO data.

#### Lesson06: Lighting a Textured Cube interactively ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEjg74AEWfVWAvpioW7a2DYZlDWYlz83eKL3FbhBfFxS0rz_cd5u5LOzIdmEA1SKJKAn-UGHIoZrUA1JGNsQhQ6oZ98ZUjVqV2uIYMNOLujMG3n9YsGTyh4Oyk8AMPud4YXlLBj43f5fYmYlGh8A7mXObnH6Ehq6jbj54jGXQqymwehII56LUCGk93yn-gkW=w200-h119)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/lesson06-phonglighted-cube.html)

In the previous posts drawing a textured cube were explained. Also, the implementation of the LightingUtil and LightedTexCube were covered. 
In this post we will try to draw a textured cube with lighting interactively. The cube looks as shown above. The cube looks elongated because aspect ratio is not applied.

#### Implementation: Drawing Text and Images ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEijT-PYv47g-jAuhS2UbbJjZC-eU-Jdwe90Qy2Qv0ey6hl2G_ULf5BdLUSUboAAZ7NIeRmpFrNgtRLqG4-5xuixiwS5HyHccVyGEpw3s2HvWbim3HFCpceR3xj1fQ0g8PUhO-XQky3f98iMZ-1VpSB1KBOc5G3dAbEhms0gTEYhq_w8gmqjucZQ1Q4mihpd=w200-h147)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/drawing-text.html)

In this post we shall understand affine transformations interactively the three kinds of affine 
transformations: Scale, Translate and Rotation on X, Y and  Z axes as discussed in the previous post.
The rotation has been already demonstrated in the earlier examples. In this post we will deep dive into Translation and Scaling.

#### Lesson07: Drawing Text and Images interactively. ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEhazaO-FVx7T1t0pLAhvxYHxZ8pCtiVvowbpto1wYBZ70h12SbFFIFt_qXV-0BI_LvjKIfiLUdvnECwIBvp5PN5DQW__Mk-VEkFf1233ClPpF5ILsXUiTpg98PglFI3nYgGf7li_-mdHe6eX5aMkMm6zOdM0UhwWr3STbIzy9raNh2J0_QcTrcaCVsJcO76=w200-h118)](https://modernopengl-vedavyasarao.blogspot.com/2022/08/lesson-07drawing-text-interactively.html)

As discussed in the  previous article, we saw how Text can be drawn in multiple fonts, sizes and colors and images can be placed side by side.
Also, the implementation of the TextMesh , TextureUtil  and TextImageSketcher were covered. 
In this post we will try to draw  text and image interactively.

#### Lesson08: Understanding Affine Transformation interactively. ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEhFsXjzHEFd785OKnwCkh1GFtSKqtrChN2jWxi7qsFBNCwDM0JB6iHPhiNJlaWHnQ3uQg7w_bmdQR8Ug7cpeYZ3BjOGLJE6UQJnHAGpiSHqEfwOjQ5uy6PW80NpRj1RaSiaLkIgUflWzBvfGYFLoeOMyb6Z75gKgIrTUr5Z3as1GjFVbFTPylfsD7OYkNw8)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/lesson06understanding-affine.html)

In this post we shall understand affine transformations interactively the three kinds of affine 
transformations: Scale, Translate and Rotation on X, Y and  Z axes as discussed in the previous post.
The rotation has been already demonstrated in the earlier examples. In this post we will deep dive into Translation and Scaling.

#### Primer: Importing WaveFront OBJ Models ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEixcR-EQofhTXR0xA8gs5nT4PZWLZfVnY7IwC9lAFTJ-NmJoPze9K7HEi_wxIPyTIw6lfm4zmnifdn4MgdE3OJDOXESaxLh7LyImQnPb4W2jMnJImt8urqAshOn_fqoeSfaGa57mYge1UNppHTUsthp_pK33m3EoAVH52jzVo2TNNG76qBHSRUQhsIAOjXD=w200-h200)](https://modernopengl-vedavyasarao.blogspot.com/2026/06/primer-importing-wavefront-obj-models.html)

Thus far we have worked with the cube geometric object to understand the concepts. In real world, more complex and detailed geometric objects or models are used. They are generated by sophisticated softwares such as blender that can be exported to plethora image formats.
Wavefront OBJ is one such image format.

#### Implementation: Importing WaveFront OBJ Models ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEgSzFTHLNCbu6tnv5HtmCnCDa6tF2pjviZf-C60Kl5FMZY1rfHx-niyozJMLLb3nb0s0PcKYtLMadEPNlgtD4keQttgVZkvNHfI39lMS-4nd8a7GpnHzB8uFrYgKRgBYqcasXaTl-89KqQxyDjyfWhJDeXdZkcFBzVsmlTKu4zmywR7TkNJocMPBczA_Ona=w200-h180)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/implementation-importing-wavefront-obj.html)

In previous examples, we saw that Position, Color, Texture data were sent to draw cubes. In this post we  will discuss how to implement  Phong lighting models and other lights such as Directional, Point and Spot Lights. For this surface normals needs to be sent as VBO data.

#### Lesson 09: Importing WaveFront OBJ Models ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEiCW-eoSjdzqC9X9oXeV4-_ftWPwuPoCGQGP9_uWtqisVb3tFRW06ZAVMrmsRnxJGT4EcjYNV4ysbf_5602pS479olMJDGBf87qGRrH5cKZH7-KMzVBP2DYIPAiG9L2mnTdX7hEDPzu6yYFKHAoREDGsAKnZ_KQuYZ8P5Uk9v2A6_hXao7iQJcrWsy0MSux)](https://modernopengl-vedavyasarao.blogspot.com/2026/06/lesson-09-importing-wavefront-obj-models.html)

As discussed in the  previous article, we saw how geometric objects can be imported from obj and mtl files. Also, the implementation of the WFObjMesh, WFOBJInfo and WFObj were covered. 
In this post we will try to render a model from "dolphin.obj" and "dolphin.mtl" files.

#### Primer: Understanding View space, Perspective and Orthographic Projections ####

[![Introduction](https://blogger.googleusercontent.com/img/a/AVvXsEhT69SM87ldzUqlHm7sLObC1zgHA5md8N65ZHygdgXNRCJGdfQfazGmAP2sFbssd8kQ3fwd2evw3_Rl6pwnRhylBH08fO35k46328wKKG_RAofN_HpfkIW7EtVkv4ZggF-cVsKoRVUvykdbtkpigTeButZfn4IENC1jnXl3AtCx6DE9mayilDLxfxD8mud-)](https://modernopengl-vedavyasarao.blogspot.com/2022/07/projection.html)

So far our camera was in a fixed position and transformations happened in object space.  In this post we will  look at the larger picture.