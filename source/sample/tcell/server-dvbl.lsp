;;;; Tascell 拠点サーバ
;;; 起動は (make-and-start-server) 必要に応じてkeyword parameterを参照
;;; 性能評価の際は，  (push :tcell-no-transfer-log *features*)
;;; をして，ログ関係のコードを無視してコンパイルする．

(eval-when (:compile-toplevel :load-toplevel :execute)
  (unless (featurep :allegro)
    (error "Sorry! This programs work only on Allegro Common Lisp!"))
  (when (featurep :mswindows)
    (setq *locale* (find-locale "japan.EUC")))
  ;; The most debuggable (and yet reasonably fast) code, use
  (proclaim '(optimize (speed 2) (safety 1) (space 1))); (debug 3)))
  (load (compile-file-if-needed (or (probe-file "sc-misc.lsp")
                                    "../../sc-misc.lsp")
                                :output-file "sc-misc.fasl"))
  ;; Ignore logging code
  ;; (push :tcell-no-transfer-log *features*)
  )
#+sc-system (use-package "SC-MISC")
(use-package :socket)

(deftype gate () (type-of (mp:make-gate nil)))

;;; 強制的に待ち受けポート再利用
(defparameter *reuse-address* t)

;;; 親への接続アドレス:ポート
(defparameter *parent-host* nil)
(defparameter *parent-port* 8888)

;;; 子からの接続ホスト/ポート
(defparameter *server-host* "localhost")
(defparameter *children-port* 8888)

;;; anyでないtreqを常に転送
(defparameter *transfer-treq-always-if-notany* t)

;;; log出力の有無
(defparameter *transfer-log* t)         ; (featurep :tcell-no-transfer-log) の場合は常に無効
(defparameter *connection-log* t)
;;; send/recvのログを出力する長さ
(defparameter *transfer-log-length* 70)

;;; read-lineで読み込める長さの最大
(defconstant *max-line-length* 128)


(defparameter *retry* 20)

(defclass host ()
  ((server :accessor host-server :type tcell-server :initarg :server)
   (host :accessor host-hostname :type string :initarg :host)
   (port :accessor host-port :type fixnum :initarg :port)
   (sock :accessor host-socket :type stream :initarg :sock)
   (sender :accessor host-sender :type sender :initform nil)
   (receiver :accessor host-receiver :type receiver :initform nil)))

(defclass parent (host)
  ((host :initform *parent-host*)
   (port :initform *parent-port*)
   (n-treq-sent :accessor parent-n-treq-sent :type fixnum :initform 0)
   (last-none-time :accessor parent-last-none-time :type integer :initform -1)
   ))

(defclass terminal-parent (parent)
  ((host :initform "Terminal")
   (port :initform -1)
   (sock :initform (make-two-way-stream *standard-input* *standard-output*))))

(defclass child (host)
  ((id :accessor child-id :type fixnum)
   (divisibility :accessor child-divisibility
                 :type boolean :initform nil) ; <dvbl>がきたらt, <treq>,<none>がきたらnil
   (work-size :accessor child-wsize :type fixnum :initform 0)
                                        ; 仕事の大きさの目安．taskがきたら(- <ndiv>)に更新
   ;; (in-treq :accessor child-in-treq :type boolean :initform nil)
   ))

(defclass ta-entry ()                   ; elemens of treq-any-list
  ((from :accessor tae-from :type host :initarg :from)
   (head :accessor tae-head :type string :initarg :head)
   ))

(defclass buffer ()
  ((body :accessor buf-body :type list :initform (make-queue))))
   
(defclass shared-buffer (buffer)
  ((lock :accessor sbuf-lock :type mp:process-lock :initform (mp:make-process-lock))
   (gate :accessor sbuf-gate :type gate :initform (mp:make-gate nil)))) ; to notify addition

(defclass sender ()
  ((buffer :accessor send-buffer :type shared-buffer
           :initform (make-instance 'shared-buffer) :initarg :buffer)
   (writer :accessor writer :type function ; obj stream -> (write to stream)
           :initform #'write-string :initarg :writer)
   (send-process :accessor send-process :type mp:process :initform nil)
   (destination :accessor sender-dest :type stream :initform nil :initarg :dest)))
(defclass receiver ()
  ((buffer :accessor receive-buffer :type shared-buffer
           :initform (make-instance 'shared-buffer) :initarg :buffer)
   (reader :accessor reader :type function ; stream -> <obj,eof-p>
           :initform #'(lambda (strm) (aif (read-line strm nil nil) (values it nil)
                                        (values nil t)))
           :initarg :reader)
   (receive-process :accessor receive-process :type mp:process :initform nil)
   (source :accessor receiver-src :type stream :initform nil :initarg :src)))

(defclass tcell-server ()
  ((host :accessor ts-hostname :initform *server-host* :initarg :local-host)
   (message-buffer :accessor ts-buffer :type shared-buffer
                   :initform (make-instance 'shared-buffer))
   (proc-cmd-process :accessor ts-proc-cmd-process :type mp:process :initform nil)
   (parent :accessor ts-parent :type parent)
   (children-port :accessor ts-chport :initform *children-port* :initarg :children-port)
   (children-sock0 :accessor ts-chsock0 :type stream) ; 待ち受け
   (children :accessor ts-children :type list :initform '())
   (eldest-child :accessor ts-eldest-child :type child :initform nil)
   (n-children :accessor ts-n-children :type fixnum :initform 0)
   (child-next-id :accessor ts-child-next-id :type fixnum :initform 0)
   (treq-any-list :accessor ts-talist :type list :initform '()) ;; treq-anyを出せていないリスト
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
      #'monitor-and-send-buffer
      (send-buffer sdr) (sender-dest sdr) (writer sdr))))

(defmethod initialize-instance :after ((rcvr receiver) &rest initargs)
  (declare (ignore initargs))
  (setf (receive-process rcvr)
    (mp:process-run-function "RECEIVE-PROCESS"
      #'receive-and-add-to-buffer
      (receive-buffer rcvr) (receiver-src rcvr) (reader rcvr))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Finalizers
(defmethod finalize ((sdr sender))
  (when (send-process sdr)
    (mp:process-kill (send-process sdr))
    (setf (send-process sdr) nil)))

(defmethod finalize ((rcvr receiver))
  (when (receive-process rcvr)
    (mp:process-kill (receive-process rcvr))
    (setf (receive-process rcvr) nil)))

(defmethod finalize ((sv tcell-server))
  (when (ts-accept-connection-process sv)
    (mp:process-kill (ts-accept-connection-process sv))
    (setf (ts-accept-connection-process sv) nil))
  (when (ts-proc-cmd-process sv)
    (mp:process-kill (ts-proc-cmd-process sv))
    (setf (ts-proc-cmd-process sv) nil))
  (finalize (ts-parent sv))
  (loop for chld in (ts-children sv)
      do (finalize chld))
  (close (ts-chsock0 sv)))

(defmethod finalize ((hst host))
  (finalize (host-sender hst))
  (finalize (host-receiver hst)))

(defmethod finalize ((hst (eql nil))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defmethod start-server ((sv tcell-server) (prnt parent))
  (unwind-protect
      (progn
        ;; 親へ接続
        (setf (ts-parent sv) (connect-to prnt))
        ;; 子からの待ち受けポートを開く
        (setf (ts-chsock0 sv)
          (make-socket :connect :passive
                       :reuse-address *reuse-address*
                       :local-host (ts-hostname sv)
                       :local-port (ts-chport sv)))
        ;; 最初の子が接続してくるのを待つ
        (format *error-output* "~&Waiting for connection to ~A:~D...~%"
          (ts-hostname sv) (ts-chport sv))
        (wait-and-add-child sv)
        ;; 子からの接続受け付け用プロセス起動
        (activate-accept-connection-process sv)
        ;; メッセージ処理用プロセス起動
        (activate-proc-cmd-process sv)
        ;; 標準入力からの入力待ち
        (let* ((msg-buf (ts-buffer sv))
               (prnt (ts-parent sv))
               (terminal-parent-p (typep prnt 'terminal-parent))
               (reader (make-receiver-reader prnt))
               (src (host-socket prnt)))
          (loop
            (multiple-value-bind (msg eof-p) (funcall reader src)
              (when terminal-parent-p
                (add-buffer msg msg-buf))
              (when eof-p (return))))
          ))
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
  (terpri *error-output*))

(defmethod wait-and-add-child ((sv tcell-server))
  (let ((next-child (make-instance 'child :server sv)))
    (awhen (connect-from next-child (ts-chsock0 sv) :wait t)
      (add-child sv it))))

(defmethod activate-accept-connection-process ((sv tcell-server))
  (setf (ts-accept-connection-process sv)
    (mp:process-run-function "ACCEPT-CHILD-CONNECTION"
      #'(lambda () (loop (wait-and-add-child sv))))))

(defmethod activate-proc-cmd-process ((sv tcell-server))
  (setf (ts-proc-cmd-process sv)
    (mp:process-run-function "PROC-CMD"
      #'(lambda (msg-buf &aux (msg-gate (sbuf-gate msg-buf)))
          (loop
            ;; 親または子からの入力待ち
            (mp:process-wait "Waiting for a message to T-Cell server."
                             #'mp:gate-open-p msg-gate)
            ;; メッセージ処理
            (while (mp:gate-open-p msg-gate)
              (destructuring-bind (host . message) (delete-buffer msg-buf)
                (proc-cmd sv host message)))))
      (ts-buffer sv))))

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
                (accept-connection sock0 :wait wait))))
    (if sock
        (progn
          (initialize-connection hst)
          hst)
      nil)))

(defmethod connect-from :after ((hst host) sock0 &key (wait t))
  (declare (ignorable sock0 wait))
  (when (host-socket hst)
    (when *connection-log*
      (format *error-output*
        "~&Accept connection from ~A.~%" (host-name-port hst)))))

(defmethod initialize-connection ((hst host) &aux (sock (host-socket hst)))
  (setf (host-hostname hst) (ipaddr-to-hostname (remote-host sock)))
  (setf (host-port hst) (remote-port sock))
  (initialize-sender hst)
  (initialize-receiver hst)
  hst)

(defmethod initialize-connection ((hst terminal-parent))
  (initialize-sender hst)
  hst)

;; make-instance -> initialize-connection -> initialize-sender/receiverなので
;; initialize-instanceでできない
(defmethod initialize-sender ((hst host) &aux (sock (host-socket hst)))
  (setf (host-sender hst)
    (make-instance 'sender :dest sock :writer #'sender-writer))
  hst)

(defmethod initialize-receiver ((hst host) &aux (sock (host-socket hst)))
  (setf (host-receiver hst)
    (make-instance 'receiver
      :src sock :reader (make-receiver-reader hst)
      :buffer (ts-buffer (host-server hst)))) ; readしたものは共有のバッファに入れる
  hst)

;;;
(defun sender-writer (obj dest)
  (etypecase obj
    (list (mapc #'(lambda (ob) (sender-writer ob dest)) obj))
    (character (write-char obj dest))
    (string (write-string obj dest))
    (array (write-sequence obj dest))))

(defun msg-log-string (obj)
  (with-output-to-string (s)
    (write-msg-log obj s)))

(defun write-msg-log (obj dest)
  (typecase obj
    (list (mapc #'(lambda (ob) (write-msg-log ob dest)) obj))
    (symbol (write-string (symbol-name obj) dest))
    (character (write-char obj dest))
    (string (write-string obj dest))
    (array (format dest "#<Binary data: SIZE=~D>" (length obj)))))

(defmethod make-receiver-reader ((hst host))
  (let ((getstr (make-array *max-line-length*
                            :element-type 'character :fill-pointer *max-line-length*)))
    #'(lambda (stream)
        (setf (fill-pointer getstr) *max-line-length*)
        (let* ((n-char (setf (fill-pointer getstr)
                         (excl:read-line-into getstr stream nil 0)))
               (eof-p (= 0 n-char))
               (msg (if eof-p '(:|eof|)
                      (let ((msg0 (split-string getstr)))
                        (rplaca msg0 (intern (car msg0) :keyword))
                        msg0))))
          #-tcell-no-transfer-log
          (when *transfer-log*          ; debug print
            (format *error-output* "Received ~A~%" getstr))
          (if (or (eq :|task| (car msg)) (eq :|rslt| (car msg)))
              (rplacd (last msg) (read-body stream)))
          #-tcell-no-transfer-log
          (when *transfer-log*          ; debug print
            (format *error-output* " from ~A~%" (hostinfo hst))
            (force-output *error-output*))
          (values (cons hst msg) eof-p))
        )))

;;; task, rsltのbody部の読み込み
(defun read-body (stream)
  (let ((ret '()))
    (loop
      (let* ((pre (read-line stream t))
             (len (length pre)))
        ;; 空行で終了
        (when (= len 0) (return))
        (push pre ret)
        (push #\Newline ret)
        #-tcell-no-transfer-log
        (when *transfer-log*            ; debug print
          (write-string pre *error-output*)
          (write-char #\Newline *error-output*))
        ;; 最初の行が#\(でおわっていたら次の行はbyte-header，次いでbyte-data
        (when (and (>= len 1)
                   (char= #\( (aref pre (- len 1))))
          #-tcell-no-transfer-log
          (when *transfer-log* (terpri *error-output*)) ; debug print
          (let* ((byte-header (read-line stream t))
                 (whole-size (parse-integer byte-header :junk-allowed t)))
            (push byte-header ret)
            (push #\Newline ret)
            #-tcell-no-transfer-log
            (when *transfer-log*        ; debug print
              (write-string byte-header *error-output*)
              (terpri *error-output*))
            (let ((byte-data (make-array whole-size)))
              (read-sequence byte-data stream :end whole-size)
              (push byte-data ret)
              #-tcell-no-transfer-log
              (when *transfer-log*      ; debug print
                (write-msg-log byte-data *error-output*))
              )))))
    (nreverse ret)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; バッファ監視->送信
(defmethod monitor-and-send-buffer ((sbuf shared-buffer) dest
                                    &optional (writer #'write-string))
  (let ((gate (sbuf-gate sbuf)))
    (loop
      (mp:process-wait "Waiting for something is added to the buffer"
                       #'mp:gate-open-p gate)
      (while (mp:gate-open-p gate)
        (funcall writer (delete-buffer sbuf) dest))
      (force-output dest))))

;; 受信->バッファに追加
(defmethod receive-and-add-to-buffer ((sbuf shared-buffer) src
                                      &optional (reader #'read-line))
  (loop
    (multiple-value-bind (obj eof-p) (funcall reader src)
      (add-buffer obj sbuf)
      (when eof-p (return)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 要素へのaccessor

;;; 子の追加・削除
(defmethod add-child ((sv tcell-server) (chld child))
  (setf (child-id chld)	(ts-child-next-id sv))
  (incf (ts-child-next-id sv))
  (incf (ts-n-children sv))
  (push chld (ts-children sv))
  (unless (ts-eldest-child sv)
    (setf (ts-eldest-child sv) chld)))

(defmethod remove-child ((sv tcell-server) (prnt parent))
  ;; do nothing
  )

(defmethod remove-child ((sv tcell-server) (chld child))
  (finalize chld)
  (setf (ts-children sv) (delete chld (ts-children sv) :count 1))
  (delete-treq-any sv chld "*")
  (when (eq (ts-eldest-child sv) chld)
    (setf (ts-eldest-child sv) (car (last (ts-children sv)))))
  (decf (ts-n-children sv))
  )

;;; (= id n) の子へのアクセス
(defmethod nth-child ((sv tcell-server) n)
  (find n (ts-children sv)
        :key #'(lambda (chld) (child-id chld))
        :test #'=))

;;; 一番仕事が残ってそうな子
(defmethod most-divisible-child ((sv tcell-server))
  (let ((dchildren (remove-if-not #'child-divisibility (ts-children sv))))
    (when dchildren
      (find-max dchildren
                :key #'child-wsize))))

(defmethod divisibility-on ((chld child))
  (unless (child-divisibility chld)
    (setf (child-divisibility chld) t)
    (setf (child-wsize chld) 0)))
(defmethod divisibility-off ((chld child))
  (setf (child-divisibility chld) nil))

;;; 子のwork-size を更新
(defmethod renew-work-size ((chld child) wsize)
  (setf (child-wsize chld) wsize))

;;; treq-any-list への要素追加
;;; p-task-head は，"<treqのfromのid>" ":" "<treqのtask-head>" 
(defmethod push-treq-any0 ((sv tcell-server) (tae ta-entry))
  (push tae (ts-talist sv)))
(defmethod push-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (push-treq-any0 sv (make-instance 'ta-entry :from from :head p-task-head)))

;;; treq-any-list のpop
(defmethod pop-treq-any ((sv tcell-server))
  (aand (pop (ts-talist sv))
        (list (tae-from it) (tae-head it))))


;;; treq-any-list から指定要素削除
(defmethod delete-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (setf (ts-talist sv)
    (delete (make-instance 'ta-entry :from from :head p-task-head)
            (ts-talist sv)
            :test #'ta-entry-match)))

;;; treq-any-list のmemberか
(defmethod member-treq-any ((sv tcell-server) (from host) (p-task-head string))
  (member (make-instance 'ta-entry :from from :head p-task-head)
          (ts-talist sv)
          :test #'ta-entry-match))

;;; ta-entry の同一性
(defmethod ta-entry-match ((x ta-entry) (y ta-entry))
  (and (eq (tae-from x) (tae-from y))
       (or (string= "*" (tae-head x))
           (string= "*" (tae-head y))
           (string= (tae-head x) (tae-head y)))))

;; ta-entry の情報
(defmethod ta-entry-info ((tae ta-entry))
  `((from ,(hostinfo (tae-from tae)))
    (head ,(tae-head tae))))

;; bufferへの追加
(defmethod add-buffer (elm (buf buffer))
  (insert-queue elm (buf-body buf)))

(defmethod add-buffer :around (elm (sbuf shared-buffer))
  (declare (ignorable elm))
  (mp:with-process-lock ((sbuf-lock sbuf))
    (prog1 (call-next-method)
      (mp:open-gate (sbuf-gate sbuf)))))

;; 空buffer
(defmethod empty-buffer-p ((buf buffer))
  (empty-queue-p (buf-body buf)))

;; bufferから取り出し
(defmethod delete-buffer ((buf buffer))
  (delete-queue (buf-body buf)))

(defmethod delete-buffer :around ((sbuf shared-buffer))
  (mp:with-process-lock ((sbuf-lock sbuf))
    (prog1 (call-next-method)
      (when (empty-buffer-p sbuf)
        (mp:close-gate (sbuf-gate sbuf))))))

;; bufferから検索して取り出し
(defmethod find-delete-buffer ((buf buffer) test &key (key #'identity))
  (find-delete-queue (buf-body buf) test :key key))

(defmethod find-delete-buffer :around ((sbuf shared-buffer) test
                                       &key (key #'identity)
                                            (wait nil))
  (declare (ignore test key))
  (block :end
    (tagbody
      :retry
      (mp:with-process-lock ((sbuf-lock sbuf))
        (let ((item (call-next-method)))
          (when item
            (when (empty-buffer-p sbuf)
              (mp:close-gate (sbuf-gate sbuf)))
            (return-from :end item))
          (unless wait
            (return-from :end nil))
          (mp:close-gate (sbuf-gate sbuf))))
      (mp:process-wait "FIND-DELETE-BUFFER-WAIT"
                       #'mp:gate-open-p (sbuf-gate sbuf))
      (go :retry))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 相対アドレス文字列の操作
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

;;; アドレスの先頭を切り取って，残りと切り取った先頭アドレス
;;; に相当するホストを返す
(defmethod head-shift ((sv tcell-server) head-string)
  (let* ((sp-head (split-string-1 head-string #\:))
         (host (hostid-to-host sv (first sp-head))))
    (unless host
      (warn "Connection from/to ~S does not exist." (first sp-head)))
    (list host (second sp-head))))

;;; アドレスhead-stringの先頭にhstのidを追加したものを返す
(defmethod head-push ((hst host) head-string)
  (string+ (hostid hst) ":" head-string))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; メッセージ送信
(defmethod send ((to host) obj)
  (add-buffer obj (send-buffer (host-sender to))))

(defmethod send ((to (eql nil)) obj)
  (format *error-output* "Failed to send ~S~%" (msg-log-string obj)))

;; debug print
#-tcell-no-transfer-log
(defmethod send :after ((to host) obj)
  (when *transfer-log*
    (format *error-output* "~&Sent ~S to ~S~%" (msg-log-string obj) (hostinfo to))))

(defmethod send-treq (to task-head treq-head)
  (send to (list "treq " task-head #\Space treq-head #\Newline)))

(defmethod send-task (to wsize-str rslt-head task-head task-no task-body)
  (send to (list "task "
                 wsize-str #\Space
                 rslt-head #\Space
                 task-head #\Space
                 task-no #\Newline
                 task-body #\Newline)))
(defmethod send-task :before ((to child) wsize-str rslt-head task-head task-no task-body)
  (declare (ignore wsize-str rslt-head task-head task-no task-body)))

#+obsolete                              ; send-taskに統一
(defmethod send-task-body (to task-body)
  (send to "~A~%" task-body))

(defmethod send-rslt (to rslt-head rslt-body)
  (send to (list "rslt " rslt-head #\Newline rslt-body #\Newline)))

#+obsolete                              ; send-rsltに統合
(defmethod send-rslt-body (to rslt-body)
  (send to "~A~%" rslt-body))

(defmethod send-none (to task-head)
  (send to (list "none " task-head #\Newline)))

(defmethod send-rack (to task-head)
  (send to (list "rack " task-head #\Newline)))

(defmethod send-dvbl (to)
  (send to #.(format nil "dvbl~%")))

(defmethod send-dvbl ((to terminal-parent))
  ;; do nothing
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; メッセージ受信->処理

#+obsolete                              ; 現在はreceive-and-add-to-buffer
(defmethod recv-cmd ((sv tcell-server) (from host))
  (let ((sock (host-socket from)))
    (awhen (aand (listen sock)
                 (read-line sock nil "eof")
                 (split-string it))
      #-tcell-no-transfer-log
      (when *transfer-log*
        (format *error-output* "~&Received ~S from ~A~%"
                it (hostinfo from)))
      it)))

#+obsolete                              ; receiver-readerでメッセージに統合
(defmethod recv-body ((sv tcell-server) (from host))
  (loop
      for x = (find-delete-buffer (ts-buffer sv)
                                  #'(lambda (x) (eq x from))
                                  :key #'car :wait t)
      when x
      do (return (strcat (second x) #\Space))))

;;; Dispatch
(defmethod proc-cmd ((sv tcell-server) (from host) cmd)
  (case (car cmd)
    (:|treq| (proc-treq sv from cmd))
    (:|task| (proc-task sv from cmd))
    (:|none| (proc-none sv from cmd))
    (:|rslt| (proc-rslt sv from cmd))
    (:|rack| (proc-rack sv from cmd))
    (:|dvbl| (proc-dvbl sv from cmd))
    (:|eof|  (remove-child sv from))
    (:|log|  (proc-log sv from cmd))
    (:|stat| (print-server-status sv))
    (:|eval| (print (eval (read-from-string (strcat (cdr cmd) #\Space)))))
    (otherwise (warn "Unknown Command:~S" cmd))))

;;; treq
(defmethod proc-treq ((sv tcell-server) (from host) cmd)
  (let ((p-task-head (head-push from (second cmd))) ; タスク要求者
        (treq-head (third cmd)))        ; 要求先
    (unless
        (if (string= "any" treq-head)
            (try-send-treq-any sv from p-task-head)
          (destructuring-bind (hst0 s-treq-head)
              (head-shift sv treq-head)
            (if *transfer-treq-always-if-notany*
                (send-treq hst0 p-task-head s-treq-head)
              (try-send-treq sv hst0 p-task-head s-treq-head))))
      ;; treqを送れなかった場合
      (refuse-treq sv from p-task-head))))

;; treqを送ってきた子のdivisibilityを0にする
(defmethod proc-treq :before ((sv tcell-server) (from child) cmd)
  (declare (ignore cmd))
  (divisibility-off from)
  )

(defmethod try-send-treq-any ((sv tcell-server) (from host) p-task-head)
  (or (awhen (most-divisible-child sv)
        (try-send-treq sv it p-task-head "any"))
      (and (eq (ts-eldest-child sv) from)
           (try-send-treq sv (ts-parent sv)
                          p-task-head "any"))))

(defmethod try-send-treq ((sv tcell-server) (to host) p-task-head s-treq-head)
  (send-treq to p-task-head s-treq-head)
  t)

(defmethod try-send-treq :around ((sv tcell-server) (to child) p-task-head s-treq-head)
  (declare (ignore p-task-head s-treq-head))
  (if (child-divisibility to)
      (call-next-method)
    nil))

(defmethod refuse-treq ((sv tcell-server) (from host) p-task-head)
  (if (member-treq-any sv from p-task-head)
      (send-none from (second (head-shift sv p-task-head)))
    (push-treq-any sv from p-task-head)))

;; treq-any-listにある要素を tryしなおす
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
      (head-shift sv (fourth cmd))      ; タスク送信先
    (let ((wsize-str (second cmd))      ; 仕事の大きさ
          (p-rslt-head (head-push from (third cmd))) ; 結果返信先
          (task-no (fifth cmd))         ; タスク番号 （fib, lu, ...）
          (task-body (nthcdr 5 cmd)))
      (send-task to wsize-str p-rslt-head s-task-head task-no
                 task-body)
      )))

(defmethod proc-task :before ((sv tcell-server) (from child) cmd)
  (let ((wsize (parse-integer (second cmd))))
    (renew-work-size from (- wsize))))

#+obsolete                              ; recv-bodyに統一
(defmethod recv-task-body ((from host))
  (read-line (host-socket from)))

;;; none
(defmethod proc-none ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd))      ; none送信先
    (send-none to s-task-head)))

(defmethod proc-none :before ((sv tcell-server) (from child) cmd)
  (declare (ignore cmd))
  (divisibility-off from))

;;; rslt
(defmethod proc-rslt ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-rslt-head)
      (head-shift sv (second cmd))      ; rslt送信先
    (let ((rslt-body (cddr cmd)))
      (send-rslt to s-rslt-head rslt-body))))

#+obsolete                              ; recv-bodyに統合
(defmethod recv-rslt-body ((from host))
  (read-line (host-socket from)))

;;; rack
(defmethod proc-rack ((sv tcell-server) (from host) cmd)
  (destructuring-bind (to s-task-head)
      (head-shift sv (second cmd))      ; rack送信先
    (send-rack to s-task-head)))

;;; dvbl
(defmethod proc-dvbl ((sv tcell-server) (from host) cmd)
  (declare (ignore cmd))
  (if (<= (retry-treq sv) 0)
      (send-dvbl (ts-parent sv))))

(defmethod proc-dvbl :before ((sv tcell-server) (from child) cmd)
  (declare (ignore cmd))
  (divisibility-on from))

;;; log
(defmethod proc-log ((sv tcell-server) (from host) cmd)
  (loop
      for str in (cdr cmd)
      as mode = (read-from-string str)
      as setter in (list #'toggle-transfer-log #'toggle-connection-log)
      do (funcall setter mode))
  (show-log-mode))

(defun toggle-transfer-log (mode)
  (setq *transfer-log* (not (not mode))))

(defun toggle-connection-log (mode)
  (setq *connection-log* (not (not mode))))

(defun show-log-mode ()
  (pprint `((*transfer-log* #-tcell-no-transfer-log ,*transfer-log*
                            #+tcell-no-transfer-log :invalidated-by-features)
            (*connection-log* ,*connection-log*))
          *error-output*)
  (terpri *error-output*))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defun make-and-start-server (&key 
                              (local-host *server-host*)
                              (children-port *children-port*)
                              (retry *retry*)
                              (terminal-parent nil)
                              (parent-host *parent-host* ph-given)
                              (parent-port *parent-port*))
  (unless ph-given (setq terminal-parent t))
  (let* ((sv (make-instance 'tcell-server
               :local-host local-host
               :children-port children-port
               :retry retry))
         (prnt (if terminal-parent
                   (make-instance 'terminal-parent :server sv)
                 (make-instance 'parent :server sv
                                :host parent-host
                                :port parent-port))))
    (start-server sv prnt)))


;; 略記
(defun ms (&rest args)
  (apply #'make-and-start-server args))

;; geroでの評価用
(defun gs ()
  (make-and-start-server :local-host "gero00"))
