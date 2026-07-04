set zipfile=lib.rar
set libfile=assimp-vc143-mt.lib
cd ..\..\..\VedaLib\externals\assimp\lib
if not exist %libfile% tar -xf %zipfile% 
exit 0


