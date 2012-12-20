ROOT="../.."
SRC="$ROOT/src"
FILES="$SRC/client/*.cpp $SRC/core/*.cpp $SRC/graphics/*.cpp $SRC/input/*.cpp $SRC/world/*.cpp $SRC/opengl/*.cpp"
INCLUDE="-I$ROOT/src -I$ROOT/lib/ -I$ROOT/lib/SFML/include"
LIB="-L$ROOT/lib/SFML/lib"
LINK="-lsfml-graphics-s -lsfml-window-s -lsfml-system-s"
OUT="$ROOT/game.exe"
DEFINES="-DSFML_STATIC"

"C:/MinGW/bin/g++.exe" -O3 -std=c++11 -g -Wall -o$OUT $INCLUDE $LIB $DEFINES $FILES $LINK 
