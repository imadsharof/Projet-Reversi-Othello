# Détection de l'OS
ifeq ($(OS),Windows_NT)
	RM = del /Q
	EXECUTABLE_EXTENSION = .exe
	# Options de liaison pour Windows
else
	RM = rm -f
	EXECUTABLE_EXTENSION =
	# Options de liaison pour Linux
endif

# Noms des exécutables pour chaque version
PROJETPRELIM = projetprelim$(EXECUTABLE_EXTENSION)
PROJET = projet$(EXECUTABLE_EXTENSION)
PROJETGRAPH = projetgraph$(EXECUTABLE_EXTENSION)

# Listes des fichiers sources et objets
SRCS = main.cpp Analyser.cpp Board.cpp Player.cpp IA.cpp Humain.cpp Fechier.cpp Game.cpp
OBJS = $(SRCS:.cpp=.o)

# Options du compilateur
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Règles de compilation
all: $(PROJETPRELIM)

$(PROJETPRELIM): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(PROJETPRELIM) $(LDFLAGS)

$(PROJET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(PROJET) $(LDFLAGS)

$(PROJETGRAPH): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(PROJETGRAPH) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et des exécutables
clean:
	$(RM) $(OBJS) $(PROJETPRELIM) $(PROJET) $(PROJETGRAPH)
