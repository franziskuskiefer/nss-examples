/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


#include "blapi.h"
#include "blapit.h"
#include "ecl-curve.h"
#include "nspr.h"
#include "nss.h"
#include "prerror.h"
#include "secdert.h"
#include "secitem.h"

static SECItem *hexString2SECItem(PLArenaPool *arena, SECItem *item,
                                  const char *str) {
  int i = 0;
  int byteval = 0;
  int tmp = PORT_Strlen(str);

  if ((tmp % 2) != 0) {
    return NULL;
  }

  /* skip leading 00's unless the hex string is "00" */
  while ((tmp > 2) && (str[0] == '0') && (str[1] == '0')) {
    str += 2;
    tmp -= 2;
  }

  item = SECITEM_AllocItem(arena, item, tmp / 2);
  if (item == NULL) {
    return NULL;
  }

  while (str[i]) {
    if ((str[i] >= '0') && (str[i] <= '9'))
      tmp = str[i] - '0';
    else if ((str[i] >= 'a') && (str[i] <= 'f'))
      tmp = str[i] - 'a' + 10;
    else if ((str[i] >= 'A') && (str[i] <= 'F'))
      tmp = str[i] - 'A' + 10;
    else
      return NULL;

    byteval = byteval * 16 + tmp;
    if ((i % 2) != 0) {
      item->data[i / 2] = byteval;
      byteval = 0;
    }
    i++;
  }

  return item;
}

void print(char *buf, int x) {
  int i;
  for (i = 0; i < x; i++) {
    if (i > 0) {
      printf(":");
    }
    printf("%08X", buf[i]);
  }
  printf("\n");
}

int main(int argc, char **argv) {
  if (NSS_NoDB_Init(NULL) != SECSuccess) {
    printf(" >>> NSS_NoDB_Init() failed.\n");
    return 1;
  }

  PLArenaPool *arena;

  ECPrivateKey *sk = NULL;
  ECPublicKey pk;
  ECParams ecParams = {0};
  ECCurveName curve = ECCurve_NIST_P521;
  char genenc[3 + 2 * 2 * MAX_ECKEY_LEN];

  SECItem signature = {0};
  SECItem digest = {0};

  arena = PORT_NewArena(DER_DEFAULT_CHUNKSIZE);
  if (!arena) {
    goto cleanup;
  }

  /* some digest */
  hexString2SECItem(arena, &digest, "1234567890");

  /* fill ecParams */
  ecParams.name = curve;
  ecParams.type = ec_params_named;
  ecParams.curveOID.data = NULL;
  ecParams.curveOID.len = 0;
  ecParams.curve.seed.data = NULL;
  ecParams.curve.seed.len = 0;
  ecParams.DEREncoding.data = NULL;
  ecParams.DEREncoding.len = 0;

  ecParams.fieldID.size = ecCurve_map[curve]->size;
  ecParams.fieldID.type = ec_field_GFp;
  hexString2SECItem(arena, &ecParams.fieldID.u.prime, ecCurve_map[curve]->irr);
  hexString2SECItem(arena, &ecParams.curve.a, ecCurve_map[curve]->curvea);
  hexString2SECItem(arena, &ecParams.curve.b, ecCurve_map[curve]->curveb);
  genenc[0] = '0';
  genenc[1] = '4';
  genenc[2] = '\0';
  strcat(genenc, ecCurve_map[curve]->genx);
  strcat(genenc, ecCurve_map[curve]->geny);
  hexString2SECItem(arena, &ecParams.base, genenc);
  hexString2SECItem(arena, &ecParams.order, ecCurve_map[curve]->order);
  ecParams.cofactor = ecCurve_map[curve]->cofactor;

  /* create a key pair */
  if (EC_NewKey(&ecParams, &sk) != SECSuccess) {
    printf(" EC_NewKey failed!\n");
    goto cleanup;
  }
  pk.ecParams = ecParams;
  pk.publicValue = sk->publicValue;

  /* sign */
  SECITEM_AllocItem(arena, &signature, 2*ecParams.order.len);
  // printf(" >> Signing: ");
  // print(digest.data, digest.len);
  if (ECDSA_SignDigest(sk, &signature, &digest) != SECSuccess) {
    printf(" ECDSA_SignDigest failed!\n");
    goto cleanup;
  }
  // printf(" >> Got signature: ");
  // print(signature.data, signature.len);

  /* verify */
  if (ECDSA_VerifyDigest(&pk, &signature, &digest) != SECSuccess) {
    printf(" ECDSA_VerifyDigest failed!\n");
    goto cleanup;
  }

  printf(" >>> Signature Verified!.\n");

cleanup:
  if (arena) {
    PORT_FreeArena(arena, PR_FALSE);
  }
  if (sk->ecParams.arena) {
    PORT_FreeArena(sk->ecParams.arena, PR_FALSE);
  }

  return NSS_Shutdown();
}
