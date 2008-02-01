(%line 1 "csample/posix-header/stdio.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_null.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/machine/_types.h")
(%line 45 "/usr/include/machine/_types.h")
(deftype csym::__int8_t signed-char)
(deftype csym::__uint8_t unsigned-char)
(deftype csym::__int16_t short)
(deftype csym::__uint16_t unsigned-short)
(deftype csym::__int32_t int)
(deftype csym::__uint32_t unsigned-int)
(deftype csym::__int64_t int)
(deftype csym::__uint64_t unsigned-int)
(%line 70 "/usr/include/machine/_types.h")
(deftype csym::__clock_t unsigned-long)
(deftype csym::__cpumask_t unsigned-int)
(deftype csym::__critical_t csym::__int32_t)
(deftype csym::__double_t double)
(deftype csym::__float_t double)
(deftype csym::__intfptr_t csym::__int32_t)
(deftype csym::__intmax_t csym::__int64_t)
(deftype csym::__intptr_t csym::__int32_t)
(deftype csym::__int_fast8_t csym::__int32_t)
(deftype csym::__int_fast16_t csym::__int32_t)
(deftype csym::__int_fast32_t csym::__int32_t)
(deftype csym::__int_fast64_t csym::__int64_t)
(deftype csym::__int_least8_t csym::__int8_t)
(deftype csym::__int_least16_t csym::__int16_t)
(deftype csym::__int_least32_t csym::__int32_t)
(deftype csym::__int_least64_t csym::__int64_t)
(deftype csym::__ptrdiff_t csym::__int32_t)
(deftype csym::__register_t csym::__int32_t)
(deftype csym::__segsz_t csym::__int32_t)
(deftype csym::__size_t csym::__uint32_t)
(deftype csym::__ssize_t csym::__int32_t)
(deftype csym::__time_t csym::__int32_t)
(deftype csym::__uintfptr_t csym::__uint32_t)
(deftype csym::__uintmax_t csym::__uint64_t)
(deftype csym::__uintptr_t csym::__uint32_t)
(deftype csym::__uint_fast8_t csym::__uint32_t)
(deftype csym::__uint_fast16_t csym::__uint32_t)
(deftype csym::__uint_fast32_t csym::__uint32_t)
(deftype csym::__uint_fast64_t csym::__uint64_t)
(deftype csym::__uint_least8_t csym::__uint8_t)
(deftype csym::__uint_least16_t csym::__uint16_t)
(deftype csym::__uint_least32_t csym::__uint32_t)
(deftype csym::__uint_least64_t csym::__uint64_t)
(deftype csym::__u_register_t csym::__uint32_t)
(deftype csym::__vm_offset_t csym::__uint32_t)
(deftype csym::__vm_ooffset_t csym::__int64_t)
(deftype csym::__vm_paddr_t csym::__uint32_t)
(deftype csym::__vm_pindex_t csym::__uint64_t)
(deftype csym::__vm_size_t csym::__uint32_t)
(%line 118 "/usr/include/machine/_types.h")
(deftype csym::__va_list csym::__builtin_va_list)
(%line 124 "/usr/include/machine/_types.h")
(deftype csym::__gnuc_va_list csym::__va_list)
(%line 38 "/usr/include/sys/_types.h")
(deftype csym::__clockid_t csym::__int32_t)
(deftype csym::__fflags_t csym::__uint32_t)
(deftype csym::__fsblkcnt_t csym::__uint64_t)
(deftype csym::__fsfilcnt_t csym::__uint64_t)
(deftype csym::__gid_t csym::__uint32_t)
(deftype csym::__id_t csym::__int64_t)
(deftype csym::__ino_t csym::__uint32_t)
(deftype csym::__key_t long)
(deftype csym::__lwpid_t csym::__int32_t)
(deftype csym::__mode_t csym::__uint16_t)
(deftype csym::__nl_item int)
(deftype csym::__nlink_t csym::__uint16_t)
(deftype csym::__off_t csym::__int64_t)
(deftype csym::__pid_t csym::__int32_t)
(deftype csym::__rlim_t csym::__int64_t)
(deftype csym::__sa_family_t csym::__uint8_t)
(deftype csym::__socklen_t csym::__uint32_t)
(deftype csym::__suseconds_t long)
(deftype csym::__timer_t csym::__int32_t)
(deftype csym::__uid_t csym::__uint32_t)
(deftype csym::__useconds_t unsigned-int)
(%line 78 "/usr/include/sys/_types.h")
(deftype csym::__ct_rune_t int)
(deftype csym::__rune_t csym::__ct_rune_t)
(deftype csym::__wchar_t csym::__ct_rune_t)
(deftype csym::__wint_t csym::__ct_rune_t)
(deftype csym::__dev_t csym::__uint32_t)
(deftype csym::__fixpt_t csym::__uint32_t)
(%line 91 "/usr/include/sys/_types.h")
(def (union csym::unnamed0) (decl csym::__mbstate8 (array char 128))
 (decl csym::_MBSTATE@l csym::__int64_t))
(deftype csym::__mbstate_t (union csym::unnamed0))
(%line 47 "csample/posix-header/stdio.h")
(deftype csym::fpos_t csym::__off_t)
(deftype csym::size_t csym::__size_t)
(deftype csym::va_list csym::__va_list)
(%line 70 "csample/posix-header/stdio.h")
(def (struct csym::__sbuf) (decl csym::_base (ptr unsigned-char))
 (decl csym::_size int))
(%line 102 "csample/posix-header/stdio.h")
(def (struct csym::__S@file) (decl csym::_p (ptr unsigned-char))
 (decl csym::_r int) (decl csym::_w int) (decl csym::_flags short)
 (decl csym::_file short) (decl csym::_bf (struct csym::__sbuf))
 (decl csym::_lbfsize int) (decl csym::_cookie (ptr void))
 (decl csym::_close (ptr (fn int (ptr void))))
 (decl csym::_read (ptr (fn int (ptr void) (ptr char) int)))
 (decl csym::_seek (ptr (fn csym::fpos_t (ptr void) csym::fpos_t int)))
 (decl csym::_write (ptr (fn int (ptr void) (ptr (const char)) int)))
 (decl csym::_ub (struct csym::__sbuf))
 (decl csym::_extra (ptr (struct csym::__S@filex))) (decl csym::_ur int)
 (decl csym::_ubuf (array unsigned-char 3))
 (decl csym::_nbuf (array unsigned-char 1))
 (decl csym::_lb (struct csym::__sbuf)) (decl csym::_blksize int)
 (decl csym::_offset csym::fpos_t))
(deftype csym::@FILE (struct csym::__S@file))
(extern-decl csym::__stdinp (ptr csym::@FILE))
(extern-decl csym::__stdoutp (ptr csym::@FILE))
(extern-decl csym::__stderrp (ptr csym::@FILE))
(%line 208 "csample/posix-header/stdio.h")
(%line 212 "csample/posix-header/stdio.h")
(decl (csym::clearerr) (fn void (ptr csym::@FILE)))
(decl (csym::fclose) (fn int (ptr csym::@FILE)))
(decl (csym::feof) (fn int (ptr csym::@FILE)))
(decl (csym::ferror) (fn int (ptr csym::@FILE)))
(decl (csym::fflush) (fn int (ptr csym::@FILE)))
(decl (csym::fgetc) (fn int (ptr csym::@FILE)))
(decl (csym::fgetpos) (fn int (ptr csym::@FILE) (ptr csym::fpos_t)))
(decl (csym::fgets) (fn (ptr char) (ptr char) int (ptr csym::@FILE)))
(decl (csym::fopen)
 (fn (ptr csym::@FILE) (ptr (const char)) (ptr (const char))))
(decl (csym::fprintf) (fn int (ptr csym::@FILE) (ptr (const char)) va-arg))
(decl (csym::fputc) (fn int int (ptr csym::@FILE)))
(decl (csym::fputs) (fn int (ptr (const char)) (ptr csym::@FILE)))
(decl (csym::fread)
 (fn csym::size_t (ptr void) csym::size_t csym::size_t (ptr csym::@FILE)))
(decl (csym::freopen)
 (fn (ptr csym::@FILE) (ptr (const char)) (ptr (const char)) (ptr csym::@FILE)))
(decl (csym::fscanf) (fn int (ptr csym::@FILE) (ptr (const char)) va-arg))
(decl (csym::fseek) (fn int (ptr csym::@FILE) long int))
(decl (csym::fsetpos) (fn int (ptr csym::@FILE) (ptr (const csym::fpos_t))))
(decl (csym::ftell) (fn long (ptr csym::@FILE)))
(decl (csym::fwrite)
 (fn csym::size_t (ptr (const void)) csym::size_t csym::size_t
  (ptr csym::@FILE)))
(decl (csym::getc) (fn int (ptr csym::@FILE)))
(decl (csym::getchar) (fn int void))
(decl (csym::gets) (fn (ptr char) (ptr char)))
(decl (csym::perror) (fn void (ptr (const char))))
(decl (csym::printf) (fn int (ptr (const char)) va-arg))
(decl (csym::putc) (fn int int (ptr csym::@FILE)))
(decl (csym::putchar) (fn int int))
(decl (csym::puts) (fn int (ptr (const char))))
(decl (csym::remove) (fn int (ptr (const char))))
(decl (csym::rename) (fn int (ptr (const char)) (ptr (const char))))
(decl (csym::rewind) (fn void (ptr csym::@FILE)))
(decl (csym::scanf) (fn int (ptr (const char)) va-arg))
(decl (csym::setbuf) (fn void (ptr csym::@FILE) (ptr char)))
(decl (csym::setvbuf) (fn int (ptr csym::@FILE) (ptr char) int csym::size_t))
(decl (csym::sprintf) (fn int (ptr char) (ptr (const char)) va-arg))
(decl (csym::sscanf) (fn int (ptr (const char)) (ptr (const char)) va-arg))
(decl (csym::tmpfile) (fn (ptr csym::@FILE) void))
(decl (csym::tmpnam) (fn (ptr char) (ptr char)))
(decl (csym::ungetc) (fn int int (ptr csym::@FILE)))
(decl (csym::vfprintf)
 (fn int (ptr csym::@FILE) (ptr (const char)) csym::__va_list))
(decl (csym::vprintf) (fn int (ptr (const char)) csym::__va_list))
(decl (csym::vsprintf) (fn int (ptr char) (ptr (const char)) csym::__va_list))
(decl (csym::snprintf)
 (fn int (ptr char) csym::size_t (ptr (const char)) va-arg))
(decl (csym::vfscanf)
 (fn int (ptr csym::@FILE) (ptr (const char)) csym::__va_list))
(decl (csym::vscanf) (fn int (ptr (const char)) csym::__va_list))
(decl (csym::vsnprintf)
 (fn int (ptr char) csym::size_t (ptr (const char)) csym::__va_list))
(decl (csym::vsscanf)
 (fn int (ptr (const char)) (ptr (const char)) csym::__va_list))
(%line 279 "csample/posix-header/stdio.h")
(decl (csym::ctermid) (fn (ptr char) (ptr char)))
(decl (csym::fdopen) (fn (ptr csym::@FILE) int (ptr (const char))))
(decl (csym::fileno) (fn int (ptr csym::@FILE)))
(decl (csym::pclose) (fn int (ptr csym::@FILE)))
(decl (csym::popen)
 (fn (ptr csym::@FILE) (ptr (const char)) (ptr (const char))))
(decl (csym::ftrylockfile) (fn int (ptr csym::@FILE)))
(decl (csym::flockfile) (fn void (ptr csym::@FILE)))
(decl (csym::funlockfile) (fn void (ptr csym::@FILE)))
(%line 298 "csample/posix-header/stdio.h")
(decl (csym::getc_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::getchar_unlocked) (fn int void))
(decl (csym::putc_unlocked) (fn int int (ptr csym::@FILE)))
(decl (csym::putchar_unlocked) (fn int int))
(decl (csym::clearerr_unlocked) (fn void (ptr csym::@FILE)))
(decl (csym::feof_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::ferror_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::fileno_unlocked) (fn int (ptr csym::@FILE)))
(decl (csym::fseeko) (fn int (ptr csym::@FILE) csym::__off_t int))
(decl (csym::ftello) (fn csym::__off_t (ptr csym::@FILE)))
(decl (csym::getw) (fn int (ptr csym::@FILE)))
(decl (csym::putw) (fn int int (ptr csym::@FILE)))
(decl (csym::tempnam) (fn (ptr char) (ptr (const char)) (ptr (const char))))
(%line 328 "csample/posix-header/stdio.h")
(decl (csym::asprintf) (fn int (ptr (ptr char)) (ptr (const char)) va-arg))
(decl (csym::ctermid_r) (fn (ptr char) (ptr char)))
(decl (csym::fgetln) (fn (ptr char) (ptr csym::@FILE) (ptr csym::size_t)))
(%line 336 "csample/posix-header/stdio.h")
(decl (csym::fmtcheck)
 (fn (ptr (const char)) (ptr (const char)) (ptr (const char))))
(decl (csym::fpurge) (fn int (ptr csym::@FILE)))
(decl (csym::setbuffer) (fn void (ptr csym::@FILE) (ptr char) int))
(decl (csym::setlinebuf) (fn int (ptr csym::@FILE)))
(decl (csym::vasprintf)
 (fn int (ptr (ptr char)) (ptr (const char)) csym::__va_list))
(%line 348 "csample/posix-header/stdio.h")
(extern-decl csym::sys_nerr (const int))
(extern-decl csym::sys_errlist (array (const (ptr (const char)))))
(%line 354 "csample/posix-header/stdio.h")
(decl (csym::funopen)
 (fn (ptr csym::@FILE) (ptr (const void))
  (ptr (fn int (ptr void) (ptr char) int))
  (ptr (fn int (ptr void) (ptr (const char)) int))
  (ptr (fn csym::fpos_t (ptr void) csym::fpos_t int))
  (ptr (fn int (ptr void)))))
(%line 367 "csample/posix-header/stdio.h")
(decl (csym::ftruncate) (fn int int csym::__off_t))
(decl (csym::lseek) (fn csym::__off_t int csym::__off_t int))
(decl (csym::mmap)
 (fn (ptr void) (ptr void) csym::size_t int int int csym::__off_t))
(decl (csym::truncate) (fn int (ptr (const char)) csym::__off_t))
(%line 386 "csample/posix-header/stdio.h")
(decl (csym::__srget) (fn int (ptr csym::@FILE)))
(decl (csym::__swbuf) (fn int int (ptr csym::@FILE)))
(%line 395 "csample/posix-header/stdio.h")
(static-def (csym::__sputc csym::_c csym::_p) (fn int int (ptr csym::@FILE))
 (if
  (or (>= (-- (fref (mref csym::_p) csym::_w)) 0)
   (and
    (>= (fref (mref csym::_p) csym::_w) (fref (mref csym::_p) csym::_lbfsize))
    (!= (cast char csym::_c) #\Newline)))
  (return (= (mref (inc (fref (mref csym::_p) csym::_p))) csym::_c))
  (return (csym::__swbuf csym::_c csym::_p))))
(%line 420 "csample/posix-header/stdio.h")
(extern-decl csym::__isthreaded int)
(%defconstant csym::i386 1)
(%defconstant csym::unix 1)
(%defconstant csym::|__I386| 1)
(%defconstant csym::|__I386__| 1)
(%defconstant csym::__unix 1)
(%defconstant csym::__unix__ 1)
(%defconstant csym::__@F@ree@BSD__ 1)
(%defconstant csym::__@GNUC__ 3)
(%defconstant csym::__@GNUC_MINOR__ 4)
(%defconstant csym::__@SIZE_TYPE__ unsigned-int)
(%defconstant csym::__@PTRDIFF_TYPE__ int)
(%defconstant csym::__@WCHAR_TYPE__ int)
(%defconstant csym::__@GNUG__ 3)
(%defconstant csym::__@LINE__ (- 1234567890))
(%defconstant csym::__@FILE__ (c-exp ""))
(%defconstant csym::__@DATE__ "Feb 13 2005")
(%defconstant csym::__@TIME__ "14:27:36")
(%defconstant csym::__@STDC__ 1)
(%defconstant csym::__@STDC_HOSTED__ 1)
(%defconstant csym::__@MCPP 2)
(%defconstant csym::time_t double)
(%defconstant csym::pid_t long)
(%defconstant csym::__complex__ (c-exp ""))
(%defmacro csym::__extension__ (csym::_arg0) csym::_arg0)
(%defmacro csym::__attribute__ (csym::_arg0)
 (common-lisp:list 'c-exp "" csym::_arg0))
(%defconstant csym::__inline (c-exp ""))
(%defmacro csym::__asm (csym::_arg0) (common-lisp:list 'c-exp "" csym::_arg0))
(%undef csym::i386)
(%undef csym::unix)
(%undef csym::__@GNUG__)
(%defconstant csym::_@STDIO_H_ (c-exp ""))
(%defconstant csym::_@SYS_CDEFS_H_ (c-exp ""))
(%defconstant csym::__@BEGIN_DECLS (c-exp ""))
(%defconstant csym::__@END_DECLS (c-exp ""))
(%defmacro csym::__@GNUC_PREREQ__ (csym::_arg0 csym::_arg1)
 (common-lisp:list 'or
                   (common-lisp:list '> 'csym::__@GNUC__ csym::_arg0)
                   (common-lisp:list 'and
                                     (common-lisp:list '==
                                                       'csym::__@GNUC__
                                                       csym::_arg0)
                                     (common-lisp:list '>=
                                                       'csym::__@GNUC_MINOR__
                                                       csym::_arg1))))
(%defmacro csym::__@P (csym::_arg0) csym::_arg0)
(%defmacro csym::__@CONCAT1 (csym::_arg0 csym::_arg1)
 '"<macro with ## operator>")
(%defmacro csym::__@CONCAT (csym::_arg0 csym::_arg1)
 (common-lisp:list 'csym::__@CONCAT1 csym::_arg0 csym::_arg1))
(%defmacro csym::__@STRING (csym::_arg0)
 (common-lisp:list 'csym::_@C2SC_STRINGIFY csym::_arg0))
(%defmacro csym::__@XSTRING (csym::_arg0)
 (common-lisp:list 'csym::__@STRING csym::_arg0))
(%defconstant csym::__const (const int))
(%defconstant csym::__signed signed-int)
(%defconstant csym::__volatile (volatile int))
(%defconstant csym::__dead2 (csym::__attribute__ csym::__noreturn__))
(%defconstant csym::__pure2 (csym::__attribute__ csym::__const__))
(%defconstant csym::__unused (csym::__attribute__ csym::__unused__))
(%defconstant csym::__used (csym::__attribute__ csym::__used__))
(%defconstant csym::__packed (csym::__attribute__ csym::__packed__))
(%defmacro csym::__aligned (csym::_arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__aligned__ csym::_arg0)))
(%defmacro csym::__section (csym::_arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__section__ csym::_arg0)))
(%defconstant csym::__pure (csym::__attribute__ csym::__pure__))
(%defconstant csym::__always_inline
 (csym::__attribute__ csym::__always_inline__))
(%defmacro csym::__nonnull (csym::_arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__nonnull__ csym::_arg0)))
(%defconstant csym::__@LONG_LONG_SUPPORTED (c-exp ""))
(%defconstant csym::__restrict (c-exp ""))
(%defmacro csym::__predict_true (csym::_arg0)
 (common-lisp:list 'csym::__builtin_expect csym::_arg0 '1))
(%defmacro csym::__predict_false (csym::_arg0)
 (common-lisp:list 'csym::__builtin_expect csym::_arg0 '0))
(%defmacro csym::__offsetof (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   'csym::size_t
                   (common-lisp:list 'ptr
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           csym::_arg0)
                                                                          '0))
                                                       csym::_arg1))))
(%defmacro csym::__rangeof (csym::_arg0 csym::_arg1 csym::_arg2)
 (common-lisp:list '-
                   (common-lisp:list 'csym::__offsetof csym::_arg0 csym::_arg2)
                   (common-lisp:list 'csym::__offsetof
                                     csym::_arg0
                                     csym::_arg1)))
(%defmacro csym::__printflike (csym::_arg0 csym::_arg1)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__format__
                                     'csym::__printf__
                                     csym::_arg0
                                     csym::_arg1)))
(%defmacro csym::__scanflike (csym::_arg0 csym::_arg1)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__format__
                                     'csym::__scanf__
                                     csym::_arg0
                                     csym::_arg1)))
(%defmacro csym::__printf0like (csym::_arg0 csym::_arg1)
 (common-lisp:list 'c-exp "" csym::_arg0 csym::_arg1))
(%defmacro csym::__strong_reference (csym::_arg0 csym::_arg1)
 (common-lisp:list 'c-exp
                   "extern __typeof ( ~0@*~a ) ~1@*~a __attribute__ ( ( __alias__ ( _C2SC_STRINGIFY ( ~0@*~a ) ) ) ) ;"
                   csym::_arg0
                   csym::_arg1))
(%defmacro csym::__weak_reference (csym::_arg0 csym::_arg1)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".weak \" _C2SC_STRINGIFY ( ~1@*~a ) ) ; __asm__ ( \".equ \" _C2SC_STRINGIFY ( ~1@*~a ) \", \" _C2SC_STRINGIFY ( ~0@*~a ) )"
                   csym::_arg0
                   csym::_arg1))
(%defmacro csym::__warn_references (csym::_arg0 csym::_arg1)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".section .gnu.warning.\" _C2SC_STRINGIFY ( ~0@*~a ) ) ; __asm__ ( \".asciz \\\"\" ~1@*~a \"\\\"\" ) ; __asm__ ( \".previous\" )"
                   csym::_arg0
                   csym::_arg1))
(%defmacro csym::__@IDSTRING (csym::_arg0 csym::_arg1)
 (common-lisp:list 'c-exp
                   "__asm__ ( \".ident\\t\\\"\" ~1@*~a \"\\\"\" )"
                   csym::_arg0
                   csym::_arg1))
(%defmacro csym::__@FBSDID (csym::_arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_
                                     'csym::__@LINE__)
                   csym::_arg0))
(%defmacro csym::__@RCSID (csym::_arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_
                                     'csym::__@LINE__)
                   csym::_arg0))
(%defmacro csym::__@RCSID_SOURCE (csym::_arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_source_
                                     'csym::__@LINE__)
                   csym::_arg0))
(%defmacro csym::__@SCCSID (csym::_arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__sccsid_
                                     'csym::__@LINE__)
                   csym::_arg0))
(%defmacro csym::__@COPYRIGHT (csym::_arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__copyright_
                                     'csym::__@LINE__)
                   csym::_arg0))
(%defmacro csym::__@DECONST (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   csym::_arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'const
                                                                          'void))
                                                       csym::_arg1))))
(%defmacro csym::__@DEVOLATILE (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   csym::_arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'volatile
                                                                          'void))
                                                       csym::_arg1))))
(%defmacro csym::__@DEQUALIFY (csym::_arg0 csym::_arg1)
 (common-lisp:list 'cast
                   csym::_arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'const
                                                                          'volatile
                                                                          'void))
                                                       csym::_arg1))))
(%defconstant csym::__@POSIX_VISIBLE 200112)
(%defconstant csym::__@XSI_VISIBLE 600)
(%defconstant csym::__@BSD_VISIBLE 1)
(%defconstant csym::__@ISO_C_VISIBLE 1999)
(%defconstant csym::@NULL (cast (ptr void) 0))
(%defconstant csym::_@SYS__TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE__TYPES_H_ (c-exp ""))
(%defconstant csym::__@GNUC_VA_LIST (c-exp ""))
(%defconstant csym::_@SIZE_T_DECLARED (c-exp ""))
(%defconstant csym::_@VA_LIST_DECLARED (c-exp ""))
(%defconstant csym::_@FSTDIO (c-exp ""))
(%defconstant csym::_@STDSTREAM_DECLARED (c-exp ""))
(%defconstant csym::__@SLBF 1)
(%defconstant csym::__@SNBF 2)
(%defconstant csym::__@SRD 4)
(%defconstant csym::__@SWR 8)
(%defconstant csym::__@SRW 16)
(%defconstant csym::__@SEOF 32)
(%defconstant csym::__@SERR 64)
(%defconstant csym::__@SMBF 128)
(%defconstant csym::__@SAPP 256)
(%defconstant csym::__@SSTR 512)
(%defconstant csym::__@SOPT 1024)
(%defconstant csym::__@SNPT 2048)
(%defconstant csym::__@SOFF 4096)
(%defconstant csym::__@SMOD 8192)
(%defconstant csym::__@SALC 16384)
(%defconstant csym::__@SIGN 32768)
(%defconstant csym::_@IOFBF 0)
(%defconstant csym::_@IOLBF 1)
(%defconstant csym::_@IONBF 2)
(%defconstant csym::@BUFSIZ 1024)
(%defconstant csym::@EOF (- 1))
(%defconstant csym::@FOPEN_MAX 20)
(%defconstant csym::@FILENAME_MAX 1024)
(%defconstant csym::@P_@tmpdir "/var/tmp/")
(%defconstant csym::@L_@tmpnam 1024)
(%defconstant csym::@TMP_MAX 308915776)
(%defconstant csym::@SEEK_SET 0)
(%defconstant csym::@SEEK_CUR 1)
(%defconstant csym::@SEEK_END 2)
(%defconstant csym::stdin csym::__stdinp)
(%defconstant csym::stdout csym::__stdoutp)
(%defconstant csym::stderr csym::__stderrp)
(%defconstant csym::@L_@cuserid 17)
(%defconstant csym::@L_@ctermid 1024)
(%defconstant csym::__@ATTR_FORMAT_ARG
 (csym::__attribute__ (csym::__format_arg__ 2)))
(%defmacro csym::fropen (csym::_arg0 csym::_arg1)
 (common-lisp:list 'csym::funopen csym::_arg0 csym::_arg1 '0 '0 '0))
(%defmacro csym::fwopen (csym::_arg0 csym::_arg1)
 (common-lisp:list 'csym::funopen csym::_arg0 '0 csym::_arg1 '0 '0))
(%defconstant csym::_@FTRUNCATE_DECLARED (c-exp ""))
(%defconstant csym::_@LSEEK_DECLARED (c-exp ""))
(%defconstant csym::_@MMAP_DECLARED (c-exp ""))
(%defconstant csym::_@TRUNCATE_DECLARED (c-exp ""))
(%defmacro csym::__sgetc (csym::_arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list '<
                                     (common-lisp:list '--
                                                       (common-lisp:list 'fref
                                                                         (common-lisp:list
                                                                          'mref
                                                                          csym::_arg0)
                                                                         'csym::_r))
                                     '0)
                   (common-lisp:list 'csym::__srget csym::_arg0)
                   (common-lisp:list 'cast
                                     'int
                                     (common-lisp:list 'mref
                                                       (common-lisp:list 'inc
                                                                         (common-lisp:list
                                                                          'fref
                                                                          (common-lisp:list
                                                                           'mref
                                                                           csym::_arg0)
                                                                          'csym::_p))))))
(%defmacro csym::__sfeof (csym::_arg0)
 (common-lisp:list '!=
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         csym::_arg0)
                                                       'csym::_flags)
                                     'csym::__@SEOF)
                   '0))
(%defmacro csym::__sferror (csym::_arg0)
 (common-lisp:list '!=
                   (common-lisp:list 'bit-and
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         csym::_arg0)
                                                       'csym::_flags)
                                     'csym::__@SERR)
                   '0))
(%defmacro csym::__sclearerr (csym::_arg0)
 (common-lisp:list 'cast
                   'void
                   (common-lisp:list 'bit-and=
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         csym::_arg0)
                                                       'csym::_flags)
                                     (common-lisp:list 'bit-not
                                                       (common-lisp:list
                                                        'bit-or
                                                        'csym::__@SERR
                                                        'csym::__@SEOF)))))
(%defmacro csym::__sfileno (csym::_arg0)
 (common-lisp:list 'fref (common-lisp:list 'mref csym::_arg0) 'csym::_file))
(%defmacro csym::feof (csym::_arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'csym::__sfeof csym::_arg0)
                   (common-lisp:list 'csym::feof csym::_arg0)))
(%defmacro csym::ferror (csym::_arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'csym::__sferror csym::_arg0)
                   (common-lisp:list 'csym::ferror csym::_arg0)))
(%defmacro csym::clearerr (csym::_arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'csym::__sclearerr csym::_arg0)
                   (common-lisp:list 'csym::clearerr csym::_arg0)))
(%defmacro csym::fileno (csym::_arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'csym::__sfileno csym::_arg0)
                   (common-lisp:list 'csym::fileno csym::_arg0)))
(%defmacro csym::getc (csym::_arg0)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'csym::__sgetc csym::_arg0)
                   (common-lisp:list 'csym::getc csym::_arg0)))
(%defmacro csym::putc (csym::_arg0 csym::_arg1)
 (common-lisp:list 'if-exp
                   (common-lisp:list 'not 'csym::__isthreaded)
                   (common-lisp:list 'csym::__sputc csym::_arg0 csym::_arg1)
                   (common-lisp:list 'csym::putc csym::_arg0 csym::_arg1)))
(%defmacro csym::getchar common-lisp:nil
 (common-lisp:list 'csym::getc 'csym::stdin))
(%defmacro csym::putchar (csym::_arg0)
 (common-lisp:list 'csym::putc csym::_arg0 'csym::stdout))
(%defmacro csym::feof_unlocked (csym::_arg0)
 (common-lisp:list 'csym::__sfeof csym::_arg0))
(%defmacro csym::ferror_unlocked (csym::_arg0)
 (common-lisp:list 'csym::__sferror csym::_arg0))
(%defmacro csym::clearerr_unlocked (csym::_arg0)
 (common-lisp:list 'csym::__sclearerr csym::_arg0))
(%defmacro csym::fileno_unlocked (csym::_arg0)
 (common-lisp:list 'csym::__sfileno csym::_arg0))
(%defmacro csym::getc_unlocked (csym::_arg0)
 (common-lisp:list 'csym::__sgetc csym::_arg0))
(%defmacro csym::putc_unlocked (csym::_arg0 csym::_arg1)
 (common-lisp:list 'csym::__sputc csym::_arg0 csym::_arg1))
(%defmacro csym::getchar_unlocked common-lisp:nil
 (common-lisp:list 'csym::getc_unlocked 'csym::stdin))
(%defmacro csym::putchar_unlocked (csym::_arg0)
 (common-lisp:list 'csym::putc_unlocked csym::_arg0 'csym::stdout))
