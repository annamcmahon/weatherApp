STD = -std=c++0x
SDLFLG = `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf
all: main.o WeatherApiClass.o sdlClass.o renderableImage.o
	g++ $(STD) main.o WeatherApiClass.o sdlClass.o renderableImage.o $(SDLFLG)
main.o:
	g++ $(STD) -c main.cpp $(SDLFLG)
WeatherApiClass.o:
	g++ -c WeatherApiClass.cpp
sdlClass.o:
	g++ $(STD) -c sdlClass.cpp $(SDLFLG)
renderableImage.o:
	g++ $(STD) -c renderableImage.cpp $(SDLFLG)
.PHONY: clean
clean:
	rm -f *.o all 
