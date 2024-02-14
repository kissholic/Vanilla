set_project("Vanilla")


set_languages("c++20")
set_warnings("error")

includes("external")
add_includedirs("external")

includes("src")
includes("test")