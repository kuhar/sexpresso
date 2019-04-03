// Author: Isak Andersson 2016 bitpuffin dot com

#ifndef SEXPRESSO_HEADER
#define SEXPRESSO_HEADER
#include <cstdint>
#include <string>
#include <vector>

namespace sexpresso {
enum class SexpValueKind : uint8_t { SEXP, STRING };

struct SexpArgumentRange;

struct Sexp {
  Sexp();
  Sexp(std::string const &strval);
  Sexp(std::vector<Sexp> const &sexpval);

  Sexp(const Sexp &other) = default;
  Sexp(Sexp &&other) = default;

  Sexp &operator=(const Sexp &other) = default;
  Sexp &operator=(Sexp &&other) = default;

  SexpValueKind kind;
  struct {
    std::vector<Sexp> sexp;
    std::string str;
  } value;
  auto addChild(Sexp sexp) -> void;
  auto addChild(std::string str) -> void;
  auto addChildUnescaped(std::string str) -> void;
  auto addExpression(std::string const &str) -> void;
  auto getHead() -> Sexp &;
  auto childCount() const -> size_t;
  auto getChild(size_t idx) -> Sexp &; // Call only if Sexp is a Sexp
  auto getString() -> std::string &;
  auto getChildByPath(std::string const &path)
      -> Sexp *; // unsafe! careful to not have the result pointer outlive the
                 // scope of the Sexp object
  auto createPath(std::vector<std::string> const &path) -> Sexp &;
  auto createPath(std::string const &path) -> Sexp &;
  auto toString() const -> std::string;
  auto isString() const -> bool;
  auto isSexp() const -> bool;
  auto isNil() const -> bool;
  auto isNumber() const -> bool;
  auto toNumber() const -> long long;
  auto equal(Sexp const &other) const -> bool;
  auto arguments() -> SexpArgumentRange;
  static auto unescaped(std::string strval) -> Sexp;

  template <typename OS> friend OS &operator<<(OS &os, const Sexp &sexp) {
    return (os << sexp.toString());
  }
};

auto parse(std::string const &str, std::string &err) -> Sexp;
auto parse(std::string const &str) -> Sexp;
auto escape(std::string const &str) -> std::string;

struct SexpArgumentRange {
  SexpArgumentRange(Sexp &sexp);
  Sexp &sexp;

  using iterator = std::vector<Sexp>::iterator;
  using const_iterator = std::vector<Sexp>::const_iterator;

  auto begin() -> iterator;
  auto end() -> iterator;
  auto begin() const -> const_iterator;
  auto end() const -> const_iterator;
  auto size() const -> size_t;
  auto empty() const -> bool;
};

struct SexpPostOrderView {
  Sexp *sexp;
  Sexp *parent;

  template <typename OS>
  friend OS &operator<<(OS &os, const SexpPostOrderView &view) {
    os << "elem: ";
    if (view.sexp)
      os << *view.sexp;
    else
      os << "nullptr";
    os << ", parent: " << view.parent
       << ", head: " << view.getHead();

    return os;
  }

  bool isHead() const;
  Sexp &getHead();
  const Sexp &getHead() const;
};

class SexpPostOrderRange {
  using WorkListTy = std::vector<SexpPostOrderView>;
  WorkListTy worklist;

public:
  using iterator = WorkListTy::reverse_iterator;

  SexpPostOrderRange(Sexp &sexp);

  auto begin() -> iterator { return worklist.rbegin(); }
  auto end() -> iterator { return worklist.rend(); }
  auto size() const -> size_t { return worklist.size(); }
  auto empty() const -> bool { return worklist.empty(); }
};

} // namespace sexpresso

#endif // SEXPRESSO_HEADER
