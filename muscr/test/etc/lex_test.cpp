#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_algorithm.hpp>
#include <boost/spirit/include/phoenix_core.hpp>

#include <boost/spirit/include/lex_lexertl.hpp>

#include <boost/spirit/include/qi.hpp>

#include "../catch.hpp"

#include "muscr/include/token_id.h"
#include "muscr/include/utility.h"


using namespace boost::spirit;
using namespace boost::spirit::ascii;


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


template <typename Lexer>
struct property_tokens : lex::lexer<Lexer>
{
    property_tokens()
            : propertyMark_("@")
            , propertyValue_("[^@:\n]*")
            , identifier_("[a-zA-Z_][a-zA-Z0-9_']*")
            , colon_(":")
            , eol_("\n")
            , m{}, v{}, i{}, c{}, e{}
    {
        using boost::phoenix::ref;

        this->self
                = propertyMark_ [++ref(m)]
                    | identifier_ [++ref(i)]
                    | colon_ [++ref(c)]
                    | propertyValue_ [++ref(v)]
                    | eol_ [++ref(e)]
                ;
    }

    std::size_t m, v, i, c, e;

    lex::token_def<> propertyMark_;
    lex::token_def<> propertyValue_;
    lex::token_def<> identifier_;
    lex::token_def<> colon_;
    lex::token_def<> eol_;
};

template <typename Lexer>
struct property_tokens1 : lex::lexer<Lexer>
{
    property_tokens1()
            : propertyMark_("@")
            , identifier_("[a-zA-Z_][a-zA-Z0-9_']*")
            , colon_(":")
            , eol_("\n")
            , any_(".")
            , m{}, a{}, i{}, c{}, e{}
    {
        using boost::phoenix::ref;

        this->self = propertyMark_ [++ref(m)]
                        | identifier_ [++ref(i)]
                        | colon_ [++ref(c)]
                        | eol_ [++ref(e)]
                        | any_ [++ref(a)];
    }

    std::size_t m, a, i, c, e;

    lex::token_def<> propertyMark_;
    lex::token_def<> identifier_;
    lex::token_def<> colon_;
    lex::token_def<> eol_;
    lex::token_def<> any_;
};

template <typename Lexer>
struct property_tokens2 : lex::lexer<Lexer>
{
    property_tokens2()
        : property_tokens2::base_type(lex::match_flags::match_default)
    {
        propertyMark_ = "@";
        identifier_ = "[a-zA-Z_][a-zA-Z0-9_']*";

        this->self.add
                (propertyMark_)
                (".", ID_ANY)
        ;

        this->self("PROPERTY").add
                (identifier_)
                (":", ID_COLON)
                (R"(\n)", ID_EOL)
                (".", ID_ANY)
        ;
    }

    lex::token_def<> propertyMark_;
    lex::token_def<> identifier_;
};

template <typename Iterator>
struct property_grammar : qi::grammar<Iterator>
{
    template <typename TokenDef>
    property_grammar(TokenDef const& tok)
            : property_grammar::base_type(start_)
    {
        start_ = *(
                    tok.propertyMark_
                            >> qi::in_state("PROPERTY")
                                    [
                                        tok.identifier_ [ std::cout << _1 ]
                                                >> token(ID_COLON) [ std::cout << _1 ]
                                                >> *token(ID_ANY) [ std::cout << _1 ]
                                                >> token(ID_EOL) [ std::cout << _1 ]
                                    ]
                  | token(ID_ANY)
                  )
        ;
    }

    qi::rule<Iterator> start_;
};


TEST_CASE("property tokens", "[lex]")
{
    using token_type = lex::lexertl::token<std::string::iterator>;
    using lexer_type = lex::lexertl::actor_lexer<token_type>;

    //word_count_tokens<lexer_type> lexer;
    //property_tokens<lexer_type> lexer;
    //property_tokens1<lexer_type> lexer;

    property_tokens2<lexer_type> lexer;

    using iterator_type = property_tokens2<lexer_type>::iterator_type;

    property_grammar<iterator_type> grammar(lexer);

    /*
    std::string str (read_from_file("/Users/gilhojang/GitHub/muscr/muscr/etc/sample_property.muscr"));
    char const* first = str.c_str();
    char const* last = &first[str.size()];

    lexer_type::iterator_type iter = lexer.begin(first, last);
    lexer_type::iterator_type end = lexer.end();

    while (iter != end && token_is_valid(*iter)) {
        ++iter;
    }
     */

    //if (iter == end) {
        /*
        std::cout << "lines: " << lexer.l
                  << ", words: " << lexer.w
                  << ", characters: " << lexer.c
                  << "\n";
        */

        /*
        std::cout << "m: " << lexer.m
                  << ", v: " << lexer.v
                  << ", i: " << lexer.i
                  << ", c: " << lexer.c
                  << ", e: " << lexer.e
                  << "\n";
        */

        /*
        std::cout << "m: " << lexer.m
                  << ", i: " << lexer.i
                  << ", c: " << lexer.c
                  << ", e: " << lexer.e
                  << ", a: " << lexer.a
                  << "\n";
                  */

    /*
    }
    else {
        std::string rest(first, last);
        std::cout << "Lexical analysis failed\n" << "stopped at: \""
                  << rest << "\"\n";
        REQUIRE(false);
    }
     */

    std::string str (read_from_file("/Users/gilhojang/GitHub/muscr/muscr/etc/sample_property.muscr"));
    token_type::iterator_type first = str.begin();
    bool r = lex::tokenize_and_parse(first, str.end(), lexer, grammar);
    if (!r) {
        std::string rest(first, str.end());
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \"" << rest << "\"\n";
        std::cout << "-------------------------\n";
        REQUIRE(r);
    }
    else {
        std::string rest(first, str.end());
        std::cout << "\nrest:\n" << rest << '\n';
    }
}

