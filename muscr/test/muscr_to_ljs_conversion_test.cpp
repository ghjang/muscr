#include "catch.hpp"

#include "muscr/include/muscr_to_ljs_conversion.h"


TEST_CASE("muscr to ljs conversion", "[muscr to ljs]")
{
    muscr::parser::leadsheet_staff_attr srcAttr;

    auto destAttr = muscr::to_ljs_song_data(srcAttr);
}
