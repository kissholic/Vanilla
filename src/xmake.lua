

includes("Core/")


target("Vanilla")
    set_kind("binary")
    add_files("*.cpp")
    add_deps("Coroutine")