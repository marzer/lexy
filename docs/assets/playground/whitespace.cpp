// INPUT:Hello\nWorld   !
struct production
{
    static constexpr auto whitespace = dsl::ascii::space;
    static constexpr auto rule                  //
        = LEXY_LIT("Hello") + LEXY_LIT("World") //
          + dsl::lit_c<'!'> + dsl::eof;
};
