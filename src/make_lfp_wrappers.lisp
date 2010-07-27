;;sbcl --noinform --load src/make_lfp_wrappers.lisp
;;;; -*- Mode: Lisp; indent-tabs-mode: nil -*-
;;;
(defpackage :make-lfp-wrapper (:use :cl) (:nicknames :mlw))
(defpackage :c-printers
  (:use :cl) (:nicknames :p)
  (:export #:c-type #:c-argument-list #:c-statement #:c-name #:c-expression))
(in-package :make-lfp-wrapper)

(defun c (fmt &rest args)
  (apply 'format t fmt args)
  (fresh-line))
(defun define (x &optional v)
  (c "#define ~A~@[ ~A~]" x v))
(defun include (&rest x)
  (dolist (i x)
    (c "#include <~A>" i)))


(eval-when (:compile-toplevel :load-toplevel :execute)
  (defun name->c-name (name)
    (etypecase name
      (symbol (substitute #\_ #\- (string-downcase name)))
      (string name)
      (cons (second name))))
  (defun make-lfp-name (name)
    (concatenate
     'string 
     (if (some #'lower-case-p name) "lfp_" "LFP_")
     name))
  (defun name->lfp-name (name)
    (typecase name
      (cons (first name))
      (t (make-lfp-name (name->c-name name)))))
  (defun p:c-name (stream name colon-p at-sign-p)
    (declare (ignore colon-p))
    (princ (funcall (if at-sign-p 'name->lfp-name 'name->c-name) name)  stream ))
  (defun p:c-type (stream type colon-p at-sign-p)
    (declare (ignore colon-p at-sign-p))
    (etypecase type
      (keyword (princ (string-downcase type) stream))
      #|(cons (ecase (car type)
              (:* ...)
              ...)|#))
  (defun p:c-argument-list (stream arglist colon-p at-sign-p)
    (declare (ignore colon-p at-sign-p))
    (format stream "~{~{~*~/p:c-type/ ~2:*~/p:c-name/~*~}~^, ~}" arglist))
  (defun p:c-expression (stream expression colon-p at-sign-p)
    (declare (ignore colon-p at-sign-p))
    (etypecase expression
      (string (princ expression stream))
      (symbol (princ (name->c-name expression) stream))
      (cons (etypecase (car expression)
              ((eql :funcall)
               (format stream "~/p:c-name/(~{~/p:c-expression/~^, ~})" (second expression) (cddr expression)))
              #|...|#))))
  (defun p:c-statement (stream statement colon-p at-sign-p)
    (declare (ignore colon-p at-sign-p))
    (etypecase statement
      (string (princ statement stream))
      (cons (etypecase (car statement)
              ((eql :return)
               (format stream "return ~/p:c-expression/;" (second statement)))
              #|...|#)))))

(defmacro defwrapper (name return-type arguments &body body)
  (let* ((lfp-name (name->lfp-name name))
         (c-name (name->c-name name))
         (argument-names (mapcar 'car arguments))
         (body (or body `((:return
                            ,(if arguments
                                 `(:funcall ,c-name ,@argument-names)
                                 c-name))))))
    (c "~/p:c-type/ ~A (~/p:c-argument-list/) {~%~{  ~/p:c-statement/~%~}}"
       return-type lfp-name arguments body)))


(c "#if defined(__linux__)")
(c "#undef _GNU_SOURCE")
(define "_XOPEN_SOURCE" 600)
(define "_LARGEFILE_SOURCE")
(define "_LARGEFILE64_SOURCE")
(define "_FILE_OFFSET_BITS" 64)
(c "#endif")


;;;-------------------------------------------------------------------------
;;; ERRNO-related functions
;;;-------------------------------------------------------------------------

(include "errno.h")

(defwrapper errno :int ())
(defwrapper set-errno :int ((value :int))
  "errno = value; return errno;")



;;;-------------------------------------------------------------------------
;;; waitpid status readers
;;;-------------------------------------------------------------------------
(include "sys/types.h" "sys/wait.h")

(defwrapper "WIFEXITED" :int ;; boolean
  ((status :int)))

(defwrapper "WEXITSTATUS" :int ;; unsigned-char
  ((status :int)))

(defwrapper "WIFSIGNALED" :int
  ((status :int)))

(defwrapper "WTERMSIG" :int
  ((status :int)))

(defwrapper "WCOREDUMP" :int ;; boolean
  ((status :int))
"
  #ifdef WCOREDUMP
  return WCOREDUMP(status);
  #else
  return 0;
  #endif
")

(defwrapper "WIFSTOPPED" :int ;; boolean
  ((status :int)))

(defwrapper "WSTOPSIG" :int
  ((status :int)))

(defwrapper "WIFCONTINUED" :int ;; boolean
  ((status :int)))



#|
;;;-------------------------------------------------------------------------
;;; Socket message readers
;;;-------------------------------------------------------------------------

(include "stdlib.h") ; needed on FreeBSD to define NULL
(include "sys/socket.h")

(declaim (inline cmsg.space cmsg.len cmsg.firsthdr cmsg.data))

(defwrapper ("CMSG_SPACE" cmsg.space) :unsigned-int
  (data-size :unsigned-int))

(defwrapper ("CMSG_LEN" cmsg.len) :unsigned-int
  (data-size :unsigned-int))

(defwrapper ("CMSG_FIRSTHDR" cmsg.firsthdr) :pointer
  (msg ("struct msghdr*" :pointer)))

(defwrapper ("CMSG_DATA" cmsg.data) :pointer
  (cmsg ("struct cmsghdr*" :pointer)))


;;;-------------------------------------------------------------------------
;;; Directory listing
;;;-------------------------------------------------------------------------

(include "sys/types.h" "dirent.h")

(declaim (inline dirfd))

(defwrapper (dirfd "dirfd") :int
  (dirp ("DIR*" :pointer)))
|#

#+sbcl (sb-ext:quit)
#+ccl (ccl:quit)

