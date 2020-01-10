

![alt text](https://github.com/jimdox/black_box/blob/master/juno_engine/res/juno-s.png)





## Loading custom .obj files:
In order to correctly load a wavefront file, apply 'split edges' along any UV texture seams, this will duplicate vertices along that seam, as the renderer currently can only process unique vertices per any texture coordinate. Additionally, mark all faces as 'shade smooth', as otherwise blender will generate multiple normals per face, which is not currently supported.

