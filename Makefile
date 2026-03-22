main: main.cpp type.cpp
	g++ main.cpp type.cpp -o build/main `pkg-config --cflags --libs gtkmm-4.0`