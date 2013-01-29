# Makefile for a game using C++11 and SFML

# List *.cpp files in src here:
CPP_SOURCES = BaseFramework.cpp

TARGET = Adventure
SRCDIR = src
OBJDIR = build

CC = g++
CFLAGS = -std=c++11 -g
LIBS = -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

OBJS = $(OBJDIR)/$(CPP_SOURCES:.cpp=.o)

all: $(TARGET)
	

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	@rm -f $(OBJDIR)/*.o
	@rm -f $(TARGET)
	@rmdir $(OBJDIR)
