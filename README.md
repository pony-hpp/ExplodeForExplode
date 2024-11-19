# Explode for Explode

... It's a cross-platform and open-source OpenGL destruction simulator game
_(actually, it's not released, so it's a simple game yet)_.

## Build

There's no official builds for this game, but you can build it yourself.

**Requirements**:
<ul>
  <li>OS: Windows/Linux (macOS not tested yet);</li>
  <li>C++17 compatible compiler;</li>
  <li>CMake 3.10+;</li>
  <li>OpenGL: GLFW and GLEW libraries;</li>
  <li>libpng;</li>
  <li>GTest (optional).</li>
</ul>

If you're using Linux, make sure you have pkgconf installed.

If you're using Microsoft Windows, you'll need Microsoft Visual Studio with
CMake and C++ support. Install the dependencies via vcpkg.

The build is the same as in an average project using CMake: build files
generation and build. If you also want build tests, use the
`-DEFE_BUILD_TESTS=1` flag when generating build files.

## Contributing

Pull requests aren't welcome. This game is my own project. But you can fork it
or easily create assets packs with your own shaders and textures and share them!

## License

This project is licensed under the [MIT](LICENSE) license.
