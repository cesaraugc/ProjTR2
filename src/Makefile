IDIR = header
CC = gcc
CFLAGS = -I $(IDIR) -Wall

ODIR = obj

_DEPS = connection.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o receive.o request.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

aracne: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
