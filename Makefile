CC          := gcc
LIBS        := -lfreebl3 -lnspr4 -lnss3  -lnssckbi -lnssdbm3  -lnsssysinit  -lnssutil3  -lplc4  -lplds4  -lsmime3  -lsoftokn3  -lsqlite3 -lssl3
CFLAGS      := -g

# change for your architecture and path
ifndef NSS_DIST_PATH
NSS_DIST_PATH=/home/franziskus/Code/dist
endif
ifndef NSS_OBJ_PATH
NSS_OBJ_PATH=Linux4.5_x86_64_cc_glibc_PTH_64_DBG.OBJ
endif
INCLUDES    := -I$(NSS_DIST_PATH)/$(NSS_OBJ_PATH)/include/ -I$(NSS_DIST_PATH)/private/nss/ -I$(NSS_DIST_PATH)/public/nss/
LIB_PATH    := -L$(NSS_DIST_PATH)/$(NSS_OBJ_PATH)/lib
STATIC_LIBS := $(NSS_DIST_PATH)/$(NSS_OBJ_PATH)/lib/*.a

all: checkcert dertest certtest b64test

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

clean:
	rm -rf checkcert dertest certtest b64test listCiphers
