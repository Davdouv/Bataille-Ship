#
# Battleship 
# (il faut que dans OBJECTS il y ait au moins un objet de même nom que TARGET.
#
TARGET= main

#
# Mettre ici l'ensemble des fichiers objets nécessaires aux programmes 
# (les fichiers peuvent être situés dans des répertoires)
#
OBJECTS=  main.o functions.o menu.o init.o placement.o attack.o

#
# Mettre ici vos options de compilation.
#
CFLAGS= -DDEBUG -g -Wall `pkg-config --cflags MLV`

#
# Mettre ici les options qui devront être passées à l'éditeur de liens.
# Par exemple : -L/directory
#
LDFLAGS = `pkg-config --libs-only-other --libs-only-L MLV`

#
# Mettre ici les noms des bibliothèques et leurs locations.
# Par exemple : -lpthread
#
LDLIBS=`pkg-config --libs-only-l MLV`

 
###############################################################################

#DEPENDS=$(OBJECTS:.o=.d)
INCLUDE_DEPENDS=$(foreach i,$(DEPENDS),$(wildcard $(i)))
 
all: $(TARGET)
 
$(TARGET): $(DEPENDS) $(OBJECTS) ; $(CC) -o $@ $(LDFLAGS) $(OBJECTS) $(LDLIBS)

%.d:%.c ; $(CC) -M $(CFLAGS) -MT '$(shell echo "$&lt;" |sed 's,\($*\)\.c,\1.o $@,' )'  < > $@
 
include $(INCLUDE_DEPENDS)
 
clean: ; rm -f $(TARGET) ; rm -f $(OBJECTS) ; rm -f $(DEPENDS)