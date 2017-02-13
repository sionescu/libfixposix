
(defpackage :libfixposix (:export :libfixposix))

(cffi:define-foreign-library libfixposix
  (t (:default #.(cl:merge-pathnames
                  "src/lib/.libs/libfixposix.so"
                  cl:*compile-file-pathname*))))
