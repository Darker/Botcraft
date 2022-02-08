![Linux](https://github.com/adepierre/Botcraft/workflows/Linux/badge.svg) ![Windows](https://github.com/adepierre/Botcraft/workflows/Windows/badge.svg)

# Botcraft

Botcraft is a cross-platform C++ library to connect and interact with Minecraft servers with (optional) integrated OpenGL renderer.

This is a "learn-by-doing" code, with some lines dating back to 2017 and never changed since. A lot of things should be improved or refactored. I only code this on my free time, when I feel like it, so don't expect it to be a fully-featured commercial grade library. I share the code because I think it can be interesting for other people, but the goal is not to make it a widely adopted library, or even a useful one. I just do it for fun, it's more a shared private repo than a real team open source project. If you're looking for more features, there are alternative libraries by people with much more time (and probably talent) than myself.

## Features

Main features are listed below. To see the eveolution of the project, check the [changelog](https://github.com/adepierre/Botcraft/wiki/Changelog).

- Connection to minecraft server (both offline mode and online connection with Mojang or Microsoft account)
- DNS server name resolution with and without SRV record
- All official releases from 1.12.2 to 1.18.1 supported
- Compression
- Physics and collisions
- World data
- Full entity support (type, data and metadata)
- (Optional) Rendering of all the blocks (including entity-blocks like chests, banners...) and entities (bounding box only)
- Bot programming with a behaviour tree system (see [the wiki page](https://github.com/adepierre/Botcraft/wiki/Behaviour-system) for details)
- Bot control with mouse and keyboard

Available bot behaviours includes:
- Path finding
- Block breaking
- Inventory managing
- Block placing
- Block interaction (button, lever etc...)

Example of pathfinding. Right of the screen is the integrated renderer
![](gifs/video.gif)

Example of entity processing. Only monsters are attacked as soon as they are in range. Passive mobs are ignored.
![](gifs/entities.gif)

More complex example with 10 survival bots collaborating on a pixel art build. They are all in survival, so they have to pick the right blocks in the chests, eat food and obviously can't fly. There is no global supervision, and they can't communicate with each other.

![](gifs/mapart.gif)

## Dependencies

All the libraries are included either directly(\*) or as submodules(†) and are built locally automatically by cmake (if not already found on your system) so you don't have to download/compile/install anything manually.

- [asio](https://think-async.com/Asio/)† for low-level TCP
- [nlohmann json](https://github.com/nlohmann/json)\* for JSON support


Optional dependencies (can be disabled with cmake options)
- [glad](https://glad.dav1d.de/)\* for OpenGL stuff
- [glfw](https://github.com/glfw/glfw)† for OpenGL window creation
- [glm](https://github.com/g-truc/glm)† for math stuff
- [imgui](https://github.com/ocornut/imgui)† for additional UI display
- [openssl](https://www.openssl.org/)† for encryption
- [rectpack2D](https://github.com/TeamHypersomnia/rectpack2D)† for texture packing
- [stb_image](https://github.com/nothings/stb)\* for texture loading
- [zlib](https://github.com/madler/zlib)† for compression

The code is cross-platform and requires a C++17 compiler.

### ProtocolCraft

ProtocolCraft is a sublibrary of the botcraft repository. It is a full implementation of the minecraft protocol for all supported versions. It's based on the [official source code mapping](https://www.minecraft.net/en-us/article/minecraft-snapshot-19w36a) provided by Mojang. I try to keep all the packets and variable names as close as possible to the source code ones. To avoid name conflicts, an underscore is sometimes appended at the end of a variable name.

## Building

To build the library for the latest version of the game with both encryption and compression support, but without OpenGL rendering support:
```
git clone https://github.com/adepierre/Botcraft.git
cd Botcraft
mkdir build
cd build
cmake -DGAME_VERSION=latest -DBOTCRAFT_BUILD_EXAMPLES=ON -DBOTCRAFT_COMPRESSION=ON -DBOTCRAFT_ENCRYPTION=ON -DBOTCRAFT_USE_OPENGL_GUI=OFF ..
make all
```

At this point, you should have all the examples compiled and ready to run. Plese note that you don't have to clone recursively or download and install the dependencies manually, cmake will automatically take care of these steps based on your build configuration and what is already installed on your machine. On Windows with Visual, you can also use cmake-gui and then compile the .sln directly from Visual.

You can check [this discussion](https://github.com/adepierre/Botcraft/discussions/45#discussioncomment-1142555) for an example of how to use botcraft with your own code.

There are several cmake options you can modify:
- GAME_VERSION [1.XX.X or latest]
- BOTCRAFT_BUILD_EXAMPLES [ON/OFF]
- BOTCRAFT_OUTPUT_DIR [PATH] Base output build path. Binaries, assets and libs will be created in subfolders of this path (default: top project dir)
- BOTCRAFT_COMPRESSION [ON/OFF] Add compression ability, must be ON to connect to a server with compression enabled
- BOTCRAFT_ENCRYPTION [ON/OFF] Add encryption ability, must be ON to connect to a server in online mode
- BOTCRAFT_USE_OPENGL_GUI [ON/OFF] If ON, botcraft will be compiled with the OpenGL GUI enabled
- BOTCRAFT_USE_IMGUI [ON/OFF] If ON, additional information will be displayed on the GUI (need BOTCRAFT_USE_OPENGL_GUI to be ON)
- BOTCRAFT_WINDOWS_BETTER_SLEEP [ON/OFF] If ON, thread sleep durations will be more precise (only for Windows 10/11, no effect on other OS)

## Examples

Examples can be found in the [Examples](Examples/) folder:
- [0_HelloWorld](Examples/0_HelloWorld): Connect to a server, sends Hello World! in the chat then disconnect
- [1_UserControlledExample](Examples/1_UserControlledExample): Best with GUI, mouse and keyboard controlled player. Can be used in a dummy offline world (without any server) to test things like physics or rendering
- [2_ChatCommandExample](Examples/2_ChatCommandExample): Simple bot that obey commands sent through vanilla chat. Known commands at this point:
  - pathfinding
  - disconnecting 
  - checking its sourroundings for spawnable blocks (useful if you want to check whether or not a perimeter is spawn proof)
  - placing a block
  - interacting with a block (lever, button ...)
- [3_SimpleAFKExample](Examples/3_SimpleAFKExample): Very leight AFK only bot. Simply connect to a server and stay still doing nothing.
- [4_MapCreatorExample](Examples/4_MapCreatorExample): Much more complex example, with autonomous behaviour implemented to build a map based pixel art. Can be launched with multiple bot simultaneously. They can share their internal representation of the world to save some RAM, at the cost of slowing down if too many share the same (due to concurrent access). Only extensively tested on 1.16.5, but should work with minor to none adaptation on previous/older versions.
- [5_MobHitterExample](Examples/5_MobHitterExample): Entity processing example. Attack every monster in range, with a per-entity cooldown of 0.5s. /!\ This is only an example about entities, no eating is performed, so would starve to death pretty quickly if used as-is.

## Clients

Botcraft has multiple ``XXXClient`` classes you can inherit from depending on what you want to achieve. For more details, you can check the corresponding [wiki page](https://github.com/adepierre/Botcraft/wiki/Clients).

## Connection

If the server has the option online-mode: false, you can connect with any username.

If the server has the option online-mode: true, you can connect with a Mojang account (login+password) or with a Microsoft account (empty login and password). For Microsoft account, you'll need to authenticate in your browser the first time. Then, credentials are stored in a file so you don't need to relog into your account everytime. Only the access token is saved in the file, Botcraft has never access to your Microsoft account password.

## To-do list

It's only a free time project, but there are still a lot of things to do! I sometimes work on stuff when I need them or when there is something I'd like to learn. Don't expect a detailed roadmap.

There are also some minor improvements to existing features that could be done, but I'm not sure how at the moment:
- Improve scafholding/ladder physics (the player can't climb the ladders)
- Improve pathfinding with real colliders (the pathfinding considers every solid block as box of size 1 but the collisions are computed with the real shapes which lead to bad situations, for example with fences)

## License

GPL v3
