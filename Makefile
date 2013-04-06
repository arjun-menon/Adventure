TARGET := Game.run
OBJDIR := obj
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix $(OBJDIR)/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
CC_FLAGS := -std=gnu++0x -O3
CC := g++-4.7

$(TARGET): $(OBJDIR) objects
	

objects: $(OBJ_FILES)
	$(CC) $(CC_FLAGS) -o $(TARGET) $^ $(LD_FLAGS)

$(OBJDIR)/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	@rm -f $(OBJDIR)/*.o
	@rm -f $(TARGET)
	@rmdir $(OBJDIR)
