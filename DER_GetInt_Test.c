#include "secutil.h"
#include "plgetopt.h"
#include "cert.h"
#include "secoid.h"
#include "cryptohi.h"
#include "nss.h"

int main(int argc, char** argv) {

    NSS_NoDB_Init("");

    SECItem der;
    unsigned char num[] = {0x82};
    der.len = 1;
    der.data = &num[0];
    int i = DER_GetInteger(&der);
    printf("parsed DER int is %d\n", i);

    return 0;
}