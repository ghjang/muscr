#ifndef MUSCR_PARSER_H
#define MUSCR_PARSER_H


#include <iostream>
#include <string>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_container.hpp>


using namespace boost::spirit;


enum TokenId
{
    ID_ANY = lex::min_token_id + 10
};

template <typename Lexer>
struct MuscrTokens : lex::lexer<Lexer>
{
    MuscrTokens()
            : MuscrTokens::base_type(lex::match_flags::match_default)
    {
        // define tokens and associate them with the lexer
        lineComment_ = "\"--\"[^\n]*";       // '--[^\n]*'
        blockCommentStart_ = "\"{--\"";      // '{--'
        blockCommentEnd_ = "\"--}\"";        // '--}'

        // The following tokens are associated with the default lexer state
        // (the "INITIAL" state). Specifying 'INITIAL' as a lexer state is
        // strictly optional.
        this->self.add
                (lineComment_)    // no explicit token id is associated
                (blockCommentStart_)
                (".", ID_ANY)    // ID_ANY is the token id associated with this token
                                 // definition
        ;

        // The following tokens are associated with the lexer state "COMMENT".
        // We switch lexer states from inside the parsing process using the
        // in_state("COMMENT")[] parser component as shown below.
        this->self("COMMENT").add
                (blockCommentEnd_)
                (".", ID_ANY)
        ;
    }

    lex::token_def<> lineComment_;
    lex::token_def<> blockCommentStart_;
    lex::token_def<> blockCommentEnd_;
};


template <typename Iterator>
struct MuscrGrammar : qi::grammar<Iterator>
{
    template <typename TokenDef>
    MuscrGrammar(TokenDef const& tok)
            : MuscrGrammar::base_type(start_)
    {
        // The in_state("COMMENT")[...] parser component switches the lexer
        // state to be 'COMMENT' during the matching of the embedded parser.
        start_ = *(   tok.blockCommentStart_
                              >>  qi::in_state("COMMENT")
                                    [
                                      // the lexer is in the 'COMMENT' state during
                                      // matching of the following parser components
                                      *token(ID_ANY) >> tok.blockCommentEnd_
                                    ]
                  |   tok.lineComment_
                  |   qi::token(ID_ANY)  [ std::cout << _1 ]
                  )
        ;
    }

    qi::rule<Iterator> start_;
};


#endif //MUSCR_PARSER_H

