set_project("Vanilla")


set_languages("c++20", "objective-c")
set_warnings("error")

add_includedirs("external")

includes("src")
includes("test")