#include <nspr.h>
#include <nss.h>
#include <ssl.h>

int main(void) {
  const PRUint16 *ciphers;
  size_t i, num_ciphers;

  PR_Init(PR_USER_THREAD, PR_PRIORITY_NORMAL, 0);
  if (NSS_NoDB_Init (NULL) != SECSuccess) {
    printf("Error initialising NSS");
    return 1;
  }

  ciphers = SSL_GetImplementedCiphers ();
  num_ciphers = SSL_GetNumImplementedCiphers ();
  for (i = 0; i < num_ciphers; i++) {
    SSLCipherSuiteInfo info;

    if (SSL_GetCipherSuiteInfo(ciphers[i], &info, sizeof(info)) != SECSuccess) {
      printf("Error in SSL_GetCipherSuiteInfo");
      return 1;
    }
    if (info.length < sizeof (info)) {
      printf("Unexpected SSLCipherSuiteInfo length\n");
      return 1;
    }
    printf("%-50s %-10s %-10s %-20s %-10s\n", info.cipherSuiteName,
           info.authAlgorithmName, info.keaTypeName, info.symCipherName,
           info.macAlgorithmName);
  }
  return 0;
}
