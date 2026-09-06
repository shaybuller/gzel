CC   = gcc

CFLAGS = -Wall -Wextra -I/usr/local/include -fpic -shared

LDLIBS             = -lxcb -lxcb-xkb -lEGL -lGL -lm
LDLIBS_EXAMPLE     = -lntr -lm -lxcb -lxcb-xkb -lEGL -lGL
LDLIBS_WEB         = -lm

TARGET     = libntr

SRCS = ntr_core.c   \
	   ntr_shapes.c \
	   ntr_fonts.c  \
	   ntr_math.c   \
       ntr_xcb.c    \
	   ntr_xcb_gl.c \
	   ntr_gl.c    
       

all: clear clean so

exm:
	$(CC) -g -Wall -Wextra example.c $(LDLIBS_EXAMPLE) -o example

so:
	$(CC) $(CFLAGS) $(LDLIBS) $(SRCS) -o ../../../lib/$(TARGET).so && \
	echo "Create libntr.so in debpkgs/project/usr/lib..."

clean:
	@if [ -f "../../../lib/libntr.so" ]; then \
		sudo rm -f /usr/lib/libntr.so && \
		echo "Removed old libntr.so from debpkgs/project/usr/lib..."; \
	fi

clear:
	clear

