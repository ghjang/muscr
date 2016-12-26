#include "../catch.hpp"

#include <string>
#include <vector>

#include <boost/spirit/include/qi.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>

#include "muscr/include/utility.h"


TEST_CASE("num_list1", "[qi]")
{
    using namespace boost::spirit;

    auto num_list_parser = [](auto begin, auto end) {
        using qi::double_;

        bool r = qi::phrase_parse(
                        begin,
                        end,
                        double_ >> *(',' >> double_),
                        ascii::space
                    );

        if (begin != end) {
            return false;
        }
        return r;
    };

    std::string s0 = "11, 22, 33";
    std::string s1 = "11 22, 33";
    std::string s2 = "abc, 11, zz";
    std::string s3 = "11  , 22, 33";
    REQUIRE(num_list_parser(s0.begin(), s0.end()));
    REQUIRE(!num_list_parser(s1.begin(), s1.end()));
    REQUIRE(!num_list_parser(s2.begin(), s2.end()));
    REQUIRE(num_list_parser(s3.begin(), s3.end()));
}

TEST_CASE("num_list2", "[qi]")
{
    using namespace boost::spirit;

    auto num_list_parser = [](auto begin, auto end, auto & v) {
        using qi::double_;

        bool r = qi::phrase_parse(
                        begin,
                        end,
                        double_ >> *(',' >> double_),
                        ascii::space,
                        v
                    );

        if (begin != end) {
            return false;
        }
        return r;
    };

    std::vector<double> v;
    std::string s0 = "1.0, 2.2, 33.3";
    REQUIRE(num_list_parser(s0.begin(), s0.end(), v));
    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1.0);
    REQUIRE(v[1] == 2.2);
    REQUIRE(v[2] == 33.3);
}

TEST_CASE("num_list3", "[qi]")
{
    using namespace boost::spirit;

    auto num_list_parser = [](auto begin, auto end, auto & v) {
        using qi::double_;

        bool r = qi::phrase_parse(
                        begin,
                        end,
                        double_ % ',',
                        ascii::space,
                        v
                    );

        if (begin != end) {
            return false;
        }
        return r;
    };

    std::vector<double> v;
    std::string s0 = "1.0, 2.2, 33.3";
    REQUIRE(num_list_parser(s0.begin(), s0.end(), v));
    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == 1.0);
    REQUIRE(v[1] == 2.2);
    REQUIRE(v[2] == 33.3);
}

TEST_CASE("actions", "[qi]")
{
    using namespace boost::spirit;

    char const * pBegin = "{123}";
    char const * pEnd = pBegin + std::strlen(pBegin);
    int i = 0;
    qi::parse(
        pBegin,
        pEnd,
        '{' >> int_[([&i](int val) { i = val; })] >> '}' // NOTE: double-left-square-bracket problem
    );
    REQUIRE(i == 123);
}

TEST_CASE("number sum", "[qi]")
{
    using namespace boost::spirit;

    char const * pBegin = "10, 22.2, 10";
    char const * pEnd = pBegin + std::strlen(pBegin);
    double d = 0;
    qi::phrase_parse(
        pBegin,
        pEnd,
        double_[([&d](auto val) { d += val; })] % ',',
        ascii::space
    );
    REQUIRE(42.2 == d);
}


namespace client
{
    struct employee
    {
        int age;
        std::string surname;
        std::string forename;
        double salary;
    };
} // namespace client

BOOST_FUSION_ADAPT_STRUCT(
    client::employee,
    (int, age)
    (std::string, surname)
    (std::string, forename)
    (double, salary)
)

namespace client
{
    using namespace boost::spirit;

    template <typename Iterator>
    struct employee_parser : qi::grammar<Iterator, employee(), ascii::space_type>
    {
        employee_parser() : employee_parser::base_type(start)
        {
            using ascii::char_;

            quoted_string %= lexeme['"' >> +(char_ - '"') >> '"'];

            start %=
                lit("employee")
                >> '{'
                >>  int_ >> ','
                >>  quoted_string >> ','
                >>  quoted_string >> ','
                >>  double_
                >>  '}'
                ;
        }

        qi::rule<Iterator, std::string(), ascii::space_type> quoted_string;
        qi::rule<Iterator, employee(), ascii::space_type> start;
    };
} // namespace client

TEST_CASE("adapt struct", "[qi]")
{
    using namespace boost::spirit;

    char const * const pStr = R"(
                                    employee{ 39, "Jang", "Gil Ho", 10000 }                                employee{ 39, "XXXX", "YYY ZZ", 10000 }                                employee{ 29, "XXXX", "YYY ZZ", 10000 }
                                )";
    char const * pBegin = pStr;
    char const * const pEnd = pStr + std::strlen(pStr);

    client::employee emp;
    client::employee_parser<char const *> parser_grammar;

    qi::phrase_parse(
        pBegin,
        pEnd,
        parser_grammar,
        ascii::space,
        emp
    );
    REQUIRE(emp.surname == "Jang");
    REQUIRE(pBegin != pEnd);

    std::vector<client::employee> empV;

    pBegin = pStr;
    qi::phrase_parse(
        pBegin,
        pEnd,
        +parser_grammar,
        ascii::space,
        empV
    );
    REQUIRE(empV.size() == 3);
    REQUIRE(empV[0].surname == "Jang");
    REQUIRE(empV[1].surname == "XXXX");
    REQUIRE(empV[2].age == 29);
    REQUIRE(pBegin == pEnd);
}

namespace client
{
    // our normal ordinary C++ user-defined struct
    struct student
    {
        std::string name_;
        std::vector<double> score_;
    };
} // namespace client

// client::student to Boost.Fusion sequence adaptation
BOOST_FUSION_ADAPT_STRUCT(
    client::student,
    (std::string, name_)
    (std::vector<double>, score_)
)

namespace client
{
    using namespace boost::spirit;

    // NOTE: a grammar itself is a parser.
    //       this parser's attribute is client::student.
    template <typename Iterator>
    struct student_score_grammar
            : qi::grammar<Iterator, student(), ascii::space_type>
    {
        student_score_grammar() : student_score_grammar::base_type(student_score_)
        {
            using ascii::char_;

            // quoted string
            name_ %= lexeme['"' >> +(char_ - '"') >> '"'];

            // grammar start point.
            // the resulting synthesized attribute for this compound parser expression will be something like:
            //      fusion::vector<
            //                  std::string,
            //                  std::vector<double>
            //      >
            // note that the synthesized attribute is compatible with client::student.
            student_score_ %=
                '{'
                >>  name_ >> ','
                >> (double_ % ',')
                >>  '}'
                ;
        }

        // NOTE: a rule itself is a parser.
        qi::rule<Iterator, std::string(), ascii::space_type> name_;
        qi::rule<Iterator, student(), ascii::space_type> student_score_;   
    };
} // namespace client

TEST_CASE("synthesized parser attribute", "[qi]")
{
    using namespace boost::spirit;

    char const * const pStr = R"(
                                    { "ghjang", 100, 99, 98, 97 }
                                    { "xxxxxx", 200, 99.9, 98.8, 97.7 }
                                    { "yyyyyy", 300, 99, 98, 97 }
                                )";
    char const * pBegin = pStr;
    char const * const pEnd = pStr + std::strlen(pStr);

    using student_scores = std::vector<client::student>;
    using student_score_record_parser = client::student_score_grammar<char const *>; 

    student_scores scores;              // result sink container
    student_score_record_parser score;  // NOTE: a grammar itself is a parser.

    qi::phrase_parse(
        pBegin,
        pEnd,
        +score,     // it is still possible to modify the parser expression at here.
                    // at lease one score record is required.
                    // this parser expression's resulting attribute is like
                    // std::vector<client::student>, i.e. student_scores.
        ascii::space,
        scores      // the parsed result will be filled in scores of which type is Fusion-compatible.
    );
    REQUIRE(scores.size() == 3);
    REQUIRE(scores[0].name_ == "ghjang");
    REQUIRE(scores[0].score_[0] == 100);
    REQUIRE(scores[1].name_ == "xxxxxx");
    REQUIRE(scores[1].score_[1] == 99.9);
    REQUIRE(scores[2].name_ == "yyyyyy");
    REQUIRE(scores[2].score_[2] == 98);
}

TEST_CASE("% repeat match", "[qi]")
{
    namespace qi = boost::spirit::qi;
    using qi::lit;

    std::string s = "A,";

    qi::rule<std::string::iterator, qi::ascii::space_type> rule{
        lit('A') % ','
    };

    auto begin = s.begin();
    auto end = s.end();
    bool r = qi::phrase_parse(begin, end, rule, qi::ascii::space);
    REQUIRE(r);
    REQUIRE(begin != end);

    s = "A, A";
    begin = s.begin();
    end = s.end();
    r = qi::phrase_parse(begin, end, rule, qi::ascii::space);
    REQUIRE(r);
    REQUIRE(begin == end);
}


namespace client
{
    struct recursive_match_attr;

    using subelement
        = std::vector<
                    boost::variant<
                        std::string,
                        boost::recursive_wrapper<recursive_match_attr>
                    >
          >;

    using element
        = boost::variant<
                std::string,
                subelement
          >;

    struct recursive_match_attr
    {
        std::vector<element> elements_;
    };
} // namespace client

BOOST_FUSION_ADAPT_STRUCT(
    client::recursive_match_attr,
    (std::vector<client::element>, elements_)
)

TEST_CASE("recursive match", "[qi]")
{
    namespace qi = boost::spirit::qi;
    using qi::char_;

    struct recursive_match
        : qi::grammar<std::string::iterator, qi::ascii::space_type>
    {
        recursive_match() : recursive_match::base_type(div_)
        {
            subdiv_ = '(' >> div_ >> ')';
            div_ = (char_("A-Z") | subdiv_) % ',';
        }

        qi::rule<std::string::iterator, qi::ascii::space_type> subdiv_;
        qi::rule<std::string::iterator, qi::ascii::space_type> div_;
    };

    std::string s = "A, (B, C), (D, E, (F, G)), H";
    auto begin = s.begin();
    auto end = s.end();
    recursive_match rule_;
    bool r = qi::phrase_parse(begin, end, rule_, qi::ascii::space);
    REQUIRE(r);
    REQUIRE(begin == end);
}

namespace std
{
    // for debug output
    template <typename T>
    inline static std::ostream& operator<<(std::ostream& os, std::vector<T> const& v) {
        os << "(";
        bool first = true;
        for (auto & el : v) {
            (first ? os : os << ", ") << el;
            first = false;
        }
        return os << ")";
    }
} // namespace std

namespace Ast
{
    using node = boost::make_recursive_variant<
                            char,
                            std::vector<boost::recursive_variant_>
                 >::type;

    using nodes = std::vector<node>;
} // namespace Ast

template <typename It = std::string::const_iterator>
struct recursive_match : boost::spirit::qi::grammar<It, Ast::nodes(), boost::spirit::qi::ascii::space_type>
{
    recursive_match() : recursive_match::base_type(list_) {
        using namespace boost::spirit::qi;

        node_ = char_("A-Z") | '(' >> list_ >> ')';
        list_ = node_ % ',';

        BOOST_SPIRIT_DEBUG_NODES((node_)(list_))
    }

  private:
    boost::spirit::qi::rule<It, Ast::node(),  boost::spirit::qi::ascii::space_type> node_;
    boost::spirit::qi::rule<It, Ast::nodes(), boost::spirit::qi::ascii::space_type> list_;
};


// refter to http://stackoverflow.com/questions/41330814/saving-boost-spirit-recursive-match-results-to-a-c-struct
TEST_CASE("recursive match attribute", "[qi]")
{
    namespace qi = boost::spirit::qi;

    using qi::char_;

    std::string const s = "A, (B, C), (D, E, (F, G)), H";
    auto begin = s.begin();
    auto end = s.end();
    recursive_match<> rule_;
    Ast::nodes parsed;
    bool ok = qi::phrase_parse(begin, end, rule_, qi::ascii::space, parsed);

    if (ok)
        std::cout << "Parsed: " << parsed << "\n";
    else
        std::cout << "Parse failed\n";

    if (begin != end)
        std::cout << "Remaining unparsed input: '" << std::string(begin, end) << "'\n";    
}
