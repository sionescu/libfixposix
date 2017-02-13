(in-package :asdf-user)
(defsystem libfixposix
  :version "0.1"
  :author "Masataro Asai"
  :mailto "guicho2.71828@gmail.com"
  :license "MIT"
  :depends-on (:cffi)
  :components ((:module "."
                :perform
                (compile-op (o c)
                            (flet ((sh (string)
                                     (run-program string
                                                  :output *standard-output*
                                                  :error-output *error-output*)))
                              (sh "autoreconf -i -f")
                              (sh "./configure")
                              (sh "make"))))
               (:file "lfp" :depends-on ("."))))
