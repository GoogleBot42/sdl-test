# sdl-test
Just a barebones SDL2 OpenGL application.  Mostly for my future reference.

# Building and Running
```sh
mkdir build
cd build
cmake ..
make
./SDL2Test
```

# Building and running for wasm target
```sh
mkdir web
cd web
emconfigure cmake ..
make
python -m http.server 8000
# Go to localhost:8000 in a web browser
```

# Controls
`w` move forward

`s` move backward

`a` move left

`d` move right

`e` and `space` move up

`q` and `lshift` move down

`mouse` look around
