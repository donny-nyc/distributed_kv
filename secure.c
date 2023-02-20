#include <openssl/ssl.h>
#include <openssl/err.h>

int main(void) {
  ERR_print_errors_fp(1);

  return 0;
}
