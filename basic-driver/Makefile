CC = gcc
CFLAGS = -Wextra -Wall -Werror
LFLAGS = 

LIBS = 
OUT = ledborg-basic
SRC = ledborg-gpio.c
OBJ = $(SRC:.c=.o)

default:	all

rebuild:	clean all

clean:
	rm *.o *~ $(OUT)

all:	$(OUT)
	chmod a+x $(OUT)
	cp $(OUT) ../$(OUT)

$(OUT):	$(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUT) $(OBJ) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
