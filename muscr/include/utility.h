#ifndef MUSCR_UTILITY_H
#define MUSCR_UTILITY_H


#include <string>
#include <iostream>
#include <fstream>

#include <boost/type_index.hpp>
#include <boost/spirit/include/qi.hpp>


inline std::string read_from_file(char const* infile)
{
    std::ifstream instream(infile);
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << infile << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);      // No white space skipping!
    return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
                       std::istreambuf_iterator<char>());
}


namespace tools
{
    namespace spirit = boost::spirit;

    template <typename Expr, typename Iterator = spirit::unused_type>
    struct attribute_of_parser
    {
        using parser_expression_type = typename spirit::result_of::compile<
                                                        spirit::qi::domain, Expr
                                                    >::type;

        using type = typename spirit::traits::attribute_of<
                                    parser_expression_type, spirit::unused_type, Iterator
                                >::type;
    };

    template <typename T>
    void display_attribute_of_parser(T const &) 
    {
        // Report invalid expression error as early as possible.
        // If you got an error_invalid_expression error message here,
        // then the expression (expr) is not a valid spirit qi expression.
        BOOST_SPIRIT_ASSERT_MATCH(spirit::qi::domain, T);

        using type = typename attribute_of_parser<T>::type;
        std::cout << boost::typeindex::type_id<type>().pretty_name() << '\n';
    }

    template <typename T>
    void display_attribute_of_parser(std::ostream& os, T const &) 
    {
        // Report invalid expression error as early as possible.
        // If you got an error_invalid_expression error message here,
        // then the expression (expr) is not a valid spirit qi expression.
        BOOST_SPIRIT_ASSERT_MATCH(spirit::qi::domain, T);

        using type = typename attribute_of_parser<T>::type;
        os << boost::typeindex::type_id<type>().pretty_name() << '\n';
    }
} // namespace tools


#endif //MUSCR_UTILITY_H
