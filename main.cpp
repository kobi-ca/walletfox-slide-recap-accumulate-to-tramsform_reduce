#include <iostream>
#include <variant>
#include <numbers> // pi_v
#include <vector>
#include <numeric> // transform_reduce

namespace {
    struct Rect final {
        Rect(double aa, double bb) : a_(aa), b_(bb) {}
        double a_{};
        double b_{};
    };

    struct SemiCircle final {
        explicit SemiCircle(double rr) : r_(rr) {}
        double r_{};
    };

    using var_shape = std::variant<Rect, SemiCircle>;

    // from cppreference
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    [[nodiscard]]
    auto area(const var_shape& v) {
        return std::visit(overloaded{
            [](const Rect& r){ return r.a_ * r.b_; },
            [](const SemiCircle& c){ return 0.5 * std::numbers::pi_v<double>*c.r_*c.r_; }
        }, v);
    }

}

int main() {
    // using {} for ctor init
    // https://stackoverflow.com/questions/15396124/calling-constructor-with-braces
    // It is uniform initialization and, in Standardese terms, direct-brace-initialization.
    // Unless a constructor accepting an std::initializer_list is present, using braces for
    // constructing objects is equivalent to using parentheses.
    const std::vector<var_shape> vec = {Rect(7,5), SemiCircle{2.5}, SemiCircle{3.5}, Rect{8,9}};

    // Today, using ADL for everyting!
    const auto total = transform_reduce(begin(vec), end(vec),
                                        0.0, // init
                                        [](const double sum, const double d){ return sum+d; }, // reduce
                                        // could also be auto
                                        [](const var_shape& s){ return area(s); }); // transform

    std::clog << total << '\n';
    return 0;
}
