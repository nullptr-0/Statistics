constexpr auto ObjectSize = 5000;
constexpr auto GroupSize = 1000;
constexpr auto SubjectSize = 1000;

constexpr auto AllGroups = 0;
constexpr auto CompulsoryGroups = 1;
constexpr auto OptionalGroups = 2;

constexpr auto AllSubjects = 0;
constexpr auto CompulsorySubjects = 1;
constexpr auto OptionalSubjects = 2;

constexpr auto ByName = 0;
constexpr auto ByObj = 1;

constexpr auto Msg_Str = 1;
constexpr auto Msg_Arr = 2;
constexpr auto Msg_Both = 3;

constexpr auto MsgSize = 101;

constexpr auto SetUncertain_Full = 1;
constexpr auto SetUncertain_Zero = 2;
constexpr auto SetUncertain_Customize = 3;

constexpr auto Error_FullMemory = 4;
constexpr auto Error_EmptyMemory = 5;
constexpr auto Error_NoSuchItem = 6;
constexpr auto Error_NoSuchItem_RankInc = 7;
constexpr auto Error_NoSuchItem_RankDec = 8;
constexpr auto Error_UncertainScore = 9;
constexpr auto Error_LackParameter = 10;
constexpr auto Error_ItemExists = 11;

constexpr auto Search_Single = 1;
constexpr auto Search_Multiple = 2;
constexpr auto Search_Fuzzy = 3;

constexpr auto ErrorHandling_IncRank = 1;
constexpr auto ErrorHandling_DecRank = 2;

using namespace std;
