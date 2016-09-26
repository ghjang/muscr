#include <string>

#include <boost/spirit/include/qi.hpp>

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
    
}
