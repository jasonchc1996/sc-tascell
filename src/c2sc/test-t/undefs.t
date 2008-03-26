/*
 *  undefs.t:
 * 1998/08          made public                                 kmatsui
 * 2002/12          slightly revised                            kmatsui
 * 2003/11          slightly revised                            kmatsui
 *
 *   Each of the following texts causes undefined behavior.  Good pre-
 * processor will diagnose or warn at these texts, or at least document the
 * behavior on these cases.
 */

#define str( a)     # a

/* u.1.1:   End of a source file without <newline>. */
#include    "unbal3.h"
int e_1;

/* u.1.2:   End of a source file with <backslash><newline>. */
#include    "unbal4.h"
;

/* u.1.3:   End of a source file with an unterminated comment.  */
#include    "unbal5.h"
*/

/* u.1.4:   End of a source file with an uncompleted macro call.    */
#include    "unbal6.h"
    y);

/* u.1.5:   Illegal characters (in other than string literal, character
        constant, header-name or comment).  */
#if     1 ||2
/*     0x1e ^ ^ 0x1f    */
#endif  /* Maybe the second error.  */

/* u.1.6:   [VT], [FF] in directive line.   */
#if     1 ||2
/*     [VT] ^ ^ [FF]    */
#endif  /* Maybe the second error.  */

/* u.1.7:   Invalid multi-byte character sequence (in string literal,
        character constant, header-name or comment).    */
#if     '�== 0x8e
/*       ^ 0x8e */
#endif  /* Maybe the second error.  */

/* u.1.8:   Unterminated character constant.    */
/*  The following "comment" may not interpreted as a comment but swallowed by
        the unterminated character constant.    */
#error  I can't understand. /* Token error prior to execution of #error.    */

/* u.1.9:   Unterminated string literal.    */
    "String literal
    across the lines.
"

/* u.1.10:  Unterminated header-name.   */
#include    <assert.h

/* u.1.11:  Header-name containing ', ", \ or "/*". */
/*  Probably illegal filename and fails to open.    */
#include    "../*line.h"
/*  \ is a legal path-delimiter in MS-DOS or some other OS's.   */
#include    "..\test-t\line.h"

/* u.1.12:  Argument of #include other than header-name.    */
#include    filename

/* u.1.13:  Excessive argument in #include directive.   */
#include    <assert.h>  Junk

/* u.1.14:  #line directive without an argument of line number. */
#line   "filename"

/* u.1.15:  #line directive with the second argument of other than string
        literal.    */
#line   1234    filename

/* u.1.16:  Excessive argument in #line directive.  */
#line   2345    "filename"  Junk

/*  83; "undefs.t"; or other undefined results. */
    __LINE__; __FILE__;

/* u.1.17:  Line number argument of #line directive should be in range of
        [1,32767].  */
/* In C99, range of line number is [1,2147483647]   */
#if __STDC_VERSION__ < 199901L
#line   32767   /* valid here   */
/* line 32767   */
/* line 32768 ? : out of range  */
    __LINE__;   /* 32769 ? or -32767 ?, maybe warned as an out-of-range */
#line   0
#line   32768
#else
#line   2147483647  /* valid here   */
/* line 2147483647  */
/* line 2147483648 ? : out of range */
    __LINE__;   /* 2147483649 ? or -2147483647 ?,
                maybe warned as an out-of-range */
#line   0
#line   2147483648
#endif

/* u.1.18:  Line number argument of #line directive should be written in
        decimal digits. */
#line   0x1000

#line   109     /* Restore to correct line number.  */

/* u.1.19:  A macro expanded to "defined" in #if expression.    */
#define DEFINED     defined
#if     DEFINED DEFINED
#endif

#undef  __linux__
#undef  __arm__
#define __linux__   1
#define HAVE_MREMAP defined(__linux__) && !defined(__arm__)
/* Wrong macro definition.
 * This macro should be defined as follows.
 *  #if defined(__linux__) && !defined(__arm__)
 *  #define HAVE_MREMAP 1
 *  #endif
 */
#if HAVE_MREMAP
    mremap();
#endif

/* u.1.20:  Undefining __FILE__, __LINE__, __DATE__, __TIME__, __STDC__,
        __STDC_VERSION__ or "defined" in #undef directive.  */
#undef  __LINE__
/*  134; or other undefined result. */
    __LINE__;

/* u.1.21:  Defining __FILE__, __LINE__, __DATE__, __TIME__, __STDC__,
        __STDC_VERSION__ or "defined" in #define directive. */
#define __LINE__    1234
/*  140; or other undefined result. */
    __LINE__;
#define defined     defined
#if     defined defined
#   error   I am not a good preprocessor.
#endif

/* u.1.22:  Result of ## operator is not a valid pp-token.  */
#define NUM( dig, exp)  dig ## E+ ## exp
/*
 *   "E+" is a sequence of two pp-tokens "E" and "+", not a single pp-token.
 * The first ## concatenates the last pp-token of first argument with "E",
 * and the second ## concatenates "+" with the first pp-token of the second
 * argument.
 *   While "12E" (one of the sequence generated by the token concatenation)
 * is a valid pp-token, "+34" (the another sequence) is not a valid pp-token
 * and causes an undefined behavior.
 */
    NUM( 12, 34);

/* u.1.23:  Result of # operator is not a valid string literal. */
/*  "\\"\"";    This sequence is parsed to three tokens "\\" \ "", and will be
        diagnosed by compiler-proper unless diagnosed by preprocessor.  */
    str( \"");

/* u.1.24:  Empty argument of macro call.   */
/*
 *   Empty argument is permitted in C99.
 *   Note: Since no argument and one empty argument cannot be distinguished
 * syntactically, additional dummy argument may be necessary for an
 * intermediate macro to process one empty argument (if possible).
 */

#if __STDC_VERSION__ < 199910L
#define ARG( a, dummy)      # a
#define EMPTY
#define SHOWN( n)       printf( "%s : %d\n", # n, n)
#define SHOWS( s)       printf( "%s : %s\n", # s, ARG( s, dummy))
#define add( a, b)      (a + b)
#define sub( a, b)      (a - b)
#define math( op, a, b)     op( a, b)
#define APPEND( a, b)       a ## b

/*  printf( "%s : %d\n", "math( sub, , y)", ( - y));
        or other undefined behavior.    */
    SHOWN( math( sub, , y));

/*  printf( "%s : %s\n", "EMPTY", "");
        or other undefined behavior.    */
    SHOWS( EMPTY);

/*  printf( "%s : %s\n", "APPEND( CON, 1)", "CON1");    */
    SHOWS( APPEND( CON, 1));

/*  printf( "%s : %s\n", "APPEND( CON, )", "CON");
        or other undefined behavior.    */
    SHOWS( APPEND( CON, ));

/*  printf( "%s : %s\n", "APPEND( , )", "");
        or other undefined behavior.    */
    SHOWS( APPEND( , ));
#endif

/* u.1.25:  Macro argument otherwise parsed as a directive. */
/*  "#define NAME"; or other undefined behaviour.   */
    str(
#define NAME
    );

#if 0   /* Added by C90: Corrigendum 1 (1994) and deleted by C99    */
/* u.1.26:  Expanded macro replacement list end with name of function-like
        macro.  */
#define SUB             sub
    SUB( a, b);
#endif

/* u.1.27:  Unknown preprocessing directive (other than #pragma).   */
#ifdefined MACRO
#endif              /* The second error.    */

#define D   define
/* u.1.28:  There are following two ways of preprocessing.
    1:  "D" isn't expanded, because # is the first token of the line.
        Preprocessor reports that "D" is an unknown directive.
    2:  "D" is expanded, because that is not a directive.
        Compiler-phase will diagnose the output of preprocess.
    Anyway, preprocessor should not interprete this line as a preprocessing
    directive.
 */
#D  A   B

/* u.2.1:   Undefined escape sequence.  */
#if     '\x'
#endif

/* u.2.2:   Illegal bit shift count.    */
#if     1 << -1
#endif
#if     1 << 64
#endif
