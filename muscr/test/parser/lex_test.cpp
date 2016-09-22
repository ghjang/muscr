#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_algorithm.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

#include "../catch.hpp"

#include "muscr/include/token_id.h"
#include "muscr/include/utility.h"


namespace lex = boost::spirit::lex;


struct distance_func
{
    template <typename Iterator1, typename Iterator2>
    struct result : boost::iterator_difference<Iterator1> {};

    template <typename Iterator1, typename Iterator2>
    typename result<Iterator1, Iterator2>::type
    operator()(Iterator1 const& begin, Iterator2 const& end) const
    {
        return std::distance(begin, end);
    }
};

boost::phoenix::function<distance_func> const distance = distance_func();


template <typename Lexer>
struct word_count_tokens : lex::lexer<Lexer>
{
    word_count_tokens()
            : c(0), w(0), l(0)
            , word("[^ \t\n]+")     // define tokens
            , eol("\n")
            , any(".")
    {
        using boost::spirit::lex::_start;
        using boost::spirit::lex::_end;
        using boost::phoenix::ref;

        // associate tokens with the lexer
        this->self
                =   word  [++ref(w), ref(c) += distance(_start, _end)]
                    |   eol   [++ref(c), ++ref(l)]
                    |   any   [++ref(c)]
                ;
    }

    std::size_t c, w, l;
    lex::token_def<> word, eol, any;
};


TEST_CASE("lex", "[lexer]")
{
    using token_type = lex::lexertl::token<char const*, lex::omit, boost::mpl::false_>;
    using lexer_type = lex::lexertl::actor_lexer<token_type>;

    word_count_tokens<lexer_type> word_count_lexer;

    std::string str (read_from_file("/Users/gilhojang/GitHub/muscr/muscr/etc/sample_property.muscr"));
    char const* first = str.c_str();
    char const* last = &first[str.size()];

    lexer_type::iterator_type iter = word_count_lexer.begin(first, last);
    lexer_type::iterator_type end = word_count_lexer.end();

    while (iter != end && token_is_valid(*iter))
        ++iter;

    if (iter == end) {
        std::cout << "lines: " << word_count_lexer.l
                  << ", words: " << word_count_lexer.w
                  << ", characters: " << word_count_lexer.c
                  << "\n";
    }
    else {
        std::string rest(first, last);
        std::cout << "Lexical analysis failed\n" << "stopped at: \""
                  << rest << "\"\n";
    }
}

