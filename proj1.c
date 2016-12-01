/**
 * @author Son Hai Nguyen
 * @date 30. 10. 2016
 * @file proj1.c
 * @brief This program work according to
 * https://wis.fit.vutbr.cz/FIT/st/cwk.php?title=IZP:Projekt1&csid=624691&id=11499
 * @note Using string_to_number, because it use -1 as error
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

//#define NDEBUG

#ifdef NDEBUG
static unsigned int _passed_tests = 0;
static unsigned int _failed_tests = 0;

#define UNUSED(x) (void)x
#define TST_TOTAL() \
    printf("----------------------------------------------------\n"); \
    printf("Tests: %d passed, %d failed\n", _passed_tests, _failed_tests); \
    printf("----------------------------------------------------\n");

// WARNING quite dangerous if it is not used correctly
#define TST_CASE(test_case_name, x) \
    printf("****** Start testing of %s ******\n", #test_case_name); \
    x \
    printf("****** Finished testing of %s ******\n\n", #test_case_name);

#define TST_VERIFY(expression) \
    if(expression) { \
        ++_passed_tests; \
        printf("PASS: %s\n", __func__); \
    } \
    else { \
        ++_failed_tests; \
        printf("FAIL %s: %d - '%s' returned '%d'\n",\
                __func__, __LINE__, #expression, expression); \
    }

#define TST_COMPARE(expression1, expression2) \
    if((expression1) == (expression2)) { \
        ++_passed_tests; \
        printf("PASS: %s\n", __func__); \
    } \
    else { \
        ++_failed_tests; \
        printf("FAIL: %s: %d - compared values are not the same" \
               "\nActual: %d\nExpected: %d\n", \
               __func__, __LINE__, expression1, expression2); \
    }
#endif

typedef unsigned int uint32;
typedef unsigned char uint8;

// *********DECLARATION OF TESTS*********
void test_math_api();
void test_string_api();
void test_hex_api();
void test_flag_api();

// *********DECLARATION OF MATH API*********
/**
 * @brief ippow Power base by exp, exp must be positive
 * @param base
 * @param exp
 * @return Powered base, return -1 if base = 0 or exp <= 1
 */
int ippow(int base, unsigned int exp);

// *********DECLARATION OF STRING API*********
/**
 * @brief strcmp Compare two strings, if match return true,
 *  if strings do not match or lengths do not match return false
 * @param str1 First string operand
 * @param str2 Second string operand
 * @return 1 or 0 <=> true or false
 */
bool string_compare(const char *str1, const char *str2);

/**
 * @brief string_len Return number of characters in
 * string. Does not count '\0'
 * @param str
 * @return
 */
unsigned int string_len(const char* str);

/**
 * @brief string_contain Return true if str contain character c
 * @param str String in which we are looking for c
 * @param c Character, which we want to find
 * @return 1 or 0 <=> true or false
 */
bool string_contain(char c, const char* str);

/**
 * @brief string_count Return number of occurencies of charater c
 * in str
 * @param c Character, which is being counted
 * @param str String in which it will be counted number of c
 * @return Number of occurrencies of c in str, number is only positive
 */
unsigned int string_count(char c, const char* str);

/**
 * @brief string_is_number Test whether str contains chararcters
 * int range '0' - '9'
 * @param str String which is tested
 * @return 1 or 0 <=> true or false
 */
bool string_is_number(const char* str);

/**
 * @brief string_to_number Convert string into number, work only with positive numbers
 * @param str String which will be converted into number
 * @return Number, if fail return -1
 */
// NOTE work only with positive numbers
int string_to_number(const char* str);

/**
 * @brief string_fill Fill string with fill, string must be initialized
 * @param fill
 * @param str String which will be filled
 * @return Pointer to str
 */
char* string_fill(char fill, char* str);

/**
 * @brief nprintf Print to stdout n-times character c
 * @param c Character, which will be printed
 * @param count Number of printed characters
 */
void nprintf(char c, unsigned int count);

// *********DECLARATION OF HEX API*********
typedef struct
{
    char hex[3];     // 2 because char = 1B => 2hex characters
} Hex8;

/**
 * @brief hex_null Set all hex characters to '\0'
 * @param h Hex which will be nulled
 */
void hex_null(Hex8* h);

// *********DECLARATION OF FLAG API*********
typedef enum
{
    UNDEFINED = -1,
    DEFAULT = 0,
    SKIP = 1,
    NUMBER_OF_CHARS = 2,
    UNFORMATED_HEX = 4,
    SPLIT = 8,
    REVERSE = 16
} Actions;

// SETTINGS OF FLAGS
const unsigned int split_minimum_word_len = 0;
const unsigned int split_maximum_word_len = 200;
// NOTE '%' means optional number param '&' means required param
static const char* STR_FLAGS[] = {"-s&", "-n&", "-x", "-S&", "-r"}; // '%' means expect unsigned int
static const int FLAGS_COUNT = sizeof(STR_FLAGS) / sizeof(char*);

typedef enum
{
    MISSING_FLAG_PARAMETER = -2,
    UNEXPECTED_PARAMETER_ERROR = -1,
    NO_ERROR = 0,
    UNKNOWN_INPUT_ERROR,
    FLAG_NOT_EXPECT_PARAMETER_ERROR,
    FLAG_DUPLICATION
} Errors;

/**
 * @brief parse_arguments Take arguments stored in argv and store
 * it into int(bit flags) and flags parameters store into flags_parameters
 * @param argc Arguments count
 * @param argv Arguments with flags and parameters
 * @param flags_parameters All flags parameters are stored in it,
 * it must have size of 16 elements == max flag value,
 * -1 == unexpected argument, -2 expected argument
 * @return Bit flags stored in int
 */
unsigned int parse_arguments(int argc, const char* argv[], int* flags_parameters);

/**
 * @brief is_flag Test str if it is in flag format, return 1
 * if it is flag, return 0 if it is not
 * @param str String which will be tested
 * @return 1 or 0 <=> true or false
 */
bool is_flag(const char* str);

/**
 * @brief distinguish_flag At first it checks whether str
 * is flag and then it find flag type suited to ArgumentFlags
 * @param str Flag stored in string
 * @return Type of flag
 */
Actions distinguish_action(const char* str);

/**
 * @brief flag_accept_param If action accept param return 1
 * else return 0
 * @param action Action which will be tested
 * @return 1 or 0 <=> true or false
 */
bool flag_accept_param(Actions action);

/**
 * @brief flag_require_param If action require param return 1
 * else return 0
 * @param action Action which will be tested
 * @return 1 or 0 <=> true or false
 */
bool flag_require_param(Actions action);

/**
 * @brief flags_validation Takes argc and argv and check all invalid flags and parameters
 * @param argc
 * @param argv
 * @return enum Errors
 */
Errors flags_validation(int argc, const char *argv[]);

/**
 * @brief flag_is_allowed Return true if flag is allowed else false
 * @param str_flag String which we want to test if it is allowed flag
 * @return 1 or 0 <=> true or false
 */
bool flag_is_allowed(const char* str_flag);

// *********DECLARATION OF ACTION API*********
/**
 * @brief action_unformated_hex Takes str from stdin and print it as hex
 */
void action_unformated_hex();

/**
 * @brief action_reverse Convert hex str from stdin to str, ignor whitespace etc.
 */
void action_reverse();

/**
 * @brief action_split Split string from stdin to "words", split by \n, \0 etc.
 * @param word_size Word must be at least >= count
 */
void action_split(unsigned int word_size);

/**
 * @brief action_default Printf address character in hex, 16 chars per line
 * @param address Define how many skip chars
 * @param count If count == -1, then ignore count
 */
void action_default(uint32 address, int count);

/**
 * @brief print_help Print allowed combinations of flags
 */
void print_help();

/**
 * @brief print_error Print specific error
 * @param err
 */
void print_error(Errors err);

/**
 * @brief run_actions Run specific actions according to flags
 * @param flags
 * @param params Number parameters of flags
 * @return 0 if successfull otherwise 1
 */
int run_actions(int flags, int* params);

// NOTE Main
int main(int argc, const char *argv[])
{
#ifdef NDEBUG
    test_math_api();
    test_string_api();
    test_hex_api();
    test_flag_api();
    TST_TOTAL();
#endif

    int params[ippow(2, FLAGS_COUNT) + 1];
    int flags = parse_arguments(argc, argv, params);

    // check for unexpected  params and flags
    Errors err = flags_validation(argc, argv);
    print_error(err);

    if(err != NO_ERROR) {
        print_help();
        return EXIT_SUCCESS;
    }

    // run actions
    return run_actions(flags, params);
}

// *********IMPLEMENTATION OF MATH API*********
int ippow(int base, unsigned int exp)
{
    if(exp == 0)
        return 1;

    int result = base;

    while(--exp)
        result *= base;

    return result;
}

// *********IMPLEMENTATION OF STRING API*********
bool string_compare(const char* str1, const char* str2)
{
    if(string_len(str1) != string_len(str2))
        return false;

    while(*str1) {
        if(*str1++ != *str2++)
            return false;
    }

    return true;
}

// NOTE return actually only number >= 0
unsigned int string_len(const char* str)
{
    int size = 0;

    while(*str++)
        ++size;

    return size;
}

bool string_contain(char c, const char* str)
{
    while(*str) {
        if(c == *str++)
            return true;
    }

    return false;
}

// NOTE return actually only number >= 0
unsigned int string_count(char c, const char* str)
{
    int count = 0;

    while(*str) {
        if(c == *str++)
            ++count;
    }

    return count;
}

bool string_is_number(const char* str)
{
    while(*str) {
        if(!isdigit(*str))
            return false;
        ++str;
    }

    return true;
}

int string_to_number(const char* str)
{
    if(!string_is_number(str))
        return -1;

    const int str_len = string_len(str);
    int result = 0;

    for(int i = str_len; *str; i--)
        result += (*str++ - '0') * ippow(10, i - 1);

    return result;
}

char* string_fill(char fill, char* str)
{
    char *orig = str;   // because str pointer is being modified

    while(*str)
        *str++ = fill;
    return orig;
}

void nprintf(char c, unsigned int count)
{
    while(count--)
        printf("%c", c);
}

// *********IMPLEMENTATION OF HEX API*********
void hex_null(Hex8* h)
{
    string_fill('\0', h->hex);
}

// *********IMPLEMENTATION OF FLAG API*********
unsigned int parse_arguments(int argc, const char* argv[], int *flags_parameters)
{
    // flags_parameters must be at least size of last value of STR_FLAGS
    unsigned int flags = 0;
    Actions action = UNDEFINED;
    Actions previous_action = UNDEFINED;

    // null flags_parameters to be sure
    for(int i = 0; i <= (ippow(2, FLAGS_COUNT - 1)); ++i)
        flags_parameters[i] = 0;

    for(int i = 1; i < argc; ++i) {
        if((action = distinguish_action(argv[i])) != UNDEFINED) {
            flags |= (int)action;
            if(flag_require_param(action))
                flags_parameters[action] = (int)MISSING_FLAG_PARAMETER;
        }
        else if(string_is_number(argv[i]) && previous_action != UNDEFINED){
            if(flag_accept_param(previous_action))
                flags_parameters[previous_action] = string_to_number(argv[i]);
            else
                flags_parameters[previous_action] = (int)UNEXPECTED_PARAMETER_ERROR;
        }
        previous_action = action;
    }

    return flags;
}

bool is_flag(const char* str)
{
    if(string_contain('-', str) && string_len(str) >= 2)
        return true;
    return false;
}

Actions distinguish_action(const char* str)
{
    char flag_without_param[3] = {'-', '\0', '\0'};

    for(int i = 0; i < FLAGS_COUNT; ++i) {
        flag_without_param[1] = STR_FLAGS[i][1];    // remove flag specifier
        if(string_compare(str, flag_without_param))
            return (Actions)(1 << i);
    }

    return UNDEFINED;
}

bool flag_accept_param(Actions action)
{
    if(action == UNDEFINED)
        return false;

    int index_of_flag = 0;
    int iaction = (int)action;

    while(iaction >>= 1)
        ++index_of_flag;

    return string_contain('%', STR_FLAGS[index_of_flag]) || string_contain('&', STR_FLAGS[index_of_flag]);
}

bool flag_require_param(Actions action)
{
    if(action == UNDEFINED)
        return false;

    int index_of_flag = 0;
    int iaction = (int)action;

    while(iaction >>= 1)
        ++index_of_flag;

    return string_contain('&', STR_FLAGS[index_of_flag]);
}

Errors flags_validation(int argc, const char* argv[])
{
    int previous_arg_was_flag = false;
    int previous_flag_required_flag = false;
    int flags = 0;
    Actions action;

    for(int i = 1; i < argc; ++i) {
        if(flag_is_allowed(argv[i]) && !previous_flag_required_flag) {
            action = distinguish_action(argv[i]);
            // check for duplcations
            if(flags & (int)action)
                return FLAG_DUPLICATION;

            flags |= (int)action;
            previous_arg_was_flag = true;
            previous_flag_required_flag = flag_require_param(action);
        }

        // if current arg is number and previous was allowed flag and flag accept param
        else if(string_is_number(argv[i])) {
            int flag_accept_parameter = flag_accept_param(distinguish_action(argv[i - 1]));

            if(previous_arg_was_flag && !flag_accept_parameter)
                return FLAG_NOT_EXPECT_PARAMETER_ERROR;
            else if(!previous_arg_was_flag)
                return UNEXPECTED_PARAMETER_ERROR;
            previous_arg_was_flag = false;
            previous_flag_required_flag = false;
        }

        else if(previous_flag_required_flag)
            return MISSING_FLAG_PARAMETER;
        else
            return UNKNOWN_INPUT_ERROR;
    }

    if(previous_flag_required_flag)
        return MISSING_FLAG_PARAMETER;

    return NO_ERROR;
}

bool flag_is_allowed(const char* str_flag)
{
    if(string_len(str_flag) < 2)
        return false;

    const uint32 str_flag_len = string_len(str_flag);
    char cleared_flag[3] = {'\0', '\0', '\0'};

    for(int i = 0; i < FLAGS_COUNT; ++i) {
        // match symbol and has '-' and lenghts are equal
        if(string_contain(STR_FLAGS[i][1], str_flag) && str_flag[0] == '-') {
            cleared_flag[0] = STR_FLAGS[i][0];
            cleared_flag[1] = STR_FLAGS[i][1];
            // beacause if accept param, there is additionally one speciefer(ont symbol);
            if(flag_accept_param(distinguish_action(cleared_flag)))
                return (string_len(STR_FLAGS[i]) - 1) == str_flag_len;
            else
                return string_len(STR_FLAGS[i]) == str_flag_len;
        }
    }

    return false;
}

// *********IMPLEMENTATION OF ACTION API*********
void action_unformated_hex()
{
    int c;
    while((c = getchar()) != EOF)
        printf("%02x", c);
    printf("\n");
}

void action_reverse()
{
    int c;
    int char_counter = 0;
    Hex8 h;

    while((c = getchar()) != EOF) {
        if((!isxdigit(c)) && (!isspace(c)))
            exit(EXIT_FAILURE);
        if(!isxdigit(c))
            continue;

        if(char_counter % 2 == 0)    // another 2 char sequence
            hex_null(&h);

        h.hex[char_counter % 2] = c;

        if(char_counter % 2)     // I have 2 hex symbols now convert to char
            printf("%c", (char)strtol(h.hex, NULL, 16));
        ++char_counter;
    }

    // len(h.hex) == 1 <=> hex has only on symbol, so convert and print it
    if(string_len(h.hex) == 1)
        printf("%c", (char)strtol(h.hex, NULL, 16));
}

void action_split(unsigned int word_size)
{
    // set interval to word_size although it is not needed, because it works up to int / 2 - 1
    if(word_size <= split_minimum_word_len || word_size >= split_maximum_word_len) {
        print_help();
        return;
    }

    register int c;
    register unsigned int number_of_printable_chars = 0;
    char buffer[word_size + 1];     // init and clear buffer
    buffer[word_size] = '\0';

    while((c = getchar()) != EOF && word_size > 0) {
        if(number_of_printable_chars == word_size)    // indicate that the word exceeded word_size
            printf("%s", buffer);

        if(isprint(c) || isblank(c)) {
            if(number_of_printable_chars < word_size)    // fill buffer
                buffer[number_of_printable_chars] = c;
            else                                      // print everything beyond filled buffer
                printf("%c", c);
            ++number_of_printable_chars;
        }

        else {      // prepare data for new word
            if(number_of_printable_chars >= word_size)
                printf("\n");
            number_of_printable_chars = 0;
        }
    }
}

void action_default(uint32 address, int count)
{
    const int one_line_len = 16;
    const int half_one_line_len = one_line_len / 2;
    register int line_char_count = 0;
    register int c = '\0';
    char buffer[] = "                ";

    if(count == 0)
        return;

    // skip characters
    for(uint32 i = address; i--;) {
        if(getchar() == EOF)
            return;
    }

    while (c != EOF) {
        c = getchar();

        if(line_char_count == one_line_len || c == EOF || count == 0) {
            nprintf(' ', (16 - line_char_count) * 3);
            nprintf(' ', line_char_count <= half_one_line_len);
            printf(" |%s|\n", buffer);
            string_fill(' ', buffer);   // clear buffer

            line_char_count = 0;

            if(c == EOF || count == 0)  // beacause EOF == -1 -> c <= 0
                break;
        }

        if(line_char_count == half_one_line_len)
            printf(" ");

        else if(line_char_count == 0)
            printf("%08x  ", address);     // print addr

        buffer[line_char_count] = (isprint(c)) ?c :'.';
        printf("%02x ", c);

        ++line_char_count;
        ++address;
        --count;
    }
}

void print_help()
{
    fprintf(stderr, "HELP: Allowed combinations of flags and parameters are follow:\n"
           "\t1. [-s M] [-n N]\n"
           "\t2. -r\n"
           "\t3. -S N, N > 0 and  N < 200\n"
           "\t4. -x\n\n");
}

void print_error(Errors err)
{
    if(err == UNEXPECTED_PARAMETER_ERROR)
        fprintf(stderr, "ERROR: Unexpected parameter in program arguments\n");
    else if(err == UNKNOWN_INPUT_ERROR)
        fprintf(stderr, "ERROR: Unknown input in program arguments\n");
    else if(err == FLAG_NOT_EXPECT_PARAMETER_ERROR)
        fprintf(stderr, "ERROR: Flag do not expect parameter\n");
    else if(err == MISSING_FLAG_PARAMETER)
        fprintf(stderr, "ERROR: Missing flag parameter\n");
    else if(err == FLAG_DUPLICATION)
        fprintf(stderr, "ERROR: Flag duplication\n");
}

int run_actions(int flags, int* params)
{
    if(((flags & (SKIP | NUMBER_OF_CHARS)) || flags == DEFAULT) &&
            (flags & (~(SKIP | NUMBER_OF_CHARS))) == DEFAULT) {
        // replace if -n N is not present, rewrite param from 0 to -1 to ignore count
        const int n_param = (params[(int)NUMBER_OF_CHARS] == 0 &&
                                  (flags & NUMBER_OF_CHARS) == 0)
                                  ?-1 :params[(int)NUMBER_OF_CHARS];
        action_default(params[(int)SKIP], n_param);
    }
    else if(flags == REVERSE)
        action_reverse();
    else if (flags == SPLIT)
        action_split(params[(int)SPLIT]);
    else if(flags == UNFORMATED_HEX)
        action_unformated_hex();
    // not allowed combinations of flags
    else {
        fprintf(stderr, "ERROR: Your combination of flags is not allowed\n");
        print_help();
        //return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

// *********IMPLEMENTATION OF TESTS*********
#ifdef NDEBUG
void test_math_api()
{
    TST_CASE(
        "ippow",
        TST_COMPARE(ippow(2, 2), 4);
        TST_COMPARE(ippow(5, 0), 1);
        TST_COMPARE(ippow(-3, 3), -27);
        TST_COMPARE(ippow(3, 3), 27);
        TST_COMPARE(ippow(4, 2), 16);
        TST_COMPARE(ippow(0, 4), 0);
    );
}

void test_string_api()
{
    TST_CASE(
        "string_len",
        TST_COMPARE(string_len("ahoj"), 4);
        TST_COMPARE(string_len("ahojky"), 6);
        TST_COMPARE(string_len(""), 0);
    );

    TST_CASE(
        "string compare",
        TST_VERIFY(string_compare("ahoj", "ahoj"));
        TST_VERIFY(!string_compare("ahojky", "ahoj"));
        TST_VERIFY(!string_compare("ahoj", "ahojky"));
        TST_VERIFY(!string_compare("jab", "ahojky"));
    );

    TST_CASE(
        "string_contain",
        TST_VERIFY(string_contain('a', "ahojf"));
        TST_VERIFY(!string_contain('1', "ahojf"));
        TST_VERIFY(!string_contain('k', "ahojf"));
        TST_VERIFY(string_contain('j', "ahojf"));
    );

    TST_CASE(
        "string_count",
        TST_COMPARE(string_count('a', "ahoj"), 1);
        TST_COMPARE(string_count('a', "ahaoja"), 3);
        TST_COMPARE(string_count('a', "ahoAj"), 1);
        TST_COMPARE(string_count('a', "ahoaj"), 2);
        TST_COMPARE(string_count('d', "ahoj"), 0);
        TST_COMPARE(string_count('6', "ahoj"), 0);
    );

    TST_CASE(
        "string_is_number",
        TST_VERIFY(string_is_number("0"));
        TST_VERIFY(string_is_number("9"));
        TST_VERIFY(string_is_number("6659"));
        TST_VERIFY(string_is_number("90"));
        TST_VERIFY(string_is_number("3434"));
        TST_VERIFY(string_is_number("33432"));
        TST_VERIFY(!string_is_number("3423a"));
        TST_VERIFY(!string_is_number("a"));
        TST_VERIFY(!string_is_number("aaa"));
        TST_VERIFY(!string_is_number("a4545"));
        TST_VERIFY(!string_is_number("45ca454"));
        TST_VERIFY(!string_is_number("aa54534a"));
        TST_VERIFY(!string_is_number("a1aa"));
    );

    TST_CASE(
        "string_to_number",
        TST_COMPARE(string_to_number("443432a"), -1);
        TST_COMPARE(string_to_number("4434y32"), -1);
        TST_COMPARE(string_to_number("a443432"), -1);
        TST_COMPARE(string_to_number("4432"), 4432);
        TST_COMPARE(string_to_number("09"), 9);
        TST_COMPARE(string_to_number("009"), 9);
        TST_COMPARE(string_to_number("090"), 90);
        TST_COMPARE(string_to_number("123"), 123);
        TST_COMPARE(string_to_number("2"), 2);
        TST_COMPARE(string_to_number("0"), 0);
    );

    char str1[] = "ahoj";
    char str2[] = "df dsf";
    string_fill('a', str1);
    string_fill('a', str2);

    TST_CASE(
        "string_fill",
        TST_VERIFY(string_compare(str1, "aaaa"));
        TST_VERIFY(string_compare(str2, "aaaaaa"));
    );
}

void test_hex_api()
{
    Hex8 h;
    h.hex[0] = 'a';
    h.hex[1] = 'b';

    hex_null(&h);

    TST_CASE(
        "hex_null",
        TST_COMPARE(h.hex[0], '\0');
        TST_COMPARE(h.hex[1], '\0');
        TST_COMPARE(h.hex[2], '\0');
    );
}

void test_flag_api()
{
    TST_CASE(
        "is_flag",
        TST_VERIFY(is_flag("-s"));
        TST_VERIFY(is_flag("-sa"));
        TST_VERIFY(!is_flag("s"));
        TST_VERIFY(!is_flag(" s"));
        TST_VERIFY(!is_flag("/s"));
        TST_VERIFY(!is_flag(" a "));
    );

    TST_CASE(
        "distinguish_flag",
        TST_COMPARE(distinguish_action("-s"), SKIP);
        TST_COMPARE(distinguish_action("-n"), NUMBER_OF_CHARS);
        TST_COMPARE(distinguish_action("-x"), UNFORMATED_HEX);
        TST_COMPARE(distinguish_action("-S"), SPLIT);
        TST_COMPARE(distinguish_action("-r"), REVERSE);
        TST_COMPARE(distinguish_action("-a"), UNDEFINED);
        TST_COMPARE(distinguish_action("1"), UNDEFINED);
        TST_COMPARE(distinguish_action(" a "), UNDEFINED);
        TST_COMPARE(distinguish_action("-1"), UNDEFINED);
    );

    TST_CASE(
        "flag_accept_param",
        TST_VERIFY(!flag_accept_param(UNDEFINED));
        TST_VERIFY(flag_accept_param(SKIP));
        TST_VERIFY(flag_accept_param(NUMBER_OF_CHARS));
        TST_VERIFY(!flag_accept_param(UNFORMATED_HEX));
        TST_VERIFY(flag_accept_param(SPLIT));
        TST_VERIFY(!flag_accept_param(REVERSE));
    );

    TST_CASE(
        "flag_require_param",
        TST_VERIFY(!flag_require_param(UNDEFINED));
        TST_VERIFY(flag_require_param(SKIP));
        TST_VERIFY(flag_require_param(NUMBER_OF_CHARS));
        TST_VERIFY(!flag_require_param(UNFORMATED_HEX));
        TST_VERIFY(flag_require_param(SPLIT));
        TST_VERIFY(!flag_require_param(REVERSE));
    );

    const char* fc1[] = {"file", "1"};
    const char* fc2[] = {"file", "-1", "-x"};
    const char* fc3[] = {"file", "-r", "4"};
    const char* fc4[] = {"file", "-x"};
    const char* fc5[] = {"file", "-S" "-1"};
    const char* fc6[] = {"file", "a"};
    const char* fc7[] = {"file", "-S", "1"};
    const char* fc8[] = {"file", "-S"};
    const char* fc9[] = {"file", "-S", "-x"};
    const char* fc10[] = {"file", "-r", "-r"};
    const char* fc11[] = {"file", "-n", "6", "-n", "2"};

    TST_CASE(
        "flags_validation",
        TST_COMPARE(flags_validation(2, fc1), UNEXPECTED_PARAMETER_ERROR);
        TST_COMPARE(flags_validation(3, fc2), UNKNOWN_INPUT_ERROR);
        TST_COMPARE(flags_validation(3, fc3), FLAG_NOT_EXPECT_PARAMETER_ERROR);
        TST_COMPARE(flags_validation(2, fc4), NO_ERROR);
        TST_COMPARE(flags_validation(3, fc5), UNKNOWN_INPUT_ERROR);
        TST_COMPARE(flags_validation(2, fc6), UNKNOWN_INPUT_ERROR);
        TST_COMPARE(flags_validation(3, fc7), NO_ERROR);
        TST_COMPARE(flags_validation(2, fc8), MISSING_FLAG_PARAMETER);
        TST_COMPARE(flags_validation(3, fc9), MISSING_FLAG_PARAMETER);
        TST_COMPARE(flags_validation(3, fc10), FLAG_DUPLICATION);
        TST_COMPARE(flags_validation(5, fc11), FLAG_DUPLICATION);
    );

    TST_CASE(
        "flag_is_allowed",
        TST_COMPARE(flag_is_allowed("-s"), true);
        TST_COMPARE(flag_is_allowed("-n"), true);
        TST_COMPARE(flag_is_allowed("-x"), true);
        TST_COMPARE(flag_is_allowed("-S"), true);
        TST_COMPARE(flag_is_allowed("-r"), true);
        TST_COMPARE(flag_is_allowed("-a"), false);
        TST_COMPARE(flag_is_allowed("a"), false);
        TST_COMPARE(flag_is_allowed("-1"), false);
        TST_COMPARE(flag_is_allowed("1"), false);
        TST_COMPARE(flag_is_allowed("-n6"), false);
    );

    int params[17];
    const char* d_test_arg[] = {"file"};
    const char* S1_test_arg[] = {"file", "-S"};
    const char* S2_test_arg[] = {"file", "-S", "4"};
    const char* r1_test_arg[] = {"file", "-r", "4"};
    const char* r2_test_arg[] = {"file", "-r"};
    const char* Ss_test_arg[] = {"file", "-S", "3", "-s"};
    const char* ns_test_arg[] = {"file", "-n", "3", "-s", "5"};

    TST_CASE(
        "parse_arguments",
        TST_COMPARE(parse_arguments(2, S1_test_arg, params), SPLIT);
        TST_COMPARE(params[(int)SPLIT], -2);

        TST_COMPARE(parse_arguments(3, S2_test_arg, params), SPLIT);
        TST_COMPARE(params[(int)SPLIT], 4);

        TST_COMPARE(parse_arguments(3, r1_test_arg, params), REVERSE);
        TST_COMPARE(params[(int)REVERSE], -1);

        TST_COMPARE(parse_arguments(2, r2_test_arg, params), REVERSE);
        TST_COMPARE(params[(int)REVERSE], 0);

        TST_COMPARE(parse_arguments(4, Ss_test_arg, params), SKIP | SPLIT);
        TST_COMPARE(params[(int)SPLIT], 3);
        TST_COMPARE(params[(int)SKIP], -2);

        TST_COMPARE(parse_arguments(1, d_test_arg, params), DEFAULT);

        TST_COMPARE(parse_arguments(5, ns_test_arg, params), SKIP | NUMBER_OF_CHARS);
        TST_COMPARE(params[(int)SKIP], 5);
        TST_COMPARE(params[(int)NUMBER_OF_CHARS], 3);
    );

}
#endif
