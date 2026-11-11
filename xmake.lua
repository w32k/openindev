


target("openindev")
    if is_os("windows") then
        add_toolchains("x86_64-mingw")
        add_linkdirs("vendor/x64-mingw")
        add_ldflags("-static-libgcc -static-libstdc++", {force = true})
        add_links("SDL3.dll", "winpthread")
    else
        add_links("SDL3")
    end
    set_targetdir(".")

    add_includedirs("vendor/include", "includes")
    -- TODO: use options to easily make people be able to switch different
    -- windowing and graphics libraries
    add_cflags("-Wall -Wextra -Werror -DGRAPHICS_GLES2 -DWINDOW_SDL3", {force = true})
    add_files("src/*.c", "src/renderer/gles2/*.c", "src/renderer/*.c", "src/window/sdl3/*.c", "src/cglm/*.c", "src/cglm/**/*.c")





toolchain("x86_64-mingw")
    set_toolset("cc", "x86_64-w64-mingw32-gcc")
    set_toolset("ld", "x86_64-w64-mingw32-gcc")
    set_toolset("cxx", "x86_64-w64-mingw32-g++")
    set_toolset("ar", "x86_64-w64-mingw32-ar")
    set_toolset("rc", "x86_64-w64-mingw32-windres")
    set_toolset("rc", "x86_64-w64-mingw32-windres")
toolchain_end()
