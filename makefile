

CXX = c++
CXXFLAGS = -std=c++23 -Wall -Wextra
TARGET = main
SOURCES = main.cpp users.cpp app_time.cpp book.cpp library_manager.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clear:
	rm -f $(TARGET)

.PHONY: clear
