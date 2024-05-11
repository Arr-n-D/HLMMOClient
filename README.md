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

> If you receive build errors, navigate to `Client/Intermediates/.packages/c/**/install.txt` for more detailed error logging. Your Terminal output will nudge you here.

#### Troubleshooting

**Error: unable to create directory ... Access is denied. (os error 5)**
This is an error coming from Rust's package manager, Cargo. There is a [closed issue on the Rust repository](https://github.com/rust-lang/cargo/issues/11544) describing this. It seems that the current commonly accepted method for fixing this is to the relevant file to the exclusions list in Windows Defender.