;;; Copyright (c) 2008 Tasuku Hiraishi <hiraisi@kuis.kyoto-u.ac.jp>
;;; All rights reserved.

;;; Redistribution and use in source and binary forms, with or without
;;; modification, are permitted provided that the following conditions
;;; are met:
;;; 1. Redistributions of source code must retain the above copyright
;;;    notice, this list of conditions and the following disclaimer.
;;; 2. Redistributions in binary form must reproduce the above copyright
;;;    notice, this list of conditions and the following disclaimer in the
;;;    documentation and/or other materials provided with the distribution.

;;; THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND
;;; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;;; ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
;;; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;;; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
;;; OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
;;; HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
;;; LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
;;; OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
;;; SUCH DAMAGE.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;; hsc.sc: Library for HSC

;; (%cinclude "<stdio.h>" "<stdlib.h>" "<sys/time.h>" "<string.h>"
;;	   (:required-macros NULL))

(c-exp "#include <sys/time.h>")
(%defconstant csym::NULL (cast (ptr void) 0))
(decl (malloc) (csym::fn (ptr void) size-t))
(decl (realloc) (csym::fn (ptr void) (ptr void) size-t))
(decl (strncpy) (csym::fn (ptr char) (ptr char) (ptr (const char)) size-t))
(decl (printf) (csym::fn int (ptr (const char)) va-arg))

(%include "rule/nestfunc-setrule.sh")
(%include "hsc.sh")

;;; gc-init��̵����(=0)�ξ��˺��Ѥ������
(%ifndef TOSIZE
	 ((%defconstant TOSIZE (* 5 1024 1024))) )
;; (%ifndef ROOTSIZEMAX
;; 	 ((%defconstant ROOTSIZEMAX (* 8 1024))) )
(%ifndef GC-STACK-SIZE
	 ((%defconstant GC-STACK-SIZE (/ (fref params tosize) (sizeof double)))) )
(%ifndef GC-LIMITED-STACK-SIZE
	 ((%defconstant GC-LIMITED-STACK-SIZE 256)) )
(%ifndef GC-LIMITED-STACK-MAX
	 ((%defconstant GC-LIMITED-STACK-MAX 32)) )

;;; �ݥ���Ƚ��
(%defmacro MREF-AS (tp p)
  `(mref (cast (ptr ,tp) ,p)))

(%defmacro IN-FROM (p)
  `(< (cast unsigned-int (- (cast (ptr char) ,p) old-memory))
      (fref params tosize)) ) 
(%defmacro IN-TOSP (p)
  `(< (cast unsigned-int (- (cast (ptr char) ,p) new-memory))
      (fref params tosize)) ) 
(%defmacro FWPTR (p)
  `(MREF-AS (ptr void) ,p))

(def (MEMCPY d s sz) (csym::fn (ptr void) (ptr void) (ptr (const void)) size-t)
  (def -des (ptr long) (cast (ptr long) d))
  (def -src (ptr long) (cast (ptr long) s))
  (def -til (ptr long) (cast (ptr long) (+ (cast (ptr char) -src) sz)) )
  (do-while (< -src -til)
    (= (mref (inc -des)) (mref (inc -src))) ))


;; util
(static-def (error s) (csym::fn void (ptr char))
  (printf "ERROR: %s~%" s)
  (exit 1))

(static-def (myalloc size) (csym::fn (ptr void) size-t)
  (def p (ptr void))
  (= p (malloc size))
  (if (== p csym::NULL)
      (error "Not enough memory."))
  (return p))

(static-def (myrealloc p size) (csym::fn (ptr void) (ptr void) size-t)
  (def q (ptr void))
  (= q (realloc p size))
  (if (== q csym::NULL)
      (error "Not enough memory."))
  (return q))


;; GC parameter
(def (struct gc-params-struct)
  (def gcv int)			        ; verbose?
  (def gctype int)			; 0: copy-gc
  (def tosize size-t)			; (/ heap-size 2)
  (def stack-size size-t)		
  (def limited-stack-max size-t)	; less than 256
  )
(deftype gc-params (struct gc-params-struct))
(static-def params gc-params)

;; root + heap area
(static-def allocated-size int)
(static-def old-memory (ptr char))
(static-def new-memory (ptr char))
(static-def old-memory-end (ptr char))
(static-def new-memory-end (ptr char))

;; Referred in move & gc-breadth-first
(static-def b (ptr char))  ; move�μ��ΰ�ư��

;; *link = move(*link);
(def (move vp) (csym::fn (ptr void) (ptr void))
  (def p (ptr char) vp)
  (def fwp (ptr void))
  (def tag size-t)
  (def d desc-t)
  (def ao (ptr (struct array-object)))
  (def size size-t)
  (def asize size-t)
  (def np (ptr char))
  (def nb (ptr char))

  ;; Object not located in From Space
  (if (not (IN-FROM p))
      (return vp))
  ;; ���֥������Ȥ���Ƭ (sizeof size-t)[byte] ��
  ;; fowarding pointer OR (bit-or least2bit-tag pointer-to-descriptor)
  (= tag (bit-and #b11 (MREF-AS size-t p)))
  (= fwp (cast (ptr void)
	       (bit-xor tag (MREF-AS size-t p))))
  ;; Already evacuated
  (if (IN-TOSP fwp)
      (return fwp))
  
  ;; ���֥������Ȥ���Ƭ sizeof(int) bytes --- (fref ,obj type)
  ;; LSB==1: type-id
  ;; LSB==0: forwarding pointer
  (switch tag
    (case TYPE-STRUCT)
    (= d (cast desc-t fwp))
    (= size (fref (mref d) size))
    (= asize (fref (mref d) asize))
    (break)
    (case TYPE-REF-ARRAY)
    (case TYPE-NONREF-ARRAY)
    (= ao (cast (ptr (struct array-object)) vp))
    (= size (fref (mref ao) size))
    (= asize (fref (mref ao) asize))
    (break)
    (default)
    (error "Illegal type ID!"))
  
  (= np b)				; ��ư��
  (= nb (+ np asize))			; ���ΰ�ư��
  (if (>= nb new-memory-end)
      (error "buffer overrun."))
  (MEMCPY np p size)
  (= b nb)                              ; ��ư��򹹿�
  (= (FWPTR p) np)                      ; ���Ϥ�fowarding pointer
  (return np))

;; BREADTH-FIRST-GC
(def (gc-breadth-first scan) (fn void sht)
  (def i int)
  (def tmp (ptr char))
  (def tag size-t)
  (def len int)
  (def link (ptr (ptr void)))
  (def d desc-t)
  (def ao (ptr (struct array-object)))
  (def el-size size-t)
  (def p (ptr char))
  (def s (ptr char))

  (if (fref params gcv)
      (printf "BREADTH-FIRST-GC start~%"))
  (= b new-memory) (= s b)
  (scan)                                ; stack����ľ�ܻؤ���Ƥ���obj��copy
  (while (< s b)                        ; for each copied object
    (= tag (bit-and #b11 (MREF-AS size-t s)))
    (switch tag
      (case TYPE-STRUCT)
      (= d (cast desc-t (bit-xor tag (MREF-AS size-t s))))
      (= p (cast (ptr char) s))
      (= len (fref (mref d) fli-len))
      (for ((= i 0) (< i len) (++ i)) ; for each reference in the object
           (= link (cast (ptr (ptr void)) (+ p (aref (fref (mref d) fli) i))))
           (= (mref link) (move (mref link))))
      (+= s (fref (mref d) asize))
      (break)
      (case TYPE-REF-ARRAY)
      (= ao (cast (ptr (struct array-object)) s))
      (= p (fref (mref ao) body))
      (= len (fref (mref ao) length))
      (= el-size (fref (mref ao) el-size))
      (for ((= i 0) (< i len) (++ i))   ; for each element in the array
           (= link (cast (ptr (ptr void)) p))
           (= (mref link) (move (mref link)))
           (+= p el-size))
      (+= s (fref (mref ao) asize))
      (break)
      (case TYPE-NONREF-ARRAY) ; no need to scan elements
      (= ao (cast (ptr (struct array-object)) s))
      (+= s (fref (mref ao) asize))
      (break)
      (default)
      (error "Illegal type ID!"))
    )
  (= allocated-size (- b new-memory))
  (= tmp new-memory)
  (= new-memory old-memory)
  (= old-memory tmp)
  (= tmp new-memory-end)
  (= new-memory-end old-memory-end)
  (= old-memory-end tmp)
  (if (fref params gcv)
      (printf "GC complete (%d)~%" allocated-size))
  )

;; ��ǽ¬����
(def gc-ttime double)
(def tp1 (struct timeval))
(def tp2 (struct timeval))

;; GC����
(def (gc scan) (fn void sht)
  (gettimeofday (ptr tp1) 0)
  (switch (fref params gctype)
    (case 0) 
    (gc-breadth-first scan)
    (break)
    )
  (gettimeofday (ptr tp2) 0)
  (+= gc-ttime
      (+    (- (fref tp2 tv-sec) (fref tp1 tv-sec))
	    (* (- (fref tp2 tv-usec) (fref tp1 tv-usec)) 0.000001)))
  )

;;; Initialize collector
(def (getmem-init p) (csym::fn void gc-params)
  (def mp (ptr char))
  (static-def called int 0)

  (if called (return) (= called 1))
  (= gc-ttime 0.0)
  (= params p)      ;; params: global variable
  (if (== (fref params tosize) 0) (= (fref params tosize) TOSIZE))
  (+= (fref params tosize) 3)
  (-= (fref params tosize) (bit-and (fref params tosize) 3))
                                        ; ����2bit��00�ˤʤ�褦�˷���夲
  (if (== (fref params stack-size) 0) 
      (= (fref params stack-size) GC-STACK-SIZE))
  (if (== (fref params limited-stack-max) 0)
      (= (fref params limited-stack-max) GC-LIMITED-STACK-MAX))

  (printf "tosize=%d, stack=%d, limit=%d~%"
          (fref params tosize)
          (fref params stack-size)
          (fref params limited-stack-max) )
  ;; heap�ΰ�γ���
  (= old-memory (myalloc (fref params tosize)))
  (= old-memory-end (+ old-memory (fref params tosize)))
  (= new-memory (myalloc (fref params tosize)))
  (= new-memory-end (+ new-memory (fref params tosize)))
  (= allocated-size 0)
  ;;  (for ((= mp new-memory) (< mp new-memory-end) (+= mp 4096))
  ;;    (= (mref mp) 1) )
  ;;  (for ((= mp old-memory) (< mp old-memory-end) (+= mp 4096))
  ;;    (= (mref mp) 1) )
  )

(def (gc-init tp tosize stack-size limited-max)
    (csym::fn void int int int int)
  (def p gc-params)
  (= (fref p gcv) 1)         ; verbose?
  (= (fref p gctype) tp)     ; 0: copy-gc
  (= (fref p tosize) tosize) ; half of heap size (from/to space)
  (= (fref p stack-size) stack-size)
  (= (fref p limited-stack-max) limited-max)
  (getmem-init p))


;;; memory allocation
;; (def (try-getmem d) (csym::fn (ptr void) desc-t)
(def (try-getmem size) (csym::fn (ptr void) size-t)
  (def p (ptr char))
  ;; (= size (fref (mref d) asize))
  (if (> (+ allocated-size size) (fref params tosize)) (return 0))
  (= p (+ old-memory allocated-size))
  (+= allocated-size size)
  (memset p 0 size)
  ;; (= (mref (cast (ptr desc-t) p)) d)
  (return p))


;;; corresponds HSC's `new' primitive
;; (def (getmem scan d) (csym::fn (ptr void) sht desc-t)
(def (getmem scan size) (fn (ptr void) sht size-t)
  (def p (ptr void))
  (= p (try-getmem size))
  (if (== p 0)
      (begin
       (gc scan)
       (= p (try-getmem size))
       (if (== p 0)
	   (error "getmem: No enough memory."))
       ))
  (return p))

;;; main: a wrapper of hsc-main
;; * convert argv into an hsc array of strings.
(%defmacro MAKE-ALIGN (exp align-size)
   `(* (/ (+ ,exp ,align-size -1)
	  ,align-size)
       ,align-size))

(decl (hsc-main) (fn int sht int (ptr (struct array-object))))

(def (main argc argv) (fn int int (ptr (ptr char)))
  (def hsc-argv (ptr (struct array-object)))
  (def hsc-argv-size size-t)
  (def hsc-argv-asize size-t)
  (def i int)
  (def body (ptr (ptr (struct array-object))))
  (def len int)
  (def strobj-size size-t)
  (def strobj-asize size-t)
  (def (scan1) (NESTFN void void)
    (= argv (move argv))
    (return))
  ;; Initialize heap
  (gc-init 0 0 0 0)
  ;; an array of strings
  (= hsc-argv-size
     (+ (sizeof (struct array-object))
	(* argc (sizeof (ptr (struct array-object))))))
  (= hsc-argv-asize (MAKE-ALIGN hsc-argv-size (sizeof align-t)))
  (= hsc-argv 
     (cast (ptr (struct array-object)) (getmem scan1 hsc-argv-asize)))
  (= (fref hsc-argv -> d) (cast desc-t TYPE-REF-ARRAY))
  (= (fref hsc-argv -> length) argc)
  (= (fref hsc-argv -> el-size) (sizeof (ptr (struct array-object))))
  (= (fref hsc-argv -> size) hsc-argv-size)
  (= (fref hsc-argv -> asize) hsc-argv-asize)
  (= body (cast (ptr (ptr (struct array-object))) 
	    (fref hsc-argv -> body)))
  ;; each string
  (for ((= i 0) (< i argc) (++ i))
    (= len (+ 1 (strlen (aref argv i))))
    (= strobj-size (+ (sizeof (struct array-object))
		      (* len (sizeof char))))
    (= strobj-asize (MAKE-ALIGN strobj-size (sizeof align-t)))
    (= (aref body i) (cast (ptr (struct array-object))
                       (getmem scan1 strobj-asize)))
    (= (fref (aref body i) -> d) (cast desc-t TYPE-NONREF-ARRAY))
    (= (fref (aref body i) -> length) len)
    (= (fref (aref body i) -> el-size) (sizeof char))
    (= (fref (aref body i) -> size) strobj-size)
    (= (fref (aref body i) -> asize) strobj-asize)
    (strncpy (cast (ptr char) (fref (aref body i) -> body))
	     (aref argv i) 
	     len)
    )
  (return (hsc-main scan1 argc hsc-argv))
  )