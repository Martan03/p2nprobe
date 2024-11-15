#!/usr/bin/bash

login=xsleza26

mkdir $login

cd docs
make
cd ..
cp docs/manual.pdf $login

cp -r src CMakeLists.txt README $login

cd $login
cat >./Makefile <<EOL
.PHONY: build
.PHONY: run
.PHONY: clean

build:
	if [ ! -f build/Makefile ]; then \\
		mkdir build; \\
		cd build && cmake .. -DCMAKE_BUILD_TYPE=Release \\
			-DCMAKE_CXX_COMPILER=g++-14.2; \\
	fi
	cd build && \$(MAKE) -j 12
	mv build/p2nprobe .

run: build
	./p2nprobe

clean:
	rm -r build obj p2nprobe
EOL

echo 'target_link_libraries(p2nprobe PRIVATE -static-libstdc++)' \
    >> CMakeLists.txt

tree >> README
