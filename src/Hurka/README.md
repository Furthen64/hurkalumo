
Getting lumo/hurka to work on your computer,

WINDOWS 7

1. Download Code::Blocks 17.12
2. Download SFML source to C:\sfml_src\
3. Build SFML source to C:\sfml\ or something, follow the instructions that includes using cmake-gui and you should be fine
4. Download the "Hurka" source here on github
5. Open up the project Hurka
6. Try to setup the right sfml binaries in project settings, debug and release,
   Make sure you use sfml-system-d.dll for debug and sfml-system.dll for release, this applies to all of them dlls
7. MOve the .dlls from SFML to C:\hurka\ or where you put it
8. "Hurka" should now startup!

