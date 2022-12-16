all: build test

build:
	cmake -DCMAKE_BUILD_TYPE=Debug -Bbuild -S.
	make -Cbuild

test:
	python -m pytest tests/test.py -vv

clean:
	rm -rf build
