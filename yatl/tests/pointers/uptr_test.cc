#include <gtest/gtest.h>

#include "yatl/pointers/uptr.h"

namespace yatl::tests {

TEST(UptrTest, GuaranteedNotNullCtor) {
  const int* raw_ptr = new int{42};
  const uptr p{raw_ptr, guarantor::guaranteed_not_null{}};
  EXPECT_EQ(&(*p), raw_ptr);
}

}  // namespace yatl::tests
