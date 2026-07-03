set zipfile=lib.rar
set libfile=assimp-vc143-mt.lib
cd ..\..\externals\assimp\lib
if not exist %libfile% tar -xf %zipfile% 
exit 0


