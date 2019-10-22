

![alt text](https://github.com/jimdox/black_box/blob/master/bx_engine/res/bx_logo-400.png)

A Note on Loading .obj files:
In order to correctly load a wavefront file, apply 'split edges' along that the the UV texture seam, this will duplicate vertices along that seam, as the renderer currently can only process unique vertices per any texture coordinate. Additionally, mark all faces as 'shade smooth', as otherwise blender will generate multiple normals per face, which is not currently supported.

