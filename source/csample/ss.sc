(%line 1 "csample/ss.c")
(def (c::main) (fn int) (def c::q123 int 10) "\"\""
     (c::printf "%d %d~|12|" (++ c::q123) 12345))
(def (c::__sgetc c::a) (fn int int) (return c::a))
(%defconstant c::i386 1)
(%defconstant c::unix 1)
(%defconstant c::__i386 1)
(%defconstant c::__i386__ 1)
(%defconstant c::__unix 1)
(%defconstant c::__unix__ 1)
(%defconstant c::__@cygwin__ 1)
(%defconstant c::__@cygwin32__ 1)
(%defconstant c::__@gnuc__ 3)
(%defconstant c::__@gnuc_minor__ 3)
(%defconstant c::__@size_type__ unsigned-int)
(%defconstant c::__@ptrdiff_type__ int)
(%defconstant c::__@wchar_type__ unsigned-short)
(%defconstant c::__@gnug__ 3)
(%defconstant c::__@line__ (- 1234567890))
(%defconstant c::__@file__ (c-exp ""))
(%defconstant c::__@date__ "Dec  5 2004")
(%defconstant c::__@time__ "19:10:26")
(%defconstant c::__@stdc__ 1)
(%defconstant c::__@stdc_hosted__ 1)
(%defconstant c::__@mcpp 2)
(%defconstant c::__extension__ (c-exp ""))
(%defmacro c::__attribute__ (c::arg0) (common-lisp:list 'c-exp ""))
(%defconstant c::__inline (c-exp ""))
(%undef c::i386)
(%undef c::unix)
(%undef c::__@gnug__)
(%defmacro c::@q (c::arg0 c::arg1) '"<macro with ## operator>")
(%defmacro c::@qq (c::arg0) '"<macro with # operator>")
(%defconstant c::@suffix c::|_0|)
(%defconstant c::@begin (c-exp "{"))
(%defconstant c::@end (c-exp "}"))
(%defconstant c::max 5)
(%defmacro c::getc (c::arg0) (common-lisp:list 'c::__sgetc c::arg0))
