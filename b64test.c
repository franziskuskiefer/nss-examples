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
#include "nssb64.h"
#include <stdbool.h>

const char b641[] = "IyBuc3MtZXhhbXBsZXMKUmVwbyB3aXRoIGV4YW1wbGUgY29kZSBhbmQgZG9jdW1lbnRhdGlvbiBmb3IgW05TU10oaHR0cHM6Ly9naXRodWIuY29tL25zcy1kZXYvbnNzKQo=";
const char b642[] = "IyBuc3MtZXhhbXBsZXMKUmVwbyB3aXRoIGV4YW1wbGUgY29kZSBhbmQgZG9jdW1lbnRhdGlvbiBmb3IgW05TU10oaHR0cHM6Ly9naXRodWIuY29tL25zcy1kZXYvbnNzKQo";
const char s1[] = "# nss-examples\nRepo with example code and documentation for [NSS](https://github.com/nss-dev/nss)\n";

const char b643[] = "TW9yZSBOU1MgYjY0IHRlc3RpbmcgISLCpyQlJi8oKT0/fjw+wrnCssKzCg==";
const char b644[] = "TW9yZSBOU1MgYjY0IHRlc3RpbmcgISLCpyQlJi8oKT0/fjw+wrnCssKzCg";
const char b645[] = "TW9yZSBOU1MgYjY0IHRlc3RpbmcgISLCpyQlJi8oKT0_fjw-wrnCssKzCg==";
const char b646[] = "TW9yZSBOU1MgYjY0IHRlc3RpbmcgISLCpyQlJi8oKT0_fjw-wrnCssKzCg";
const char s2[] = "More NSS b64 testing !\"§$\45&/()=?~<>¹²³\n";

void check(PLArenaPool *arena, const char b64[], const char s[], const char sName[], bool url) {
  void *mark = NULL;
  SECItem *b64Item;
  mark = PORT_ArenaMark(arena);
  b64Item = SECITEM_AllocItem(arena, NULL, strlen(b64));
  if(b64Item == NULL){
    printf(">>> Couldn't allocate memore, SECITEM_AllocItem failed.\n");
  }
  strncpy(b64Item->data, b64, b64Item->len);

  SECItem b64Decoded;
  PORT_Memset(&b64Decoded, 0, sizeof(b64Decoded));
  if (url) {
    if (!NSSBase64_URL_DecodeBuffer(arena, &b64Decoded,
                                    b64Item->data,
                                    b64Item->len)) {
      PORT_FreeArena(arena, 0);
      printf(">>> Couldn't b64decode, NSSBase64_DecodeBuffer failed (URL).\n");
      return;
    }
  } else {
    if (!NSSBase64_DecodeBuffer(arena, &b64Decoded,
                                b64Item->data,
                                b64Item->len)) {
      PORT_FreeArena(arena, 0);
      printf(">>> Couldn't b64decode, NSSBase64_DecodeBuffer failed.\n");
      return;
    }
  }
  if (strcmp(s,b64Decoded.data) != 0) {
    printf(">>> DECODING ERROR (%s):/\n>>> expected:\n%s\ngot:\n", sName, s);
    printf("%.*s---\n---", b64Decoded.len, b64Decoded.data);
  } else {
    printf("Successful decoded %s string\n", sName);
  }
  PORT_ArenaUnmark(arena, mark);
}

int main(int argc, char **argv) {
  if (NSS_NoDB_Init(NULL) != SECSuccess) {
    printf(" >>> NSS_NoDB_Init() failed.\n");
    return 1;
  }

  PLArenaPool *arena;
  arena = PORT_NewArena(DER_DEFAULT_CHUNKSIZE);
  if (!arena) {
    printf(">>> OUT OF MEMORY :(\n");
    return 1;
  }

  check(arena, b641, s1, "b641-s1", false);
  check(arena, b642, s1, "b642-s1", false);
  check(arena, b643, s2, "b643-s2", false);
  check(arena, b644, s2, "b644-s2", false);
  check(arena, b645, s2, "b645-s2", true);
  check(arena, b646, s2, "b646-s2", true);

  PORT_FreeArena(arena, 0);
  printf("!!! Done.\n");
  return 0;
}
