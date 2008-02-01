(%line 1 "/usr/include/wchar.h")
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
(%line 1 "/usr/include/machine/_limits.h")
(%line 1 "/usr/include/_ctype.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 74 "/usr/include/_ctype.h")
(decl (csym::___runetype) (fn unsigned-long csym::__ct_rune_t))
(decl (csym::___tolower) (fn csym::__ct_rune_t csym::__ct_rune_t))
(decl (csym::___toupper) (fn csym::__ct_rune_t csym::__ct_rune_t))
(%line 1 "/usr/include/runetype.h")
(%line 1 "/usr/include/sys/cdefs.h")
(%line 1 "/usr/include/sys/_types.h")
(%line 52 "/usr/include/runetype.h")
(def (struct csym::unnamed1) (decl csym::__min csym::__rune_t)
 (decl csym::__max csym::__rune_t) (decl csym::__map csym::__rune_t)
 (decl csym::__types (ptr unsigned-long)))
(deftype csym::_@r@UNE@e@NTRY (struct csym::unnamed1))
(def (struct csym::unnamed2) (decl csym::__nranges int)
 (decl csym::__ranges (ptr csym::_@r@UNE@e@NTRY)))
(deftype csym::_@r@UNE@r@ANGE (struct csym::unnamed2))
(def (struct csym::unnamed3) (decl csym::__magic (array char 8))
 (decl csym::__encoding (array char 32))
 (decl csym::__sgetrune
  (ptr
   (fn csym::__rune_t (ptr (const char)) csym::__size_t
    (ptr (ptr (const char))))))
 (decl csym::__sputrune
  (ptr (fn int csym::__rune_t (ptr char) csym::__size_t (ptr (ptr char)))))
 (decl csym::__invalid_rune csym::__rune_t)
 (decl csym::__runetype (array unsigned-long (<< 1 8)))
 (decl csym::__maplower (array csym::__rune_t (<< 1 8)))
 (decl csym::__mapupper (array csym::__rune_t (<< 1 8)))
 (decl csym::__runetype_ext csym::_@r@UNE@r@ANGE)
 (decl csym::__maplower_ext csym::_@r@UNE@r@ANGE)
 (decl csym::__mapupper_ext csym::_@r@UNE@r@ANGE)
 (decl csym::__variable (ptr void)) (decl csym::__variable_len int))
(deftype csym::_@r@UNE@l@OCALE (struct csym::unnamed3))
(%line 81 "/usr/include/runetype.h")
(extern-decl csym::_@d@EFAULT@r@UNE@l@OCALE csym::_@r@UNE@l@OCALE)
(extern-decl csym::_@c@URRENT@r@UNE@l@OCALE (ptr csym::_@r@UNE@l@OCALE))
(%line 98 "/usr/include/_ctype.h")
(static-def (csym::__maskrune csym::_c csym::_f)
 (fn int csym::__ct_rune_t unsigned-long)
 (return
  (bit-and
   (if-exp (or (< csym::_c 0) (>= csym::_c (<< 1 8)))
    (csym::___runetype csym::_c)
    (aref (fref (mref csym::_@c@URRENT@r@UNE@l@OCALE) csym::__runetype)
     csym::_c))
   csym::_f)))
(static-def (csym::__istype csym::_c csym::_f)
 (fn int csym::__ct_rune_t unsigned-long)
 (return (not (not (csym::__maskrune csym::_c csym::_f)))))
(static-def (csym::__isctype csym::_c csym::_f)
 (fn int csym::__ct_rune_t unsigned-long)
 (return
  (if-exp (or (< csym::_c 0) (>= csym::_c (<< 1 8))) 0
   (not
    (not
     (bit-and
      (aref (fref csym::_@d@EFAULT@r@UNE@l@OCALE csym::__runetype) csym::_c)
      csym::_f))))))
(static-def (csym::__toupper csym::_c) (fn csym::__ct_rune_t csym::__ct_rune_t)
 (return
  (if-exp (or (< csym::_c 0) (>= csym::_c (<< 1 8)))
   (csym::___toupper csym::_c)
   (aref (fref (mref csym::_@c@URRENT@r@UNE@l@OCALE) csym::__mapupper)
    csym::_c))))
(static-def (csym::__tolower csym::_c) (fn csym::__ct_rune_t csym::__ct_rune_t)
 (return
  (if-exp (or (< csym::_c 0) (>= csym::_c (<< 1 8)))
   (csym::___tolower csym::_c)
   (aref (fref (mref csym::_@c@URRENT@r@UNE@l@OCALE) csym::__maplower)
    csym::_c))))
(static-def (csym::__wcwidth csym::_c) (fn int csym::__ct_rune_t)
 (decl csym::_x unsigned-int) (if (== csym::_c 0) (return 0))
 (= csym::_x
  (cast unsigned-int
   (csym::__maskrune csym::_c
    (bit-or (c2sc::cast c2sc::long 3758096384)
     (c2sc::cast c2sc::long 262144)))))
 (if (!= (bit-and csym::_x (c2sc::cast c2sc::long 3758096384)) 0)
  (return (>> (bit-and csym::_x (c2sc::cast c2sc::long 3758096384)) 30)))
 (return
  (if-exp (!= (bit-and csym::_x (c2sc::cast c2sc::long 262144)) 0) 1 (- 1))))
(%line 77 "/usr/include/wchar.h")
(deftype csym::mbstate_t csym::__mbstate_t)
(deftype csym::size_t csym::__size_t)
(deftype csym::wchar_t csym::__wchar_t)
(deftype csym::wint_t csym::__wint_t)
(%line 107 "/usr/include/wchar.h")
(decl (csym::btowc) (fn csym::wint_t int))
(decl (csym::fgetwc) (fn csym::wint_t (ptr (struct csym::__S@file))))
(decl (csym::fgetws)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) int (ptr (struct csym::__S@file))))
(decl (csym::fputwc)
 (fn csym::wint_t csym::wchar_t (ptr (struct csym::__S@file))))
(decl (csym::fputws)
 (fn int (ptr (const csym::wchar_t)) (ptr (struct csym::__S@file))))
(decl (csym::fwide) (fn int (ptr (struct csym::__S@file)) int))
(decl (csym::fwprintf)
 (fn int (ptr (struct csym::__S@file)) (ptr (const csym::wchar_t)) va-arg))
(decl (csym::fwscanf)
 (fn int (ptr (struct csym::__S@file)) (ptr (const csym::wchar_t)) va-arg))
(decl (csym::getwc) (fn csym::wint_t (ptr (struct csym::__S@file))))
(decl (csym::getwchar) (fn csym::wint_t void))
(decl (csym::mbrlen)
 (fn csym::size_t (ptr (const char)) csym::size_t (ptr csym::mbstate_t)))
(decl (csym::mbrtowc)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (const char)) csym::size_t
  (ptr csym::mbstate_t)))
(decl (csym::mbsinit) (fn int (ptr (const csym::mbstate_t))))
(decl (csym::mbsrtowcs)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (ptr (const char))) csym::size_t
  (ptr csym::mbstate_t)))
(decl (csym::putwc)
 (fn csym::wint_t csym::wchar_t (ptr (struct csym::__S@file))))
(decl (csym::putwchar) (fn csym::wint_t csym::wchar_t))
(decl (csym::swprintf)
 (fn int (ptr csym::wchar_t) csym::size_t (ptr (const csym::wchar_t)) va-arg))
(decl (csym::swscanf)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t)) va-arg))
(decl (csym::ungetwc)
 (fn csym::wint_t csym::wint_t (ptr (struct csym::__S@file))))
(decl (csym::vfwprintf)
 (fn int (ptr (struct csym::__S@file)) (ptr (const csym::wchar_t))
  csym::__va_list))
(decl (csym::vswprintf)
 (fn int (ptr csym::wchar_t) csym::size_t (ptr (const csym::wchar_t))
  csym::__va_list))
(decl (csym::vwprintf) (fn int (ptr (const csym::wchar_t)) csym::__va_list))
(decl (csym::wcrtomb)
 (fn csym::size_t (ptr char) csym::wchar_t (ptr csym::mbstate_t)))
(decl (csym::wcscat)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))))
(decl (csym::wcschr)
 (fn (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::wchar_t))
(decl (csym::wcscmp)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t))))
(decl (csym::wcscoll)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t))))
(decl (csym::wcscpy)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))))
(decl (csym::wcscspn)
 (fn csym::size_t (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t))))
(decl (csym::wcsftime)
 (fn csym::size_t (ptr csym::wchar_t) csym::size_t (ptr (const csym::wchar_t))
  (ptr (const (struct csym::tm)))))
(decl (csym::wcslen) (fn csym::size_t (ptr (const csym::wchar_t))))
(decl (csym::wcsncat)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  csym::size_t))
(decl (csym::wcsncmp)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t)) csym::size_t))
(decl (csym::wcsncpy)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  csym::size_t))
(decl (csym::wcspbrk)
 (fn (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  (ptr (const csym::wchar_t))))
(decl (csym::wcsrchr)
 (fn (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::wchar_t))
(decl (csym::wcsrtombs)
 (fn csym::size_t (ptr char) (ptr (ptr (const csym::wchar_t))) csym::size_t
  (ptr csym::mbstate_t)))
(decl (csym::wcsspn)
 (fn csym::size_t (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t))))
(decl (csym::wcsstr)
 (fn (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  (ptr (const csym::wchar_t))))
(decl (csym::wcsxfrm)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::size_t))
(decl (csym::wctob) (fn int csym::wint_t))
(decl (csym::wcstod)
 (fn double (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t))))
(decl (csym::wcstok)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  (ptr (ptr csym::wchar_t))))
(decl (csym::wcstol)
 (fn long (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t)) int))
(decl (csym::wcstoul)
 (fn unsigned-long (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t)) int))
(decl (csym::wmemchr)
 (fn (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::wchar_t
  csym::size_t))
(decl (csym::wmemcmp)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t)) csym::size_t))
(decl (csym::wmemcpy)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  csym::size_t))
(decl (csym::wmemmove)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) (ptr (const csym::wchar_t))
  csym::size_t))
(decl (csym::wmemset)
 (fn (ptr csym::wchar_t) (ptr csym::wchar_t) csym::wchar_t csym::size_t))
(decl (csym::wprintf) (fn int (ptr (const csym::wchar_t)) va-arg))
(decl (csym::wscanf) (fn int (ptr (const csym::wchar_t)) va-arg))
(extern-decl csym::__stdinp (ptr (struct csym::__S@file)))
(extern-decl csym::__stdoutp (ptr (struct csym::__S@file)))
(extern-decl csym::__stderrp (ptr (struct csym::__S@file)))
(decl (csym::vfwscanf)
 (fn int (ptr (struct csym::__S@file)) (ptr (const csym::wchar_t))
  csym::__va_list))
(decl (csym::vswscanf)
 (fn int (ptr (const csym::wchar_t)) (ptr (const csym::wchar_t))
  csym::__va_list))
(decl (csym::vwscanf) (fn int (ptr (const csym::wchar_t)) csym::__va_list))
(decl (csym::wcstof)
 (fn float (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t))))
(decl (csym::wcstold)
 (fn long-double (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t))))
(decl (csym::wcstoll)
 (fn long-long (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t)) int))
(decl (csym::wcstoull)
 (fn unsigned-long-long (ptr (const csym::wchar_t)) (ptr (ptr csym::wchar_t))
  int))
(decl (csym::wcswidth) (fn int (ptr (const csym::wchar_t)) csym::size_t))
(decl (csym::wcwidth) (fn int csym::wchar_t))
(decl (csym::fgetwln)
 (fn (ptr csym::wchar_t) (ptr (struct csym::__S@file)) (ptr csym::size_t)))
(decl (csym::mbsnrtowcs)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (ptr (const char))) csym::size_t
  csym::size_t (ptr csym::mbstate_t)))
(decl (csym::wcsnrtombs)
 (fn csym::size_t (ptr char) (ptr (ptr (const csym::wchar_t))) csym::size_t
  csym::size_t (ptr csym::mbstate_t)))
(decl (csym::wcslcat)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::size_t))
(decl (csym::wcslcpy)
 (fn csym::size_t (ptr csym::wchar_t) (ptr (const csym::wchar_t)) csym::size_t))
(%defconstant csym::i386 1)
(%defconstant csym::unix 1)
(%defconstant csym::|__I386| 1)
(%defconstant csym::|__I386__| 1)
(%defconstant csym::__unix 1)
(%defconstant csym::__unix__ 1)
(%defconstant csym::__@F@ree@BSD__ 1)
(%defconstant csym::__@GNUC__ 3)
(%defconstant csym::__@GNUC_MINOR__ 3)
(%defconstant csym::__@SIZE_TYPE__ unsigned-int)
(%defconstant csym::__@PTRDIFF_TYPE__ int)
(%defconstant csym::__@WCHAR_TYPE__ int)
(%defconstant csym::__@GNUG__ 3)
(%defconstant csym::__@LINE__ (- 1234567890))
(%defconstant csym::__@FILE__ (c-exp ""))
(%defconstant csym::__@DATE__ "Dec 20 2004")
(%defconstant csym::__@TIME__ "22:37:37")
(%defconstant csym::__@STDC__ 1)
(%defconstant csym::__@STDC_HOSTED__ 1)
(%defconstant csym::__@MCPP 2)
(%defconstant csym::__extension__ (c-exp ""))
(%defmacro csym::__attribute__ (csym::arg0) (common-lisp:list 'c-exp ""))
(%defconstant csym::__inline (c-exp ""))
(%undef csym::i386)
(%undef csym::unix)
(%undef csym::__@GNUG__)
(%defconstant csym::_@WCHAR_H_ (c-exp ""))
(%defconstant csym::_@SYS_CDEFS_H_ (c-exp ""))
(%defconstant csym::__@BEGIN_DECLS (c-exp ""))
(%defconstant csym::__@END_DECLS (c-exp ""))
(%defmacro csym::__@GNUC_PREREQ__ (csym::arg0 csym::arg1)
 (common-lisp:list 'or
                   (common-lisp:list '> 'csym::__@GNUC__ csym::arg0)
                   (common-lisp:list 'and
                                     (common-lisp:list '==
                                                       'csym::__@GNUC__
                                                       csym::arg0)
                                     (common-lisp:list '>=
                                                       'csym::__@GNUC_MINOR__
                                                       csym::arg1))))
(%defmacro csym::__@P (csym::arg0) csym::arg0)
(%defmacro csym::__@CONCAT1 (csym::arg0 csym::arg1) '"<macro with ## operator>")
(%defmacro csym::__@CONCAT (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__@CONCAT1 csym::arg0 csym::arg1))
(%defmacro csym::__@STRING (csym::arg0) '"<macro with # operator>")
(%defmacro csym::__@XSTRING (csym::arg0)
 (common-lisp:list 'csym::__@STRING csym::arg0))
(%defconstant csym::__const (const int))
(%defconstant csym::__signed signed-int)
(%defconstant csym::__volatile (volatile int))
(%defconstant csym::__dead2 (csym::__attribute__ csym::__noreturn__))
(%defconstant csym::__pure2 (csym::__attribute__ csym::__const__))
(%defconstant csym::__unused (csym::__attribute__ csym::__unused__))
(%defconstant csym::__used (csym::__attribute__ csym::__used__))
(%defconstant csym::__packed (csym::__attribute__ csym::__packed__))
(%defmacro csym::__aligned (csym::arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__aligned__ csym::arg0)))
(%defmacro csym::__section (csym::arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__section__ csym::arg0)))
(%defconstant csym::__pure (csym::__attribute__ csym::__pure__))
(%defconstant csym::__always_inline
 (csym::__attribute__ csym::__always_inline__))
(%defmacro csym::__nonnull (csym::arg0)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__nonnull__ csym::arg0)))
(%defconstant csym::__@LONG_LONG_SUPPORTED (c-exp ""))
(%defconstant csym::__restrict (c-exp ""))
(%defmacro csym::__predict_true (csym::arg0)
 (common-lisp:list 'csym::__builtin_expect csym::arg0 '1))
(%defmacro csym::__predict_false (csym::arg0)
 (common-lisp:list 'csym::__builtin_expect csym::arg0 '0))
(%defmacro csym::__offsetof (csym::arg0 csym::arg1)
 (common-lisp:list 'cast
                   'csym::size_t
                   (common-lisp:list 'ptr
                                     (common-lisp:list 'fref
                                                       (common-lisp:list 'mref
                                                                         (common-lisp:list
                                                                          'cast
                                                                          (common-lisp:list
                                                                           'ptr
                                                                           csym::arg0)
                                                                          '0))
                                                       csym::arg1))))
(%defmacro csym::__rangeof (csym::arg0 csym::arg1 csym::arg2)
 (common-lisp:list '-
                   (common-lisp:list 'csym::__offsetof csym::arg0 csym::arg2)
                   (common-lisp:list 'csym::__offsetof csym::arg0 csym::arg1)))
(%defmacro csym::__printflike (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__format__
                                     'csym::__printf__
                                     csym::arg0
                                     csym::arg1)))
(%defmacro csym::__scanflike (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::__attribute__
                   (common-lisp:list 'csym::__format__
                                     'csym::__scanf__
                                     csym::arg0
                                     csym::arg1)))
(%defmacro csym::__printf0like (csym::arg0 csym::arg1)
 (common-lisp:list 'c-exp ""))
(%defmacro csym::__strong_reference (csym::arg0 csym::arg1)
 '"<macro with # operator>")
(%defmacro csym::__weak_reference (csym::arg0 csym::arg1)
 '"<macro with # operator>")
(%defmacro csym::__warn_references (csym::arg0 csym::arg1)
 '"<macro with # operator>")
(%defmacro csym::__@IDSTRING (csym::arg0 csym::arg1)
 (common-lisp:list 'c-exp "__asm__(\".ident\\t\\\"\" ~a  \"\\\"\")" csym::arg1))
(%defmacro csym::__@FBSDID (csym::arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_
                                     'csym::__@LINE__)
                   csym::arg0))
(%defmacro csym::__@RCSID (csym::arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_
                                     'csym::__@LINE__)
                   csym::arg0))
(%defmacro csym::__@RCSID_SOURCE (csym::arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__rcsid_source_
                                     'csym::__@LINE__)
                   csym::arg0))
(%defmacro csym::__@SCCSID (csym::arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__sccsid_
                                     'csym::__@LINE__)
                   csym::arg0))
(%defmacro csym::__@COPYRIGHT (csym::arg0)
 (common-lisp:list 'csym::__@IDSTRING
                   (common-lisp:list 'csym::__@CONCAT
                                     'csym::__copyright_
                                     'csym::__@LINE__)
                   csym::arg0))
(%defmacro csym::__@DECONST (csym::arg0 csym::arg1)
 (common-lisp:list 'cast
                   csym::arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'const
                                                                          'void))
                                                       csym::arg1))))
(%defmacro csym::__@DEVOLATILE (csym::arg0 csym::arg1)
 (common-lisp:list 'cast
                   csym::arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'volatile
                                                                          'void))
                                                       csym::arg1))))
(%defmacro csym::__@DEQUALIFY (csym::arg0 csym::arg1)
 (common-lisp:list 'cast
                   csym::arg0
                   (common-lisp:list 'cast
                                     'csym::uintptr_t
                                     (common-lisp:list 'cast
                                                       (common-lisp:list 'ptr
                                                                         (common-lisp:list
                                                                          'const
                                                                          'volatile
                                                                          'void))
                                                       csym::arg1))))
(%defconstant csym::__@POSIX_VISIBLE 200112)
(%defconstant csym::__@XSI_VISIBLE 600)
(%defconstant csym::__@BSD_VISIBLE 1)
(%defconstant csym::__@ISO_C_VISIBLE 1999)
(%defconstant csym::@NULL (cast (ptr void) 0))
(%defconstant csym::_@SYS__TYPES_H_ (c-exp ""))
(%defconstant csym::_@MACHINE__TYPES_H_ (c-exp ""))
(%defconstant csym::__@GNUC_VA_LIST (c-exp ""))
(%defconstant csym::_@MACHINE__LIMITS_H_ (c-exp ""))
(%defconstant csym::__@CHAR_BIT 8)
(%defconstant csym::__@SCHAR_MAX 127)
(%defconstant csym::__@SCHAR_MIN (- (- 127) 1))
(%defconstant csym::__@UCHAR_MAX 255)
(%defconstant csym::__@USHRT_MAX 65535)
(%defconstant csym::__@SHRT_MAX 32767)
(%defconstant csym::__@SHRT_MIN (- (- 32767) 1))
(%defconstant csym::__@UINT_MAX (c2sc::cast c2sc::unsigned-int 4294967295))
(%defconstant csym::__@INT_MAX 2147483647)
(%defconstant csym::__@INT_MIN (- (- 2147483647) 1))
(%defconstant csym::__@ULONG_MAX (c2sc::cast c2sc::unsigned-long 4294967295))
(%defconstant csym::__@LONG_MAX (c2sc::cast c2sc::long 2147483647))
(%defconstant csym::__@LONG_MIN (- (- (c2sc::cast c2sc::long 2147483647)) 1))
(%defconstant csym::__@ULLONG_MAX
 (c2sc::cast c2sc::unsigned-long-long 18446744073709551615))
(%defconstant csym::__@LLONG_MAX
 (c2sc::cast c2sc::long-long 9223372036854775807))
(%defconstant csym::__@LLONG_MIN
 (- (- (c2sc::cast c2sc::long-long 9223372036854775807)) 1))
(%defconstant csym::__@SSIZE_MAX csym::__@INT_MAX)
(%defconstant csym::__@SIZE_T_MAX csym::__@UINT_MAX)
(%defconstant csym::__@OFF_MAX csym::__@LLONG_MAX)
(%defconstant csym::__@OFF_MIN csym::__@LLONG_MIN)
(%defconstant csym::__@UQUAD_MAX csym::__@ULLONG_MAX)
(%defconstant csym::__@QUAD_MAX csym::__@LLONG_MAX)
(%defconstant csym::__@QUAD_MIN csym::__@LLONG_MIN)
(%defconstant csym::__@LONG_BIT 32)
(%defconstant csym::__@WORD_BIT 32)
(%defconstant csym::__@CTYPE_H_ (c-exp ""))
(%defconstant csym::_@CTYPE_A (c2sc::cast c2sc::long 256))
(%defconstant csym::_@CTYPE_C (c2sc::cast c2sc::long 512))
(%defconstant csym::_@CTYPE_D (c2sc::cast c2sc::long 1024))
(%defconstant csym::_@CTYPE_G (c2sc::cast c2sc::long 2048))
(%defconstant csym::_@CTYPE_L (c2sc::cast c2sc::long 4096))
(%defconstant csym::_@CTYPE_P (c2sc::cast c2sc::long 8192))
(%defconstant csym::_@CTYPE_S (c2sc::cast c2sc::long 16384))
(%defconstant csym::_@CTYPE_U (c2sc::cast c2sc::long 32768))
(%defconstant csym::_@CTYPE_X (c2sc::cast c2sc::long 65536))
(%defconstant csym::_@CTYPE_B (c2sc::cast c2sc::long 131072))
(%defconstant csym::_@CTYPE_R (c2sc::cast c2sc::long 262144))
(%defconstant csym::_@CTYPE_I (c2sc::cast c2sc::long 524288))
(%defconstant csym::_@CTYPE_T (c2sc::cast c2sc::long 1048576))
(%defconstant csym::_@CTYPE_Q (c2sc::cast c2sc::long 2097152))
(%defconstant csym::_@CTYPE_SW0 (c2sc::cast c2sc::long 536870912))
(%defconstant csym::_@CTYPE_SW1 (c2sc::cast c2sc::long 1073741824))
(%defconstant csym::_@CTYPE_SW2 (c2sc::cast c2sc::long 2147483648))
(%defconstant csym::_@CTYPE_SW3 (c2sc::cast c2sc::long 3221225472))
(%defconstant csym::_@CTYPE_SWM (c2sc::cast c2sc::long 3758096384))
(%defconstant csym::_@CTYPE_SWS 30)
(%defconstant csym::_@RUNETYPE_H_ (c-exp ""))
(%defconstant csym::_@CACHED_RUNES (<< 1 8))
(%defconstant csym::_@CRMASK (bit-not (- csym::_@CACHED_RUNES 1)))
(%defconstant csym::_@RUNE_MAGIC_1 "RuneMagi")
(%defconstant csym::_@MBSTATE_T_DECLARED (c-exp ""))
(%defconstant csym::_@SIZE_T_DECLARED (c-exp ""))
(%defconstant csym::_@WCHAR_T_DECLARED (c-exp ""))
(%defconstant csym::_@WINT_T_DECLARED (c-exp ""))
(%defconstant csym::@WCHAR_MIN csym::__@INT_MIN)
(%defconstant csym::@WCHAR_MAX csym::__@INT_MAX)
(%defconstant csym::@WEOF (- csym::wint_t 1))
(%defconstant csym::_@STDSTREAM_DECLARED (c-exp ""))
(%defmacro csym::getwc (csym::arg0) (common-lisp:list 'csym::fgetwc csym::arg0))
(%defmacro csym::getwchar common-lisp:nil
 (common-lisp:list 'csym::fgetwc 'csym::__stdinp))
(%defmacro csym::putwc (csym::arg0 csym::arg1)
 (common-lisp:list 'csym::fputwc csym::arg0 csym::arg1))
(%defmacro csym::putwchar (csym::arg0)
 (common-lisp:list 'csym::fputwc csym::arg0 'csym::__stdoutp))
(%defmacro csym::wcwidth (csym::arg0)
 (common-lisp:list 'csym::__wcwidth csym::arg0))
