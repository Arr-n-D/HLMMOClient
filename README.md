# HLMMOClient
> We're a wizards, Harry

### Prerequisites
1. Don't use Mac - why would you do that?
2. Install [xmake](https://xmake.io/) for compiling
3. Install [Visual Studio Community - Desktop C++](https://visualstudio.microsoft.com/downloads/) for the compiling (you can use VSCode if you want)
4. Install [Rust for Windows](https://doc.rust-lang.org/cargo/getting-started/installation.html)
5. DON'T clone the repository yet before reading setup instructions

### Getting Started
First, create an [Epic Games account](https://epicgames.com/) and link your GitHub account to it under [Apps & Accounts](https://www.epicgames.com/account/connections). Once that is completed, **check your email** for an invitation to the Epic Games GitHub organization. This step is required to properly download all dependencies.

The client requires a few more dependencies to properly compile and run. To keep things neat and tidy, first, make a folder of your choice to house the `HLMMOClient` repository, as well as an additional group of Unreal Engine Modding dependencies. For example, create a folder at `~/Developer/HLMMO/Client`. Inside the `Client` folder you just created, do the following steps:

1. Clone the `HLMMOClient` repository into the folder
2. Clone the [RE-UE4SS](https://github.com/UE4SS-RE/RE-UE4SS) repository into the same `Client` folder (it should  be a sibling of `HLMMOClient` in the folder structure)
3. Navigate into the `RE-UE4SS` repo you just cloned and run `git submodule update --init --recursive` - this will download a few additional dependencies and plop them into the `deps` folder inside the `RE-UE4SS` repo.
4. In the root folder, `Client`, create a `xmake.lua` file and put the following contents: 

```lua
includes("RE-UE4SS")
includes("HLMMOClient")
```

5. In the `Client` folder, run:

```ssh
xmake f -m "Game__Shipping__Win64"
xmake
```

6. After that is completed, install the latest version of [UE4SS and follow the instructions](https://docs.ue4ss.com/installation-guide.html) - in the context of the installation, if you installed Hogwarts Legacy via Steam, you should be placing the files in `steamapps\common\Hogwarts Legacy\Phoenix\Binaries\Win64`

7. Follow the instructions for [Installing a C++ Mod](https://docs.ue4ss.com/guides/installing-a-c++-mod.html) and make sure you copy over the following file from your `Client/Binaries/Game__Shipping_Win64/HLMMOClient` folder: `HLMMOClient.dll` - make sure to rename the `HLMMOClient.dll` to `main.dll` as per the UE4SS instructions

8. In the `Client/Binaries/Game__Shipping_Win64/UE4SS`, copy `UE4SS.dll` and `UE4SS.lib` over into `steamapps\common\Hogwarts Legacy\Phoenix\Binaries\Win64` and if asked, select to overwrite existing files

9. Copy over the GameNetworking library files from the `HLMMOServer` repo -> `GameNetworkingSockets.dll`, `GameNetworkingSockets.lib`, `libcrypto-3-x64.dll`, and `libprotobufd.dll`

10. Your `Mods/HLMMOClient` folder should look like this:

```
dlls/
  main.dll
  GameNetworkingSockets.dll
  GameNetworkingSockets.lib
  libcrypto-3-x64.dl
  libprotobufd.dll
```

10. In the `\Phoenix\Binaries\Win64` folder, open `UE4SS-settings.ini` and adjust the following settings:

```
# Update the Console settings to ensure its enabled
ConsoleEnabled = 1
GuiConsoleEnabled = 1
GuiConsoleVisible = 1

# Update the console font scaling to ensure its visible
GuiConsoleFontScaling = 3
```

11. Run your compiled server from the `HLMMOServer` repository (directions in that repo)
12. Launch Hogwarts Legacy - you should see your Server executable receiving a UDP connection and it should launch a UE4SS debug window and another terminal window describing the connection
13. Congratulations, you did it!

> If you receive build errors, navigate to `Client/Intermediates/.packages/c/**/install.txt` for more detailed error logging. Your Terminal output will nudge you here.

#### Troubleshooting

**Error: unable to create directory ... Access is denied. (os error 5)**
This is an error coming from Rust's package manager, Cargo. There is a [closed issue on the Rust repository](https://github.com/rust-lang/cargo/issues/11544) describing this. It seems that the current commonly accepted method for fixing this is to the relevant file to the exclusions list in Windows Defender.
