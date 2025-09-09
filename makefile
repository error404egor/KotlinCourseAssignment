CXX = c++
CXXFLAGS = -std=c++23 -Wall -Wextra
SOURCES = main.cpp users.cpp app_time.cpp book.cpp library_manager.cpp


ifeq ($(OS),Windows_NT)
    TARGET = main.exe
    RM = del /Q
else
    TARGET = main
    RM = rm -f
endif

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clear:
	$(RM) $(TARGET)

.PHONY: clear
