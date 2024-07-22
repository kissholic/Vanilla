
-- Add third party libraries
add_requires("spdlog")
add_requires("yyjson")
-- add_requires("mujoco")
add_requires("glfw")  
add_requires("imgui", {configs = {glfw_vulkan = true}})
add_requires("vulkansdk")
add_requires("bgfx")

includes("core/")
includes("window")
includes("graphics")

target("Vanilla")
    set_kind("binary")
    add_options("external_macro")
    add_files("*.cpp")
    add_deps(
        "Coroutine",
        "window"
    )
    add_packages(
        "spdlog", 
        "yyjson",
        "glfw",
        "imgui",
        "graphics",
        "bgfx",
        "io_context"
    )