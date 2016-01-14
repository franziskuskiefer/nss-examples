
CC          := gcc
LIBS        := -lfreebl3 -lnspr4 -lnss3  -lnssckbi -lnssdbm3  -lnsssysinit  -lnssutil3  -lplc4  -lplds4  -lsmime3  -lsoftokn3  -lsqlite3 -lssl3
CFLAGS      := -g

# change for your architecture and path
INCLUDES    := -I/home/franziskus/Code/dist/Linux4.2_x86_64_cc_glibc_PTH_64_DBG.OBJ/include/ -I/home/franziskus/Code/dist/private/nss/ -I/home/franziskus/Code/dist/public/nss/
LIB_PATH    := -L/home/franziskus/Code/dist/Linux4.2_x86_64_cc_glibc_PTH_64_DBG.OBJ/lib
STATIC_LIBS := /home/franziskus/Code/dist/Linux4.2_x86_64_cc_glibc_PTH_64_DBG.OBJ/lib/*.a

all: checkcert dertest certtest b64test

checkcert:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) checkcert.c -o checkcert $(STATIC_LIBS)

certtest:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) certtest.c -o certtest $(STATIC_LIBS)

dertest:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) DER_GetInt_Test.c -o dertest $(STATIC_LIBS)

b64test:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) b64test.c -o b64test $(STATIC_LIBS)

clean:
	rm -rf checkcert dertest certtest b64test
