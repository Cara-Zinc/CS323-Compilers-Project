git submodule init && git submodule update --recursive && sudo -S apt-get install -y flex bison libfl-dev llvm && cd src/frontend/parser && make clean && make && cp structura_parser ../../../gui && cd ../../../gui && npm run build
