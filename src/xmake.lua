
-- Add third party libraries
add_requires("spdlog")
add_requires("yyjson")
add_requires("glfw")

includes("Core/")
includes("window")

target("Vanilla")
    set_kind("binary")
    add_files("*.cpp")
    add_deps(
        "Coroutine",
        "window"
    )
    add_packages(
        "spdlog", 
        "yyjson",
        "glfw"
    )