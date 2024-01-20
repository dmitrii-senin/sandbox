#include <utility>

#if __cplusplus < 202002L
#error "C++20 or above is required"
#endif

namespace yatl {

struct guarantor {
  struct guaranteed_not_null {};
};

template <typename T>
class uptr {
 public:
  uptr() = delete;
  uptr(std::nullptr_t) = delete;
  uptr(const uptr&) = delete;
  uptr& operator=(const uptr&) = delete;

  constexpr uptr(T* ptr, guarantor::guaranteed_not_null /* tag */) noexcept
      : ptr_{ptr} {}

  constexpr ~uptr() { reset(); }

  constexpr uptr(uptr&& other) noexcept
      : ptr_{std::exchange(other.ptr_, nullptr)} {}

  constexpr uptr& operator=(uptr&& other) {
    reset();
    ptr_ = std::exchange(other.ptr_, nullptr);
  }

  constexpr void swap(uptr& other) noexcept {
    using std::swap;
    swap(ptr_, other.ptr_);
  }

  T& operator*() const noexcept { return *ptr_; }
  const T* operator->() const noexcept { return ptr_; }

 private:
  constexpr void reset() {
    delete ptr_;
    ptr_ = nullptr;
  }

  T* ptr_ = nullptr;
};

template <typename T, typename... TArgs>
constexpr make_uptr(TArgs&&... args) {
  return uptr<T>{new T(std::forward<TArgs>(args)...),
                 guarantor::guaranteed_not_null{}};
}

}  // namespace yatl
