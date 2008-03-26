;;;; �桼�ƥ���ƥ��ؿ����ޥ���
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#+sc-system
(progn
  (provide "SC-MISC")
  (eval-when (:compile-toplevel :load-toplevel :execute)
    (require "SC-DECL" "sc-decl.lsp"))
  
  (in-package "SC-MISC")
  )

;;;;;

;;; �������ѿ�̾����ʥޥ��������ѡ�
(eval-when (:compile-toplevel :load-toplevel :execute)
  (defmacro with-fresh-variables (varlist &body body)
    `(let ,(mapcar
            #'(lambda (var) `(,var (gensym ,(symbol-name var))))
            varlist)
       ,@body)))

;;; Anaphoric Variants (from "On Lisp")
(defmacro aif (test-form then-form &optional else-form)
  `(let ((it ,test-form))
     (declare (ignorable it))
     (if it ,then-form ,else-form)))

(defmacro acase (test-form &body body)
  `(let ((it ,test-form))
     (declare (ignorable it))
     (case it ,@body)))

(defmacro awhen (test-form &body body)
  `(aif ,test-form
       (progn ,@body)))

(defmacro aunless (test-form &body body)
  `(aif ,test-form
       it
     (progn ,@body)))

(defmacro awhile (expr &body body)
  `(do ((it ,expr ,expr)) ((not it))
     ,@body))

(defmacro aand (&rest args)
  (cond ((null args) t)
        ((null (cdr args)) (car args))
        (t `(aif ,(car args) (aand ,@(cdr args))))))

(defmacro acond (&rest clauses)
  (if (null clauses)
      nil
    (let ((cl1 (car clauses))
          (sym (gensym)))
      `(let ((,sym ,(car cl1)))
         (if ,sym
             (let ((it ,sym))
               (declare (ignorable it))
               ,@(cdr cl1))
           (acond ,@(cdr clauses)))))))

;;; ���ޥ�ɥ饤��μ¹ԡʼ�����¸��ʬ��ۼ���
#+(or allegro kcl ecl cmu clisp)
(defun command-line (command &key args verbose other-options)
  (let ((cat-string (strcat (cons command args) #\Space)))
    (prin1 cat-string verbose)
    (fresh-line verbose)
    #+allegro(apply #'excl:run-shell-command cat-string :wait t
                    other-options)
    #+kcl(apply #'system cat-string other-options)
    #+ecl(apply #'si::system cat-string other-options)
    #+(or cmu clisp)
    (apply #'ext:run-program command
           :arguments args
           :wait t
           other-options)
    ))

;;;;; �ե��������

;;; *readtable* ����Ū�����ؤ�
(defmacro with-readtable (readtable &body forms)
  `(let ((*readtable* ,readtable))
     ,@forms))

;;; *package* ����Ū�����ؤ�
(defmacro with-package (package &body forms)
  (let ((package-var (gensym "package")))
    `(let ((,package-var (find-package ,package)))
       (unless ,package-var
         (error "Package ~S not found." ,package-var))
       (let ((*package* ,package-var))
         ,@forms)) ))

;;; pname �Υѥå����������Ĥ���ʤ���п����˺�ä��֤�
(defun find-package2 (pname &rest args)
  (or (find-package pname)
      (apply #'make-package pname args)))

;;; �ե����뤫��S�����ɤ߹���
(defun read-file (file-name 
                  &key
                  ((:package *package*) *package*)
                  ((:readtable *readtable*) *readtable*)
                  &aux ret)
  (unless (probe-file file-name)
    (error "Can't open ~S." file-name))
  (with-open-file (istream file-name :direction :input)
    (do ((y (read istream nil 'eof) (read istream nil 'eof)))
        ((eq y 'eof) (nreverse ret))
      (push y ret))))

;;; �ե������S���ޤ���ʸ�����񤭹���
(defun write-file (file-name list 
                   &key
                   (overwrite nil) (write-string nil)
                   ((:print-case *print-case*) *print-case*)
                   ((:package *package*) *package*)
                   ((:readtable *readtable*) *readtable*))
  (when (probe-file file-name)
    (if overwrite
        (delete-file file-name)
      (error "~S already exists." file-name)))
  (with-open-file (ostream file-name :direction :output)
    (if write-string
        (princ list ostream)
      (dolist (elm list t)
        (format ostream "~S" elm)
        (terpri ostream)))))

;; eof��?
(defun eof-p (stream)
  (let ((ch (read-char-no-hang stream nil t)))
    (cond
     ((eq t ch)
      t)
     ((characterp ch)
      (unread-char ch stream)
      nil)
     (t
      nil))))

;; �ե������1�Ԥ��Ȥν���
(defmacro do-text-line ((line istream &optional ret) &body body)
  `(do ((,line (read-line ,istream nil) (read-line ,istream nil)))
       ((null ,line) ,ret)
     ,@body))

;; input-stream �ΥХåե��ˤ��ޤäƤ����Τ��Ф��Ƥν���
(defmacro do-input-stream-buffer ((var istream &optional ret) &body body)
  (with-fresh-variables (is-var)
    `(let ((,is-var ,istream))
       (do ((,var (read-char-no-hang ,is-var nil nil)
                  (read-char-no-hang ,is-var nil nil)))
           ((null ,var) ,ret)
         ,@body))))

;; input-stream �ΥХåե��ˤ��ޤäƤ����Τ�output-stream��ή��
(defun input-buffer-to-output (istream &rest ostreams &aux (cnt 0))
  (do-input-stream-buffer (ch istream cnt)
    (incf cnt)
    (dolist (ost ostreams)
      (write-char ch ost))))

;; input-stream �ΥХåե��ˤ��ޤäƤ����Τ�ʸ������Ѵ�
(defun input-buffer-to-string (istream)
  (with-output-to-string (os)
    (input-buffer-to-output istream os)))

;;; path-list ����Ӹ��ߤΥǥ��쥯�ȥ�(current-directory=t�λ�)
;;; ����ե�����򸡺�
(defun path-search (filespec path-list 
                    &optional 
                    (current-directory t)
                    (error-when-unfound nil))
  (setq path-list (mapcar #'pathname (mklist path-list)))
  (when current-directory
    (push (make-pathname :directory '(:relative)) path-list))
  (dolist (path path-list
            (when error-when-unfound
              (error "~S was not found in ~S" filespec path-list)))
    (let ((candidate
           (merge-pathnames path filespec)))
      (when (probe-file candidate)
        (return candidate)))))

;;; �ʥǥ��쥯�ȥ�򴹤����ˡ˥ե�����̾���Ѵ�
(defun change-filename (pathname newfilename)
  (unless (pathnamep pathname)
    (setq pathname (pathname pathname)))
  (make-pathname
   :name (pathname-name newfilename)
   :type (pathname-type newfilename)
   :directory (pathname-directory pathname)))

;;; �ե�����̾�γ�ĥ���Ѵ�, �ʤ�����ɲ�
(defun change-extension (pathname newext)
  (unless (pathnamep pathname)
    (setq pathname (pathname pathname)))
  (unless (or (null newext) (stringp newext))
    (setq newext (string-downcase (string newext))))
  (make-pathname
   :name (pathname-name pathname)
   :type newext
   :directory (pathname-directory pathname)))

;;; �ե�����̾�γ�ĥ�ҳ���
(defun get-extension (pathname)
  (let* ((pathstring (namestring pathname))
         (i (position #\. pathstring :test #'char=)))
    (if i
        (subseq pathstring (1+ i))
      "" )))

;;; �ե�����ο������
(defun file-newer (path1 path2)
  (let ((exists1-p (probe-file path1))
        (exists2-p (probe-file path2)))
    (cond
     ((and exists1-p (not exists2-p))
      t)
     ((and (not exists1-p) exists2-p)
      nil)
     (t
      (> (file-write-date path1)
         (file-write-date path2))))))

;;; CLTL�ˤʤ�namestring��
(defun name-namestring (pathname)
  (namestring (make-pathname :name (pathname-name pathname))))

;;;;;; ʸ�������

;;; ��ʸ������ʸ���ε�ž
(defun char-invertcase (char)
  (cond
   ((char<= #\a char #\z)
    (char-upcase char))
   ((char<= #\A char #\Z)
    (char-downcase char))
   (t
    char)))

(defun case-mixed-p (seq)
  (and (some #'upper-case-p seq)
       (some #'lower-case-p seq)))

(defun string-invertcase (str)
  (map 'string #'char-invertcase str))

;;; str2 �� str1 �ǻϤޤ�ʸ����ʤ顢���λĤ��ʸ������֤�
(defun string-begin-with (str1 str2)
  (let ((pos (search str1 str2)))
    (and (eql 0 pos)
         (string-left-ntrim str2 (length str1)))))

;;; character or string or symbol �Υꥹ�Ȥ�ʸ����Ȥ��Ʒ��
(defun string+ (&rest strings)
  (apply #'concatenate 'string
         (mapcar #'string strings)))

;;; tree��ˤ������Ƥ�ʸ�������
(defun string+-rec (&rest trees)
  (with-output-to-string (s)
    (do-all-atoms #'(lambda (x) (when x (princ x s)))
      trees)))

;;; �ꥹ�����ʸ�������
(defun strcat (string-list &optional (inter "") (prev "") (post ""))
  (apply #'string+
         (separate-list string-list inter prev post)))

;;; ʸ���󻲾�
(defun string-ref (str n)
  (check-type str string)
  (if (>= n (length str))
      nil
    (aref str n)))

;;; ʸ�����ɲ�
(defmacro add-string (str &rest slist)
  (let ((newstr (nconc `(concatenate 'string ,str) slist)))
    `(setq ,str ,newstr)))

;;; �ǽ��nʸ��
(defun string-firstn (str n &optional (ellipsis "..."))
  (with-output-to-string (s-out)
    (with-input-from-string (s-in str)
      (loop for i from 1 to n
          do (aif (read-char s-in nil nil)
                 (write-char it s-out)
               (loop-finish))
          finally 
            (when (and ellipsis (read-char s-in nil nil))
              (write-string ellipsis s-out))))))

;;; nʸ�����
(defun string-left-ntrim (str &optional (n 1))
  (values (subseq str n)
          (subseq str 0 n)))

;;; ��������˹礦ʸ������
(defun string-left-trim-if (str func)
  (if (or (string= "" str)
          (not (funcall func (aref str 0))))
      (values str "")
    (let ((str-list (concatenate 'list str)))
      (do ((prev str-list (cdr prev))
           (curr (cdr str-list) (cdr curr)))
          ((or (endp curr)
               (not (funcall func (car curr))))
           (rplacd prev nil)
           (values (concatenate 'string curr)
                   (concatenate 'string str-list)))))))

;;; ������
(defun string-left-trim-space (string)
  (string-left-trim-if
   string
   #'(lambda (c) 
       (member c '(#\Tab #\Newline #\Page #\Return #\Space)))))

;;; ����Ǥʤ���Τ���
(defun string-left-trim-notspace (string)
  (string-left-trim-if
   string
   #'(lambda (c) 
       (not
        (member c '(#\Tab #\Newline #\Page #\Return #\Space))))))


;; ch����ʬ��str���֤�������
(defun substitute-string (newstr old string)
  (with-output-to-string (ost)
    (map nil #'(lambda (ch)
                 (if (char= ch old)
                     (write-string newstr ost)
                   (write-char ch ost)))
         string)))

;; ʸ����str��chars�˴ޤޤ��ʸ����ʬ�䤷�ƥꥹ�Ȥˤ���
(defun split-string (str &optional (chars '(#\Tab #\Newline #\Page #\Return #\Space))
                     &aux (ret (list)))
  (setq chars (mklist chars))
  (with-input-from-string (s-in str)
    (loop
      (do ((ch (read-char s-in nil nil)
               (read-char s-in nil nil)))
          ((or (and (null ch)
                    (return-from split-string (nreverse ret)))
               (not (member ch chars)))
           (unread-char ch s-in)))
      (push (with-output-to-string (s-out)
              (do ((ch (read-char s-in nil nil)
                       (read-char s-in nil nil)))
                  ((or (null ch)
                       (member ch chars)))
                (write-char ch s-out)))
            ret))))

;; ʸ����str��ǽ��chars�˴ޤޤ��ʸ����ʬ�䤷��2���ǤΥꥹ�Ȥ��֤�
(defun split-string-1 (str &optional (chars '(#\Tab #\Newline #\Page #\Return #\Space)))
  (setq chars (mklist chars))
  (with-input-from-string (s-in str)
    (let* ((ret1
            (with-output-to-string (s-out)
              (do ((ch (read-char s-in nil nil)
                       (read-char s-in nil nil)))
                  ((or (null ch)
                       (member ch chars)))
                (write-char ch s-out))))
           (ret2
            (with-output-to-string (s-out)
              (do ((ch (read-char s-in nil nil)
                       (read-char s-in nil nil)))
                  ((or (null ch)))
                (write-char ch s-out)))))
      (list ret1 ret2))))

;; case��ʸ������
(defmacro string-case (exp &body case-clauses)
  (with-fresh-variables (exp-var)
    `(let ((,exp-var ,exp))
       (cond
        ,@(loop for clause in case-clauses 
              collect
                (cond 
                 ((eq 'cl:otherwise (car clause))
                  `(t ,@(cdr clause)))
                 (t
                  `((or ,@(mapcar
                           #'(lambda (str)
                               `(string= ,exp-var ,str))
                           (mklist (car clause))))
                    ,@(cdr clause)))))))))

;;;;;; multiple-value ��Ϣ
(defmacro nth-multiple-value (n form)
  `(nth ,n
        (multiple-value-list ,form)))

;;;;;; ����ܥ����

;;; �ͤθ�
(defmacro swap (place1 place2)
  (let ((temp (gensym)))
    `(let ((,temp ,place1))
       (setf ,place1 ,place2
             ,place2 ,temp))))

;;; x ��� pkg2 ����Ͽ����Ƥ��륷��ܥ�( inherited ��ޤ� )
;;; �� pkg1 ����Ͽ��ľ������Τ��֤�
(defun immigrate-package (x pkg1 &optional pkg2)
  (map-all-atoms 
      #'(lambda (xx)
          (if (and (symbolp xx)
                   (or (null pkg2)
                       (eq xx (find-symbol (symbol-name xx) pkg2))))
              (intern (symbol-name xx) pkg1)
            xx))
    x))

;;; concatenate symbols
(defun cat-symbol (sym1 sym2)
  (unless (and (symbolp sym1) (symbolp sym2))
    (error "~s or ~s is not symbol" sym1 sym2))
  (make-symbol
   (concatenate 'string (symbol-name sym1) (symbol-name sym2))))

(defun symbol+ (sym1 &rest rest-syms)
  (let ((package (symbol-package sym1))
        (symstr (apply #'string+ 
                       (symbol-name sym1)
                       (mapcar #'symbol-name rest-syms))))
    (if package
        (intern symstr package)
      (make-symbol symstr))))

;;; ��Ͽ����Ƥ���packge�˰ͤ餺������ܥ�̾��Ʊ�����ɤ�����Ƚ��
(defun symbol= (sym1 sym2)
  (and
   (symbolp sym1)
   (symbolp sym2)
   (string= (symbol-name sym1) (symbol-name sym2))))

;;;;;; �ꥹ�����

;;; list �� `(,@prev ,el1 ,@inter ,el2 ,@inter ... ,eln ,@post)
(defun separate-list (elms separator
		      &optional (head nil head-p) (tail nil tail-p))
  (nconc (when head-p (list head))
         (when elms
           (cons (car elms)
                 (mapcan #'(lambda (x) (list separator x))
                         (cdr elms))))
         (when tail-p (list tail))))

;;; member-from-tail
(defun member-from-tail (item list &key (key #'identity) (test #'eql) test-not)
  (do ((cur (member item list :key key :test test)
            (member item (cdr cur) :key key :test test))
       (prev nil cur))
      ((endp cur)
       (if prev prev test-not))))

;;; �ꥹ�Ȥ������������ɲ�
(defmacro add-element (place &rest elist)
  `(setf ,place (nconc ,place ,`(list ,@elist)))) 

;;; ʣ�����push����٤˽�
(defmacro pushs (&rest args &aux (place (car (last args))))
  `(progn
     ,@(mapcar #'(lambda (x)
                   `(push ,x ,place))
               (butlast args))))

;; list��n���ܤ�ľ���� xs ��append������Τ��֤���n=0��append��Ʊ����
(defun insert (xs list &optional (n 0))
  (multiple-value-bind (prev post)
      (list-until list (nthcdr n list))
    (nconc prev (append xs post))))

;; list��n���ܤ�ľ���� x ��cons������Τ��֤���n=0��cons��Ʊ����
(defun insert1 (x list &optional (n 0))
  (insert (list x) list n))

;;; �ꥹ�Ȥ���Ƭn���Ǥ����
(defun firstn (x &optional (n 1) &key (func #'nreverse))
  (labels ((firstn-tail (x n acc)
             (if (or (= n 0) (null x))
                 acc
               (firstn-tail (cdr x) (1- n) (cons (car x) acc)))))
    (funcall func (firstn-tail x n nil))))

;;; �ꥹ�Ȥ�n���ܤ�new ���֤��������Τ�Τ��֤�
;;; n-new-list == (n1 new1 n2 new2 ...)
;; > (substitute-n '(1 2 3 4) 1 nil 2 10)
;; (1 nil 10 4)
(defun substitute-n (list &rest n-new-list)
  (loop
      for x in list
      as i from 0
      collect 
        (progn (loop for n-new on n-new-list by #'cddr
                   thereis (and (= i (car n-new))
                                (progn (setq x (cadr n-new))
                                       t)))
               x)))

;;; position-rec
(defun position-rec (x list &key (test #'eql) (key #'identity))
  (labels ((rec (focus acc)
             (cond
              ((funcall test x (funcall key focus))
               acc)
              ((atom focus)
               nil)
              (t
               (or (rec (car focus) (cons 'car acc))
                   (rec (cdr focus) (cons 'cdr acc)))))))
    (rec list nil)))

;;; �ꥹ�Ȥ����Ǥ� :key �����Ǥ� :test �ΰ�̣�����������Ʊ�ΤǤޤȤ��
;; > (assort '((1 2) (1 3) (3 4) (2 5) (9 3) (3 2)) :key #'car)
;; (((9 3)) ((2 5)) ((3 2) (3 4)) ((1 3) (1 2)))
(defun assort (lst &key (test #'eql) (key #'identity))
  (let ((retval '()))
    (dolist (elm lst (mapcar #'cdr retval))
      (let ((keyval (funcall key elm)))
        (aif (member keyval retval :test test :key #'car)
            (push elm (cdar it))
          (push (cons keyval (list elm)) retval))))))

;; �ꥹ�Ȥν��֤���¸����assort
;; > (stable-assort '((1 2) (1 3) (3 4) (2 5) (9 3) (3 2)) :key #'car)
;; (((1 2) (1 3)) ((3 4) (3 2)) ((2 5)) ((9 3)))
(defun stable-assort (lst &key (test #'eql) (key #'identity))
  (nreverse (mapcar #'nreverse (assort lst :test test :key key))))


;; �ꥹ�Ȥ��椫��Ϳ����줿���Ǥκǹ����Ǥ��դ���
(defun find-max (lst &key (test #'>) (key #'identity))
  (let* ((ret (car lst))
         (ret-key (funcall key ret)))
    (mapc #'(lambda (x)
              (let ((x-key (funcall key x)))
                (when (funcall test x-key ret-key)
                  (setq ret x
                        ret-key x-key))))
          (cdr lst))
    (values ret ret-key)))

;; �ꥹ�Ȥ��椫��ǽ�˾��˹�ä���Τ�����ơ���������Τ��֤�
(defmacro find-pop (place test &key (key #'identity))
  `(let ((test-val ,test)
         (key-val ,key))
     (if (funcall test-val (funcall key-val (car ,place)))
         (pop ,place)
       (loop 
           for prev on ,place
           as x = (cadr prev)
           when (funcall test-val (funcall key-val x))
           do (rplacd prev (cddr prev))
              (return x)))))

;; �����
(defun median (lst &key (test #'>) (key #'identity))
  (nth (truncate (/ (length lst) 2))
       (sort (copy-list lst) test :key key)))

;;; list�Ǥʤ�������ꥹ�Ȳ�
(defun mklist (obj)
  (if (listp obj) obj (list obj)))

;;; �ꥹ�Ȥ�������obj���ɲ�
(defun append1 (lst obj)
  (append lst (list obj)))


;;; �ꥹ�Ȥ�Ĺ�������������
(defun list-length>= (list n)
  (if (endp list) (<= n 0)
    (loop for x in list
        as i upfrom 1
        thereis (>= i n))))

(defun list-length= (list n)
  (if (endp list) (= n 0)
    (loop for rest on list
        as i upfrom 1
        thereis (and (= i n)
                     (return (endp (cdr rest)))))))


;;;;; ����黻

;;; �ҽ���
(defun power-set (list)
  (if (endp list)
      '(())
    (let ((remain-power-set (power-set (cdr list))))
      (append remain-power-set
              (mapcar (prefixed-func #'cons (car list))
                      remain-power-set)))))

;;; list �� p ��Ʊ�����Ǥ�ľ���ޤǤ���ʬ�Υ��ԡ���
;;; �������ǰʸ�Υꥹ�Ȥ��֤���
;;; test = #'eq , key = #'identity �ΤȤ��� ldiff �Ȥۤ�Ʊ��
(defun list-until (list p &key (test #'eq) (key #'identity))
  (labels ((l-u-tail (list acc)
             (if (or (endp list)
                     (funcall test (funcall key list) p))
                 (values (nreverse acc) list)
               (l-u-tail (cdr list) (cons (car list) acc)))))
    (l-u-tail list nil)))

;;; list ��cd..dr ����Ĵ�١�test��������ľ���ޤǤΥ��ԡ���
;;; �������ǰʸ�Υꥹ�Ȥ��֤���
(defun list-until-if (test list &key (key #'identity))
  (list-until list t 
              :test #'(lambda (x y)
                        (declare (ignore y))
                        (funcall test x))
              :key key))

;;; (a b) (1 2) (x y z) => (a 1 x) (a 1 y) (a 1 z) (a 2 x) ... (b 2 z)
(defun make-all-comb (&rest lists)
  (if (endp lists)
      '( () )
    (let ((fst (car lists))
          (rem-comb (apply #'make-all-comb (cdr lists))))
      (mapcan #'(lambda (fst-x)
                  (mapcar #'(lambda (comb) (cons fst-x comb)) rem-comb))
              fst))))

;;; make an integer list from 'from' to 'end' by step 'step'
(defun integer-list (from end &optional (step 1))

  (labels ((nl-tail (from acc)
             (if (or (and (> step 0) (> from end))
                     (and (< step 0) (< from end)))
                 acc
               (nl-tail (+ from step) (cons from acc)))))
    (reverse (nl-tail from nil))))

;;; recursive member
(defun member-rec (item list &rest args-member-rest)
  (let (ret)
    (if (setq ret (apply #'member item list args-member-rest))
        ret
      (dolist (el list nil)
        (when (and (listp el)
                   (setq ret (apply #'member-rec item el args-member-rest)))
          (return-from member-rec ret))))))

;;; ((a b c)(x y z)(m n o)...)->((a x m)(b y n)(c z o) ...)
(defun combine-each-nth (lst &key (rev t) (n (length (car lst))))
  "Args: (list)
LIST must be a list of list.
Returns a list whose Nth element is a list whose Mth element is Nth of Mth of list."
  (labels ((c-e-n-tail (lst acc)
             (if (endp lst)
                 acc
               (c-e-n-tail
                (cdr lst)
                (let ((ithcdr-el
                       (firstn (car lst) n)))
                  (mapcar #'(lambda (x)
                              (prog1
                                  (cons (car ithcdr-el) x)
                                (setq ithcdr-el (cdr ithcdr-el))))
                          acc))))))
    (mapcar
     (if rev #'reverse #'identity)
     (c-e-n-tail lst (make-list n)))))

;;; make ((a . 1) (b . 2)) from (a b) (1 2 3)
(defun cmpd-list (a b &aux ab)
  "Args: (x y)
Returns a list whose Nth element is (cons (nth x) (nth y))"
  (unless (and (listp a) (listp b))
    (error "~s or ~s is not list" a b))
  (do ((aa a (cdr aa)) (bb b (cdr bb)))
      ((endp aa) (setq ab (nreverse ab)))
    (push `(,(car aa) . ,(car bb)) ab)))

;;; Queue
(defun make-queue ()
  (cons nil nil))
(defun empty-queue-p (queue)
  (null (car queue)))
(defun front-queue (queue)
  (if (empty-queue-p queue) nil
    (caar queue)))
(defun queue-list (queue)
  (car queue))
(defun insert-queue (item queue)
  (let ((new-pair (cons item '())))
    (cond ((empty-queue-p queue)
           (rplaca queue new-pair)
           (rplacd queue new-pair))
          (t
           (rplacd (cdr queue) new-pair)
           (rplacd queue new-pair)))
    queue))
(defun delete-queue (queue)
  (if (empty-queue-p queue) nil
    (let* ((front (car queue))
           (item (car front)))
      (rplaca queue (cdr front))
      item)))

(defun find-delete-queue (queue test &key (key #'identity))
  (let ((qlist (queue-list queue)))
    (if (funcall test (funcall key (car qlist)))
        (progn
          (rplaca queue (cdr qlist))
          (car qlist))
      (loop 
          for prev on qlist
          as x = (cadr prev)
          when (funcall test (funcall key x))
          do (when (eq (cdr queue) (cdr prev))
               (rplacd queue prev))
             (rplacd prev (cddr prev))
             (return x)))))

;;;;;; �ϥå���ơ��֥�
(defun list-to-hashtable (list &optional (default-value t)
                          &rest make-hash-table-args)
  (let ((ret-hashtable (apply #'make-hash-table make-hash-table-args)))
    (loop for elm in list
        do (setf (gethash elm ret-hashtable)
             default-value))
    ret-hashtable))

(defun alist-to-hashtable (alist &rest make-hash-table-args)
  (let ((ret-hashtable (apply #'make-hash-table make-hash-table-args)))
    (loop for (key . val) in alist
        do (setf (gethash key ret-hashtable) val))
    ret-hashtable))

;;;;;; �ⳬ�ؿ��桼�ƥ���ƥ�

;;; ������֤��ؿ�
(defun const-f (&rest values)
  #'(lambda (&rest dummy)
      (declare (ignore dummy))
      (values-list values)))

;;; �����nil���Ǥ������Ǥ���nil���֤�mapcar
;;; multiple-value �б�
(defun check-mapcar (func list)
  (catch 'suspend
    (apply 
     #'values
     (combine-each-nth
      (mapcar 
       #'(lambda (x &aux (fx (multiple-value-list (funcall func x))))
           (if (car fx)
               fx 
             (throw 'suspend nil)))
       list)))))

;;; recursive mapcar
(defun rmapcar (fn &rest args)
  (if (some #'atom args)
      (apply fn args)
    (apply #'mapcar 
           #'(lambda (&rest args)
               (apply #'rmapcar fn args))
           args)))

;;; tree�����Ƥ�atom���Ǥ�fn��Ŭ�Ѥ���
;;; �֤��ͤ�arg����
(defun do-all-atoms (fn tree)
  (if (atom tree)
      (funcall fn tree)
    (progn
      (map-all-atoms fn (car tree))
      (map-all-atoms fn (cdr tree)))))

;;; tree�����Ƥ�atom���Ǥ�fn��Ŭ�Ѥ�����̤��֤�������tree�����������֤�
(defun map-all-atoms (fn tree)
  (if (atom tree)
      (funcall fn tree)
    (cons (map-all-atoms fn (car tree))
	  (map-all-atoms fn (cdr tree)))))

;;; ���˲�Ū��mapcan
(defun mappend (fn &rest lists)
  (mapcan #'copy-list (apply #'mapcar fn lists)))

;;; quote����Ƥ��뼰��
(defun quoted-p (exp)
  (and (consp exp)
       (eq 'quote (car exp))))

(defun tagged-p (tag exp &key (test #'eq))
  (and (consp exp)
       (funcall test tag (car exp))))

;;; (funall (tagger 'x 'y) 1 2) -> (x y 1 2) 
(defun tagger (&rest tags)
  #'(lambda (&rest remains)
      (append tags remains)))

;;; (funall (tail-tagger 'x 'y) 1 2) -> (1 2 x y) 
(defun tail-tagger (&rest tags)
  #'(lambda (&rest remains)
      (append remains tags)))

;;; compose functions
(defun compose-rev (&rest fns)
  (if fns
      (let* ((fn1 (car fns))
             (fns (cdr fns)))
        #'(lambda (&rest args)
            (let ((ret (apply fn1 args)))
              (loop for fn in fns
                  do (setq ret (funcall fn ret)))
              ret)))
    #'identity))

(defun compose (&rest fns)
  (apply #'compose-rev (nreverse fns)))

(defun compose-n (n fn)
  (cond
   ((= n 0)
    #'identity)
   ((> n 0)
    #'(lambda (&rest args)
        (let ((ret (apply fn args)))
          (loop for i from 2 to n
              do (setq ret (funcall fn ret)))
          ret)))
   (t
    (error "compose-n: n must be positive or zero."))
   ))

(defun f-and (&rest fns)
  #'(lambda (&rest args)
      (apply #'every fns args)))

(defun f-or (&rest fns)
  #'(lambda (&rest args)
      (some #'every fns args)))

;; fun: �ؿ��������ڥ졼�� (from "On Lisp")
(defmacro fun (expr) `#',(rbuild expr))
(defun rbuild (expr)
  (if (or (atom expr) (eq (car expr) 'lambda))
      expr
    (if (eq (car expr) 'compose)
        (build-compose (cdr expr))
      (build-call (car expr) (cdr expr)))))
(defun build-call (op fns)
  (let ((g (gensym)))
    `(lambda (,g)
       (,op ,@(mapcar #'(lambda (f)
                          `(,(rbuild f) ,g))
                      fns)))))
(defun build-compose (fns)
  (let ((g (gensym)))
    `(lambda (,g)
       ,(labels ((rec (fns)
                   (if fns
                       `(,(rbuild (car fns))
                         ,(rec (cdr fns)))
                     g)))
          (rec fns)))))

;;;;;; iteration macros
(defmacro vwhile (test &body body)
  (let ((retval (gensym)))
    `(do ((,retval nil (progn ,@body)))
         ((not ,test) ,retval))))

(defmacro till (test &body body)
  (let ((retval (gensym)))
    `(do ((,retval nil (progn ,@body)))
         (,test ,retval))))

(defmacro for ((var start stop) &body body)
  (let ((gstop (gensym)))
    `(do ((,var ,start (1+ ,var))
          (,gstop ,stop))
         ((> ,var ,gstop))
       ,@body)))

;;; memoized function 
(defun memoize (fn &key
                   (test #'eql)
                   (size nil size-p)
                   (rehash-size nil rehash-size-p)
                   (rehash-threshold nil rehash-threshold-p)
                   (use-multiple-values nil)
                   )
  (let ((cache (apply #'make-hash-table :test test
                      (nconc (when size-p (list :size size))
                             (when rehash-size-p (list :rehash-size rehash-size))
                             (when rehash-threshold-p
                               (list :rehash-threshold rehash-threshold))))))
    (if use-multiple-values
        #'(lambda (arg)
            (multiple-value-bind (val win) (gethash arg cache)
              (if win
                  (values-list val)
                (let ((rets (multiple-value-list (funcall fn arg))))
                  (setf (gethash arg cache) rets)
                  (values-list rets)))))
      #'(lambda (arg)
          (multiple-value-bind (val win) (gethash arg cache)
            (if win
                val
              (let ((ret (funcall fn arg)))
                (setf (gethash arg cache) ret)
                ret)))) )
    ))

;;; argument fixed function
(defun prefixed-func (func &rest prefix-args)
  #'(lambda (&rest args)
      (apply func (append prefix-args args))))

(defun suffixed-func (func &rest suffix-args)
  #'(lambda (&rest args)
      (apply func (append args suffix-args))))

(defun argfixed-func (func &optional prefixes suffixes)
  (apply #'prefixed-func
         (apply #'suffixed-func func suffixes)
         prefixes))

;;; ��å�������ɽ�����ƥ桼���������Ϥ����
(defun query (message)
  (format *error-output* "~A? " message)
  (read))

;;; �ꥹ�Ȥ�������Ǥ�1�ġ��桼����������Ϥˤ�ä�����
(defun query-select-list (list &key
                               (print-elm #'write-to-string) 
                               (message "Select number"))
  (assert (consp list))
  (let ((i 0)
        (user-input nil))
    (dolist (elm list (values))
      (incf i)
      (format *error-output* "~2d:~a~%" i (funcall print-elm elm)))
    (till (and (numberp user-input)
               (<= 1 user-input i))
          (setq user-input (query message)))
    (nth (1- user-input) list)))

;;;;;;; ���顼ɽ��

;; CL-USER> (error-indent ("In function ~S:" 'f)
;; 	   (iwarn "123")
;; 	   (error-indent ("In function ~S: 'g") 
;; 	     (iwarn "cde"))
;; 	   (iwarn "abc")
;; 	   (iwarn "ABC"))
;; In function f:
;;   Warning: 123
;;   In function g:
;;     Warning: cde
;;   Warning: abc
;;   Warning: ABC

(defconstant *error-indent-step* 2)
(defvar *error-indent* 0)
(defvar *error-indent-spaces* "")
(defvar *error-annotations* nil)

(defvar *no-iwarn* nil)

(declaim (inline ierror iwarn iformat))

(defmacro error-indent (message &body body)
  `(let* ((*error-indent* (+ *error-indent-step* *error-indent*))
          (*error-annotations*
           (cons (string+ *error-indent-spaces* ,message) *error-annotations*))
          (*error-indent-spaces*
           (make-string *error-indent* :initial-element #\Space)))
     ,@body))

(defun print-annotations ()
  (loop
      for annot in (reverse *error-annotations*)
      when annot
      do
        (fresh-line *error-output*)
        (format *error-output* "~A~%" annot))
  ;; ����ɽ������annotation ��ɽ�����ʤ���
  (mapl #'(lambda (sl) (rplaca sl nil)) *error-annotations*)
  (setq *error-annotations* nil))       ; ����loop�μ�֤�ʤ�����

(defun ierror (&rest args)
  (print-annotations)
  (apply #'error args))

(defun iwarn (&rest args)
  (unless *no-iwarn*
    (print-annotations)
    (fresh-line *error-output*)
    (format *error-output* "~AWarning: ~?~%"
      *error-indent-spaces* (car args) (cdr args))))

(defun iformat (fstr &rest args)
  (print-annotations)
  (fresh-line *error-output*)
  (format *error-output* "~A~?"
    *error-indent-spaces* fstr args))

;;;;;;; �ǥХå��ѥġ���

;;; recexpand����Ͽ����Ƥ��륷��ܥ뤬��Ƭ�Υե�����ˤĤ��Ƥ�
;;; �Ƶ�Ū��macroexpand���롥
;;; �ºݤ�ɾ�������ե����फ��Ƚ��ʥǡ����Ȥζ��̡ˤϤ��ʤ�
;;; once ����nil�ʤ������٤�expand����
(defvar *recexpand-symbols* (list))
(defun macroexpand-rec (form &optional (once t))
  (macroexpand-rec2 (funcall (if once #'macroexpand-1 #'identity)
                             form)))
(defun macroexpand-rec2 (form)
  (cond
   ((atom form) form)
   ((member (car form) *recexpand-symbols*)
    (macroexpand-rec2 (macroexpand-1 form)))
   (t
    (loop for rest on form
        as cur = (car rest)
        collect (macroexpand-rec2 cur)))))

;;; *recexpand-symbols*�ؤ���Ͽ�����
(defun recexpand (&rest symbols)
  (setq *recexpand-symbols*
    (delete-duplicates (nconc *recexpand-symbols* symbols))))
(defun unrecexpand (&rest symbols)
  (loop for sym in symbols
      do (setq *recexpand-symbols* (delete sym *recexpand-symbols*)))
  *recexpand-symbols*)
(defun recexpand-clean ()
  (setq *recexpand-symbols* nil))



