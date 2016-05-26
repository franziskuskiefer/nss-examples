/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "secutil.h"
#include "plgetopt.h"
#include "cert.h"
#include "secoid.h"
#include "cryptohi.h"
#include "nspr.h"
#include "prio.h"
#include "prnetdb.h"
#include "prerror.h"
#include "nss.h"
#include "pk11pub.h"

int main(int argc, char **argv)
{
  if (NSS_NoDB_Init(NULL) != SECSuccess) {
    printf(" >>> NSS_NoDB_Init() failed.\n");
    return 1;
  }

  if (argc < 3) {
    printf(" >>> I need a DER encoded file to read and have to know what to do with it [decode, private]!\n");
    return 1;
  }

  PRFileDesc* file = PR_Open(argv[1], PR_RDONLY, 0);

  SECItem data = {0, NULL, 0};
  if (SECU_ReadDERFromFile(&data, file, PR_FALSE, PR_FALSE) != SECSuccess) {
    printf(" >>> SECU_ReadDERFromFile() failed.\n");
    return 1;
  }
  PR_Close(file);

  if (strcmp(argv[2], "decode") == 0) {
    CERTCertificate *cert = CERT_DecodeCertFromPackage((char*)data.data, data.len);
    if (cert){
      printf(" >>> read cert!\n");
      printf(" >>> SN: %s\n", cert->subjectName);
      printf(" >>> IN: %s\n", cert->issuerName);
      CERT_DestroyCertificate(cert);
    } else {
      printf(" >>> CERT_DecodeCertFromPackage failed.\n");
      SECITEM_FreeItem(&data, PR_FALSE);
      return 1;
    }
  }

  if (argv[2] == "private") {
    PK11SlotInfo* slot = PK11_GetInternalSlot();
    if (!slot) {
      printf(" >>> PK11_GetInternalSlot() failed.\n");
      SECITEM_FreeItem(&data, PR_FALSE);
      return 1;
    }

    SECKEYPrivateKey* privKey;
    if (PK11_ImportDERPrivateKeyInfoAndReturnKey(slot, &data, NULL, NULL, PR_FALSE, PR_FALSE, KU_ALL, &privKey, NULL) != SECSuccess) {
      printf(" >>> PK11_ImportDERPrivateKeyInfoAndReturnKey() failed.\n");
      SECITEM_FreeItem(&data, PR_FALSE);
      return 1;
    }
  }

  printf(" !!! Done.\n");
  return 0;
}
