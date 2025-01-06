cd ../src/frontend/parser
make clean
make
cd ../../../gui
cp ../src/frontend/parser/structura_parser .
cnpm run dev