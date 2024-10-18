.PHONY: build
.PHONY: run
.PHONY: clean

build:
	if [ ! -f build/Makefile ]; then \
		mkdir build; \
		cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug; \
	fi
	cd build && $(MAKE)

run: build
	./build/p2nprobe

clean:
	rm -r build obj
