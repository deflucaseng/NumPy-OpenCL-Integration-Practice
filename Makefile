.PHONY: build clean run full

build:
	sudo pip install -e .


clean:
	sudo rm -rf build/
	sudo rm -rf *.egg-info/
	sudo rm -rf matrix_opencl/core/*.so

run:
	sudo python3 -m unittest test.py

full:
	sudo rm -rf build/
	sudo rm -rf *.egg-info/
	sudo rm -rf matrix_opencl/core/*.so
	sudo pip install -e .

	sudo python3 -m unittest test.py