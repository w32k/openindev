


target("openindev")
    add_toolchains("x86_64-mingw")
    set_targetdir(".")
    add_linkdirs("vendor/x64-mingw")
    add_includedirs("vendor/include", "includes")
    add_cflags("-Wall -Wextra -Werror", {force = true})
    add_files("src/*.c")
    add_links("SDL3.dll")



toolchain("x86_64-mingw")
    set_toolset("cc", "x86_64-w64-mingw32-gcc")
    set_toolset("ld", "x86_64-w64-mingw32-gcc")
    set_toolset("cxx", "x86_64-w64-mingw32-g++")
    set_toolset("ar", "x86_64-w64-mingw32-ar")
    set_toolset("rc", "x86_64-w64-mingw32-windres")
    set_toolset("rc", "x86_64-w64-mingw32-windres")
toolchain_end()
