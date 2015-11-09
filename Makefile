
CC          := gcc
LIBS        := -lfreebl3 -lnspr4 -lnss3  -lnssckbi -lnssdbm3  -lnsssysinit  -lnssutil3  -lplc4  -lplds4  -lsmime3  -lsoftokn3  -lsqlite3 -lssl3
CFLAGS      := -g

# change for your architecture and path
INCLUDES    := -I/home/franziskus/Code/dist/Linux4.2_x86_64_cc_glibc_PTH_64_DBG.OBJ/include/ -I/home/franziskus/Code/dist/private/nss/ -I/home/franziskus/Code/dist/public/nss/
LIB_PATH    := -L/home/franziskus/Code/dist/Linux4.2_x86_64_cc_glibc_PTH_64_DBG.OBJ/lib
STATIC_LIBS := /home/franziskus/Code/dist/Linux4.2_x86_64_cc_glibc_PTH_64_DBG.OBJ/lib/*.a

all: checkcert

checkcert:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) checkcert.c -o checkcert $(STATIC_LIBS)

clean:
	rm -rf checkcert