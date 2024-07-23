add_rules("mode.debug", "mode.release")

add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

set_allowedplats("windows")

add_requires("nlohmann_json 3.11.3", {
	alias = "json"
})

option("test_exe")
    set_default(false)
    set_showmenu(true)
	set_description("Enables the compilation of a test exe to play with")

target("mpvwrapper")
	set_kind("shared")
	add_files("src/**.cpp")
	add_includedirs("include")
	add_headerfiles("include/**.hpp")
	set_languages("c++20")
	add_packages("json")
	add_defines("MPV_WRAPPER_EXPORTS")
target_end()

if has_config("test_exe") then 
	target("mpvwrptest")
		set_kind("binary")
		add_includedirs("include")
		add_files("test_exe/**.cpp")
		set_languages("c++20")
		add_packages("json")
		add_deps("mpvwrapper")
		add_links("mpvwrapper")
end