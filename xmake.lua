set_project("Vanilla")


set_languages("c++20")
set_warnings("error")

includes("external")
-- add_includedirs("external")
-- add_includedirs("external/bgfx.cmake/bgfx/include")
-- add_includedirs("external/bgfx.cmake/bx/include")
-- add_includedirs("external/bgfx.cmake/bimg/include")

-- option("external_macro")
--     add_defines("BX_CONFIG_DEBUG=" .. (is_mode("debug") and "1" or "0"))
-- option_end()

includes("src")
includes("test")