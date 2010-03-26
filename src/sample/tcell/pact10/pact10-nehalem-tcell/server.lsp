;;; Copyright (c) 2009 Tasuku Hiraishi <tasuku@media.kyoto-u.ac.jp>
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

;;;; Tascell server
;;; ��ư�� (make-and-start-server) ɬ�פ˱�����keyword parameter�򻲾�
;;; ��ǽɾ���κݤϡ�  (push :tcell-no-transfer-log *features*)
;;; �򤷤ơ������ط��Υ����ɤ�̵�뤷�ƥ���ѥ��뤹�롥

(eval-when (:compile-toplevel :load-toplevel :execute)
  (unless (featurep :allegro)
    (error "Sorry! This programs work only on Allegro Common Lisp!"))
  (when (featurep :mswindows)
    (setq *locale* (find-locale "japan.EUC")))
  ;; The most debuggable (and yet reasonably fast) code, use
  ;; (proclaim '(optimize (speed 3) (safety 1) (space 1))); (debug 3)))
  (proclaim '(optimize (speed 3) (safety 0) (space 1)))

  ;; compile and load external lisp modules
  (deftype gate () (type-of (mp:make-gate nil)))
  (use-package :socket)
  (let ((pathname (or *compile-file-pathname* *load-pathname*)))
    (load (compile-file-if-needed
           (or (probe-file (make-pathname 
                            :name "sc-misc" :type "lsp"
                            :directory (pathname-directory pathname)))
               "../../sc-misc.lsp")
           :output-file "sc-misc.fasl"))
    (use-package "MISC")
    (load (compile-file-if-needed
           (or (probe-file (make-pathname 
                            :name "queue" :type "lsp"
                            :directory (pathname-directory pathname)))))))
  ;; Uncomment to ignore logging code
  ;; (push :tcell-no-transfer-log *features*)
  )


;;; logging, debug print
(defvar *log-lock* (mp:make-process-lock))

(defun tcell-print-log (format-string &rest args)
  (mp:with-process-lock (*log-lock*)
    (apply #'format *error-output* format-string args)
    (force-output *error-output*)))

#-tcell-no-transfer-log
(defmacro tcell-server-dprint (format-string &rest args)
  `(when *transfer-log*                 ; *transfer-log* is defined below
     (mp:with-process-lock (*log-lock*)
       (format *error-output* ,format-string ,@args)
       ;; (force-output *error-output*)
       )))
#+tcell-no-transfer-log
(defmacro tcell-server-dprint (&rest args)
  (declare (ignore args))
  '(progn))



;;; ����Ū���Ԥ������ݡ��Ⱥ�����
(defparameter *reuse-address* t)

;;; �Ƥؤ���³���ɥ쥹:�ݡ���
(defparameter *parent-host* nil)
(defparameter *parent-port* 8888)

;;; �Ҥ������³�ۥ���/�ݡ���
(defparameter *server-host* "localhost")
(defparameter *children-port* 8888)

;;; any�Ǥʤ�treq����ž��
(defparameter *transfer-treq-always-if-notany* t)

;;; log���Ϥ�̵ͭ
(defparameter *transfer-log* t)         ; (featurep :tcell-no-transfer-log) �ξ��Ͼ��̵��
(defparameter *connection-log* t)
;;; send/recv�Υ�������Ϥ���Ĺ��
(defparameter *transfer-log-length* 70)

;;; read-line���ɤ߹����Ĺ���ʵ�����륳�ޥ�ɹԤ�Ĺ���ˤκ���
(defconstant *max-line-length* 128)
;;; task, rslt, data�ΥХåե���ž������Ȥ��ΥХåե��Υ�����
;;; ��ʬ�礭�����ʤ��ȥǥåɥ��å��θ���
(defconstant *body-buffer-size* 4096)

;;; ���ޥ�ɤ�³���ƥǡ�����Ȥ�ʤ����ޥ��
;;; These constants are referred to in compile time with #. reader macros.
(eval-when (:execute :load-toplevel :compile-toplevel)
  (defconstant *commands* '("treq" "task" "none" "back" "rslt" "rack" "dreq" "data" "leav" "log"
                            "stat" "verb" "eval" "exit"))
  (defconstant *commands-with-data* '("task" "rslt" "data"))
  (defconstant *commands-without-data* (set-difference *commands* *commands-with-data* :test #'string=)))

(defparameter *retry* 20)


(defclass host ()
  ((server :accessor host-server :type tcell-server :initarg :server)
   (host :accessor host-hostname :type string :initarg :host)
   (port :accessor host-port :type fixnum :initarg :port)
   (sock :accessor host-socket :type stream :initarg :sock)
   (sender :accessor host-sender :type sender :initform nil)
   (receiver :accessor host-receiver :type receiver :initform nil)
   (last-none-time :accessor host-none-time :type fixnum :initform -1)
                                        ; �Ǹ��none�������ä������get-internal-real-time�ǳ�����
                                        ; task�����ä������Ф��Ƥϥꥻ�åȤ���
   ))
  

(defclass parent (host)
  ((host :initform *parent-host*)
   (port :initform *parent-port*)
   (n-treq-sent :accessor parent-n-treq-sent :type fixnum :initform 0) ; ̤����
   (diff-task-rslt :accessor parent-diff-task-rslt :type fixnum :initform 0)
                                        ; <task����ä����>-<rslt�����ä����> (child�Ȥϰ㤦�Τ�����)
   ))

;; terminal-parent��auto-rack, auto-resend-task��¸����뤿���
;; �Ф��Ƥ���task���ޥ�ɤξ���
(defstruct task-home
  task-cmd                              ; task���ޥ�ɤ��Τ��
  rack-to                               ; rack������
  rack-task-head                        ; rack��task-head
  start-time)                           ; ���ϻ��� (get-internal-real-time)

(defclass terminal-parent (parent)
  ((host :initform "Terminal")
   (port :initform -1)
   (sock :initform (make-two-way-stream *standard-input* *standard-output*))
   (auto-rack :accessor parent-auto-rack :type boolean :initarg :auto-rack :initform nil)
                                        ; ��ưŪ��rack���֤�
   (auto-resend-task :accessor parent-auto-resend-task :type fixnum :initarg :auto-resend-task :initform 0)
                                        ; auto-rack�塤��ưŪ��Ʊ��task�������������
   (auto-exit :accessor parent-auto-exit :type boolean :initarg :auto-exit :initform nil)
                                        ; auto-rack�塤����auto-resend-task���Ԥ��Ƥ����鼫ư��exit������
   (auto-treq-response-func :accessor parent-auto-treq-response-func :type function :initform nil)
                                        ; treq�����ä��鼫ưŪ�˼¹Ԥ����ؿ�
   (task-home :accessor parent-task-home :type list :initform ())
                                        ; ����¸����뤿��˿Ƥ�������줿task��Ф��Ƥ����ꥹ��
   ))

(defclass child (host)
  ((id :accessor child-id :type fixnum)
   (diff-task-rslt :accessor child-diff-task-rslt :type fixnum :initform 0)
                                        ; <task�����ä����>-<rslt���֤äƤ������>
   (work-size :accessor child-wsize :type fixnum :initform 0)
                                        ; �Ż����礭�����ܰ¡�
                                        ; task��������/���ä���(- <ndiv>)�˹���
   ;; (in-treq :accessor child-in-treq :type boolean :initform nil)
   ))

(defclass ta-entry ()                   ; elemens of treq-any-list
  ((from :accessor tae-from :type host :initarg :from)
   (head :accessor tae-head :type string :initarg :head)
   ))

(defclass sender ()
  ((queue :accessor send-queue :type shared-queue
          :initform (make-instance 'shared-queue) :initarg :queue)
   (writer :accessor writer :type function ; obj stream -> (write to stream)
           :initform #'write-string :initarg :writer)
   (send-process :accessor send-process :type mp:process :initform nil)
   (destination :accessor sender-dest :type stream :initform nil :initarg :dest)))
(defclass receiver ()
  ((queue :accessor receive-queue :type shared-queue
          :initform (make-instance 'shared-queue) :initarg :queue)
   (reader :accessor reader :type function ; stream -> <obj,eof-p>
           :initform #'(lambda (strm) (aif (read-line strm nil nil) (values it nil)
                                        (values nil t)))
           :initarg :reader)
   (receive-process :accessor receive-process :type mp:process :initform nil)
   (source :accessor receiver-src :type stream :initform nil :initarg :src)))

(defclass tcell-server ()
  ((host :accessor ts-hostname :initform *server-host* :initarg :local-host)
   (message-queue :accessor ts-queue :type shared-queue
                  :initform (make-instance 'shared-queue))
   (proc-cmd-process :accessor ts-proc-cmd-process :type mp:process :initform nil)
   (read-cmd-process :accessor ts-read-cmd-process :type mp:process :initform nil)
   (parent :accessor ts-parent :type parent)
   (children-port :accessor ts-chport :initform *children-port* :initarg :children-port)
   (children-sock0 :accessor ts-chsock0 :type stream) ; �Ԥ�����
   (children :accessor ts-children :type list :initform '())
   (eldest-child :accessor ts-eldest-child :type child :initform nil)
   (n-children :accessor ts-n-children :type fixnum :initform 0)
   (n-wait-children :accessor ts-n-wait-children :type fixnum :initform 0 :initarg :n-wait-children)
                                        ; ���ο���child����³����ޤǥ�å�����������Ԥ�ʤ�
   (child-next-id :accessor ts-child-next-id :type fixnum :initform 0)
   (exit-gate :accessor ts-exit-gate :type gate :initform (mp:make-gate nil))
   (treq-any-list :accessor ts-talist :type list :initform '()) ;; treq-any��Ф��Ƥ��ʤ��ꥹ��
   (accept-connection-process :accessor ts-accept-connection-process
                              :type mp:process :initform nil)
   (retry :accessor ts-retry :type fixnum :initform *retry* :initarg :retry)
   ))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Initializers
(defmethod initialize-instance :after ((sdr sender) &rest initargs)
  (declare (ignore initargs))
  (setf (send-process sdr)
    (mp:process-run-function "SEND-PROCESS"
      #'monitor-and-send-queue
      (send-queue sdr) (sender-dest sdr) (writer sdr))))

(defmethod initialize-instance :after ((rcvr receiver) &rest initargs)
  (declare (ignore initargs))
  (setf (receive-process rcvr)
    (mp:process-run-function "RECEIVE-PROCESS"
      #'receive-and-add-to-queue
      (receive-queue rcvr) (receiver-src rcvr) (reader rcvr))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Finalizers
(defmethod finalize ((sdr sender))
  (when (send-process sdr)
    (mp:process-wait-with-timeout "Wait until send queue becomes empty"
                                  5
                                  #'empty-queue-p (send-queue sdr))
    (mp:process-kill (send-process sdr))
    (setf (send-process sdr) nil)))

(defmethod finalize ((rcvr receiver))
  (when (receive-process rcvr)
    ;; (mp:process-wait-with-timeout "Wait until receive queue becomes empty"
    ;;                               5
    ;;                               #'empty-queue-p (receive-queue rcvr))
    (mp:process-kill (receive-process rcvr))
    (setf (receive-process rcvr) nil)))

(defmethod finalize ((sv tcell-server))
  (when (ts-accept-connection-process sv)
    (mp:process-kill (ts-accept-connection-process sv))
    (setf (ts-accept-connection-process sv) nil))
  (when (ts-proc-cmd-process sv)
    (mp:process-kill (ts-proc-cmd-process sv))
    (setf (ts-proc-cmd-process sv) nil))
  (when (ts-read-cmd-process sv)
    (mp:process-kill (ts-read-cmd-process sv))
    (setf (ts-read-cmd-process sv) nil))
  (finalize (ts-parent sv))
  (loop for chld in (ts-children sv)
      do (finalize chld))
  (close (ts-chsock0 sv)))

(defmethod finalize ((hst host))
  (finalize (host-sender hst))
  (finalize (host-receiver hst)))

(defmethod finalize :before ((hst child))
  (send-exit hst))

(defmethod finalize ((hst (eql nil))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmethod start-server ((sv tcell-server) (prnt parent))
  (unwind-protect
      (progn
        ;; �Ƥ���³
        ;; sv.parent = connect_to (prnt):
        (setf (ts-parent sv) (connect-to prnt))
        ;; �Ҥ�����Ԥ������ݡ��Ȥ򳫤�
        (setf (ts-chsock0 sv)
          (make-socket :connect :passive
                       :reuse-address *reuse-address*
                       :local-host (ts-hostname sv)
                       :local-port (ts-chport sv)))
        ;; �ǽ�λҤ���³���Ƥ���Τ��Ԥ�
        (format *error-output* "~&Waiting for connection to ~A:~D...~%"
          (ts-hostname sv) (ts-chport sv))
        (wait-and-add-child sv)
        ;; �Ҥ������³�����դ��ѥץ�������ư
        (activate-accept-connection-process sv)
        ;; ɬ�׿��λҤ���³���Ƥ���Τ��Ԥġ�n-wait-children���ץ�����
        (wait-children-connections sv)
        ;; ��å����������ѥץ�������ư
        (activate-proc-cmd-process sv)
        ;; �Ƥ����ϥ��ȥ꡼�फ��������Ԥ��ץ�������ư
        (activate-read-cmd-process sv prnt)
        ;; exit���ޥ�ɤˤ�ä�exit-gate��open�ˤʤ�ޤǤ��������
        (with1 gate (ts-exit-gate sv)
          (mp:process-wait "Wait for exit command" #'mp:gate-open-p gate))
        )
    (finalize sv)))

(defmethod print-server-status ((sv tcell-server))
  (fresh-line *error-output*)
  (pprint (list `(parent ,(hostinfo (ts-parent sv)))
                `(children ,@(mapcar #'hostinfo (ts-children sv)))
                `(eldest-child ,(awhen (ts-eldest-child sv) (hostinfo it)))
                `(n-children ,(ts-n-children sv))
                `(child-next-id ,(ts-child-next-id sv))
                `(children-port ,(ts-chport sv))
                `(treq-any-list ,@(mapcar #'ta-entry-info (ts-talist sv)))
                `(retry ,(ts-retry sv)))
          *error-output*)
  (terpri *error-output*)
  (force-output *error-output*))

(defmethod wait-and-add-child ((sv tcell-server))
  (let ((next-child (make-instance 'child :server sv)))
    (awhen (connect-from next-child (ts-chsock0 sv) :wait t)
      (add-child sv it))))

(defmethod wait-children-connections ((sv tcell-server))
  (mp:process-wait "Wait for connections of children."
                   #'(lambda () (<= (ts-n-wait-children sv) (ts-n-children sv)))))

(defmethod activate-accept-connection-process ((sv tcell-server))
  (setf (ts-accept-connection-process sv)
    (mp:process-run-function "ACCEPT-CHILD-CONNECTION"
      #'(lambda () (loop (wait-and-add-child sv))))))

;;; �����ФΥ�å����������ץ�������ư
(defmethod activate-proc-cmd-process ((sv tcell-server))
  (setf (ts-proc-cmd-process sv)
    (mp:process-run-function "PROC-CMD"
      #'(lambda (msg-q &aux (msg-gate (sq-gate msg-q)))
          (loop
            ;; �Ƥޤ��ϻҤ���������Ԥ�
            (mp:process-wait "Waiting for a message to Tascell server."
                             #'mp:gate-open-p msg-gate)
            ;; ��å���������
            (while (mp:gate-open-p msg-gate)
              (destructuring-bind (host . message) (delete-queue msg-q)
                (proc-cmd sv host message))
              (retry-treq sv))))
      (ts-queue sv))))

;;; �����ФΥ�å������ɤ߹��ߥץ�������ư
(defmethod activate-read-cmd-process ((sv tcell-server) (prnt parent))
  (setf (ts-read-cmd-process sv)
    (mp:process-run-function "READ-FROM-PARENT"
      #'(lambda ()
          (let* ((msg-q (ts-queue sv))
                 (prnt (ts-parent sv))
                 (reader (make-receiver-reader prnt))
                 (src (host-socket prnt)))
            (loop
              (multiple-value-bind (msg eof-p) (funcall reader src)
                (add-queue msg msg-q)
                (when eof-p (return))))
            )))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmethod host-name-port ((hst host))
  (string+ (host-hostname hst) ":" (write-to-string (host-port hst))))
(defmethod host-name-port ((hst terminal-parent))
  "Terminal")

(defmethod hostinfo ((chld child))
  (string+ (host-name-port chld)
           " (child " (hostid chld) ")"))
(defmethod hostinfo ((prnt parent))
  (string+ (host-name-port prnt)
           " (parent)"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmethod connect-to ((hst host))
  (setf (host-socket hst)
    (make-socket :remote-host (host-hostname hst)
                 :remote-port (host-port hst)))
  (initialize-connection hst)
  hst)

(defmethod connect-to ((hst terminal-parent))
  (assert (host-socket hst))
  (initialize-connection hst)
  hst)

(defmethod connect-from ((hst host) sock0 &key (wait t))
  (let ((sock (setf (host-socket hst)
                (socket:accept-connection sock0 :wait wait))))
    (if sock
        (progn
          (initialize-connection hst)
          hst)
      nil)))

(defmethod connect-from :after ((hst host) sock0 &key (wait t))
  (declare (ignorable sock0 wait))
  (when (host-socket hst)
    (when *connection-log*
      (tcell-print-log "~&Accept connection from ~A.~%" (host-name-port hst)))))

(defmethod initialize-connection ((hst host) &aux (sock (host-socket hst)))
  (setf (host-hostname hst) (with1 ipaddr (socket:remote-host sock)
                              (or (socket:ipaddr-to-hostname ipaddr)
                                  (socket:ipaddr-to-dotted ipaddr))))
  (setf (host-port hst) (socket:remote-port sock))
  hst)

(defmethod initialize-connection ((hst terminal-parent))
  (initialize-sender hst)
  hst)

;; make-instance -> initialize-connection -> initialize-sender/receiver�ʤΤ�
;; initialize-instance�ǤǤ��ʤ�
(defmethod initialize-sender ((hst host) &aux (sock (host-socket hst)))
  (setf (host-sender hst)
    (make-instance 'sender :dest sock :writer #'sender-writer))
  hst)

(defmethod initialize-receiver ((hst host) &aux (sock (host-socket hst)))
  (setf (host-receiver hst)
    (make-instance 'receiver
      :src sock :reader (make-receiver-reader hst)
      :queue (ts-queue (host-server hst)))) ; read������Τ϶�ͭ�ΥХåե��������
  hst)

;;;
(defun sender-writer (obj dest)
  (etypecase obj
    (list (mapc #'(lambda (ob) (sender-writer ob dest)) obj))
    (character (write-char obj dest))
    (string (write-string obj dest))
    (function (funcall obj dest))
    (array (write-sequence obj dest))))

(defun msg-log-string (obj &optional (separator #\Space))
  (with-output-to-string (s)
    (write-msg-log obj s separator)))

(defun write-msg-log (obj dest &optional (separator #\Space))
  (typecase obj
    (list (write-msg-log (car obj) dest separator)
          (mapc #'(lambda (ob)
                    (when separator (write-char separator dest))
                    (write-msg-log ob dest separator))
                (cdr obj)))
    ;; (symbol (write-string (symbol-name obj) dest))
    (character (if (char= #\Newline obj)
                   (write-string "_" dest)
                 (write-char obj dest)))
    (string (write-string obj dest))
    (function (write-string "#<data body>" dest))
    (array (format dest "#<Binary data: SIZE=~D>" (length obj)))))

(defmethod make-receiver-reader ((hst host))
  (let ((line-buffer (make-array *max-line-length*
                                 :element-type 'standard-char :fill-pointer *max-line-length*))
        (body-buffer (make-array *body-buffer-size* :element-type 'unsigned-byte :adjustable t))
        (gate (mp:make-gate t)))        ; body-buffer �λ��ѵ���
    #'(lambda (stream)
        (setf (fill-pointer line-buffer) *max-line-length*)
        (let* ((n-char (setf (fill-pointer line-buffer)
                         (excl:read-line-into line-buffer stream nil 0)))
               (eof-p (= 0 n-char))
               (msg (if eof-p '("leav") (split-string line-buffer))))
          (string-case-eager (car msg)
            (#.*commands-with-data*
             (mp:process-wait "Waiting for finishing reading body from the buffer"
                              #'mp:gate-open-p gate)
             (mp:close-gate gate)
             (setq msg (nconc msg (read-body-into-buffer stream body-buffer) ; read-body-into-buffer��
                              (list #'(lambda (dummy) (declare (ignore dummy))
                                              (mp:open-gate gate)))))
             #+comment                  ;forward-body��
             (setq msg (nconc msg (list #'(lambda (ostream)
                                            (forward-body stream ostream line-buffer body-buffer
                                                          *max-line-length* *body-buffer-size*)
                                            (mp:open-gate gate))))))
            (#.*commands-without-data* nil)
            (otherwise (error "Unknown command ~S from ~S." msg (hostinfo hst))))
          (tcell-server-dprint "(~D): Received ~S from ~S~%"
                               (get-internal-real-time)
                               (msg-log-string msg) (hostinfo hst))
          (values (cons hst msg) eof-p))
        )))

;;; "task", "rslt", "data" ��body�����ɤ߹��ߡ��ꥹ�Ȥˤ����֤�
;;; �Х��ʥ���ʬ��buffer�˽񤭹��ߡ��ɤ߽Ф�����δؿ����Ѱդ���
(defun read-body-into-buffer (stream buffer)
  (let ((ret '())
        (buf-used 0))
    (declare (fixnum buf-used))
    (loop
      (let* ((pre (read-line stream t))
             (len (length pre)))
        ;; ���Ԥǽ�λ
        (when (= len 0) (return))
        (pushs pre #\Newline ret)
        (tcell-server-dprint "~A~%" pre)
        ;; #\(�Ǥ���äƤ����鼡�ιԤ�byte-header��������byte-data
        (when (char= #\( (aref pre (- len 1)))
          ;; �إå�: <whole-size> <elm-size> <endian(0|1)>
          (let* ((byte-header (read-line stream t))
                 (whole-size (parse-integer byte-header :junk-allowed t)))
            (declare (fixnum whole-size))
            (pushs byte-header #\Newline ret)
            (tcell-server-dprint "Binary data header: ~A~%" byte-header)
            (let ((start buf-used) (end (+ buf-used whole-size)))
              (when (> end (length buffer))
                (with1 newsize
                    (do ((sz (* 2 (length buffer)) (+ sz sz)))
                        ((>= sz end) sz))
                  (adjust-array buffer newsize)
                  (tcell-server-dprint "Extended buffer size to ~D~%" newsize)))
              (read-sequence buffer stream :start start :end end)
              (push #'(lambda (ostream)
                        (write-sequence buffer ostream :start start :end end))
                    ret))
            (tcell-server-dprint "#<byte-data size=~D>~%" whole-size)
            ;; ���θ塤terminator ")\n" �����뤬��
            ;; ����iteration�ǡ�ñ��¾��ʸ����ǡ�����Ʊ�ͤ˽���
            ))))
    (nreverse ret)))

;;; "task", "rslt", "data" ��body����istream�����ɤ߹����ostream������
#+comment                               ; write-* �μ¹Ԥ��֥��å����ơ��ǥåɥ��å��β�ǽ��
(defun forward-body (istream ostream
                     &optional (line-buffer (make-array *max-line-length*
                                                        :element-type 'standard-char :fill-pointer *max-line-length*))
                               (buffer (make-array *body-buffer-size* :element-type 'unsigned-byte))
                               (line-bufsize (length line-buffer))
                               (bufsize (length buffer)))
  (loop
    (setf (fill-pointer line-buffer) line-bufsize)
    (let* ((len (setf (fill-pointer line-buffer)
                  (excl:read-line-into line-buffer istream nil 0))))
      ;; ���Ԥǽ�λ
      (when (= len 0) (return))
      (write-string line-buffer ostream)
      (terpri ostream)
      (tcell-server-dprint "~A~%" line-buffer)
      ;; #\(�Ǥ���äƤ����鼡�ιԤ�byte-header��������byte-data
      (when (char= #\( (aref line-buffer (- len 1)))
        (setf (fill-pointer line-buffer) line-bufsize)
        ;; �إå�read: <whole-size> <elm-size> <endian(0|1)>
        (setf (fill-pointer line-buffer)
          (excl:read-line-into line-buffer istream nil 0))
        (let* ((whole-size (parse-integer line-buffer :junk-allowed t)))
          (write-string line-buffer ostream)
          (terpri ostream)
          (tcell-server-dprint "Binary data header: ~A~%" line-buffer)
          ;; �Х��ʥ�����
          (loop for rem-size from whole-size downto 1 by bufsize
              as rd-size = (if (> rem-size bufsize) bufsize rem-size)
              do (read-sequence buffer istream :end rd-size)
                 (write-sequence buffer ostream :end rd-size))
          (tcell-server-dprint "#<byte-data size=~D>~%" whole-size)
          ;; ���θ塤terminator ")\n" �����뤬��
          ;; ����iteration�ǡ�ñ��¾��ʸ����ǡ�����Ʊ�ͤ˽���
          )))))

;;; "task", "rslt", "data" ��body�����ɤ߹��ߡ��ꥹ�Ȥˤ����֤�
#+comment
(defun read-body (stream)
  (let ((ret '()))
    (loop
      (let* ((pre (read-line stream t))
             (len (length pre)))
        ;; ���Ԥǽ�λ
        (when (= len 0) (return))
        (pushs pre #\Newline ret)
        (tcell-server-dprint "~A~%" pre)
        ;; #\(�Ǥ���äƤ����鼡�ιԤ�byte-header��������byte-data
        (when (char= #\( (aref pre (- len 1)))
          ;; �إå�: <whole-size> <elm-size> <endian(0|1)>
          (let* ((byte-header (read-line stream t))
                 (whole-size (parse-integer byte-header :junk-allowed t)))
            (pushs byte-header #\Newline ret)
            (tcell-server-dprint "Binary data header: ~A~%" byte-header)
            (let ((byte-data (make-array whole-size)))
              (read-sequence byte-data stream :end whole-size)
              (push byte-data ret)
              #-tcell-no-transfer-log   ; debug print
              (when *transfer-log* (write-msg-log byte-data *error-output*))
              )))))
    (nreverse ret)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; �Хåե��ƻ�->����
(defmethod monitor-and-send-queue ((sq shared-queue) dest
                                   &optional (writer #'write-string))
  (let ((gate (sq-gate sq)))
    (loop
      (mp:process-wait "Waiting for something is added to the queue"
                       #'mp:gate-open-p gate)
      (while (mp:gate-open-p gate)
        (funcall writer (delete-queue sq) dest))
      (force-output dest))))

;; ����->�Хåե����ɲ�
(defmethod receive-and-add-to-queue ((sq shared-queue) src
                                     &optional (reader #'read-line))
  (loop
    (multiple-value-bind (obj eof-p) (funcall reader src)
      (add-queue obj sq)
      (when eof-p (return)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; ���Ǥؤ�accessor

;;; �Ҥ��ɲá����
(defmethod add-child ((sv tcell-server) (chld child))
  ;; �Ҥξ���ν������sender��receiver�ε�ư��
  (setf (child-id chld)	(ts-child-next-id sv))
  (initialize-sender chld)
  (initialize-receiver chld)
  ;; server���֤ι���
  (incf (ts-child-next-id sv))
  (incf (ts-n-children sv))
  (push chld (ts-children sv))
  (tcell-print-log "~&Added a new child (~D children in total).~%" (ts-n-children sv))
  (unless (ts-eldest-child sv)
    (setf (ts-eldest-child sv) chld)))

(defmethod remove-child ((sv tcell-server) (chld child))
  (finalize chld)
  (setf (ts-children sv) (delete chld (ts-children sv) :count 1))
  (delete-treq-any sv chld "*")
  (when (eq (ts-eldest-child sv) chld)
    (setf (ts-eldest-child sv) (car (last (ts-children sv)))))
  (decf (ts-n-children sv))
  )

;;; (= id n) �λҤؤΥ�������
(defmethod nth-child ((sv tcell-server) n)
  (find n (ts-children sv)
        :key #'(lambda (chld) (child-id chld))
        :test #'=))

;;; ���ֻŻ����ĤäƤ����ʻ�
(defmethod most-divisible-child ((sv tcell-server) (from host))
  (with* (cur-time (get-internal-real-time)
          limit (* 0 internal-time-units-per-second)
          candidates (remove-if-not #'(lambda (x)
                                        (and (> (child-diff-task-rslt x) 0)
                                             (not (eq x from))
                                             (>= (- cur-time (host-none-time x)) limit)))
                                    (ts-children sv)))
    ;; Strategy1: �����ä�task�ο�-������ä�rslt�ο���>0 ���� �Ǹ�����ä�task��ndiv���Ǿ�
    #+comment
    (let ((max nil) (maxchld nil))
      (loop for chld in candidates
          do (when (or (null max)
                       (> (child-wsize chld) max))
               (setq max (child-wsize chld))
               (setq maxchld chld)))
      #-tcell-no-transfer-log
      (when maxchld
        (tcell-server-dprint "~&Most-divisible-child selected ~S: (child-diff-task-rslt ~D), (child-wsize ~D)~%"
                             (hostinfo maxchld) (child-diff-task-rslt maxchld) (child-wsize maxchld)))
      maxchld)
    ;; Strategy2: �����ä�task�ο�-������ä�rslt�ο���>0 �Τ�Τ��������
    (if candidates (list-random-select candidates) nil))
    )

;;; �Ҥ�work-size �򹹿�
(defmethod renew-work-size ((chld child) wsize)
  (setf (child-wsize chld) wsize))

;;; treq-any-list �ؤ������ɲ�
;;; p-task-head �ϡ�"<treq��from��id>" ":" "<treq��task-head>" 
(defmethod push-treq-any0 ((sv tcell-server) (tae ta-entry))
  (push tae (ts-talist sv)))
(defmethod push-treq-any1 ((sv tcell-server) (tae ta-entry))
  (push tae (cdr (ts-talist sv))))
(defmethod push-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (let ((entry (make-instance 'ta-entry :from from :head p-task-head)))
    (if (or (eq from (ts-eldest-child sv)) ; eldest-child�����treq��ͥ��Ū����Ƭ
            (null (ts-talist sv)))      ;  ; ��retry������Ƭ��������ä���ʹߤ򤢤���
        (push-treq-any0 sv entry)       ;  ;   ���褦�ˤ��Ƥ��뤬�����λ��˿Ƥ�
      (push-treq-any1 sv entry))))      ;  ;   �褦�ˤ��뤿���

;;; treq-any-list ��pop
(defmethod pop-treq-any ((sv tcell-server))
  (aand (pop (ts-talist sv))
        (list (tae-from it) (tae-head it))))


;;; treq-any-list ����������Ǻ��
(defmethod delete-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (setf (ts-talist sv)
    (delete (make-instance 'ta-entry :from from :head p-task-head)
            (ts-talist sv)
            :test #'ta-entry-match)))

;;; treq-any-list ��member��
(defmethod member-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (member (make-instance 'ta-entry :from from :head p-task-head)
          (ts-talist sv)
          :test #'ta-entry-match))

;;; ta-entry ��Ʊ����
(defmethod ta-entry-match ((x ta-entry) (y ta-entry))
  (and (eq (tae-from x) (tae-from y))
       (or (string= "*" (tae-head x))
           (string= "*" (tae-head y))
           (string= (tae-head x) (tae-head y)))))

;; ta-entry �ξ���
(defmethod ta-entry-info ((tae ta-entry))
  `((from ,(hostinfo (tae-from tae)))
    (head ,(tae-head tae))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; ���Х��ɥ쥹ʸ��������
(defmethod hostid ((chld child))
  (format nil "~D" (child-id chld)))

(defmethod hostid ((prnt parent))
  "p")

(defmethod hostid-to-host ((sv tcell-server) hostid)
  (cond
   ((string= "p" hostid)
    (ts-parent sv))
   (t
    (nth-child sv (parse-integer hostid)))))

;;; ���ɥ쥹����Ƭ���ڤ��äơ��Ĥ���ڤ��ä���Ƭ���ɥ쥹
;;; ����������ۥ��Ȥ��֤�
(defmethod head-shift ((sv tcell-server) head-string)
  (let* ((sp-head (split-string-1 head-string #\:))
         (host (hostid-to-host sv (first sp-head))))
    (unless host
      (warn "Connection from/to ~S does not exist." (first sp-head)))
    (list host (second sp-head))))

;;; ���ɥ쥹head-string����Ƭ��hst��id���ɲä�����Τ��֤�
(defmethod head-push ((hst host) head-string)
  (let ((sp-head (split-string-1 head-string #\:)))
    (if (string= "f" (first sp-head))   ; forward => �ɲä�����f�ʹߤΥ��ɥ쥹���֤�
        (second sp-head)
      (string+ (hostid hst) ":" head-string))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; ��å���������
(defmethod send ((to host) obj)
  (add-queue obj (send-queue (host-sender to))))

(defmethod send ((to (eql nil)) obj)
  (format *error-output* "Failed to send ~S~%" (msg-log-string obj nil)))

;; debug print
#-tcell-no-transfer-log
(defmethod send :after ((to host) obj)
  (tcell-server-dprint "~&(~D): Sent ~S to ~S~%"
                       (get-internal-real-time)
                       (msg-log-string obj nil) (hostinfo to)))

(defmethod send-treq (to task-head treq-head)
  (send to (list "treq " task-head #\Space treq-head #\Newline)))

;; treq�ؤα����Ȥ��ơ�task���ޤ���exit��ư�������ʥХå��¹��ѡ�
(defmethod send-treq :after ((to terminal-parent) task-head treq-head)
  (awhen (parent-auto-treq-response-func to)
    (funcall it to task-head treq-head)
    (setf (parent-auto-treq-response-func to) nil)))

(defmethod send-task (to wsize-str rslt-head task-head task-no task-body)
  (send to (list "task "
                 wsize-str #\Space
                 rslt-head #\Space
                 task-head #\Space
                 task-no #\Newline
                 task-body #\Newline)))
(defmethod send-task :after ((to host) wsize-str rslt-head task-head task-no task-body)
  (setf (host-none-time to) -1))
(defmethod send-task :after ((to child) wsize-str rslt-head task-head task-no task-body)
  (declare (ignore rslt-head task-head task-no task-body))
  (incf (child-diff-task-rslt to))
  (let ((wsize (parse-integer wsize-str)))
    (renew-work-size to (- wsize))))

(defmethod send-rslt (to rslt-head rslt-body)
  (send to (list "rslt " rslt-head #\Newline rslt-body #\Newline)))

(defmethod send-rslt :after ((to parent) rslt-head rslt-body)
  (declare (ignore rslt-head rslt-body))
  (decf (parent-diff-task-rslt to)))

(defmethod send-rslt :after ((to terminal-parent) rslt-head rslt-body)
  (declare (ignore rslt-body))
  ;; rack��task��exit��ư�����������task����������ǽɾ���ѡ�
  (when (parent-auto-rack to)
    (let ((end-time (get-internal-real-time)))
      (aif (find rslt-head (parent-task-home to) :test #'string=
                 :key #'(lambda (x) (third (task-home-task-cmd x))))
          (let* ((cmd (task-home-task-cmd it))
                 (rslt-head (third cmd))
                 (rslt-no (parse-integer rslt-head :junk-allowed t))
                 (rack-to (task-home-rack-to it))
                 (rack-task-head (task-home-rack-task-head it))
                 (start-time (task-home-start-time it)))
            ;; rack�ֿ�
            (format *error-output*
                "~&Time: ~S~%~
                Auto-send \"rack ~A\" to ~S~%"
              (/ (- end-time start-time)
                 internal-time-units-per-second 1.0)
              rack-task-head (hostinfo rack-to))
            (send-rack rack-to rack-task-head)
            ;; n-resend�����å���task��ư�������ؿ��򥻥å�
            ;; rslt-no��rslt-head�κǸ�ο����ˤ����ä�����Υ�����Ȥ����
            (if (< rslt-no (parent-auto-resend-task to))
                (let ((new-rslt-head (format nil "~D" (1+ rslt-no))))
                  (setf (caddr cmd) new-rslt-head)
                  ;; treq�������鼫ưŪ��task�����������褦�˴ؿ��򥻥å�
                  (setf (parent-auto-treq-response-func to)
                    #'(lambda (&rest args)
                        (declare (ignore args))
                        (sleep 1)
                        (excl:gc t)
                        (format *error-output* "~&auto-resend-task~%")
                        (setf (task-home-start-time it) (get-internal-real-time))
                        (incf (parent-diff-task-rslt to))
                        (send-task rack-to ; rack���������Ʊ���Ǥ褤���Ȥ���
                                   (second cmd) (head-push to new-rslt-head)
                                   rack-task-head (fifth cmd) (nthcdr 5 cmd)))))
              (progn
                ;; task-home�Υ���ȥ���
                (delete it (parent-task-home to) :count 1)
                ;; exit��ư��������褦�˴ؿ��򥻥å�
                (when (parent-auto-exit to)
                  (setf (parent-auto-treq-response-func to)
                    #'(lambda (&rest args)
                        (declare (ignore args))
                        (format *error-output* "~&auto-send-exit~%")
                        (proc-cmd (host-server to) to '("exit"))))))
              ))
        (warn "No task-home corresponding to rslt ~S" rslt-head)))))

(defmethod send-none (to task-head)
  (send to (list "none " task-head #\Newline)))

(defmethod send-back (to task-head)
  (send to (list "back " task-head #\Newline)))

(defmethod send-back ((to parent) task-head)
  (declare (ignore task-head))
  (decf (parent-diff-task-rslt to)))

(defmethod send-rack (to task-head)
  (send to (list "rack " task-head #\Newline)))

(defmethod send-dreq (to data-head dreq-head range)
  (send to (list "dreq " data-head #\Space dreq-head #\Space range #\Newline)))

(defmethod send-data (to data-head range data-body)
  (send to (list "data " data-head #\Space range #\Newline data-body #\Newline)))

(defmethod send-leav (to)
  (send to (list "leav" #\Newline)))

(defmethod send-stat (to task-head)
  (send to (list "stat " task-head #\Newline)))

(defmethod send-verb (to task-head)
  (send to (list "verb " task-head #\Newline)))

(defmethod send-exit (to)
  (send to (list "exit" #\Newline)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Dispatch
(defmethod proc-cmd ((sv tcell-server) (from host) cmd)
  (string-case-eager (car cmd)
    ("treq" (proc-treq sv from cmd))
    ("task" (proc-task sv from cmd))
    ("none" (proc-none sv from cmd))
    ("back" (proc-back sv from cmd))
    ("rslt" (proc-rslt sv from cmd))
    ("rack" (proc-rack sv from cmd))
    ("dreq" (proc-dreq sv from cmd))
    ("data" (proc-data sv from cmd))
    ("leav" (proc-leav sv from cmd))
    ("log"  (proc-log sv from cmd))
    ("stat" (proc-stat sv from cmd))
    ("verb" (proc-verb sv from cmd))
    ("eval" (print (eval (read-from-string (strcat (cdr cmd) #\Space)))))
    ("exit" (proc-exit sv from cmd))
    (otherwise (warn "Unknown Command:~S" cmd))))

;;; treq
(defmethod proc-treq ((sv tcell-server) (from host) cmd)
  (let ((p-task-head (head-push from (second cmd))) ; �������׵��
        (treq-head (third cmd)))        ; �׵���
    (unless
        (if (string= "any" treq-head)
            (try-send-treq-any sv from p-task-head)
          (destructuring-bind (hst0 s-treq-head)
              (head-shift sv treq-head)
            (if *transfer-treq-always-if-notany*
                (send-treq hst0 p-task-head s-treq-head)
              (try-send-treq sv hst0 p-task-head s-treq-head))))
      ;; treq������ʤ��ä����
      (refuse-treq sv from p-task-head))))

(defmethod try-send-treq-any ((sv tcell-server) (from host) p-task-head)
  (or (awhen (most-divisible-child sv from)
        (try-send-treq sv it p-task-head "any"))
      ;; ��ʬ�ΤȤ����˻Ż����ʤ���С�eldest�ʻҤ���ɽ���ƿƤ�ʹ���ˤ���
      (and (eq (ts-eldest-child sv) from)
           (try-send-treq sv (ts-parent sv)
                          p-task-head "any"))))

(defmethod try-send-treq ((sv tcell-server) (to host) p-task-head s-treq-head)
  (send-treq to p-task-head s-treq-head)
  t)

;; terminal-parent�ˤĤ��Ƥϡ����ä��Ż���ò�������Ƥ��ʤ����treq���ʤ�
(defmethod try-send-treq :around ((sv tcell-server) (to terminal-parent) p-task-head s-treq-head)
  (declare (ignore p-task-head s-treq-head))
  (if (>= 0 (parent-diff-task-rslt to))
      (call-next-method)
    nil))

;; �����������Ƥ����send
(defmethod try-send-treq :around ((sv tcell-server) (to child) p-task-head s-treq-head)
  (declare (ignore p-task-head s-treq-head))
  (if (> (child-diff-task-rslt to) 0)
      (call-next-method)
    nil))

(defmethod refuse-treq ((sv tcell-server) (from host) p-task-head)
  (if (member-treq-any sv from p-task-head)
      (send-none from (second (head-shift sv p-task-head)))
    (push-treq-any sv from p-task-head)))

;; treq-any-list�ˤ������Ǥ� try���ʤ���
(defmethod retry-treq ((sv tcell-server))
  (loop
      for n-sent upfrom 0
      do (aif (pop-treq-any sv)
             (destructuring-bind (from head) it
               (unless (try-send-treq-any sv from head)
                 (push-treq-any sv from head)
                 (loop-finish)))
           (loop-finish))
      finally (return n-sent)))

;;; task
(defmethod proc-task ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (fourth cmd))      ; ������������
    (let ((wsize-str (second cmd))      ; �Ż����礭��
          (p-rslt-head (head-push from (third cmd))) ; ����ֿ���
          (task-no (fifth cmd))         ; �������ֹ� ��fib, lu, ...��
          (task-body (nthcdr 5 cmd)))
      (send-task to wsize-str p-rslt-head s-task-head task-no
                 task-body)
      )))

(defmethod proc-task :before ((sv tcell-server) (from child) cmd)
  (let ((wsize (parse-integer (second cmd))))
    (renew-work-size from (- wsize))))

(defmethod proc-task :before ((sv tcell-server) (from parent) cmd)
  (declare (ignorable sv cmd))
  (incf (parent-diff-task-rslt from)))

;; rack��ư�����Τ���˼�����ä�task��Ф��Ƥ���
(defmethod proc-task :after ((sv tcell-server) (from terminal-parent) cmd)
  (when (parent-auto-rack from)
    (destructuring-bind (to s-task-head)
        (head-shift sv (fourth cmd))
      (let ((th-entry (make-task-home
                       :task-cmd cmd
                       :rack-to   to    ; rack������
                       :rack-task-head s-task-head ; rack��task-head
                       :start-time 0))) ; ���ϻ���ʤ��Ȥǡ�
        (push th-entry (parent-task-home from))
        (setf (task-home-start-time th-entry) (get-internal-real-time))))))

;;; none
(defmethod proc-none ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd))      ; none������
    (send-none to s-task-head)))

(defmethod proc-none :after ((sv tcell-server) (from host) cmd)
  ;; none�������ä�����򵭲�
  (declare (ignorable sv cmd))
  (setf (host-none-time from) (get-internal-real-time)))

;;; back
(defmethod proc-back ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd))      ; back������
    (send-back to s-task-head)))

(defmethod proc-back :before ((sv tcell-server) (from child) cmd)
  (declare (ignore cmd))
  (when (< (decf (child-diff-task-rslt from)) 0)
    (warn "~S: diff-task-rslt less than 0!" (hostinfo from))))

;;; rslt
(defmethod proc-rslt ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-rslt-head)
      (head-shift sv (second cmd))      ; rslt������
    (let ((rslt-body (cddr cmd)))
      (send-rslt to s-rslt-head rslt-body))))

(defmethod proc-rslt :before ((sv tcell-server) (from child) cmd)
  (declare (ignore cmd))
  (when (< (decf (child-diff-task-rslt from)) 0)
    (warn "~S: diff-task-rslt less than 0!" (hostinfo from))))

;;; rack
(defmethod proc-rack ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd))      ; rack������
    (send-rack to s-task-head)))

;;; dreq
(defmethod proc-dreq ((sv tcell-server) (from host) cmd)
  (let ((p-data-head (head-push from (second cmd))) ; �ǡ����׵��
        (range (fourth cmd)))           ; �ǡ����׵��ϰ�
    (destructuring-bind (hst0 s-dreq-head) ; �ǡ����׵���
        (head-shift sv (third cmd))
      (send-dreq hst0 p-data-head s-dreq-head range))))

;;; data
(defmethod proc-data ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-data-head)  ; data������
      (head-shift sv (second cmd))
    (let ((range (third cmd))           ; �ǡ����׵��ϰ�
          (data-body (cdddr cmd)))      ; �ǡ�������
      (send-data to s-data-head range data-body))))

;;; leav
;; �Ҥ��颪��Ͽ�򳰤����Ƥ��颪̵��
(defmethod proc-leav ((sv tcell-server) (from child) cmd)
  (declare (ignore cmd))
  (remove-child sv from))

(defmethod proc-leav ((sv tcell-server) (from parent) cmd)
  (declare (ignore cmd))
  )

;;; stat: �����С�����ξ��֤����
(defmethod proc-stat ((sv tcell-server) (from host) cmd)
  (if (cdr cmd)
      ;; ������Ϳ�������Ϥ�����stat���ޥ�ɤ�ž��
      (destructuring-bind (to s-task-head)
          (head-shift sv (second cmd))  ; stat������
        (send-stat to s-task-head))
    ;; ̵�����ξ��ϥ����Фξ��֤�ɽ��
    (print-server-status sv)))

;;; verb: �����verbose-level���ѹ�
;;; "verb <������>:<level>" �ǡ�
(defmethod proc-verb ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd))      ; verb������
    (send-verb to s-task-head)))

;;; log: �����Ф�verbose-level������
(defmethod proc-log ((sv tcell-server) (from host) cmd)
  (loop
      for str in (cdr cmd)
      as mode = (read-from-string str)
      as setter in (list #'toggle-transfer-log #'toggle-connection-log)
      do (funcall setter mode))
  (show-log-mode))

;;; exit: �����Ф�λ
(defmethod proc-exit ((sv tcell-server) (from host) cmd)
  (declare (ignore cmd))
  (mp:open-gate (ts-exit-gate sv)))

(defun toggle-transfer-log (mode)
  (setq *transfer-log* (not (not mode))))

(defun toggle-connection-log (mode)
  (setq *connection-log* (not (not mode))))

(defun show-log-mode ()
  (pprint `((*transfer-log* #-tcell-no-transfer-log ,*transfer-log*
                            #+tcell-no-transfer-log :invalidated-by-features)
            (*connection-log* ,*connection-log*))
          *error-output*)
  (terpri *error-output*)
  (force-output *error-output*))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun make-and-start-server (&key 
                              (local-host *server-host*)
                              (children-port *children-port*)
                              (n-wait-children 0)
                              (retry *retry*)
                              (terminal-parent t)
                              (auto-rack t) ; for terminal parent
                              (auto-initial-task nil)
                                        ; �ǽ�˼�ưŪ����������task
                                        ; "<task-no> <param1> <param2> ..." ��ʸ����ǻ���
                                        ; ��param�֤β��Ԥ�Ǹ�β��Ԥ���Ӷ������ס�
                              (auto-resend-task 0) ; for terminal-parent
                              (auto-exit nil) ; for terminal parent
                              (parent-host *parent-host* ph-given)
                              (parent-port *parent-port*)
                              )
  (when ph-given (setq terminal-parent nil))
  (let* ((sv (make-instance 'tcell-server
               :local-host local-host
               :children-port children-port
               :n-wait-children n-wait-children
               :retry retry))
         (prnt (if terminal-parent
                   (make-instance 'terminal-parent
                     :server sv :auto-rack auto-rack
                     :auto-resend-task auto-resend-task
                     :auto-exit auto-exit)
                 (make-instance 'parent :server sv
                                :host parent-host
                                :port parent-port))))
    ;; auto-initial-task �Υ��å�
    (when (and terminal-parent auto-initial-task)
      (when (stringp auto-initial-task)
        (setq auto-initial-task (split-string auto-initial-task)))
      (with (task-no (car auto-initial-task)
             task-body (strcat (cdr auto-initial-task) #\Newline "" #\Newline))
        (setf (parent-auto-treq-response-func prnt)
          #'(lambda (to task-head treq-head &rest args)
              (declare (ignore treq-head args))
              (sleep 1)
              (excl:gc t)
              (format *error-output* "~&auto-send-initial-task~%")
              (with1 msg (list* "task" "0" "0" task-head task-no task-body)
                (add-queue (cons to msg) (ts-queue sv)))))))
    ;; �����е�ư
    (start-server sv prnt)))

;; ά��
(defun ms (&rest args)
  (apply #'make-and-start-server args))

;; gero�Ǥ�ɾ����
(defun gs (&rest args)
  (apply #'make-and-start-server :auto-resend-task 0 :local-host "gero00" args))