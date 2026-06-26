# frostbyte

This repo contains the engine module for [frostbyte](https://github.com/frostbyte-engine).

# PROJECT STATE

This project is in early development stages! This is why there is no Windows support!

In addition, I am frequently making drastic changes on my local machine before pushing to GitHub (I bounce back and forth between areas) so the state of the project rarely matches what is public.

[Issues](../../issues), however, usually closely match the project's real state.

# BUILDING THE MODULE
NOTE: frostbyte CURRENTLY does _not_ have a process for building neither for or on Windows. It is likely possible to cross compile via mingw, but that would require manual steps.

THIS SECTION IS UNDER CONSTRUCTION! I AM TOO LAZY TO TYPE PROPER STEPS BUT YOU CAN READ [this build script](./buildwrap.sh) FOR MORE SOME IDEAS

ALSO SEE THE [workflow file](./github/workflows/build-frostbyte-action.yml) FOR SYSTEM DEPENDENCY INFORMATION

# LUAU
frostbyte embeds [Luau](https://github.com/luau-lang/luau). See [luau_LICENSE.txt](luau_LICENSE.txt) for licensing information.
<br>
![](repoassets/luau.png)
