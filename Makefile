.PHONY: build clean run

build:
	sudo pip install -e .


clean:
	sudo rm -rf build/
	sudo rm -rf *.egg-info/
	sudo rm -rf matrix_opencl/core/*.so

run:
	sudo python3 test.py