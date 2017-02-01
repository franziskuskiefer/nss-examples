CC          := gcc
CCC         := g++
LIBS        := -lfreeblpriv3 -lnspr4 -lnss3  -lnssckbi -lnssdbm3  -lnsssysinit  -lnssutil3  -lplc4  -lplds4  -lsmime3  -lsoftokn3  -lsqlite3 -lssl3
#-lgtest1
CFLAGS      := -g -O2

# change for your architecture and path
ifndef NSS_DIST_PATH
NSS_DIST_PATH=/home/franziskus/Code/dist
endif
ifndef NSS_OBJ_PATH
NSS_OBJ_PATH=Debug
endif
INCLUDES    := -I$(NSS_DIST_PATH)/$(NSS_OBJ_PATH)/include/nspr/ -I$(NSS_DIST_PATH)/private/nss/ -I$(NSS_DIST_PATH)/public/nss/
# -I/usr/lib/modules/4.8.10-1-ARCH/build/arch/x86/include/ -I/usr/lib/modules/4.8.10-1-ARCH/build/include/
INCLUDES    := $(INCLUDES) -I$(NSS_DIST_PATH)/../nss/lib/ssl/ -I$(NSS_DIST_PATH)/../nss/freebl/
LIB_PATH    := -L$(NSS_DIST_PATH)/$(NSS_OBJ_PATH)/lib
STATIC_LIBS := $(NSS_DIST_PATH)/$(NSS_OBJ_PATH)/lib/*.a

all: checkcert dertest certtest b64test listciphers signing ct-cbc

checkcert:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) checkcert.c -o checkcert $(STATIC_LIBS)

certtest:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) certtest.c -o certtest $(STATIC_LIBS)

dertest:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) DER_GetInt_Test.c -o dertest $(STATIC_LIBS)

b64test:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) b64test.c -o b64test $(STATIC_LIBS)

listciphers:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) ciphers.c -o listCiphers $(STATIC_LIBS)

signing:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) signing.c -o signing $(STATIC_LIBS)

ct-cbc:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) constant-time-cbc.c -o constant-time-cbc $(STATIC_LIBS)

randomness:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) randomness-tests.c -o randomness $(STATIC_LIBS)

ct-test:
	$(CC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) ct-tests.c -o ct-test $(STATIC_LIBS)

cpu-asm:
	$(CC) -O2 cpu-cycles.c -S -o cpu-cycles.asm

certprint:
	$(CCC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) certprint.cc -o certprint

quickder:
	$(CCC) $(INCLUDES) $(LIB_PATH) $(LIBS) $(CFLAGS) quickder.cc -o quickder

clean:
	rm -rf checkcert dertest certtest b64test listCiphers signing constant-time-cbc randomness ct-test cpu-cycles.asm certprint quickder
