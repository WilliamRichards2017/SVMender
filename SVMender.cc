#include "GSSWGraph.h"
#include "gssw.h"
#include <string>


static uint32_t test_match = 1;
static uint32_t test_mismatch = 4;

void test_graph() {

  int8_t* nt_table = gssw_create_nt_table();
  int8_t* mat = gssw_create_score_matrix(test_match, test_mismatch);

  std::string alignment = "ACGT";
  std::cout << alignment << std::endl;
}

int main() {
  test_graph();
}
