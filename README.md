# 3D animation for palletization process by UNIGINE
## Build
```bash
mkdir build && cd build
cmake ../source
cmake --build .
```
## Make video from sequence of screenshots from main camera
During running, screenshots are captured and saved in `frames` directory. We will make video from this by tool `ffmpeg`.
```bash
cd frames
ffmpeg -framerate 15 -i screenshot_%d.jpg -c:v libx264 -pix_fmt yuv420p palletizer.mp4
```