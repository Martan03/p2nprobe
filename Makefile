login:=xsleza26

.PHONY: build
build:
	if [ ! -f build/Makefile ]; then \
		mkdir build; \
		cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug; \
	fi
	cd build && $(MAKE) -j 12
	mv build/p2nprobe .

.PHONY: n2tcap
n2tprobe:
	cd n2tcap && make

.PHONY: run
run: build
	./p2nprobe

.PHONY: docs
docs:
	cd docs && make

.PHONY: submit
submit:
	./submit.sh
	tar -cvf $(login).tar $(login)

.PHONY: clean
clean:
	cd docs && make clean
	rm -r build obj $(login) $(login).tar p2nprobe
