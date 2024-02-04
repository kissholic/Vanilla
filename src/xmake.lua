
-- Add third party libraries
add_requires("spdlog")
add_requires("yyjson")

includes("Core/")


target("Vanilla")
    set_kind("binary")
    add_files("*.cpp")
    add_deps("Coroutine")
    add_packages("spdlog", "yyjson")