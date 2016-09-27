#include <string>
#include <vector>

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

#include "../catch.hpp"


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
