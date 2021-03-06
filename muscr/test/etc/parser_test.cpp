#include <fstream>

#include "../catch.hpp"

#include "muscr/include/parser.h"
#include "muscr/include/utility.h"


TEST_CASE("parse", "[parser]")
{
    // iterator type used to expose the underlying input stream
    using base_iterator_type = std::string::iterator;

    // lexer type
    using lexer_type = lex::lexertl::lexer<lex::lexertl::token<base_iterator_type>>;

    // iterator type exposed by the lexer
    using iterator_type = MuscrTokens<lexer_type>::iterator_type;

    // now we use the types defined above to create the lexer and grammar
    // object instances needed to invoke the parsing process
    MuscrTokens<lexer_type> l;          // Our lexer
    MuscrGrammar<iterator_type> g(l);   // Our parser

    // Parsing is done based on the token stream, not the character
    // stream read from the input.
    std::string str (read_from_file("/Users/gilhojang/GitHub/muscr/muscr/etc/sample.muscr"));
    base_iterator_type first = str.begin();

    bool r = lex::tokenize_and_parse(first, str.end(), l, g);
    if (!r) {
        std::string rest(first, str.end());
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \"" << rest << "\"\n";
        std::cout << "-------------------------\n";
        REQUIRE(r);
    }
}

