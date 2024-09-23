/* If you are unsatisfied with the existing code, modify as you want. */
#include <cstdint>
#include <limits>
#include <memory>
/* Add other headers here... */

#define fn  auto
#define let auto

/*!
    See [Enumeration declaration - cppreference.com](https://en.cppreference.com/w/cpp/language/enum) for usagel
    of enumerations.
*/
enum class Op : std::uint32_t {
    invalid = 0,
    binary_plus,
    binary_minus,
};
struct EOS {
    let static constexpr value = ';';
};

struct Token {
    enum class Kind : std::uint32_t {
        end_of_statement = std::numeric_limits<std::uint32_t>::max(), /// `EOS` for short of `end_of_statement`
        invalid = 0,
        integer,
        op,
    };
    union Value {
        std::int32_t integer;
        Op op;
        [[no_unique_address]]
        EOS end_of_statement;
        ~Value() {}
    };

    Kind kind;
    Value value;

    /*!
        Use following template to generate output with std::format().
        See [std::format - cppreference.com](https://en.cppreference.com/w/cpp/utility/format/format) for doc.
    */
    let static constexpr fmt_template = 
        "\"{}\": {{" "\n"
        "   \"variable_type\": \"{}\"," "\n"
        "   \"kind\": \"{}\"," "\n"
        "   \"value\": \"{}\"" "\n"
        "}}";

    ~Token()
    {
        switch (this->kind) {
        case Kind::invalid:
            std::destroy_at(std::addressof(this->value));
            break;
        case Kind::integer:
            std::destroy_at(std::addressof(this->value.integer));
            break;
        case Kind::op:
            std::destroy_at(std::addressof(this->value.op));
            break;
        case Kind::end_of_statement:
            std::destroy_at(std::addressof(this->value.end_of_statement));
            break;
        }
    }

    static fn Get_From_Istream() -> Token
    {
        Token tok{ .kind = Kind::invalid, .value = {} };
        std::string tok_str = {};
        char ch = '\0';

        /*
            Read token character by character.  
            Possible states for ${ch}:
                - null  ( ch == '\0' )
                - digit ( isdigit(ch) == true )
                - punct ( ispunct(ch) == true && (ch == ';' || ch == '+' || ch == '-') )
                - blank ( isblank(ch) == true )
                - invalid ( otherwise )

            State transformation:
                null  \
                blank ----> digit: token is a integer
                        |-> punct: token is an operator or EOS
                        |-> blank: ignore
                        `-> invalid: invalid token
                digit ----> digit: append digit to ${tok_str}
                        |-> punct, blank, invalid: token is completed
                punct ----> ${any}: token is completed
            
            Complete this part below.
        */

        switch (tok.kind) {
        case Kind::invalid:
            break;
        case Kind::integer:
            /* convert tok_str to integer and assign to tok.value */
            break;
        case Kind::op:
            /* convert tok_str to Op and assign to tok.value */
            break;
        case Kind::end_of_statement:
            /* convert tok_str to EOS and assign to tok.value */
            break;
        }

        return tok;
    }

    /* Add other member here if needed... */
};

/* Add anything you need here... */

int main()
{
    /* Any code here... */
}