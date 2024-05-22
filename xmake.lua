local projectName = "HLMMOClient"
add_rules("mode.debug", "mode.release")

add_requires("boost")

target(projectName)
    set_kind("shared")
    set_languages("cxx20")
    set_exceptions("cxx")

    add_packages("boost")
    add_packages("msgpack")
    add_includedirs(".")
    add_includedirs("./include")

     if is_mode("debug") then
        add_defines("DEBUG")
    end

    add_files("dllmain.cpp", "Core/core.cpp", "Game/Game.cpp", "Networking/NetworkManager.cpp", "Game/Player.cpp" )
    

    add_deps("UE4SS")
    add_links("GameNetworkingSockets")

    -- Add link search directory
    add_linkdirs(".")

    on_load(function (target)
        import("build_configs", { rootdir = get_config("scriptsRoot") })
        build_configs:set_output_dir(target)
    end)
    
    on_config(function (target)
        import("build_configs", { rootdir = get_config("scriptsRoot") })
        build_configs:config(target)
    end)
    
    after_clean(function (target)
        import("build_configs", { rootdir = get_config("scriptsRoot") })
        build_configs:clean_output_dir(target)
    end)
