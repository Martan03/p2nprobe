.PHONY: build
build:
	if [ ! -f build/Makefile ]; then \
		mkdir build; \
		cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug; \
	fi
	cd build && $(MAKE) -j 12
	mv build/n2tcap .

.PHONY: run
run: build
	./n2tcap

.PHONY: clean
clean:
	rm -r build obj n2tcap
