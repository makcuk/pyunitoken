pyunitoken:
# MacOS X
	clang++  -Wall -g -o pyunitoken.so src/pyunitoken.cpp UniToken/UniToken.so -framework CoreFoundation -framework IOKit -framework Python -I /System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7/ -I UniToken/ -I src/ -shared -arch x86_64 
clean:
	rm pyunitoken.so

test:
	python test_pyunitoken.py
