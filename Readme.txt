This is a very simple framework written to test and learn about directx programming and real time rendering.
For this reason the code is written to be as easier to read and experiment with as possible without any thought on performances (yet).

Current Features are :
Forward renderer that implements per pixel lighting with directional and point lights. Also the geometries can use diffuse textures.

Planned Features are :
Everything I can implement :). The main new features I want to have are normal and specular mapping, I'd also like to try some shadow mapping tecniques and deferred shading, but thats a bit off in the future.

The Code :
If you want to learn the code start from Drawer.cpp. The scene is currently built there in the OnInit function. Other important classes are the actual renderer DirectXRenderer.h/cpp class and the Geometry.h/cpp class that defines a geometry sent to the renderer

If you have questions or suggestion please contact me !!



