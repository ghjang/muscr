#ifndef MUSCR_TOKEN_ID_H
#define MUSCR_TOKEN_ID_H


#include <boost/spirit/include/lex_lexertl.hpp>


enum TokenId
{
    ID_PROPERTY_MARK = boost::spirit::lex::min_token_id + 10,
    ID_IDENTIFIER,
    ID_COLON,
    ID_EOL,
    ID_ANY
};


#endif //MUSCR_TOKEN_ID_H

