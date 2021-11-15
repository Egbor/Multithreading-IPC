CC = gcc

SRCDIR = src/
OBJDIR = obj/
OUTDIR = bin/

SOURCES = $(wildcard $(SRCDIR)*.c)
OUTNAME = multithreading-ipc

all: dirs $(patsubst %.c,%.o,$(SOURCES))
	$(CC) -o $(OUTDIR)$(OUTNAME).out $(SRCDIR)$(OBJDIR)*.o -lpthread
clean:
	rm -r -f $(OUTDIR)
	rm -r -f $(SRCDIR)$(OBJDIR)
dirs:
	mkdir -p $(OUTDIR)
	mkdir -p $(SRCDIR)$(OBJDIR)
%.o: %.c
	$(CC) -c -MD $< -g -o $(@D)/$(OBJDIR)$(@F)

include $(wildcard $(SRCDIR)$(OBJDIR)*.d)
