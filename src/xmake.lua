
-- Add third party libraries
add_requires("spdlog")
add_requires("yyjson")
add_requires("mujoco")
-- add_requires("glfw")  
-- add_requires("vulkansdk")
-- add_requires("imgui", {configs = {glfw_vulkan = true}})

includes("Core/")
-- includes("window")

target("Vanilla")
    set_kind("binary")
    add_files("*.cpp")
    add_deps(
        "Coroutine"
        -- "window"
    )
    add_packages(
        "spdlog", 
        "yyjson",
        "mujoco"
        -- "glfw",
        -- "imgui",
        -- "vulkansdk"
    )