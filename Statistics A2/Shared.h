constexpr auto OBJECTSIZE = 5000;
constexpr auto GROUPSIZE = 1000;
constexpr auto SUBJECTSIZE = 1000;

constexpr auto ALLGROUPS = 0;
constexpr auto NOTCHOOSABLEGROUPS = 1;
constexpr auto CHOOSABLEGROUPS = 2;

constexpr auto ALLSUBJECTS = 0;
constexpr auto NOTCHOOSABLESUBJECTS = 1;
constexpr auto CHOOSABLESUBJECTS = 2;

constexpr auto BYNAME = 0;
constexpr auto BYOBJ = 1;

constexpr auto MSG_STR = 1;
constexpr auto MSG_ARR = 2;
constexpr auto MSG_BOTH = 3;

constexpr auto MSGSIZE = 101;

constexpr auto SETUNCERTAIN_FULLMARK = 1;
constexpr auto SETUNCERTAIN_ZERO = 2;
constexpr auto SETUNCERTAIN_CUSTOMIZE = 3;

constexpr auto ERR_NOMEMORY = 4;
constexpr auto ERR_EMPTYMEMORY = 5;
constexpr auto ERR_NOSUCHITEM = 6;
constexpr auto ERR_NOSUCHITEM_POSINC = 7;
constexpr auto ERR_NOSUCHITEM_POSDEC = 8;
constexpr auto ERR_UNCERTAINSCORE = 9;
constexpr auto ERR_LACKPARAMETER = 10;
constexpr auto ERR_ITEMALREADYEXISTS = 11;

constexpr auto SEARCH_SINGLE = 1;
constexpr auto SEARCH_MULTIPLE = 2;
constexpr auto SEARCH_FUZZY = 3;

constexpr auto ERRPOLICY_POSINC = 1;
constexpr auto ERRPOLICY_POSDEC = 2;

using namespace std;
