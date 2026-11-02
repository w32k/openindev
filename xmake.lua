option("prog-plat")
    set_values("win32", "posix")
    set_default("win32")

option("prog-win")
    set_values("win32", "x11", "wayland")
    set_default("win32")

option("prog-gfx")
    set_values("d3d9", "ogl11", "ogl46")
    set_default("d3d9")


target("w64")
    add_toolchains("x86_64-mingw")
    set_targetdir("bin")
    set_filename("indev-w64.exe")
    add_cflags("-std=c99 -Wall -Wextra -Werror", {force = true})
    add_files("src/main.c",
        "src/platform-$(prog-plat).c", "src/window-$(prog-win).c",
        "src/graphics-d3d9.c")
    if is_config("prog-gfx", "ogl11") then
        add_files("src/extern/ogl11-gl.c")
    end
    add_links("user32", "opengl32", "d3d9")

target("w32")
    add_toolchains("i686-mingw")
    set_targetdir("bin")
    set_filename("indev-w32.exe")
    add_cflags("-std=c99 -Wall -Wextra -Werror", {force = true})
    add_files("src/main.c",
        "src/platform-$(prog-plat).c", "src/window-$(prog-win).c",
        "src/graphics-d3d9.c")
    if is_config("prog-gfx", "ogl11") then
        add_files("src/extern/ogl11-gl.c")
    end
    add_links("user32", "opengl32", "d3d9")



toolchain("x86_64-mingw")
    set_toolset("cc", "x86_64-w64-mingw32-gcc")
    set_toolset("ld", "x86_64-w64-mingw32-gcc")
    set_toolset("cxx", "x86_64-w64-mingw32-g++")
    set_toolset("ar", "x86_64-w64-mingw32-ar")
    set_toolset("rc", "x86_64-w64-mingw32-windres")
    set_toolset("rc", "x86_64-w64-mingw32-windres")
toolchain_end()

toolchain("i686-mingw")
    set_toolset("cc", "i686-w64-mingw32-gcc")
    set_toolset("ld", "i686-w64-mingw32-gcc")
    set_toolset("cxx", "i686-w64-mingw32-c++")
    set_toolset("ar", "i686-w64-mingw32-ar")
    set_toolset("rc", "i686-w64-mingw32-windres")
    set_toolset("rc", "i686-w64-mingw32-windres")
toolchain_end()
