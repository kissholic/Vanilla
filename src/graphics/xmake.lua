



target("graphics")
    set_kind("static")
    add_files("*.cpp")
    add_packages(
        "glfw",
        "spdlog",
        "vulkansdk"
    )