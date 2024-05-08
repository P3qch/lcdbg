CC = g++
CFLAGS = -c -ggdb
LDFLAGS =
SRCDIR = sources
INCDIR = headers
OBJDIR = objects
EXE = lcdbg

SOURCES=$(wildcard $(SRCDIR)/*.cpp)
HEADERS=$(wildcard $(INCDIR)/*.hpp)
OBJECTS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -rf $(OBJDIR)
